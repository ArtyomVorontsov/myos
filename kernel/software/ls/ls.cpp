#include <software/ls/ls.hpp>
#include <stdio.h>

Ls::Ls(FATVFS *fatvfs)
{
    this->fatvfs = fatvfs;
}

void Ls::run(char *args)
{
    FATFileEnumerator *currentDirectory = this->fatvfs->getCurrentDirectory();

    for (int i = 0; i < currentDirectory->childrenAmount; i++)
    {
        printf("%s\n", currentDirectory->children[i].directoryEntry.name);
    }

    printf("\n");
}