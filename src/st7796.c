// ST7796-ILI9488 Driver library for STM32

// Includes --------------------------------------------------------------------
#include "st7796.h"
#include "spi.h"
#include "gpio.h"
#include "tim.h"
// #include "main.h"
// #include "stm32f4xx.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// #include "5x5_font.h"
#include "glcdfont.h"


// Global Variables ------------------------------------------------------------
volatile uint16_t LCD_HEIGHT = ST7796_SCREEN_HEIGHT;
volatile uint16_t LCD_WIDTH	= ST7796_SCREEN_WIDTH;

/* Initialize SPI */
void ST7796_SPI_Init(void)
{	
    LCD_CS_ON;	//CS OFF
}

/*Send data (char) to LCD*/
void ST7796_SPI_Send(unsigned char SPI_Data)
{
    SPI1_Send_Array(&SPI_Data, 1);
}

/* Send command (char) to LCD */
void ST7796_Write_Command(uint8_t Command)
{
    LCD_RS_CMD;
    LCD_CS_ON;
    ST7796_SPI_Send(Command);
    LCD_CS_OFF;	
}

/* Send Data (char) to LCD */
void ST7796_Write_Data(uint8_t Data)
{
    LCD_RS_DATA;	
    LCD_CS_ON;
    ST7796_SPI_Send(Data);	
    LCD_CS_OFF;
}

