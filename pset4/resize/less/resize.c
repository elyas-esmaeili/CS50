// Resize a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    int scale = atoi(argv[1]);
    if (scale <= 0 || scale > 100)
    {
        fprintf(stderr, "n must be a positive integer less"
                "than or equal to 100.\n");
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
    bi.biWidth *= scale;
    bi.biHeight *= scale;
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
    for (int i = 0; i < inHeight; i++)
    {
        // Scale horizontally
        // Creat an array that keep each pixel(RGBTRIPLE) scale times
        RGBTRIPLE *pixelArray = malloc(sizeof(RGBTRIPLE) *
                                       inWidth * scale);
        int counter = 0;
        for (int j = 0; j < inWidth; j++)
        {
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            for (int k = 0; k < scale; k++)
            {
                pixelArray[counter] = triple;
                counter++;
            }
        }

        // Scale vertically
        // Write each element of pixelArry to outfile
        for (int j = 0; j < scale; j++)
        {
            counter = 0;
            for (int k = 0, n = inWidth * scale; k < n; k++)
            {
                fwrite(&pixelArray[counter], sizeof(RGBTRIPLE), 1, outptr);
                counter++;
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
