#ifndef __MY_OS__VFS_FAT_FILE_READER_H
#define __MY_OS__VFS_FAT_FILE_READER_H

#include <vfs/file-reader.hpp>
#include <filesystem/fat.hpp>

class FATFileReader : public FileReader
{
public:
    FATFileReader(AdvancedTechnologyAttachment *hd, DirectoryEntryFat32 *directoryEntry, uint32_t startInSectorsDATA, uint32_t sectorPerCluster);
    void read();
    DirectoryEntryFat32 *directoryEntry;
    uint32_t startInSectorsDATA;
    uint32_t sectorPerCluster;
    AdvancedTechnologyAttachment *hd;
};

#endif