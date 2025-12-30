#ifndef __MY_OS__VFS_FILE_ENUMERATOR_H
#define __MY_OS__VFS_FILE_ENUMERATOR_H

#include <vfs/file-reader.hpp>
#include <vfs/file-writer.hpp>
#include <types.h>

class FileEnumerator
{
public:
    virtual uint8_t *GetName();
    virtual FileReader *GetReader();
    virtual FileWriter *GetWriter();
    virtual uint32_t GetSize();
    virtual FileEnumerator *Next();
    virtual uint8_t getType(); // 0 - file, 1 - dir
};

#endif