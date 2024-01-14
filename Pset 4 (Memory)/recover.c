#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");

    // Create a buffer for a block of data
    uint8_t buffer[512];

    // Initialise variables
    int counter = 0;
    char filename[8];
    FILE *img = NULL;

    // While there's still data left to read from the memory card
    // We are reading from <card> <512> counts of <1> byte and storing in <buffer>
    while (fread(buffer, 1, 512, card) == 512)
    {
        // If we find the start of a new JPEG
        // use a "bitwise AND" - this sets the last four bits of buffer[3] to 0
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if there's a previous img, close it
            if (img != NULL)
            {
                fclose(img);
            }

            // put new image title in filename array
            sprintf(filename, "%03i.jpg", counter);

            // open new image
            img = fopen(filename, "w");
            if (img != NULL)
            {
                fwrite(buffer, 1, 512, img);
                counter++;
            }
        }

        // If it's not the start of a new JPEG
        // Keep writing into current file
        else
        {
            if (img != NULL)
            {
                fwrite(buffer, 1, 512, img);
            }
        }
    }

    // Close any open image
    if (img != NULL)
    {
        fclose(img);
    }

    // Close card
    fclose(card);
}