#ifndef __MY_OS__VFS_FAT_H
#define __MY_OS__VFS_FAT_H

#include <filesystem/fat-directory-traversal.hpp>
#include <vfs/vfs.hpp>
#include <filesystem/fat.hpp>
#include <filesystem/fat-file-enumerator.hpp>

struct PartitionInfo
{
    uint8_t partitionId;
    bool bootable;
    uint32_t partitionOffset;
};

class FATVFS : public VFS
{

public:
    FATVFS(AdvancedTechnologyAttachment *hd);
    void printPartitionInfo();
    void printDirectoryInfo(DirectoryEntryFat32 *directoryEntry, uint8_t level);
    void printFileInfo(DirectoryEntryFat32 *directoryEntry, uint8_t level);
    void printDirectoryTraversal();
    void printDirectoryTraversalRecursive(FATFileEnumerator *parentDir, uint32_t level);
    FATFileEnumerator *traverseDirectories(
        FATFileEnumerator *rootFileEnumerator,
        uint8_t level);
    FATFileEnumerator *directoryTraversal;
    uint8_t partitionAmount;
    PartitionInfo partitionInfo[4];

    AdvancedTechnologyAttachment *hd;
    uint32_t startInSectorsFAT;
    uint32_t startInSectorsDATA;
    uint8_t sectorPerCluster;
};

#endif