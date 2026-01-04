#include <software/cd/cd.hpp>
#include <stdio.h>

Cd::Cd(FATVFS *fatvfs)
{
    this->fatvfs = fatvfs;
}

void Cd::run(char *args)
{
    // TODO: move to lib
    // transform name to uppercase
    for (int i = 0; '\0' != args[i]; i++)
    {
        if (args[i] >= 'a' && args[i] <= 'z')
        {
            args[i] = args[i] - 32;
        }
        else
        {
            args[i] = args[i];
        }
    }

    this->fatvfs->setCurrentDirectory(args);

    printf("\n");
}