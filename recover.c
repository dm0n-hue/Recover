// Implement a program that recovers JPEGs from a forensic image

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;
#define block 512
BYTE buffer[block];
bool first = false;

//JPEG - 0xff 0xd8 0xff
int main(int argc, char *argv[])
{
    // Check usage
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    // Open memory card
    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        return 1;
    }

    //updates file number
    int i = 0;
    //how many characters there are in the file name
    char filename[9];

    //checks if the "new card" can be written to
    sprintf(filename, "%03i.jpg", i);
    FILE *img = fopen(filename, "w");

    if (img == NULL)
    {
        printf("Error in creating photo. Please try again");
        return 1;
    }

    //repeats until the end of the card
    while (0 < fread(buffer, sizeof(BYTE), block, file))
    {
        // Checks first four bytes, then writes JPEG to "new card"
        if (first == false && buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            fwrite(buffer, sizeof(BYTE), block, img);
            first = true;
            continue;
        }

        //writes to "new card" if it's the beginning of a JPEG
        if (first == true && buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            fclose(img);
            i++;
            sprintf(filename, "%03i.jpg", i);
            img = fopen(filename, "w");
            fwrite(buffer, sizeof(BYTE), block, img);
        }
        //if already found a JPEG
        else
        {
            if (first == true)
            {
                fwrite(buffer, sizeof(BYTE), block, img);
            }
        }
    }

    //close the file
    fclose(file);
    return 0;
}
