#ifndef __MY_OS__VFS_DIRECTORY_TRAVERSAL_H
#define __MY_OS__VFS_DIRECTORY_TRAVERSAL_H

#include <vfs/file-enumerator.hpp>


class DirectoryTraversal
{
public:
    FileEnumerator *root;
};

#endif