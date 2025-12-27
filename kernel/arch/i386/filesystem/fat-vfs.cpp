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

        // ReadBiosBlock(hd, mbr.primaryPartition[i].start_lba);
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

    uint32_t fatDataStartInSectors = fatTableStartInSectors + fatTableSizeInSectors * biosParameterBlock.fatCopies;

    uint32_t rootDirectoryStartInSectors = fatDataStartInSectors + biosParameterBlock.sectorPerCluster * (biosParameterBlock.rootCluster - 2);

    traverseDirectories(hd, rootDirectoryStartInSectors, fatDataStartInSectors, sectorsPerCluster, 0);
}

void FATVFS::traverseDirectories(AdvancedTechnologyAttachment *hd, uint32_t directoryStartInSectors, uint32_t fatDataStartInSectors, uint8_t sectorsPerCluster, uint8_t level)
{
    ++level;
    uint8_t CLUSTER_SIZE_IN_FAT_ENTRIES = 16;

    DirectoryEntryFat32 directoryEntries[CLUSTER_SIZE_IN_FAT_ENTRIES];

    hd->Read28(directoryStartInSectors, (uint8_t *)&directoryEntries[0], sizeof(DirectoryEntryFat32) * CLUSTER_SIZE_IN_FAT_ENTRIES);

    for (int i = 0; i < CLUSTER_SIZE_IN_FAT_ENTRIES; i++)
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
            uint32_t firstFileCluster = ((uint32_t)directoryEntries[i].firstClusterHi) << 16 |
                                        ((uint32_t)directoryEntries[i].firstClusterLow);

            int32_t nextFileCluster = firstFileCluster;
            uint32_t fileSector = fatDataStartInSectors + sectorsPerCluster * (nextFileCluster - 2);

            this->traverseDirectories(hd, fileSector, fatDataStartInSectors, sectorsPerCluster, level);
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
