#include <software/cat/cat.hpp>
#include <stdio.h>

Cat::Cat(FATVFS *fatvfs)
{
    this->fatvfs = fatvfs;
}

void Cat::run(char *args)
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

    FATFileEnumerator *file = this->fatvfs->getFileByNameInCurrentDirectory(args);

    FATFileReader *fileReader = file->GetReader();

    fileReader->read();

    printf("\n");
}