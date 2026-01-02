#ifndef __MY_OS__VFS_FAT_FILE_ENUMERATOR_H
#define __MY_OS__VFS_FAT_FILE_ENUMERATOR_H

#include <vfs/file-enumerator.hpp>
#include <filesystem/fat-file-writer.hpp>
#include <filesystem/fat-file-reader.hpp>
#include <types.h>
#include <filesystem/fat.hpp>

class FATFileEnumerator : public FileEnumerator
{
public:
    FATFileEnumerator(DirectoryEntryFat32 directoryEntry, FATFileReader *fileReader);
    uint8_t *GetName();
    FileWriter *GetWriter();
    FileReader *GetReader();
    uint32_t GetSize();
    FileEnumerator *Next();
    uint8_t getType(); // 0 - file, 1 - dir
    DirectoryEntryFat32 directoryEntry;
    uint8_t *data;               // pointer to file data
    FATFileEnumerator *children; // if directory there will be list of children
    uint32_t childrenAmount;

private:
    uint32_t startInSectorsFAT;
    uint32_t startInSectorsDATA;
    uint32_t clusterNumberFAT;
    uint32_t clusterNumberOffsetFAT;
    uint32_t fileClusterNumberInDataRegion;
    FATFileReader *fileReader;
};

#endif