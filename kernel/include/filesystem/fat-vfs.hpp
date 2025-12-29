#ifndef __MY_OS__VFS_FAT_H
#define __MY_OS__VFS_FAT_H

#include <filesystem/fat-directory-traversal.hpp>
#include <vfs/vfs.hpp>
#include <filesystem/fat.hpp>

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
    void traverseDirectories(
        AdvancedTechnologyAttachment *hd,
        uint32_t startInSectorsFAT,
        uint32_t startInSectorsDATA,
        uint32_t directoryClusterNumberFAT,
        uint32_t directoryStartInSectorsDATA,
        uint8_t sectorsPerCluster,
        uint8_t level);
    FATDirectoryTraversal directoryTraversal;
    uint8_t partitionAmount;
    PartitionInfo partitionInfo[4];
};

#endif