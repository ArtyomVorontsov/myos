#include <filesystem/fat-vfs.hpp>
#include <filesystem/msdospart.hpp>
#include <stdio.h>

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_LFN 0x0F

FATVFS::FATVFS(AdvancedTechnologyAttachment *hd)
{

    MasterBootRecord mbr;
    printf("MBR: \n");

    hd->Read28(0, (uint8_t *)&mbr, sizeof(MasterBootRecord));

    if (mbr.magicnumber != 0xAA55)
    {
        printf("illegal MBR");
        return;
    }

    for (int i = 0; i < 4; i++)
    {

        // check for Old / unused / invalid partition type, ignore that
        uint32_t INVALID_PARTITION_TYPE = 0x80;
        if (mbr.primaryPartition[i].partition_id == INVALID_PARTITION_TYPE)
            continue;

        this->partitionAmount++;
        this->partitionInfo[i].partitionId = mbr.primaryPartition[i].partition_id;
        this->partitionInfo[i].bootable = mbr.primaryPartition[i].bootable == 0x80;
        this->partitionInfo[i].partitionOffset = mbr.primaryPartition[i].start_lba;
    }

    // Do directory traversal

    // first fat partition

    PartitionInfo partition = this->partitionInfo[0];

    BiosParameterBlock32 biosParameterBlock;

    // Read FAT32 bios parameter block
    hd->Read28(partition.partitionOffset, (uint8_t *)&biosParameterBlock, sizeof(BiosParameterBlock32));

    uint8_t sectorsPerCluster = biosParameterBlock.sectorPerCluster;

    // Get File Allocation Table address start in memory (in sectors)
    uint32_t fatTableStartInSectors = partition.partitionOffset + biosParameterBlock.reservedSectors;
    // Get File Allocation Table size in sectors
    uint32_t fatTableSizeInSectors = biosParameterBlock.tableSize;

    uint32_t startInSectorsDATA = fatTableStartInSectors + fatTableSizeInSectors * biosParameterBlock.fatCopies;

    uint32_t directoryStartInSectorsDATA = startInSectorsDATA + biosParameterBlock.sectorPerCluster * (biosParameterBlock.rootCluster - 2);

    traverseDirectories(hd, fatTableStartInSectors, directoryStartInSectorsDATA, biosParameterBlock.rootCluster, startInSectorsDATA, sectorsPerCluster, 0);
}