/* Set Address - Location block - to draw into */
void ST7796_Set_Address(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{
    ST7796_Write_Command(0x2A);
    ST7796_Write_Data(X1>>8);
    ST7796_Write_Data(X1);
    ST7796_Write_Data(X2>>8);
    ST7796_Write_Data(X2);

    ST7796_Write_Command(0x2B);
    ST7796_Write_Data(Y1>>8);
    ST7796_Write_Data(Y1);
    ST7796_Write_Data(Y2>>8);
    ST7796_Write_Data(Y2);

    ST7796_Write_Command(0x2C);
}

/* Set Address - Location block - to draw into DMA */
void LCD_Send_Data_DMA(uint16_t x, uint16_t y, uint16_t  x_end, uint16_t  y_end, uint8_t  *p)
{ 
    ST7796_Set_Address(x, y, x_end, y_end);
    LCD_CS_ON;
    LCD_RS_DATA;	
    // if ( HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)p, (y_end - y + 1) * (x_end - x + 1) * 2) != HAL_OK)
    // {
    // 	//  while(1);	/*Halt on error*/
    // }	
}

/*HARDWARE RESET*/
void ST7796_Reset(void)
{
    LCD_RST_ON;
    Wait_ms(200);
    // LCD_CS_ON;
    // Wait_ms(200);
    LCD_RST_OFF;
}

/*Ser rotation of the screen - changes x0 and y0*/
void ST7796_Set_Rotation(uint8_t Rotation) 
{
    uint8_t screen_rotation = Rotation;
    ST7796_Write_Command(0x36);
    Wait_ms(1);
    switch(screen_rotation) 
    {
    case SCREEN_VERTICAL_1:
	ST7796_Write_Data(0x40|0x08);
	LCD_WIDTH = 320;
	LCD_HEIGHT = 480;
	break;
    case SCREEN_HORIZONTAL_1:
	ST7796_Write_Data(0x20|0x08);
	LCD_WIDTH  = 480;
	LCD_HEIGHT = 320;
	break;
    case SCREEN_VERTICAL_2:
	ST7796_Write_Data(0x80|0x08);
	LCD_WIDTH  = 320;
	LCD_HEIGHT = 480;
	break;
    case SCREEN_HORIZONTAL_2:
	ST7796_Write_Data(0x40|0x80|0x20|0x08);
	LCD_WIDTH  = 480;
	LCD_HEIGHT = 320;
	break;
    default:
//EXIT IF SCREEN ROTATION NOT VALID!
	break;
    }
}

/*Enable LCD display*/
void ST7796_Enable(void)
{
    LCD_RST_OFF;
    Wait_ms(100);	
    LCD_RST_ON;	
    Wait_ms(100);	
    LCD_RST_OFF;	
}

/*Initialize LCD display*/
void ST7796_Init(void)
{

    ST7796_Enable();
    ST7796_SPI_Init();
    // ST7796_Reset();

//SOFTWARE RESET
    ST7796_Write_Command(0x01);
    Wait_ms(1000);
//POWER CONTROL A
    ST7796_Write_Command(0xCB);
    ST7796_Write_Data(0x39);
    ST7796_Write_Data(0x2C);
    ST7796_Write_Data(0x00);
    ST7796_Write_Data(0x34);
    ST7796_Write_Data(0x02);

//POWER CONTROL B
    ST7796_Write_Command(0xCF);
    ST7796_Write_Data(0x00);
    ST7796_Write_Data(0xC1);
    ST7796_Write_Data(0x30);

//DRIVER TIMING CONTROL A
    ST7796_Write_Command(0xE8);
    ST7796_Write_Data(0x85);
    ST7796_Write_Data(0x00);
    ST7796_Write_Data(0x78);

//DRIVER TIMING CONTROL B
    ST7796_Write_Command(0xEA);
    ST7796_Write_Data(0x00);
    ST7796_Write_Data(0x00);

//POWER ON SEQUENCE CONTROL
    ST7796_Write_Command(0xED);
    ST7796_Write_Data(0x64);
    ST7796_Write_Data(0x03);
    ST7796_Write_Data(0x12);
    ST7796_Write_Data(0x81);

//PUMP RATIO CONTROL
    ST7796_Write_Command(0xF7);
    ST7796_Write_Data(0x20);

//POWER CONTROL,VRH[5:0]
    ST7796_Write_Command(0xC0);
    ST7796_Write_Data(0x23);

//POWER CONTROL,SAP[2:0];BT[3:0]
    ST7796_Write_Command(0xC1);
    ST7796_Write_Data(0x10);

//VCM CONTROL
    ST7796_Write_Command(0xC5);
    ST7796_Write_Data(0x3E);
    ST7796_Write_Data(0x28);

//VCM CONTROL 2
    ST7796_Write_Command(0xC7);
    ST7796_Write_Data(0x86);

//MEMORY ACCESS CONTROL
    ST7796_Write_Command(0x36);
    ST7796_Write_Data(0x48);

//PIXEL FORMAT
    ST7796_Write_Command(0x3A);
    ST7796_Write_Data(0x55);

//FRAME RATIO CONTROL, STANDARD RGB COLOR
    ST7796_Write_Command(0xB1);
    ST7796_Write_Data(0x00);
    ST7796_Write_Data(0x18);

//DISPLAY FUNCTION CONTROL
    ST7796_Write_Command(0xB6);
    ST7796_Write_Data(0x08);
    ST7796_Write_Data(0x82);
    ST7796_Write_Data(0x27);

//3GAMMA FUNCTION DISABLE
    ST7796_Write_Command(0xF2);
    ST7796_Write_Data(0x00);

//GAMMA CURVE SELECTED
    ST7796_Write_Command(0x26);
    ST7796_Write_Data(0x01);

//POSITIVE GAMMA CORRECTION
    ST7796_Write_Command(0xE0);
    ST7796_Write_Data(0x0F);
    ST7796_Write_Data(0x31);
    ST7796_Write_Data(0x2B);
    ST7796_Write_Data(0x0C);
    ST7796_Write_Data(0x0E);
    ST7796_Write_Data(0x08);
    ST7796_Write_Data(0x4E);
    ST7796_Write_Data(0xF1);
    ST7796_Write_Data(0x37);
    ST7796_Write_Data(0x07);
    ST7796_Write_Data(0x10);
    ST7796_Write_Data(0x03);
    ST7796_Write_Data(0x0E);
    ST7796_Write_Data(0x09);
    ST7796_Write_Data(0x00);

//NEGATIVE GAMMA CORRECTION
    ST7796_Write_Command(0xE1);
    ST7796_Write_Data(0x00);
    ST7796_Write_Data(0x0E);
    ST7796_Write_Data(0x14);
    ST7796_Write_Data(0x03);
    ST7796_Write_Data(0x11);
    ST7796_Write_Data(0x07);
    ST7796_Write_Data(0x31);
    ST7796_Write_Data(0xC1);
    ST7796_Write_Data(0x48);
    ST7796_Write_Data(0x08);
    ST7796_Write_Data(0x0F);
    ST7796_Write_Data(0x0C);
    ST7796_Write_Data(0x31);
    ST7796_Write_Data(0x36);
    ST7796_Write_Data(0x0F);

//EXIT SLEEP
    ST7796_Write_Command(0x11);
    Wait_ms(120);

//TURN ON DISPLAY
    ST7796_Write_Command(0x29);

//STARTING ROTATION
    ST7796_Set_Rotation(SCREEN_VERTICAL_1);
}


#define LCD_WR_REG(X)    ST7796_Write_Command(X)
#define LCD_WR_DATA(X)    ST7796_Write_Data(X)
void ST7796_Init2(void)
{

    ST7796_Enable();
    ST7796_SPI_Init();
    // ST7796_Reset();

// //SOFTWARE RESET
//     ST7796_Write_Command(0x01);
//     Wait_ms(1000);

    // new init ---------------------
    // LCD_RESET(); //LCD
    ST7796_Reset();
    
//*************4.0 ST7796S TN**********//	
    LCD_WR_REG(0xF0);
    LCD_WR_DATA(0xC3);
    LCD_WR_REG(0xF0);
    LCD_WR_DATA(0x96);
    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x48);	
    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x05);	
    LCD_WR_REG(0xB0);
    LCD_WR_DATA(0x80);	
    LCD_WR_REG(0xB6);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x02);	
    LCD_WR_REG(0xB5);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x04);
    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0x80);	
    LCD_WR_DATA(0x10);	
    LCD_WR_REG(0xB4);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0xB7);
    LCD_WR_DATA(0xC6);
    LCD_WR_REG(0xC5);
    LCD_WR_DATA(0x1C);
    LCD_WR_REG(0xE4);
    LCD_WR_DATA(0x31);
    LCD_WR_REG(0xE8);
    LCD_WR_DATA(0x40);
    LCD_WR_DATA(0x8A);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x19);
    LCD_WR_DATA(0xA5);
    LCD_WR_DATA(0x33);
    LCD_WR_REG(0xC2);
    LCD_WR_REG(0xA7);
    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0xF0);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0x2B);
    LCD_WR_DATA(0x3C);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x4B);
    LCD_WR_DATA(0x1B);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x1D);
    LCD_WR_DATA(0x21);

    LCD_WR_REG(0XE1);
    LCD_WR_DATA(0xF0);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x0D);
    LCD_WR_DATA(0x27);
    LCD_WR_DATA(0x3B);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x4D);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x1D);
    LCD_WR_DATA(0x21);

    LCD_WR_REG(0xF0);
    LCD_WR_DATA(0x3C);
    LCD_WR_REG(0xF0);
    LCD_WR_DATA(0x69);
    LCD_WR_REG(0X13);
    LCD_WR_REG(0X11);
    LCD_WR_REG(0X29);

    // LCD_direction(USE_HORIZONTAL);    
