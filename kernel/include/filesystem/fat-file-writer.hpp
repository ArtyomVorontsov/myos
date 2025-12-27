#ifndef __MY_OS__VFS_FAT_FILE_WRITER_H
#define __MY_OS__VFS_FAT_FILE_WRITER_H

#include <vfs/file-writer.hpp>

class FATFileWriter : public FileWriter
{
public:
    void write(char *data);
};

#endif