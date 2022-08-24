#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open memory card
    FILE *input = fopen(argv[1], "r");

    if (input == NULL)
    {
        printf("Could not open the file\n");
        return 1;
    }

    // create the buffer to store data from input file statically
    BYTE buffer[512];

    // create the image pointer to trace whether the file status
    FILE *img = NULL;

    // create the char array to store the filename
    char filename [8];

    bool jpgFound = false;
    bool fileOpen = false;
    int fileNo = 0;

    // iterate through the brunch of 512 bytes block in the memory card
    // read 512 bytes into buffer block by block
    while (fread(buffer, 512, 1, input) == 1)
    {
        // check the start of new JPEG in every chunk of 512 bytes block
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            jpgFound = true;

            // check if the jpeg is new and file is open
            if (fileOpen == true)
            {
                fclose(img);
                fileOpen = false;
            }

            // create file name
            sprintf(filename, "%03i.jpg", fileNo);

            // open new jpeg file
            img = fopen(filename, "w");

            //write the data of 512-byte block in the file
            fwrite(buffer, 512, 1, img);
            fileNo++;
            fileOpen = true;

        }
        else
        {
            if (jpgFound == true)
            {
                // **keep writing the data to the file if the jpeg is found
                fwrite(buffer, 512, 1, img);
            }

        }
    }

    // Close all files
    fclose(input);
    fclose(img);
    return 0;

}