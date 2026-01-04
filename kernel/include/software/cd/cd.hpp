#ifndef __MY_OS__SOFTWARE__CD
#define __MY_OS__SOFTWARE__CD

#include <filesystem/fat-vfs.hpp>

class Cd
{
public:
    Cd(FATVFS *fatvfs);

    void run(char *args);

    private:
        FATVFS *fatvfs;
};

#endif