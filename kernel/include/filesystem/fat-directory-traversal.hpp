#ifndef __MY_OS__VFS_FAT_DIRECTORY_TRAVERSAL_H
#define __MY_OS__VFS_FAT_DIRECTORY_TRAVERSAL_H

#include <vfs/file-enumerator.hpp>
#include <vfs/directory-traversal.hpp>

class FATDirectoryTraversal : public DirectoryTraversal
{
public:
    FileEnumerator *root;
};

#endif