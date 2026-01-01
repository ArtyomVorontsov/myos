#include <filesystem/fat-file-enumerator.hpp>
#include <stdio.h>

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_LFN 0x0F

FATFileEnumerator::FATFileEnumerator(DirectoryEntryFat32 directoryEntry)
{
    this->directoryEntry = directoryEntry;
}

uint8_t *FATFileEnumerator::GetName()
{
    uint8_t *name = this->directoryEntry.name;

    return name;
}

FileReader *FATFileEnumerator::GetReader()
{
}

FileWriter *FATFileEnumerator::GetWriter()
{
}

uint32_t FATFileEnumerator::GetSize()
{
    return this->directoryEntry.size;
}

FileEnumerator *FATFileEnumerator::Next()
{
}

uint8_t FATFileEnumerator::getType()
{

    uint8_t attr = directoryEntry.attributes;

    bool isLFN = (attr == ATTR_LFN);
    bool isDirectory = (attr & ATTR_DIRECTORY);
    bool isVolume = (attr & ATTR_VOLUME_ID);
    bool isFile = (!isLFN &&
                   !isVolume &&
                   !(attr & ATTR_DIRECTORY));

    return isFile ? 0 : 1;
}
