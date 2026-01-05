#include <software/pwd/pwd.hpp>
#include <stdio.h>

Pwd::Pwd(FATVFS *fatvfs)
{
    this->fatvfs = fatvfs;
}

void Pwd::run(char *args)
{
    FATFileEnumerator *currentDirectory = this->fatvfs->getCurrentDirectory();
    uint8_t pathReversed[128] = {'\0'};
    uint8_t path[128] = {'\0'};
    uint8_t pathReversedIndex = 1;
    uint8_t pathIndex = 1;

    // root
    pathReversed[0] = '/';
    path[0] = '/';

    while (currentDirectory->inode != 2)
    {
        uint8_t *name = currentDirectory->GetName();

        for (int i = 0; i < 8; i++)
        {
            if (name[i] != ' ')
            {
                pathReversed[pathReversedIndex++] = name[i];
            }
        }
        pathReversed[pathReversedIndex++] = '/';

        currentDirectory = currentDirectory->getParent();
    }

    pathReversedIndex--;
    while (pathReversedIndex--)
    {
        if (pathReversed[pathReversedIndex] == '/')
        {
            for (int i = pathReversedIndex + 1;; i++)
            {
                path[pathIndex] = pathReversed[i];
                pathIndex++;

                if (pathReversed[i] == '/')
                {
                    break;
                }
            }
        }
    }

    printf("%s\n", path);
}
