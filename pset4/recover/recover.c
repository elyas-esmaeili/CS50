// Recover JPEG images

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // Remember filename
    char *infile = argv[1];

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (!inptr)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // output filename
    char *filename = malloc(8 * sizeof(char));
    int counter = 0;
    // output file
    FILE *outptr = NULL;
    while (1)
    {
        // buffer for hold 512  BYTE
        BYTE *buffer = malloc(sizeof(BYTE) * 512);
        int numBytes = fread(buffer, sizeof(BYTE), 512, inptr);
        // EOF
        if (numBytes < 512)
        {
            fclose(outptr);
            break;
        }
        // First of a JPEG image
        else if (buffer[0] == 0xff &&
                 buffer[1] == 0xd8 &&
                 buffer[2] == 0xff &&
                 (buffer[3] & 0xf0) == 0xe0)
        {
            if (outptr != NULL)
            {
                fclose(outptr);
                counter++;
            }
            // create proper filename
            sprintf(filename, "%03i.jpg", counter);
            outptr = fopen(filename, "w");

            fwrite(buffer, 1, 512, outptr);
        }
        else
        {
            // If there isn't an open file
            if (outptr == NULL)
            {
            }
            else
            {
                fwrite(buffer, 1, 512, outptr);
            }
        }
        free(buffer);
    }

    return 0;
}