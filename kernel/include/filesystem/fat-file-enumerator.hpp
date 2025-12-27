#ifndef __MY_OS__VFS_FAT_FILE_ENUMERATOR_H
#define __MY_OS__VFS_FAT_FILE_ENUMERATOR_H

#include <vfs/file-enumerator.hpp>
#include <filesystem/fat-file-reader.hpp>
#include <filesystem/fat-file-writer.hpp>
#include <types.h>

class FATFileEnumerator : public FileEnumerator
{
public:
    char *GetName();
    FATFileReader *GetReader();
    FATFileWriter *GetWriter();
    int GetSize();
    FATFileEnumerator *Next();
    uint8_t getType(); // 0 - file, 1 - dir
};

#endif