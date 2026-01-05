#ifndef __MY_OS__SOFTWARE__PWD
#define __MY_OS__SOFTWARE__PWD

#include <filesystem/fat-vfs.hpp>

class Pwd
{
public:
    Pwd(FATVFS *fatvfs);

    void run(char *args);

    private:
        FATVFS *fatvfs;
};

#endif