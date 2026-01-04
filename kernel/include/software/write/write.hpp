#ifndef __MY_OS__SOFTWARE__WRITE
#define __MY_OS__SOFTWARE__WRITE

#include <filesystem/fat-vfs.hpp>

class Write
{
public:
    Write(FATVFS *fatvfs);

    void run(char *arg1, char *arg2);

    private:
        FATVFS *fatvfs;
};

#endif