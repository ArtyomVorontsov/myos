#include <filesystem/fat-vfs.hpp>
#include <filesystem/msdospart.hpp>
#include <kernel/memory-management.hpp>
#include <filesystem/fat-file-reader.hpp>
#include <filesystem/fat-file-writer.hpp>
#include <stdio.h>
#include <string.h>

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

    this->hd = hd;
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

    this->sectorPerCluster = biosParameterBlock.sectorPerCluster;

    // Get File Allocation Table address start in memory (in sectors)
    uint32_t fatTableStartInSectors = partition.partitionOffset + biosParameterBlock.reservedSectors;

    this->startInSectorsFAT = fatTableStartInSectors;

    // Get File Allocation Table size in sectors
    uint32_t fatTableSizeInSectors = biosParameterBlock.tableSize;

    this->startInSectorsDATA = fatTableStartInSectors + fatTableSizeInSectors * biosParameterBlock.fatCopies;

    uint32_t fileClusterNumberInDataRegion = biosParameterBlock.rootCluster;
    uint32_t directoryStartInSectorsDATA = startInSectorsDATA + biosParameterBlock.sectorPerCluster * (fileClusterNumberInDataRegion - 2);

    DirectoryEntryFat32 *rootDirectoryEntry = (DirectoryEntryFat32 *)MemoryManager::activeMemoryManager->malloc(sizeof(DirectoryEntryFat32));

    rootDirectoryEntry->name[0] = 'r';
    rootDirectoryEntry->name[1] = 'o';
    rootDirectoryEntry->name[2] = 'o';
    rootDirectoryEntry->name[3] = 't';

    rootDirectoryEntry->firstClusterHi = 0x0000;
    rootDirectoryEntry->firstClusterLow = 0x0002;

    FATFileEnumerator *rootFileEnumerator = (FATFileEnumerator *)MemoryManager::activeMemoryManager->malloc(sizeof(FATFileEnumerator));

    FATFileReader *fileReader = (FATFileReader *)MemoryManager::activeMemoryManager->malloc(sizeof(FATFileReader));
    FATFileWriter *fileWriter = (FATFileWriter *)MemoryManager::activeMemoryManager->malloc(sizeof(FATFileWriter));

    new (rootFileEnumerator) FATFileEnumerator(*rootDirectoryEntry, fileReader, fileWriter, NULL, 2);

    this->currentDirectory = rootFileEnumerator;

    this->directoryTraversal = traverseDirectories(
        rootFileEnumerator,
        0);
}

