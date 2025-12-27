#include <filesystem/fat.hpp>
#include <stdio.h>

void ReadBiosBlock(AdvancedTechnologyAttachment *hd, uint32_t partitionOffset)
{
    BiosParameterBlock32 bpb;
    hd->Read28(partitionOffset, (uint8_t *)&bpb, sizeof(BiosParameterBlock32));

    printf("sectors per cluster: ");
    printfHex(bpb.sectorPerCluster);
    printf("\n");

    uint32_t fatStart = partitionOffset + bpb.reservedSectors;
    uint32_t fatSize = bpb.tableSize;

    uint32_t dataStart = fatStart + fatSize * bpb.fatCopies;

    uint32_t rootStart = dataStart + bpb.sectorPerCluster * (bpb.rootCluster - 2);

    DirectoryEntryFat32 dirent[16];

    hd->Read28(rootStart, (uint8_t *)&dirent[0], 16 * sizeof(DirectoryEntryFat32));

    printf("Read files --- \n");
    for (int i = 0; i < 16; i++)
    {

        if (dirent[i].name[0] == 0x00)
        {
            printf("No more files --- ");
            break;
        }

        if ((dirent[i].attributes & 0x0F) == 0x0F)
        {
            // Skip Long File Name record for now
            continue;
        }

        // Print file name
        printf("File name: ");
        char *fname = "        ";
        for (int j = 0; j < 8; j++)
            fname[j] = dirent[i].name[j];

        printf(fname);
        printf("\n");

        // Print file extension
        printf("File extension: ");
        char *ext = "   \n";
        for (int j = 0; j < 3; j++)
            ext[j] = dirent[i].ext[j];

        printf(ext);

        if ((dirent[i].attributes & 0x10) == 0x10)
        {
            printf("Directory --- ");
            continue;
        }

        // Print file content
        printf("File content: ");

        uint32_t firstFileCluster = ((uint32_t)dirent[i].firstClusterHi) << 16 |
                                    ((uint32_t)dirent[i].firstClusterLow);

        int32_t SIZE = dirent[i].size;
        int32_t nextFileCluster = firstFileCluster;
        int32_t amountOfClustersRead = 0;
        uint8_t buffer[513];
        uint8_t fatBuffer[513];

        while (SIZE > 0)
        {
            uint32_t fileSector = dataStart + bpb.sectorPerCluster * (nextFileCluster - 2);
            int sectorOffset = 0;

            amountOfClustersRead++;
            for (; SIZE > 0; SIZE -= 512)
            {
                hd->Read28(fileSector + sectorOffset, buffer, 512);

                buffer[SIZE > 512 ? 512 : SIZE] = '\0';
                printf((char *)buffer);

                if (++sectorOffset > bpb.sectorPerCluster)
                {
                    break;
                }
            }

            uint32_t fatSectorForCurrentCluster = nextFileCluster / (512 / sizeof(uint32_t));
            hd->Read28(fatStart + fatSectorForCurrentCluster, fatBuffer, 512);
            uint32_t fatOffsetInSectorForCurrentCluster = nextFileCluster % (512 / sizeof(uint32_t));
            nextFileCluster = ((uint32_t *)&fatBuffer)[fatOffsetInSectorForCurrentCluster] & 0x0FFFFFFF;
        }

        printf("\n");
        printf("Amount of clusters was read: %d\n", amountOfClustersRead);
    }
}