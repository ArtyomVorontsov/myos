#include <filesystem/fat-file-reader.hpp>
#include <kernel/memory-management.hpp>
#include <stdio.h>

FATFileReader::FATFileReader(AdvancedTechnologyAttachment *hd, DirectoryEntryFat32 *directoryEntry, uint32_t startInSectorsDATA, uint32_t sectorPerCluster)
{
    this->directoryEntry = directoryEntry;
    this->startInSectorsDATA = startInSectorsDATA;
    this->sectorPerCluster = sectorPerCluster;
    this->hd = hd;
}

void FATFileReader::read()
{
    int32_t SIZE = directoryEntry->size;

    uint32_t currentClusterNumber = ((uint32_t)directoryEntry->firstClusterHi) << 16 |
                                    ((uint32_t)directoryEntry->firstClusterLow);

    uint8_t FATBuffer[513];
    uint8_t fileBuffer[513];

    while (SIZE > 0)
    {
        uint32_t currentClusterAddressDATA = startInSectorsDATA + sectorPerCluster * (currentClusterNumber - 2);
        int sectorOffset = 0;

        for (int i = 0; SIZE > 0; SIZE -= 512, i++)
        {
            hd->Read28(currentClusterAddressDATA + sectorOffset, fileBuffer, 512);

            fileBuffer[SIZE > 512 ? 512 : SIZE] = '\0';
            printf((char *)fileBuffer);

            if (++sectorOffset > sectorPerCluster)
            {
                break;
            }
        }

        // get next cluster
        uint32_t fatSectorForCurrentCluster = currentClusterNumber / 128;
        hd->Read28(fatSectorForCurrentCluster, FATBuffer, 512);
        uint32_t fatSectorOffsetForCurrentCluster = fatSectorForCurrentCluster % 128;
        currentClusterNumber = ((uint32_t *)FATBuffer)[fatSectorOffsetForCurrentCluster] && 0x0FFFFFFF;
    }
}
