//-------------------------------------------------------
// Convert bmp images in RGB565 to vector code in RGB 565
//-------------------------------------------------------

#include <string.h>
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
    BMPFILEHEADER header;
    BMPIMAGEHEADER imageHeader;

    if (argc != 3)
    {
	printf("please provide the bmp file name and header to create filename\n");
	return 1;
    }

    image = fopen(argv[1], "rb");

    if(!image) {
	printf("Could not open the file %s.", argv[1]);
	fclose(image);
	return 1;
    }

    printf("bmp file header size: %d\n", sizeof(BMPFILEHEADER));
    unsigned int bytes_cnt = 0;
    unsigned int bytes_cnt_acc = 0;
    bytes_cnt = fread(&header, 1, sizeof(BMPFILEHEADER), image);
   
    printf("File header information bytes: %d", bytes_cnt);
    printFileHeader(header);

    if(header.bfType[0] != 'B' || header.bfType[1] != 'M') {
	printf("The file %s is not a valid BMP.", argv[1]);
	return 1;
    }

    bytes_cnt_acc = bytes_cnt;
    bytes_cnt = fread(&imageHeader, 1, sizeof(BMPIMAGEHEADER), image);
    bytes_cnt_acc += bytes_cnt;
    
    printf("\nImage header information bytes %d total %d:\n", bytes_cnt, bytes_cnt_acc);
    printImageHeader(imageHeader);

    if(imageHeader.biCompression != 3 || imageHeader.biBitCount != 16) {
	printf("The file %s is not a valid BMP 16bit RGB565.\n", argv[1]);
	fclose(image);
	return 1;
    }


    // fclose(image);
    // row size determination
    unsigned int rows_size = ((imageHeader.biWidth * imageHeader.biBitCount + 31) / 32) * 4;
    printf("row size: %d bytes\n", rows_size);
    
    // unsigned int rows_padding = rows_size - imageHeader.biWidth * (imageHeader.biBitCount >> 3);
    // printf("file rows_size: %d rows_padding: %d\n", rows_size, rows_padding);

    if ((imageHeader.biWidth * 2) != rows_size)
    {
	printf("rows padding not supported!!!\n");
	fclose(image);
	return 1;
    }
    
    // create the buffer
    unsigned int buff_size = (imageHeader.biWidth * imageHeader.biHeight) * 2;
    printf("creating a buff565 with %d bytes\n", buff_size);
    unsigned char * buff565 = (unsigned char *) malloc(buff_size);

    // set image to rgb data
    fseek(image, header.bfOffBits ,SEEK_SET);

    // read and load buff
    unsigned int bytes_readed = 0;
    for (int j = imageHeader.biHeight - 1; j >= 0; j--)    
    // for (int j = 0; j < imageHeader.biHeight; j++)
    {
	for (int i = 0; i < rows_size; i++)
	{
	    unsigned char data = 0;
	    fread(&data, 1, 1, image);
	    *(buff565 + j * rows_size + i) = data;
	    bytes_readed++;
	}
    }

    printf("bytes read from file: %d\n", bytes_readed);
    fclose(image);

    // create binary file
    image = fopen(argv[2], "wb");
    if(!image)
    {
	printf("Could not open the file %s.\n", argv[2]);
	fclose(image);
	free(buff565);
	return 1;
    }

    // binary file creation version with bytes swapped
    char strbuff [100] = { 0 };
    for (unsigned int j = 0; j < imageHeader.biHeight; j++)
    {
	// rows
	for (unsigned int i = 0; i < rows_size; i+=2)
	{
	    fwrite((buff565 + j * rows_size + i + 1), 1, 1, image);
	    fwrite((buff565 + j * rows_size + i + 0), 1, 1, image);
	}
    }

    printf("%s bin file created size: %d\n", argv[2], ftell(image));
    fclose(image);
    free(buff565);
    // end of file creation version with bytes swapped
    
    return 0;
}

