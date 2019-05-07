# Questions

## What's `stdint.h`?

It'a a header file for fixed width integer types.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

To declare ints with specified width (number of bits).
for example int_32 use 32 bits but unit8_t use 8 bits.
and u in front of unit8_t indicates that this number is unsigned.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

1, 2, 4, 4

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

The first two bytes of BMP files indicates bfType that equals to BM and in hexadecimal 0x4d42.

## What's the difference between `bfSize` and `biSize`?

bfSize is the size, in bytes, of the bitmap file. BUT
biSize is the number of bytes required by the BITMAPINFOHEADER structure.

## What does it mean if `biHeight` is negative?

If biHeight is negative, indicating a top-down DIB and it's origin is the lower-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

You might don't have permissions to open that file.
The file is open in a mode that doesn't allow ohter accesses.

## Why is the third argument to `fread` always `1` in our code?

Because we need only 1 element of size that specified in fread's second argument.

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3

## What does `fseek` do?

fseek is used to move file pointer associated with a given file to a specific position;

## What is `SEEK_CUR`?

It denotes file pointer's current position.
