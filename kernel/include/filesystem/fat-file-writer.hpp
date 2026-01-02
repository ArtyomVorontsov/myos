#ifndef __MY_OS__VFS_FAT_FILE_WRITER_H
#define __MY_OS__VFS_FAT_FILE_WRITER_H

#include <vfs/file-writer.hpp>
#include <filesystem/fat.hpp>

class FATFileWriter : public FileWriter
{

public:
    FATFileWriter(AdvancedTechnologyAttachment *hd, DirectoryEntryFat32 *directoryEntry, uint32_t startInSectorsDATA, uint32_t sectorPerCluster);
    void write(uint8_t *data);
    DirectoryEntryFat32 *directoryEntry;
    uint32_t startInSectorsDATA;
    uint32_t sectorPerCluster;
    AdvancedTechnologyAttachment *hd;
};

#endif