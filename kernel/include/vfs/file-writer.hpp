#ifndef __MY_OS__VFS_FILE_WRITER_H
#define __MY_OS__VFS_FILE_WRITER_H
#include <types.h>

class FileWriter
{    
public:
    void virtual write(uint8_t *data);
};

#endif
