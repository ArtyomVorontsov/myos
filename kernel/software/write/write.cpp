#include <software/write/write.hpp>
#include <stdio.h>

Write::Write(FATVFS *fatvfs)
{
    this->fatvfs = fatvfs;
}

void Write::run(char *arg1, char *arg2)
{
    // TODO: move to lib
    // transform name to uppercase
    for (int i = 0; '\0' != arg1[i]; i++)
    {
        if (arg1[i] >= 'a' && arg1[i] <= 'z')
        {
            arg1[i] = arg1[i] - 32;
        }
        else
        {
            arg1[i] = arg1[i];
        }
    }

    FATFileEnumerator *file = this->fatvfs->getFileByNameInCurrentDirectory(arg1);

    FATFileWriter *fileWriter = file->GetWriter();

    // TODO: fix hardcoded value length to write
    fileWriter->write((uint8_t *)arg2, 20);
}