//STARTING ROTATION
    ST7796_Set_Rotation(SCREEN_HORIZONTAL_2);
}


#define writecommand(X)    ST7796_Write_Command(X)
#define writedata(X)    ST7796_Write_Data(X)
void ST7796_Init3(void)
{

    ST7796_Enable();
    ST7796_SPI_Init();
    // ST7796_Reset();

// //SOFTWARE RESET
//     ST7796_Write_Command(0x01);
//     Wait_ms(1000);

    // new init ---------------------
    // LCD_RESET(); //LCD
    ST7796_Reset();
    
    Wait_ms(120);

    writecommand(0x01); //Software reset
    Wait_ms(120);

    writecommand(0x11); //Sleep exit                                            
    Wait_ms(120);

    writecommand(0xF0); //Command Set control                                 
    writedata(0xC3);    //Enable extension command 2 partI
    writecommand(0xF0); //Command Set control                                 
    writedata(0x96);    //Enable extension command 2 partII
    writecommand(0x36); //Memory Data Access Control MX, MY, RGB mode                                    
    writedata(0x48);    //X-Mirror, Top-Left to right-Buttom, RGB  
    writecommand(0x3A); //Interface Pixel Format                                    
    writedata(0x55);    //Control interface color format set to 16
    writecommand(0xB4); //Column inversion 
    writedata(0x01);    //1-dot inversion

    writecommand(0xB6); //Display Function Control
    writedata(0x80);    //Bypass
    writedata(0x02);    //Source Output Scan from S1 to S960, Gate Output scan from G1 to G480, scan cycle=2
    writedata(0x3B);    //LCD Drive Line=8*(59+1)


    writecommand(0xE8); //Display Output Ctrl Adjust
    writedata(0x40);
    writedata(0x8A);	
    writedata(0x00);
    writedata(0x00);
    writedata(0x29);    //Source eqaulizing period time= 22.5 us
    writedata(0x19);    //Timing for "Gate start"=25 (Tclk)
    writedata(0xA5);    //Timing for "Gate End"=37 (Tclk), Gate driver EQ function ON
    writedata(0x33);
    writecommand(0xC1); //Power control2                          
    writedata(0x06);    //VAP(GVDD)=3.85+( vcom+vcom offset), VAN(GVCL)=-3.85+( vcom+vcom offset)
 
    writecommand(0xC2); //Power control 3                                      
    writedata(0xA7);    //Source driving current level=low, Gamma driving current level=High
 
    writecommand(0xC5); //VCOM Control
    writedata(0x18);    //VCOM=0.9

    Wait_ms(120);
//ST7796 Gamma Sequence
    writecommand(0xE0); //Gamma"+"                                             
    writedata(0xF0);
    writedata(0x09); 
    writedata(0x0b);
    writedata(0x06); 
    writedata(0x04);
    writedata(0x15); 
    writedata(0x2F);
    writedata(0x54); 
    writedata(0x42);
    writedata(0x3C); 
    writedata(0x17);
    writedata(0x14);
    writedata(0x18); 
    writedata(0x1B); 
 
    writecommand(0xE1); //Gamma"-"                                             
    writedata(0xE0);
    writedata(0x09); 
    writedata(0x0B);
    writedata(0x06); 
    writedata(0x04);
    writedata(0x03); 
    writedata(0x2B);
    writedata(0x43); 
    writedata(0x42);
    writedata(0x3B); 
    writedata(0x16);
    writedata(0x14);
    writedata(0x17); 
    writedata(0x1B);

    Wait_ms(120);
    writecommand(0xF0); //Command Set control                                 
    writedata(0x3C);    //Disable extension command 2 partI

    writecommand(0xF0); //Command Set control                                 
    writedata(0x69);    //Disable extension command 2 partII

    // end_tft_write();
    Wait_ms(120);
    // begin_tft_write();

    writecommand(0x29); //Display on                          
    // LCD_direction(USE_HORIZONTAL);    
//STARTING ROTATION
    // ST7796_Set_Rotation(SCREEN_HORIZONTAL_1);
    ST7796_Set_Rotation(SCREEN_HORIZONTAL_2);    
}


//INTERNAL FUNCTION OF LIBRARY, USAGE NOT RECOMENDED, USE Draw_Pixel INSTEAD
/*Sends single pixel colour information to LCD*/
void ST7796_Draw_Colour(uint16_t Colour)
{
//SENDS COLOUR
    unsigned char TempBuffer[2] = {Colour>>8, Colour};	
    LCD_RS_DATA;	
    LCD_CS_ON;
    SPI1_Send_Array(TempBuffer, 2);
    LCD_CS_OFF;
}

