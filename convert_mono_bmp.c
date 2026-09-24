//----------------------------------------------
// Convert bmp images to monochrome 1 bit pixel
//----------------------------------------------
#include "lain_mono.h"


#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

// Module Private Types Constants and Macros -----------------------------------
typedef struct BmpFileHeader {
   char bfType[2];
   unsigned int bfSize;
   unsigned short int __bfReserved1;
   unsigned short int __bfReserved2;
   unsigned int bfOffBits;
} __attribute__ ((packed)) BMPFILEHEADER; 


typedef struct BmpImageHeader {
   unsigned int biSize;
   int biWidth;
   int biHeight;
   unsigned short int biPlanes;
   unsigned short int biBitCount;
   unsigned int biCompression;
   unsigned int biSizeImage;
   int biXPelsPerMeter;
   int biYPelPerMeter;
   unsigned int biClrUsed;
   unsigned int biClrImportant;  
} __attribute__ ((packed)) BMPIMAGEHEADER;



// Module Private Functions ----------------------------------------------------
void printFileHeader(BMPFILEHEADER fileHeader)
{
    printf("\nType: %c%c.\n", fileHeader.bfType[0],fileHeader.bfType[1]);
    printf("Size: %d.\n", fileHeader.bfSize);
    printf("Verify (Must be 0 0): %d %d.\n");
    printf("Offset : %d.\n", fileHeader.bfOffBits);
};


void printImageHeader(BMPIMAGEHEADER imageHeader)
{
    printf("\nSize of header: %d.\n", imageHeader.biSize);
    printf("Width: %d.\n", imageHeader.biWidth);
    printf("Height: %d.\n", imageHeader.biHeight);
    printf("Color Planes: %d.\n", imageHeader.biPlanes);
    printf("Bits per Pixel: %d.\n", imageHeader.biBitCount);
    printf("Compression: %d.\n", imageHeader.biCompression);
    printf("Image size: %d.\n", imageHeader.biSizeImage);
    printf("Preferred resolution in pixels per meter (X-Y): %d-%d.\n", imageHeader.biXPelsPerMeter, imageHeader.biYPelPerMeter);
    printf("Number color map: %d.\n", imageHeader.biClrUsed);
    printf("Number of significant colors: %d.\n", imageHeader.biClrImportant);
}

// Globals ---------------------------------------------------------------------


// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{
    FILE *image;
    // BMPFILEHEADER header;
    // BMPIMAGEHEADER imageHeader;

    if (argc != 2)
    {
	printf("please provide the bmp file name\n");
	return 1;
    }

    image = fopen(argv[1], "w");

    if(!image) {
	printf("Could not open the file %s.", argv[1]);
	fclose(image);
	return 1;
    }

    struct BmpFileHeader header;
    header.bfType[0] = 'B';
    header.bfType[1] = 'M';
    header.bfSize = 480 * 320 * 3 + 14;
    header.bfOffBits = 54;

    struct BmpImageHeader imageHeader;
    imageHeader.biSize = 40;
    imageHeader.biWidth = 480;
    imageHeader.biHeight = 320;
    imageHeader.biPlanes = 1;
    imageHeader.biBitCount = 24;
    imageHeader.biCompression = 0;
    imageHeader.biSizeImage = 480 * 320 * 3;
    imageHeader.biXPelsPerMeter = 3780;
    imageHeader.biYPelPerMeter = 3780;
    imageHeader.biClrUsed = 0;
    imageHeader.biClrImportant = 0;
    
    fwrite (&header, 1, sizeof(BMPFILEHEADER), image);
    fwrite (&imageHeader, 1, sizeof(BMPIMAGEHEADER), image);

    unsigned char * ptr = p01;
    unsigned char bgr_buff [3] = { 0 };
    for (int j = 0; j < 320; j++)
    {
	for (int i = 0; i < 60; i++)
	{
	    for (int k = 0; k < 8; k++)
	    {
		unsigned char mask = 0x80;
		mask >>= k;
		if (*(ptr + j * 60 + i) & mask)
		{
		    bgr_buff[0] = 255;
		    bgr_buff[1] = 255;
		    bgr_buff[2] = 255;
		    fwrite (bgr_buff, 1, 3, image);
		}
		else
		{
		    bgr_buff[0] = 0;
		    bgr_buff[1] = 0;
		    bgr_buff[2] = 0;
		    fwrite (bgr_buff, 1, 3, image);		    
		}
	    }
	}
    }

    fclose(image);
    return 0;
}

