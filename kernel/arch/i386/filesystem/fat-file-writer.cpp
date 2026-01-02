#include <filesystem/fat-file-writer.hpp>
#include <kernel/memory-management.hpp>
#include <stdio.h>

FATFileWriter::FATFileWriter(AdvancedTechnologyAttachment *hd, DirectoryEntryFat32 *directoryEntry, uint32_t startInSectorsDATA, uint32_t sectorPerCluster)
{
    this->directoryEntry = directoryEntry;
    this->startInSectorsDATA = startInSectorsDATA;
    this->sectorPerCluster = sectorPerCluster;
    this->hd = hd;
}

void FATFileWriter::write(uint8_t *data, uint32_t dataSize = 512)
{
    int32_t SIZE = directoryEntry->size;

    uint32_t currentClusterNumber = ((uint32_t)directoryEntry->firstClusterHi) << 16 |
                                    ((uint32_t)directoryEntry->firstClusterLow);

    uint8_t FATBuffer[513];
    uint8_t fileBuffer[513];

    // Clear file buffer
    for (int i = 0; i < 513; i++)
    {
        fileBuffer[i] = '\0';
    }

    // Populate file buffer with data
    for (int i = 0; i < dataSize; i++)
    {
        fileBuffer[i] = data[i];
    }
    

    while (SIZE > 0)
    {
        uint32_t currentClusterAddressDATA = startInSectorsDATA + sectorPerCluster * (currentClusterNumber - 2);
        int sectorOffset = 0;

        for (int i = 0; SIZE > 0; SIZE -= 512, i++)
        {
            hd->Write28(currentClusterAddressDATA + sectorOffset, fileBuffer, 512);
            hd->Flush();

            fileBuffer[SIZE > 512 ? 512 : SIZE] = '\0';

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
