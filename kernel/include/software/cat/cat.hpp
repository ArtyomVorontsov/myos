#ifndef __MY_OS__SOFTWARE__CAT
#define __MY_OS__SOFTWARE__CAT

#include <filesystem/fat-vfs.hpp>

class Cat
{
public:
    Cat(FATVFS *fatvfs);

    void run(char *args);

    private:
        FATVFS *fatvfs;
};

#endif