//INTERNAL FUNCTION OF LIBRARY
/*Sends block colour information to LCD*/
void ST7796_Draw_Colour_Burst(uint16_t Colour, uint32_t Size)
{
//SENDS COLOUR
    uint32_t Buffer_Size = 0;
    if((Size*2) < BURST_MAX_SIZE)
    {
	Buffer_Size = Size;
    }
    else
    {
	Buffer_Size = BURST_MAX_SIZE;
    }
    LCD_RS_DATA;	
    LCD_CS_ON;

    unsigned char chifted = Colour>>8;;
    unsigned char burst_buffer[Buffer_Size];
    for(uint32_t j = 0; j < Buffer_Size; j+=2)
    {
	burst_buffer[j] = chifted;
	burst_buffer[j+1] = Colour & 0xFF;
	// burst_buffer[j+1] = Colour;	
    }

    uint32_t Sending_Size = Size*2;
    uint32_t Sending_in_Block = Sending_Size/Buffer_Size;
    uint32_t Remainder_from_block = Sending_Size%Buffer_Size;

    if(Sending_in_Block != 0)
    {
	for(uint32_t j = 0; j < (Sending_in_Block); j++)
	{
	    SPI1_Send_Array((unsigned char *)burst_buffer, Buffer_Size);
	}
    }

//REMAINDER!
    SPI1_Send_Array((unsigned char *)burst_buffer, Remainder_from_block);
    LCD_CS_OFF;
}

//FILL THE ENTIRE SCREEN WITH SELECTED COLOUR (either #define-d ones or custom 16bit)
/*Sets address (entire screen) and Sends Height*Width ammount of colour information to LCD*/
void ST7796_Fill_Screen(uint16_t Colour)
{
    ST7796_Set_Address(0,0,LCD_WIDTH,LCD_HEIGHT);	
    ST7796_Draw_Colour_Burst(Colour, LCD_WIDTH*LCD_HEIGHT);	
}

//DRAW PIXEL AT XY POSITION WITH SELECTED COLOUR
//
//Location is dependant on screen orientation. x0 and y0 locations change with orientations.
//Using pixels to draw big simple structures is not recommended as it is really slow
//Try using either rectangles or lines if possible
//
void ILI9341_Draw_Pixel(uint16_t X,uint16_t Y,uint16_t Colour) 
{
    if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;	//OUT OF BOUNDS!
//ADDRESS
    LCD_RS_CMD;	
    LCD_CS_ON;
    ST7796_SPI_Send(0x2A);
    LCD_RS_DATA;	
    LCD_CS_OFF;	

//XDATA
    LCD_CS_ON;	
    unsigned char Temp_Buffer[4] = {X>>8,X, (X+1)>>8, (X+1)};
    SPI1_Send_Array(Temp_Buffer, 4);
    LCD_CS_OFF;

//ADDRESS
    LCD_RS_CMD;	
    LCD_CS_ON;	
    ST7796_SPI_Send(0x2B);
    LCD_RS_DATA;	
    LCD_CS_OFF;	

//YDATA
    LCD_CS_ON;
    unsigned char Temp_Buffer1[4] = {Y>>8,Y, (Y+1)>>8, (Y+1)};
    SPI1_Send_Array(Temp_Buffer1, 4);    
    LCD_CS_OFF;

//ADDRESS	
    LCD_RS_CMD;	
    LCD_CS_ON;	
    ST7796_SPI_Send(0x2C);
    LCD_RS_DATA;	
    LCD_CS_OFF;	

//COLOR	
    LCD_CS_ON;
    unsigned char Temp_Buffer2[2] = {Colour>>8, Colour};
    SPI1_Send_Array(Temp_Buffer2, 2);    
    LCD_CS_OFF;
}


//DRAW RECTANGLE OF SET SIZE AND HEIGTH AT X and Y POSITION WITH CUSTOM COLOUR
//
//Rectangle X and Y positions mark the upper left corner of rectangle
//As with all other draw calls x0 and y0 locations dependant on screen orientation
//

void ST7796_Draw_Rectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Colour)
{
    if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;
    if((X+Width-1)>=LCD_WIDTH)
    {
	Width=LCD_WIDTH-X;
    }
    if((Y+Height-1)>=LCD_HEIGHT)
    {
	Height=LCD_HEIGHT-Y;
    }
    ST7796_Set_Address(X, Y, X+Width-1, Y+Height-1);
    ST7796_Draw_Colour_Burst(Colour, Height*Width);
}


void ST7796_Draw_Filled_Rectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Colour)
{
    ST7796_Draw_Rectangle(X, Y, Width, Height, Colour);
}


void ST7796_Draw_Hollow_Rectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Colour)
{
    ST7796_Draw_Horizontal_Line(X, Y, Width, Colour);
    ST7796_Draw_Horizontal_Line(X, Y + Height, Width, Colour);
    // not draw extreme pixels again
    ST7796_Draw_Vertical_Line(X, Y + 1, Height - 1, Colour);
    ST7796_Draw_Vertical_Line(X + Width, Y + 1, Height - 1, Colour);    
}


