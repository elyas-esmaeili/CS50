// Resize a BMP file

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    float scale = atof(argv[1]);
    if (scale <= 0.0 || scale > 100.0)
    {
        fprintf(stderr, "f must be a floating-point value"
                "in (0.0, 100.0]\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Copy infile's biWidth & biHeight
    LONG inWidth = bi.biWidth;
    LONG inHeight = bi.biHeight;
    // determine padding for infile's scanlines
    int inPadding = (4 - (inWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Update BITMAPINFOHEADER for outfile
    bi.biWidth = floor(bi.biWidth * scale);
    bi.biHeight = floor(bi.biHeight * scale);
    // determine padding for outfile's scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding)  * abs(bi.biHeight);
    // Update BITMAPFILEHEADER
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) +
                sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    inHeight = abs(inHeight);
    int scanline = 0;
    for (int i = 0; i < inHeight; i++)
    {
        // Declare array that keeps scanline's RGBTRIPLE
        RGBTRIPLE *pixelArray = malloc(sizeof(RGBTRIPLE) * inWidth);
        for (int j = 0; j < inWidth; j++)
        {
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            pixelArray[j] = triple;
        }

        // counter used for scale vertically
        int counter = 0;
        while (1)
        {
            int temp = floor((float)scanline * ((float)inHeight / (float)abs(bi.biHeight)));
            if (temp == i)
            {
                counter++;
                scanline++;
            }
            else
            {
                break;
            }
        }

        // Scale vertically
        for (int j = 0; j < counter; j++)
        {
            // Scale horizontally
            for (int k = 0; k < bi.biWidth; k++)
            {
                int index = floor((float)k * ((float)inWidth / (float)bi.biWidth));
                fwrite(&pixelArray[index], sizeof(RGBTRIPLE), 1, outptr);
            }

            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
        }

        // skip over padding in infile
        fseek(inptr, inPadding, SEEK_CUR);
        free(pixelArray);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