FATFileEnumerator *FATVFS::traverseDirectories(
    FATFileEnumerator *rootFileEnumerator,
    uint8_t level)
{
    // TODO: change hardcoded values to dynamic ones
    uint32_t CLUSTER_SIZE_IN_BYTES = 4096;
    uint32_t SECTOR_SIZE_IN_BYTES = 512;
    uint8_t NUMBER_OF_DIRECTORY_ENTRIES_PER_SECTOR = SECTOR_SIZE_IN_BYTES / sizeof(DirectoryEntryFat32);
    uint32_t CLUSTER_SIZE_IN_DATA_ENTRIES = CLUSTER_SIZE_IN_BYTES / sizeof(DirectoryEntryFat32);
    uint32_t END_OF_CLUSTER_CHAIN = 0x0FFFFFF8;

    // Buffer to store directory entries
    // TODO: fix clusters limitation
    DirectoryEntryFat32 *directoryEntries = (DirectoryEntryFat32 *)MemoryManager::activeMemoryManager->malloc(sizeof(DirectoryEntryFat32) * 256);

    uint32_t nextFileClusterNumber = 0;
    uint32_t currentFileClusterNumber = ((uint32_t)rootFileEnumerator->directoryEntry.firstClusterHi) << 16 |
                                        ((uint32_t)rootFileEnumerator->directoryEntry.firstClusterLow);
    // Stacl allocation, be careful
    uint8_t bufferFAT[513];
    uint32_t clusterAmountRead = 0;

    // File system level
    ++level;

    // -2 is because first 2 clusters are not used in FAT, but in data they are... so cluster number 2 in FAT is 0 in DATA
    uint32_t directoryStartInSectorsDATA = this->startInSectorsDATA + this->sectorPerCluster * (currentFileClusterNumber - 2);

    // Read cluster chain
    while (true)
    {
        clusterAmountRead++;

        // Read whole cluster
        hd->Read28(
            directoryStartInSectorsDATA,
            (uint8_t *)directoryEntries + CLUSTER_SIZE_IN_BYTES * (clusterAmountRead - 1),
            sizeof(DirectoryEntryFat32) * CLUSTER_SIZE_IN_DATA_ENTRIES);

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
        directoryStartInSectorsDATA = startInSectorsDATA + sectorPerCluster * (nextFileClusterNumber - 2);
    }

    uint32_t fileEnumeratorEntriesAmount = 0;
    FATFileEnumerator *fileEnumeratorEntries =
        (FATFileEnumerator *)MemoryManager::activeMemoryManager->malloc(
            sizeof(FATFileEnumerator) * CLUSTER_SIZE_IN_DATA_ENTRIES * clusterAmountRead);

    for (int i = 0; i < CLUSTER_SIZE_IN_DATA_ENTRIES * clusterAmountRead; i++)
    {
        uint8_t attr = directoryEntries[i].attributes;

        uint32_t inode = ((rootFileEnumerator->directoryEntry.firstClusterHi >> 16) |
                          rootFileEnumerator->directoryEntry.firstClusterLow) +
                         i;

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

        if (directoryEntries[i].name[0] == 0xE5)
        {
            // deleted entry, skip
            continue;
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
        {
            DirectoryEntryFat32 *directoryEntry = (DirectoryEntryFat32 *)MemoryManager::activeMemoryManager->malloc(sizeof(DirectoryEntryFat32));
            *directoryEntry = directoryEntries[i];

            FATFileReader *fileReader = (FATFileReader *)MemoryManager::activeMemoryManager->malloc(sizeof(FATFileReader));
            new (fileReader) FATFileReader(hd, directoryEntry, startInSectorsDATA, sectorPerCluster);

            FATFileWriter *fileWriter = (FATFileWriter *)MemoryManager::activeMemoryManager->malloc(sizeof(FATFileWriter));
            new (fileWriter) FATFileWriter(hd, directoryEntry, startInSectorsDATA, sectorPerCluster);

            FATFileEnumerator *fileEnumeratorPtr = fileEnumeratorEntries + fileEnumeratorEntriesAmount;
            new (fileEnumeratorPtr) FATFileEnumerator(*directoryEntry, fileReader, fileWriter, rootFileEnumerator, inode);

            fileEnumeratorEntriesAmount++;
        }

        if (isDirectory)
        {
            DirectoryEntryFat32 *directoryEntry = (DirectoryEntryFat32 *)MemoryManager::activeMemoryManager->malloc(sizeof(DirectoryEntryFat32));
            *directoryEntry = directoryEntries[i];

            FATFileReader *fileReader = (FATFileReader *)MemoryManager::activeMemoryManager->malloc(sizeof(FATFileReader));
            new (fileReader) FATFileReader(hd, directoryEntry, startInSectorsDATA, sectorPerCluster);

            FATFileWriter *fileWriter = (FATFileWriter *)MemoryManager::activeMemoryManager->malloc(sizeof(FATFileWriter));
            new (fileWriter) FATFileWriter(hd, directoryEntry, startInSectorsDATA, sectorPerCluster);

            FATFileEnumerator *fileEnumeratorPtr = fileEnumeratorEntries + fileEnumeratorEntriesAmount;
            new (fileEnumeratorPtr) FATFileEnumerator(*directoryEntry, fileReader, fileWriter, rootFileEnumerator, inode);

            this->traverseDirectories(fileEnumeratorPtr, level);

            fileEnumeratorEntriesAmount++;
        }
    }

    rootFileEnumerator->childrenAmount = fileEnumeratorEntriesAmount;
    rootFileEnumerator->children = fileEnumeratorEntries;

    MemoryManager::activeMemoryManager->free(directoryEntries);

    return rootFileEnumerator;
}

void FATVFS::printDirectoryTraversal()
{
    this->printDirectoryTraversalRecursive(this->directoryTraversal, 1);
}

FATFileEnumerator *FATVFS::getFileByNameInCurrentDirectory(char *name)
{
    uint8_t directoryIndex = 0;
    char fileName[8];
    char fileName2[8];

    for (size_t i = 0; i < this->currentDirectory->childrenAmount; i++)
    {

        // TODO: rework this mess
        bool spaceWasRead = false;
        for (int j = 0; j < 8; j++)
        {
            if (spaceWasRead || this->currentDirectory->children[i].directoryEntry.name[j] == ' ')
            {
                fileName[j] = '\0';
            }
            else
            {
                fileName[j] = this->currentDirectory->children[i].directoryEntry.name[j];

                if (fileName[j] == ' ')
                {
                    spaceWasRead = true;
                }
            }

            fileName2[j] = name[j];
        }
        fileName[7] = '\0';
        fileName2[7] = '\0';

        if (strcmp(fileName, fileName2) == 0)
        {
            directoryIndex = i;
            break;
        }
    }

    return &this->currentDirectory->children[directoryIndex];
}

// TODO: rewrite on file id
void FATVFS::setCurrentDirectory(char *name)
{
    // TODO this .., . should be real directories from FAT32, rework
    if (strcmp("..", name) == 0)
    {
        this->currentDirectory = this->currentDirectory->getParent();
    }
    else if (strcmp(".", name) == 0)
    {
        this->currentDirectory = this->currentDirectory;
    }
    else
    {
        // TODO: rewrite on file id
        this->currentDirectory = this->getFileByNameInCurrentDirectory(name);
    }
}

FATFileEnumerator *FATVFS::getCurrentDirectory()
{
    return this->currentDirectory;
}

void FATVFS::printDirectoryTraversalRecursive(FATFileEnumerator *parentDir, uint32_t level)
{
    for (uint32_t i = 0; i < parentDir->childrenAmount; i++)
    {

        for (int i = 0; i < level; i++)
        {
            printf("   ");
        }

        char *fname = "        ";
        for (int j = 0; j < 8; j++)
        {
            fname[j] = parentDir->children[i].directoryEntry.name[j];
        }
        printf(fname);
        printf("\n");

        if (parentDir->children[i].childrenAmount)
        {
            this->printDirectoryTraversalRecursive(parentDir->children + i, level + 1);
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
