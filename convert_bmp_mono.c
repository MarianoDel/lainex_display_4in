//----------------------------------------------
// Convert bmp images to monochrome 1 bit pixel
//----------------------------------------------

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
	printf("The file %s is not a valid BMP.", "test.bmp");
	return 1;
    }

    bytes_cnt_acc = bytes_cnt;
    bytes_cnt = fread(&imageHeader, 1, sizeof(BMPIMAGEHEADER), image);
    bytes_cnt_acc += bytes_cnt;
    
    printf("\nImage header information bytes %d total %d:\n", bytes_cnt, bytes_cnt_acc);
    printImageHeader(imageHeader);

    if(imageHeader.biSize != 40 || imageHeader.biCompression != 0 || imageHeader.biBitCount != 24) {
	printf("The file %s is not a valid BMP.", "test.bmp");
	fclose(image);
	return 1;
    }

    unsigned int rows_size = ((imageHeader.biWidth * imageHeader.biBitCount + 31) / 32) * 4;
    unsigned int rows_padding = rows_size - imageHeader.biWidth * (imageHeader.biBitCount >> 3);
    printf("file rows_size: %d rows_padding: %d\n", rows_size, rows_padding);

    // convert pixel data to monochrome data 24bits BGR -> 1bit
    // create the buffer
    unsigned int buff_mono_size = (imageHeader.biWidth * imageHeader.biHeight) >> 3;
    printf("creating a buff_mono with %d bytes\n", buff_mono_size);
    unsigned char * buff_mono = (unsigned char *) malloc(buff_mono_size);

    if (rows_padding)
    {
	printf("padding not supported\n");
	fclose(image);
	free(buff_mono);
	return 1;
    }

    unsigned int width = imageHeader.biWidth >> 3;
    // columns
    for (unsigned int j = 0; j < imageHeader.biHeight; j++)
    {
	unsigned char bit_cnt = 0;
	unsigned char bit_data = 0;
	unsigned char bgr_buff [3] = { 0 };
	unsigned char byte_cnt = 0;
	unsigned char data = 0;

	// rows
	for (unsigned int i = 0; i < imageHeader.biWidth; i++)
	{
	    // pixel bytes
	    for (unsigned int k = 0; k < 3; k++)
	    {
		fread(&data, 1, 1, image);
		bgr_buff[k] = data;
	    }

	    if ((bgr_buff[0] > 127) && (bgr_buff[1] > 127) && (bgr_buff[2] > 127))
	    {
		// printf("data ");
		data = 1;
		// if (j > 300)
		// {
		// 	printf("row: %d col: %d B: %d G: %d R: %d\n", j, i >> 3,
		// 	       bgr_buff[0], bgr_buff[1], bgr_buff[2]);
		// }
	    }
	    else
		data = 0;

	    if (bit_cnt < 8 - 1)
	    {
		bit_data |= (data << (7 - bit_cnt));
		bit_cnt++;
	    }
	    else
	    {
		bit_data |= data;
		unsigned int offset = j * width;
		// printf("offset: %d j: %d bcnt: %d\n", offset, j, byte_cnt);
		*(buff_mono + offset + byte_cnt) = bit_data;
		// if (bit_data)
		// 	printf("%d r: %d i: %d\n", bit_data, r, i);
		bit_data = 0;
		bit_cnt = 0;
		byte_cnt++;
	    }
	}
    }

    // with old counters
    // for (unsigned int j = 0; j < imageHeader.biHeight; j++)
    // {
    // 	unsigned char bit_cnt = 0;
    // 	unsigned char bit_data = 0;
    // 	unsigned char bgr_cnt = 0;
    // 	unsigned char bgr_buff [3] = { 0 };
    // 	unsigned char byte_cnt = 0;
	
    // 	// rows
    // 	for (unsigned int i = 0; i < rows_size; i++)
    // 	{
    // 	    unsigned char data = 0;
    // 	    fread(&data, 1, 1, image);

    // 	    //BGR cnt
    // 	    bgr_buff[bgr_cnt] = data;
    // 	    if (bgr_cnt < 3 - 1)
    // 		bgr_cnt++;
    // 	    else
    // 	    {
    // 		if ((bgr_buff[0] > 127) && (bgr_buff[1] > 127) && (bgr_buff[2] > 127))
    // 		{
    // 		    // printf("data ");
    // 		    data = 1;
    // 		    // if (j > 300)
    // 		    // {
    // 		    // 	printf("row: %d col: %d B: %d G: %d R: %d\n", j, i >> 3,
    // 		    // 	       bgr_buff[0], bgr_buff[1], bgr_buff[2]);
    // 		    // }
    // 		}
    // 		else
    // 		    data = 0;

    // 		if (bit_cnt < 8 - 1)
    // 		{
    // 		    bit_data |= (data << bit_cnt);
    // 		    bit_cnt++;
    // 		}
    // 		else
    // 		{
    // 		    bit_data |= data;
    // 		    unsigned int offset = j * width;
    // 		    // printf("offset: %d j: %d bcnt: %d\n", offset, j, byte_cnt);
    // 		    *(buff_mono + offset + byte_cnt) = bit_data;
    // 		    // if (bit_data)
    // 		    // 	printf("%d r: %d i: %d\n", bit_data, r, i);
    // 		    bit_data = 0;
    // 		    bit_cnt = 0;
    // 		    byte_cnt++;
    // 		}
		
    // 		bgr_cnt = 0;
    // 	    }
    // 	}
    // }
    // end of old counters
    
    fclose(image);

    // create header file
    image = fopen(argv[2], "w");
    if(!image)
    {
	printf("Could not open the file %s.", argv[2]);
	fclose(image);
	free(buff_mono);
	return 1;
    }

    // file creation version
    char strbuff [100] = { 0 };
    fwrite ("unsigned char p01 [] = {\n", 1, sizeof("unsigned char p01 [] = {\n") - 1, image);
    for (unsigned int j = 0; j < imageHeader.biHeight; j++)
    {
	// rows
	for (unsigned int i = 0; i < width; i++)
	{
	    sprintf(strbuff, "0x%02x,", *(buff_mono + j * width + i));
	    fwrite(strbuff, 1, strlen(strbuff), image);
	}
	fwrite("\n", 1, sizeof("\n") - 1, image);
    }
    fwrite("};\n", 1, sizeof("};\n") - 1, image);    
    fclose(image);
    // end of file creation version
    
    // console print version
    // printf("\n{\n");
    // // buff_mono filed
    // for (unsigned int j = 0; j < imageHeader.biHeight; j++)
    // {
    // 	// rows
    // 	for (unsigned int i = 0; i < width; i++)
    // 	{
    // 	    printf("0x%02x,", *(buff_mono + j * width + i));
    // 	}
    // 	printf("\n");
    // }
    // printf("}\n");
    // end of console print version


    free(buff_mono);
    return 0;
}

