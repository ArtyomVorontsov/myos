#ifndef __MY_OS__VFS_H
#define __MY_OS__VFS_H

#include <vfs/directory-traversal.hpp>
#include <driver/ata.hpp>
#include <types.h>

class VFS
{
public:
    DirectoryTraversal directoryTraversal;
};


#endif