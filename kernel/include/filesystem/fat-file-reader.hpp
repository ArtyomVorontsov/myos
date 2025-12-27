#ifndef __MY_OS__VFS_FAT_FILE_READER_H
#define __MY_OS__VFS_FAT_FILE_READER_H

#include <vfs/file-reader.hpp>

class FATFileReader : public FileReader
{
public:
    char *read();
};

#endif