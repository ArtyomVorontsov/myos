#ifndef __MY_OS__SOFTWARE__LS
#define __MY_OS__SOFTWARE__LS

#include <filesystem/fat-vfs.hpp>

class Ls
{
public:
    Ls(FATVFS *fatvfs);

    void run(char *args);

    private:
        FATVFS *fatvfs;
};

#endif