#define drawFastHLine(W,X,Y,Z)    ST7796_Draw_Horizontal_Line((W),(X),(Y),(Z))
#define drawFastVLine(W,X,Y,Z)    ST7796_Draw_Vertical_Line((W),(X),(Y),(Z))
#define drawPixel(X,Y,Z)    ILI9341_Draw_Pixel((X),(Y),(Z))
#define drawCircleHelper(X1,X2,X3,X4,X5)    ST7796_drawCircleHelper((X1),(X2),(X3),(X4),(X5))
void ST7796_drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color)
{
    // smarter version
    drawFastHLine(x + r  , y    , w - r - r, color); // Top
    drawFastHLine(x + r  , y + h - 1, w - r - r, color); // Bottom
    drawFastVLine(x    , y + r  , h - r - r, color); // Left
    drawFastVLine(x + w - 1, y + r  , h - r - r, color); // Right
    // draw four corners
    drawCircleHelper(x + r    , y + r    , r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r    , r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r    , y + h - r - 1, r, 8, color);
}


void ST7796_drawCircleHelper( int32_t x0, int32_t y0, int32_t rr, uint8_t cornername, uint32_t color)
{
    if (rr <= 0) return;
    int32_t f     = 1 - rr;
    int32_t ddF_x = 1;
    int32_t ddF_y = -2 * rr;
    int32_t xe    = 0;
    int32_t xs    = 0;
    int32_t len   = 0;


    do
    {
	while (f < 0) {
	    ++xe;
	    f += (ddF_x += 2);
	}
	f += (ddF_y += 2);

	if (xe-xs==1) {
	    if (cornername & 0x1) { // left top
		drawPixel(x0 - xe, y0 - rr, color);
		drawPixel(x0 - rr, y0 - xe, color);
	    }
	    if (cornername & 0x2) { // right top
		drawPixel(x0 + rr    , y0 - xe, color);
		drawPixel(x0 + xs + 1, y0 - rr, color);
	    }
	    if (cornername & 0x4) { // right bottom
		drawPixel(x0 + xs + 1, y0 + rr    , color);
		drawPixel(x0 + rr, y0 + xs + 1, color);
	    }
	    if (cornername & 0x8) { // left bottom
		drawPixel(x0 - rr, y0 + xs + 1, color);
		drawPixel(x0 - xe, y0 + rr    , color);
	    }
	}
	else {
	    len = xe - xs++;
	    if (cornername & 0x1) { // left top
		drawFastHLine(x0 - xe, y0 - rr, len, color);
		drawFastVLine(x0 - rr, y0 - xe, len, color);
	    }
	    if (cornername & 0x2) { // right top
		drawFastVLine(x0 + rr, y0 - xe, len, color);
		drawFastHLine(x0 + xs, y0 - rr, len, color);
	    }
	    if (cornername & 0x4) { // right bottom
		drawFastHLine(x0 + xs, y0 + rr, len, color);
		drawFastVLine(x0 + rr, y0 + xs, len, color);
	    }
	    if (cornername & 0x8) { // left bottom
		drawFastVLine(x0 - rr, y0 + xs, len, color);
		drawFastHLine(x0 - xe, y0 + rr, len, color);
	    }
	}
	xs = xe;
    } while (xe < rr--);
}


#define fillRect(X1,X2,X3,X4,X5)    ST7796_Draw_Filled_Rectangle((X1),(X2),(X3),(X4),(X5))
#define fillCircleHelper(X1,X2,X3,X4,X5,X6)    ST7796_fillCircleHelper((X1),(X2),(X3),(X4),(X5),(X6))
void ST7796_fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color)
{
    // smarter version
    fillRect(x, y + r, w, h - r - r, color);

    // draw four corners
    fillCircleHelper(x + r, y + h - r - 1, r, 1, w - r - r - 1, color);
    fillCircleHelper(x + r    , y + r, r, 2, w - r - r - 1, color);
}


void ST7796_fillCircleHelper(int32_t x0, int32_t y0, int32_t r, uint8_t cornername, int32_t delta, uint32_t color)
{
    int32_t f     = 1 - r;
    int32_t ddF_x = 1;
    int32_t ddF_y = -r - r;
    int32_t y     = 0;

    delta++;

    while (y < r) {
	if (f >= 0) {
	    if (cornername & 0x1) drawFastHLine(x0 - y, y0 + r, y + y + delta, color);
	    if (cornername & 0x2) drawFastHLine(x0 - y, y0 - r, y + y + delta, color);
	    r--;
	    ddF_y += 2;
	    f     += ddF_y;
	}

	y++;
	ddF_x += 2;
	f     += ddF_x;

	if (cornername & 0x1) drawFastHLine(x0 - r, y0 + y, r + r + delta, color);
	if (cornername & 0x2) drawFastHLine(x0 - r, y0 - y, r + r + delta, color);
    }
}

//DRAW LINE FROM X,Y LOCATION to X+Width,Y LOCATION
void ST7796_Draw_Horizontal_Line(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Colour)
{
    if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;
    if((X+Width-1)>=LCD_WIDTH)
    {
	Width=LCD_WIDTH-X;
    }
    ST7796_Set_Address(X, Y, X+Width-1, Y);
    ST7796_Draw_Colour_Burst(Colour, Width);
}

//DRAW LINE FROM X,Y LOCATION to X,Y+Height LOCATION
void ST7796_Draw_Vertical_Line(uint16_t X, uint16_t Y, uint16_t Height, uint16_t Colour)
{
    if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;
    if((Y+Height-1)>=LCD_HEIGHT)
    {
	Height=LCD_HEIGHT-Y;
    }
    ST7796_Set_Address(X, Y, X, Y+Height-1);
    ST7796_Draw_Colour_Burst(Colour, Height);
}