void FATVFS::traverseDirectories(
    AdvancedTechnologyAttachment *hd,
    uint32_t startInSectorsFAT,
    uint32_t startInSectorsDATA,
    uint32_t directoryClusterNumberInDataRegion,
    uint32_t directoryStartInSectorsDATA,
    uint8_t sectorsPerCluster,
    uint8_t level)
{
    // TODO: change hardcoded values to dynamic ones
    uint32_t CLUSTER_SIZE_IN_BYTES = 4096;
    uint32_t SECTOR_SIZE_IN_BYTES = 512;
    uint8_t NUMBER_OF_DIRECTORY_ENTRIES_PER_SECTOR = SECTOR_SIZE_IN_BYTES / sizeof(DirectoryEntryFat32);
    uint32_t CLUSTER_SIZE_IN_DATA_ENTRIES = CLUSTER_SIZE_IN_BYTES / sizeof(DirectoryEntryFat32);
    uint32_t END_OF_CLUSTER_CHAIN = 0x0FFFFFF8;

    // Buffer to store directory entries
    DirectoryEntryFat32 directoryEntries[CLUSTER_SIZE_IN_DATA_ENTRIES * 2];
    uint32_t nextFileClusterNumber = 0;
    uint32_t currentFileClusterNumber = directoryClusterNumberInDataRegion;
    uint8_t bufferFAT[513];
    uint32_t clusterAmountRead = 0;

    // File system level
    ++level;

    // -2 is because first 2 clusters are not used in FAT, but in data they are... so cluster number 2 in FAT is 0 in DATA
    directoryStartInSectorsDATA = startInSectorsDATA + sectorsPerCluster * (directoryClusterNumberInDataRegion - 2);

    // Read cluster chain
    while (true)
    {
        clusterAmountRead++;
        // Read whole cluster
        for (int j = 0; j < sectorsPerCluster; j++)
        {
            // Read whole sector
            hd->Read28(
                directoryStartInSectorsDATA + j,
                (uint8_t *)&directoryEntries[j * NUMBER_OF_DIRECTORY_ENTRIES_PER_SECTOR],
                sizeof(DirectoryEntryFat32) * NUMBER_OF_DIRECTORY_ENTRIES_PER_SECTOR);
        }

        // Get next cluster

        // Get FAT sector number where current file cluster is mentioned 
        uint32_t sectorForCurrentClusterInFAT = currentFileClusterNumber / (SECTOR_SIZE_IN_BYTES / sizeof(uint32_t));

        // Get FAT sector offset where current file cluster is mentioned 
        uint32_t offsetInSectorForCurrentClusterInFAT = currentFileClusterNumber % (SECTOR_SIZE_IN_BYTES / sizeof(uint32_t));

        // Read FAT sector where current cluster number is mentioned
        hd->Read28(startInSectorsFAT + sectorForCurrentClusterInFAT, bufferFAT, SECTOR_SIZE_IN_BYTES);

        // Get next cluster number from FAT buffer
        // We need to mask that because only 28 bits are used for cluster numbers
        nextFileClusterNumber = ((uint32_t *)&bufferFAT)[offsetInSectorForCurrentClusterInFAT] & 0x0FFFFFFF;

        if (nextFileClusterNumber >= END_OF_CLUSTER_CHAIN)
        {
            // last cluster
            break;
        }

        // In next iteration nextFileClusterNumber is current one 
        currentFileClusterNumber = nextFileClusterNumber;

        // Calculate new file start in sectors 
        // -2 is because first 2 clusters are not used in FAT, but in data they are... so cluster number 2 in FAT is 0 in DATA
        directoryStartInSectorsDATA = startInSectorsDATA + sectorsPerCluster * (nextFileClusterNumber - 2);
    }

    for (int i = 0; i < CLUSTER_SIZE_IN_DATA_ENTRIES * clusterAmountRead; i++)
    {
        uint8_t attr = directoryEntries[i].attributes;

        bool isLFN = (attr == ATTR_LFN);
        bool isDirectory = (attr & ATTR_DIRECTORY);
        bool isVolume = (attr & ATTR_VOLUME_ID);
        bool isFile = (!isLFN &&
                       !isVolume &&
                       !(attr & ATTR_DIRECTORY));

        if (directoryEntries[i].name[0] == 0x00)
        {
            // No more files
            break;
        }

        if (directoryEntries[i].name[0] == '.')
        {
            // Current directory or parent directory
            // TODO: handle properly
            continue;
        }

        if (isLFN)
        {
            // Skip Long File Name record for now
            continue;
        }

        if (isFile)
            this->printFileInfo(&(directoryEntries[i]), level);

        if (isDirectory)
        {
            this->printDirectoryInfo(&(directoryEntries[i]), level);

            // Get file content
            uint32_t directoryClusterNumberInDataRegion = ((uint32_t)directoryEntries[i].firstClusterHi) << 16 |
                                                     ((uint32_t)directoryEntries[i].firstClusterLow);

            this->traverseDirectories(
                hd,
                startInSectorsFAT,
                startInSectorsDATA,
                directoryClusterNumberInDataRegion,
                directoryStartInSectorsDATA,
                sectorsPerCluster,
                level);
        }
    }
}

void FATVFS::printDirectoryInfo(DirectoryEntryFat32 *directoryEntry, uint8_t level)
{
    // Print level
    for (int i = 0; i < level; i++)
    {
        printf("   ");
    }

    printf("D");
    printf(", ");

    // Print file name
    char *fname = "        ";
    for (int j = 0; j < 8; j++)
        fname[j] = directoryEntry->name[j];

    printf(fname);
    printf("\n");
}

void FATVFS::printFileInfo(DirectoryEntryFat32 *directoryEntry, uint8_t level)
{
    // Print level
    for (int i = 0; i < level; i++)
    {
        printf("   ");
    }

    printf("F");
    printf(", ");

    // Print file name
    char *fname = "        ";
    for (int j = 0; j < 8; j++)
        fname[j] = directoryEntry->name[j];

    printf(fname);
    printf("\n");
}

void FATVFS::printPartitionInfo()
{
    for (size_t i = 0; i < this->partitionAmount; i++)
    {
        printf("==================\n");
        printf("PartitionID ");
        printfHex(i & 0xFF);

        if (this->partitionInfo[i].bootable == 0x80)
            printf(" bootable. Type ");
        else
            printf(" not bootable. Type ");

        printfHex(this->partitionInfo[i].partitionId);
        printf("\n");

        printf("PartitionOffset ");
        printfHex(this->partitionInfo[i].partitionOffset & 0xFF);
        printfHex((this->partitionInfo[i].partitionOffset >> 8) & 0xFF);
        printfHex((this->partitionInfo[i].partitionOffset >> 16) & 0xFF);
        printfHex((this->partitionInfo[i].partitionOffset >> 24) & 0xFF);
        printf("\n");
    }
}