/*Draws a character (fonts imported from fonts.h) at X,Y location with specified font colour, size and Background colour*/
/*See fonts.h implementation of font on what is required for changing to a different font when switching fonts libraries*/
void ST7796_Draw_Char(char Character, uint16_t X, uint16_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour) 
{
    // uint8_t function_char;
    // uint8_t i,j;
    // function_char = Character;
    // if (function_char < ' ') {
    //     Character = 0;
    // } else {
    //     function_char -= 32;
    // }
   	
    // char temp[CHAR_WIDTH];
    // for(uint8_t k = 0; k<CHAR_WIDTH; k++)
    // {
    // 	temp[k] = font[function_char][k];
    // }
    // // Draw pixels
    // ST7796_Draw_Rectangle(X, Y, CHAR_WIDTH*Size, CHAR_HEIGHT*Size, Background_Colour);
    // for (j=0; j<CHAR_WIDTH; j++) {
    //     for (i=0; i<CHAR_HEIGHT; i++) {
    //         if (temp[j] & (1<<i)) {	
    // 		if(Size == 1)
    // 		{
    // 		    ILI9341_Draw_Pixel(X+j, Y+i, Colour);
    // 		}
    // 		else
    // 		{
    // 		    ST7796_Draw_Rectangle(X+(j*Size), Y+(i*Size), Size, Size, Colour);
    // 		}
    //         }	
    //     }
    // }

    // glcdfont.h
    unsigned char column[5] = { 0 };
    for (int i = 0; i < 5; i++)
	column[i] = font[(Character * 5) + i];

    // draw pixels
    uint8_t mask = 0x01;
    for (int8_t j = 0; j < 8; j++)
    {
	for (int8_t k = 0; k < 5; k++ )
	{
	    if (column[k] & mask)
	    {
		// tft_Write_16(color);
		// ILI9341_Draw_Pixel(X+j, Y+k, Colour);
		if (Size == 1)
		    ILI9341_Draw_Pixel(X+k, Y+j, Colour);
		else
		    ST7796_Draw_Rectangle(X+(k*Size), Y+(j*Size), Size, Size, Colour);
	    }
	    else
	    {
		// tft_Write_16(bg);
		// ILI9341_Draw_Pixel(X+j, Y+k, Background_Colour);
		if (Size == 1)
		    ILI9341_Draw_Pixel(X+k, Y+j, Background_Colour);
		else
		    ST7796_Draw_Rectangle(X+(k*Size), Y+(j*Size), Size, Size, Background_Colour);
	    }
	}
	mask <<= 1;
	// tft_Write_16(bg);
	// ILI9341_Draw_Pixel(X+CHAR_WIDTH, Y+j, Background_Colour);
	// ILI9341_Draw_Pixel(X, Y+j, Background_Colour);	
    }
    
    // ST7796_Draw_Rectangle(X, Y, , CHAR_HEIGHT*Size, Background_Colour);
    // for (j=0; j<CHAR_WIDTH; j++) {
    //     for (i=0; i<CHAR_HEIGHT; i++) {
    //         if (temp[j] & (1<<i)) {	
    // 		if(Size == 1)
    // 		{
		    
    // 		}
    
}

/*Draws an array of characters (fonts imported from fonts.h) at X,Y location with specified font colour, size and Background colour*/
/*See fonts.h implementation of font on what is required for changing to a different font when switching fonts libraries*/
void ST7796_Draw_Text(const char* Text, uint16_t X, uint16_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour)
{
    while (*Text) {
        ST7796_Draw_Char(*Text++, X, Y, Colour, Size, Background_Colour);
        X += CHAR_WIDTH*Size;
    }
}

/*Draws a full screen picture from flash. Image converted from RGB .jpeg/other to C array using online converter*/
//USING CONVERTER: http://www.digole.com/tools/PicturetoC_Hex_converter.php
//65K colour (2Bytes / Pixel)
void ILI9341_Draw_Image(const char* Image_Array, uint8_t Orientation)
{
    if(Orientation == SCREEN_HORIZONTAL_1)
    {
	ST7796_Set_Rotation(SCREEN_HORIZONTAL_1);
	ST7796_Set_Address(0,0,ST7796_SCREEN_WIDTH,ST7796_SCREEN_HEIGHT);
	LCD_RS_DATA;
	LCD_CS_ON;
	unsigned char Temp_small_buffer[BURST_MAX_SIZE];
	uint32_t counter = 0;
	for(uint32_t i = 0; i < ST7796_SCREEN_WIDTH*ST7796_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
	{	
	    for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
	    {
		Temp_small_buffer[k]	= Image_Array[counter+k];	
	    }
	    SPI1_Send_Array((unsigned char*)Temp_small_buffer, BURST_MAX_SIZE);
	    counter += BURST_MAX_SIZE;	
	}
	LCD_CS_OFF;
    }
    else if(Orientation == SCREEN_HORIZONTAL_2)
    {
	ST7796_Set_Rotation(SCREEN_HORIZONTAL_2);
	ST7796_Set_Address(0,0,ST7796_SCREEN_WIDTH,ST7796_SCREEN_HEIGHT);
	LCD_RS_DATA;
	LCD_CS_ON;
	unsigned char Temp_small_buffer[BURST_MAX_SIZE];
	uint32_t counter = 0;
	for(uint32_t i = 0; i < ST7796_SCREEN_WIDTH*ST7796_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
	{	
	    for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
	    {
		Temp_small_buffer[k]	= Image_Array[counter+k];	
	    }
	    SPI1_Send_Array((unsigned char*)Temp_small_buffer, BURST_MAX_SIZE);	    
	    counter += BURST_MAX_SIZE;	
	}
	LCD_CS_OFF;	
    }
    else if(Orientation == SCREEN_VERTICAL_2)
    {
	ST7796_Set_Rotation(SCREEN_VERTICAL_2);
	ST7796_Set_Address(0,0,ST7796_SCREEN_HEIGHT,ST7796_SCREEN_WIDTH);
	LCD_RS_DATA;
	LCD_CS_ON;
	unsigned char Temp_small_buffer[BURST_MAX_SIZE];
	uint32_t counter = 0;
	for(uint32_t i = 0; i < ST7796_SCREEN_WIDTH*ST7796_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
	{	
	    for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
	    {
		Temp_small_buffer[k]	= Image_Array[counter+k];	
	    }
	    SPI1_Send_Array((unsigned char*)Temp_small_buffer, BURST_MAX_SIZE);	    
	    counter += BURST_MAX_SIZE;	
	}
	LCD_CS_OFF;
    }
    else if(Orientation == SCREEN_VERTICAL_1)
    {
	ST7796_Set_Rotation(SCREEN_VERTICAL_1);
	ST7796_Set_Address(0,0,ST7796_SCREEN_HEIGHT,ST7796_SCREEN_WIDTH);
	LCD_RS_DATA;
	LCD_CS_ON;
	unsigned char Temp_small_buffer[BURST_MAX_SIZE];
	uint32_t counter = 0;
	for(uint32_t i = 0; i < ST7796_SCREEN_WIDTH*ST7796_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
	{	
	    for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
	    {
		Temp_small_buffer[k]	= Image_Array[counter+k];	
	    }	
	    SPI1_Send_Array((unsigned char*)Temp_small_buffer, BURST_MAX_SIZE);
	    counter += BURST_MAX_SIZE;	
	}
	LCD_CS_OFF;	
    }
}


void ST7796_Draw_Image_480_320_Monochrome (unsigned char * ptr)
{
    // ST7796_Set_Rotation(SCREEN_HORIZONTAL_1);
    
    // for (int j = 0; j < 320; j++)
    // {
    // 	for (int i = 0; i < 60; i++)
    // 	{
    // 	    for (int k = 0; k < 8; k++)
    // 	    {
    // 		unsigned char mask = 0x80;
    // 		mask >>= k;
    // 		if (*(ptr + j * 60 + i) & mask)
    // 		{
    // 		    ILI9341_Draw_Pixel(i * 8 + k, 319 - j, WHITE);
    // 		}
    // 		else
    // 		{
    // 		    ILI9341_Draw_Pixel(i * 8 + k, 319 - j, BLACK);
    // 		}
    // 	    }
    // 	}
    // }

    // old
    // ST7796_Set_Address(0,0,ST7796_SCREEN_WIDTH,ST7796_SCREEN_HEIGHT);
    // LCD_RS_DATA;
    // LCD_CS_ON;
    // unsigned char Temp_small_buffer[BURST_MAX_SIZE];
    // uint32_t counter = 0;
    // for(uint32_t i = 0; i < ST7796_SCREEN_WIDTH*ST7796_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
    // {	
    // 	for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
    // 	{
    // 	    // Temp_small_buffer[k]	= Image_Array[counter+k];
    // 	    Temp_small_buffer[k]	= ptr[counter+k];

    // 	    // create 2 bytes from bit
	    
    // 	}
    // 	SPI1_Send_Array((unsigned char*)Temp_small_buffer, BURST_MAX_SIZE);
    // 	counter += BURST_MAX_SIZE;	
    // }
    // LCD_CS_OFF;
    // end of old

    // new
    ST7796_Set_Address(0,0,ST7796_SCREEN_WIDTH - 1,ST7796_SCREEN_HEIGHT - 1);
    LCD_RS_DATA;
    LCD_CS_ON;
    unsigned char Temp_small_buffer[480 * 2];
    // uint32_t counter = 0;
    // for(uint32_t j = 0; j < 320; j++)
    for(uint32_t j = 319; j >= 1; j--)    // si quito el >=1 se traba la pantalla
    {	
	for(uint32_t i = 0; i < 60; i++)
	{
	    // create two bytes from single bit
	    for (int k = 0; k < 8; k++)
	    {
		unsigned char mask = 0x80;
		mask >>= k;
		if (*(ptr + j * 60 + i) & mask)
		{
		    Temp_small_buffer[i * 16 + k * 2 + 0] = 255;
		    Temp_small_buffer[i * 16 + k * 2 + 1] = 255;	    
		}
		else
		{
		    Temp_small_buffer[i * 16 + k * 2 + 0] = 0;
		    Temp_small_buffer[i * 16 + k * 2 + 1] = 0;	    
		}
	    }
	}
	SPI1_Send_Array((unsigned char*)Temp_small_buffer, sizeof(Temp_small_buffer));
    }
    LCD_CS_OFF;
    // end of new
    
    // for (int i = 0; i < 480; i++)
    // {
    // 	ILI9341_Draw_Pixel(i, 100, WHITE);
    // }
    // for (int i = 0; i < 480; i++)
    // {
    // 	ILI9341_Draw_Pixel(i, 200, WHITE);
    // }
    // for (int i = 0; i < 320; i++)
    // {
    // 	ILI9341_Draw_Pixel(20, i, WHITE);
    // }
}


void ST7796_Draw_Image_Monochrome (unsigned char * ptr, unsigned short y1, unsigned short y2, unsigned short y3, unsigned short y4)
{
    // new
    ST7796_Set_Address(0, y3, ST7796_SCREEN_WIDTH, y4);
    LCD_RS_DATA;
    LCD_CS_ON;
    unsigned char Temp_small_buffer[480 * 2];
    // uint32_t counter = 0;
    for(uint32_t j = y2; j >= y1 + 1; j--)
    // for(uint32_t j = y1; j < y2; j++)	
    {	
	for(uint32_t i = 0; i < 60; i++)
	{
	    // create two bytes from single bit
	    for (int k = 0; k < 8; k++)
	    {
		unsigned char mask = 0x80;
		mask >>= k;
		if (*(ptr + j * 60 + i) & mask)
		{
		    Temp_small_buffer[i * 16 + k * 2 + 0] = 255;
		    Temp_small_buffer[i * 16 + k * 2 + 1] = 255;	    
		}
		else
		{
		    Temp_small_buffer[i * 16 + k * 2 + 0] = 0;
		    Temp_small_buffer[i * 16 + k * 2 + 1] = 0;	    
		}
	    }
	}
	// SPI1_Send_Array((unsigned char*)Temp_small_buffer, sizeof(Temp_small_buffer));
	while (!SPI1_DMA_Check_Free());
	SPI1_DMA_Send_Array((unsigned char*)Temp_small_buffer, sizeof(Temp_small_buffer));
    }
    while (!SPI1_DMA_Check_Free());
    SPI1_DMA_Disable();
    LCD_CS_OFF;
}


void ST7796_Draw_Image_Monochrome1 (unsigned char * ptr, unsigned short y1, unsigned short y2)
{
    // new
    ST7796_Set_Address(0, y1, ST7796_SCREEN_WIDTH, y2);
    LCD_RS_DATA;
    LCD_CS_ON;
    unsigned char Temp_small_buffer[480 * 2];
    // uint32_t counter = 0;
    for(uint32_t j = y2; j >= y1; j--)
    // for(uint32_t j = y1; j < y2; j++)	
    {	
	for(uint32_t i = 0; i < 60; i++)
	{
	    // create two bytes from single bit
	    for (int k = 0; k < 8; k++)
	    {
		unsigned char mask = 0x80;
		mask >>= k;
		if (*(ptr + j * 60 + i) & mask)
		{
		    Temp_small_buffer[i * 16 + k * 2 + 0] = 255;
		    Temp_small_buffer[i * 16 + k * 2 + 1] = 255;	    
		}
		else
		{
		    Temp_small_buffer[i * 16 + k * 2 + 0] = 0;
		    Temp_small_buffer[i * 16 + k * 2 + 1] = 0;	    
		}
	    }
	}
	// SPI1_Send_Array((unsigned char*)Temp_small_buffer, sizeof(Temp_small_buffer));
	while (!SPI1_DMA_Check_Free());
	SPI1_DMA_Send_Array((unsigned char*)Temp_small_buffer, sizeof(Temp_small_buffer));
    }
    while (!SPI1_DMA_Check_Free());
    SPI1_DMA_Disable();
    LCD_CS_OFF;
}


void ST7796_Draw_Image_Rgb565 (unsigned char * ptr, unsigned short x1, unsigned short y1, unsigned short sizex, unsigned short sizey)
{
    // new
    ST7796_Set_Address(x1, y1, x1 + sizex - 1, y1 + sizey - 1);
    LCD_RS_DATA;
    LCD_CS_ON;

    unsigned char buff0[480 * 2];

    for(uint32_t j = 0; j < sizey; j++)	
    {	
	for(uint32_t i = 0; i < sizex * 2; i+=2)
	{
	    // get two bytes from each pixel
	    // rows index
	    unsigned int rindex = j * sizex * 2;
	    buff0[i + 1] = *(ptr + rindex + i + 0);
	    buff0[i + 0] = *(ptr + rindex + i + 1);
	}
	// SPI1_Send_Array((unsigned char*)buff0, sizex * 2);
	while (!SPI1_DMA_Check_Free());
	SPI1_DMA_Send_Array((unsigned char*)buff0, sizex * 2);
    }
    while (!SPI1_DMA_Check_Free());
    SPI1_DMA_Disable();
    LCD_CS_OFF;
}


void ST7796_Draw_Image_By_Pixel (unsigned char * ptr, unsigned short xsize, unsigned short ysize)
{
    for(uint32_t j = 0; j < ysize; j++)	
    {	
	for(uint32_t i = 0; i < xsize * 2; i+=2)
	{
	    // get two bytes from each pixel
	    // rows index
	    unsigned int rindex = j * 128 * 2;
	    unsigned short pix = 0;
	    // pix = *(ptr + rindex + i + 0) << 8;
	    // pix |= *(ptr + rindex + i + 1);	    
	    pix = *(ptr + rindex + i + 1) << 8;
	    pix |= *(ptr + rindex + i + 0);	    
	    ILI9341_Draw_Pixel(i >> 1, j, pix);
	}
    }
}

