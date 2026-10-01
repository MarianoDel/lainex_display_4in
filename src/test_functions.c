//----------------------------------------------------------
// #### MAGNET PROJECT - Custom Board ####
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.C ###################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_functions.h"
#include "hard.h"
#include "stm32f10x.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include "dma.h"
#include "tim.h"
#include "dac.h"

#include "spi.h"
#include "st7796.h"
#include "sst25.h"

#include "comms.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];
extern volatile unsigned short wait_ms_var;
extern volatile unsigned short timer_standby;
// extern volatile unsigned char usart3_have_data;


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Led (void);
void TF_Led_Timers (void);
void TF_Led_Dac (void);

void TF_Usart1_Tx_String (void);
void TF_Usart1_Tx_Rx_Int (void);

void TF_Adc_Usart1_Tx (void);
void TF_Adc_Usart1_Voltages (void);

void TF_Led_Lcd_Rst (void);
void TF_Led_Lcd_Rs (void);
void TF_Led_Lcd_Cs (void);   

void TF_Spi1_Send_Byte (void);
void TF_Spi1_st7796 (void);

void TF_Spi1_st7796_test1 (void);
void TF_Spi1_st7796_initial_video (void);
void TF_Spi1_st7796_cups_video (void);
void TF_Spi1_st7796_cups_pixel (void);

void TF_Usart1_And_Memory_Only_Jedec (void);

void TF_Spi2_Send_Byte (void);
void TF_Usart1_And_Memory_Debug (void);
void TF_Usart1_And_Memory_Write_Read (void);
void TF_Usart1_And_Memory_Write_Read_Lcd (void);
void TF_Usart1_And_Memory_Only_Read_Lcd (void);
void TF_Usart1_And_Memory_Only_Read_Lcd_Full (void);
void TF_Usart1_And_Memory_Only_Read_Compare (void);

void TF_Usart1_And_Memory_Managment (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // TF_Led ();
    // TF_Led_Timers ();
    
    // TF_Led_Lcd_Rst ();
    // TF_Led_Lcd_Rs ();
    // TF_Led_Lcd_Cs ();        

    // TF_Spi1_Send_Byte ();

    // TF_Spi1_st7796 ();

    // TF_Spi1_st7796_initial_video ();

    // TF_Spi1_st7796_cups_video ();

    // TF_Spi1_st7796_cups_pixel ();    

    // TF_Usart1_Tx_String ();
    // TF_Usart1_Tx_Rx_Int ();    
    // TF_Adc_Usart1_Tx ();
    // TF_Adc_Usart1_Voltages ();

    // TF_Spi2_Send_Byte ();
    // TF_Usart1_And_Memory_Only_Jedec ();
    // TF_Usart1_And_Memory_Debug ();
    // TF_Usart1_And_Memory_Write_Read ();

    // TF_Usart1_And_Memory_Write_Read_Lcd ();
    // TF_Usart1_And_Memory_Only_Read_Lcd ();
    // TF_Usart1_And_Memory_Only_Read_Lcd_Full ();    
    // TF_Usart1_And_Memory_Only_Read_Compare ();

    TF_Usart1_And_Memory_Only_Read_Lcd_Full ();        
    // TF_Usart1_And_Memory_Managment ();
}


void TF_Led (void)
{
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;
        
        Wait_ms(200);
    }
}


void TF_Led_Timers (void)
{
    int state = 0;

    while (1)
    {
        if (state == 0)
        {
            if (!timer_standby)
            {
                ChangeLed_With_Timer (LED_TREATMENT_STANDBY, 4000);
                timer_standby = 20000;
                state++;
            }
        }

        if (state == 1)
        {
            if (!timer_standby)
            {
                ChangeLed_With_Timer (LED_TREATMENT_STANDBY, 0);
                timer_standby = 20000;
                state++;
            }
        }

        if (state == 2)
        {
            if (!timer_standby)
            {
                ChangeLed (LED_TREATMENT_SINE_RUNNING);
                timer_standby = 20000;
                state++;
            }
        }

        if (state == 3)
        {
            if (!timer_standby)
            {
                ChangeLed (LED_TREATMENT_SQUARE_RUNNING);
                timer_standby = 20000;
                state = 0;
            }
        }

        UpdateLed();
    }
}


void TF_Led_Lcd_Rst (void)
{
    while (1)
    {
        if (LED)
	{
            LED_OFF;
	    Lcd_Rst_On();
	}
        else
	{
            LED_ON;
	    Lcd_Rst_Off();
	}
        
        Wait_ms(5000);
    }
}


void TF_Led_Lcd_Rs (void)
{
    while (1)
    {
        if (LED)
	{
            LED_OFF;
	    Lcd_Rs_Cmd();
	}
        else
	{
            LED_ON;
	    Lcd_Rs_Data();
	}
        
        Wait_ms(5000);
    }
}


void TF_Led_Lcd_Cs (void)
{
    while (1)
    {
        if (LED)
	{
            LED_OFF;
	    Lcd_Cs_On();
	}
        else
	{
            LED_ON;
	    Lcd_Cs_Off();
	}
        
        Wait_ms(5000);
    }
}


void TF_Spi1_Send_Byte (void)
{
    Lcd_Cs_Off();
    SPI1_Config();

    while (1)
    {
	LED_ON;
	SPI1_Send_Single(0x55);
	LED_OFF;
	        
        Wait_ms(200);
    }
}


void TF_Spi1_st7796 (void)
{
    Lcd_Cs_Off();
    SPI1_Config();

    Wait_ms(200);

    // new init --------------
    // ST7796_Init2();
    ST7796_Init3();
    Wait_ms(200);
    ST7796_Fill_Screen(RED);
    // ST7796_Fill_Screen(BLUE);    

    Wait_ms(2000);

    ST7796_Draw_Text("PEPOYA", 10, 10, BLACK, 2, RED);

    Wait_ms(2000);

    ST7796_Draw_Rectangle(100, 100, 100, 100, BLACK);    
    
    while (1);
}


extern const unsigned char p07[];
extern const unsigned char p10[];
extern const unsigned char p13[];
extern const unsigned char p15[];
extern const unsigned char p18[];
extern const unsigned char p21[];
extern const unsigned char p23[];
extern const unsigned char p25[];
extern const unsigned char p26[];
extern const unsigned char p30[];
extern const unsigned char p53[];
extern const unsigned char p77[];
extern const unsigned char p80[];
extern const unsigned char p81[];
extern const unsigned char p83[];
extern const unsigned char p85[];
extern const unsigned char p87[];
extern const unsigned char p89[];
extern const unsigned char p91[];
extern const unsigned char p94[];
extern const unsigned char p98[];

void dibujarEstructuraTaza(int x, int y);
void animarLlenadoTaza(int x, int y, int porcentaje);
void TF_Spi1_st7796_test1 (void)
{
    Lcd_Cs_Off();
    SPI1_Config();

    Wait_ms(200);

    // new init --------------
    // ST7796_Init2();
    ST7796_Init3();
    Wait_ms(200);
    // ST7796_Fill_Screen(RED);
    // ST7796_Fill_Screen(YELLOW);
    // ST7796_Fill_Screen(GREEN);        
    ST7796_Fill_Screen(BLACK);

    // nueva interfaz
    // ST7796_Draw_Hollow_Rectangle(5, 5, 470, 310, LIGHTGREY);
    // ST7796_Draw_Text("CONTROL DE PROCESO", 30, 20, WHITE, 4, BLACK);
    // dibujarEstructuraTaza( 60, 120);
    // dibujarEstructuraTaza(200, 120);
    // dibujarEstructuraTaza(340, 120);

    // ST7796_Draw_Text("Grupo 1",  60, 120 - 30, CYAN, 2, BLACK);
    // ST7796_Draw_Text("Grupo 2", 200, 120 - 30, CYAN, 2, BLACK);
    // ST7796_Draw_Text("Grupo 3", 340, 120 - 30, CYAN, 2, BLACK);
    // char buff_t [10] = { 0 };

    // ST7796_Draw_Text("00",  60 + 20, 120 + 90, YELLOW, 1, BLACK);
    // ST7796_Draw_Text("00", 200 + 20, 120 + 90, YELLOW, 2, BLACK);
    // ST7796_Draw_Text("00", 340 + 20, 120 + 90, YELLOW, 6, BLACK);
    // Wait_ms(1000);
    
    // ST7796_Draw_Rectangle(       0,        0, 30, 30, GREEN);
    // ST7796_Draw_Rectangle(480 - 30,        0, 30, 30, GREEN);
    // ST7796_Draw_Rectangle(       0, 320 - 30, 30, 30, GREEN);
    // ST7796_Draw_Rectangle(480 - 30, 320 - 30, 30, 30, GREEN);

    // ST7796_Draw_Hollow_Rectangle(  5,  5, (480 -  5 *2), (320 -  5 *2), RED);
    // ST7796_Draw_Hollow_Rectangle( 10, 10, (480 - 10 *2), (320 - 10 *2), RED);
    // ST7796_Draw_Hollow_Rectangle( 15, 15, (480 - 15 *2), (320 - 15 *2), RED);
    // ST7796_Draw_Hollow_Rectangle( 20, 20, (480 - 20 *2), (320 - 20 *2), RED);
    // ST7796_Draw_Hollow_Rectangle( 25, 25, (480 - 25 *2), (320 - 25 *2), RED);

    // while(1);
    // void ST7796_Draw_Text(const char* Text, uint8_t X, uint8_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour);
    // tft.setTextColor(TFT_WHITE, TFT_BLACK);
    // tft.setTextSize(3);
    // tft.drawString("CONTROL DE PROCESO", 30, 20);

    // ST7796_Draw_Rectangle(0, 0, 40, 40, BLUE);
    // ST7796_Draw_Rectangle(480 - 40, 0, 40, 40, BLUE);
    // ST7796_Draw_Rectangle(480 - 40, 320 - 40, 40, 40, BLUE);
    // ST7796_Draw_Rectangle(0, 320 - 40, 40, 40, BLUE);
    // ST7796_Draw_Horizontal_Line(0, 100, 480, PURPLE);
    

    // ST7796_Draw_Text("Grupo 1",  60, 120 - 30, CYAN, 1, BLACK);
    // ST7796_Draw_Text("Grupo 2", 200, 120 - 30, CYAN, 1, BLACK);
    // ST7796_Draw_Text("Grupo 3", 340, 120 - 30, CYAN, 1, BLACK);
    
    // unsigned char t1 = 0;
    // unsigned char t2 = 0;
    // unsigned char t3 = 0;

    // // taza 1
    // for (int i = 0; i <= 20; i++)
    // {
    // 	animarLlenadoTaza( 60, 120, i * 5); 
    // 	sprintf(buff_t, "%02d", i);
    // 	ST7796_Draw_Text(buff_t,  60 + 20, 120 + 90, YELLOW, 1, BLACK);
    // 	Wait_ms(1000);	
    // }

    // // taza 2
    // for (int i = 0; i <= 20; i++)
    // {
    // 	animarLlenadoTaza(200, 120, i * 5); 
    // 	sprintf(buff_t, "%02d", i);
    // 	ST7796_Draw_Text(buff_t, 200 + 20, 120 + 90, YELLOW, 2, BLACK);
    // 	Wait_ms(1000);
    // }

    // // taza 3
    // for (int i = 0; i <= 20; i++)
    // {
    // 	animarLlenadoTaza(340, 120, i * 5); 
    // 	sprintf(buff_t, "%02d", i);
    // 	ST7796_Draw_Text(buff_t, 340 + 20, 120 + 90, YELLOW, 6, BLACK);
    // 	Wait_ms(1000);
    // }
    
    // Wait_ms(2000);

    const unsigned char * ptr;
    // ptr = p53;
    // ST7796_Draw_Image_480_320_Monochrome ((unsigned char *) ptr);
    // Wait_ms(1000);
    ptr = p07;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p10;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p13;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p15;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p18;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p21;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p23;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p25;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p26;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p30;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p53;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    // ST7796_Draw_Image_Monochrome1 ((unsigned char *) ptr, 105, 216);
    Wait_ms(1500);
    
    ptr = p77;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);
    // ST7796_Draw_Image_Monochrome1 ((unsigned char *) ptr, 105, 216);
    // Wait_ms(2000);
    ptr = p80;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);
    // ST7796_Draw_Image_Monochrome1 ((unsigned char *) ptr, 105, 216);

    ptr = p81;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);
    // ST7796_Draw_Image_Monochrome1 ((unsigned char *) ptr, 105, 216);

    ptr = p83;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);
    // ST7796_Draw_Image_Monochrome1 ((unsigned char *) ptr, 105, 216);

    ptr = p85;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);

    ptr = p87;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p89;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p91;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p94;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p98;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    






    // ST7796_Draw_Image_Monochrome1 ((unsigned char *) ptr, 105, 216);


    // Wait_ms(1200);
    // ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 105, 165, 165, 216);

    // ST7796_Draw_Text("PEPOYA", 10, 10, BLACK, 2, RED);

    // Wait_ms(2000);
    ST7796_Fill_Screen(BLACK);
    // ST7796_Draw_Rectangle(100, 100, 100, 100, BLACK);    
    
    while (1);
}


void dibujarEstructuraTaza(int x, int y)
{
    // tft.drawRoundRect(x, y, 80, 70, 8, TFT_WHITE);
    // tft.drawFastHLine(x + 5, y, 70, TFT_BLACK); 
    // tft.drawRoundRect(x + 79, y + 15, 20, 35, 10, TFT_WHITE); 
    // tft.fillRoundRect(x - 10, y + 72, 100, 6, 3, TFT_DARKGREY);

    ST7796_drawRoundRect(x, y, 80, 70, 8, WHITE);
    ST7796_Draw_Horizontal_Line(x + 5, y, 70, BLACK);
    ST7796_drawRoundRect(x + 79, y + 15, 20, 35, 10, WHITE);
    ST7796_fillRoundRect(x - 10, y + 72, 100, 6, 3, DARKGREY);
}


void animarLlenadoTaza(int x, int y, int porcentaje)
{
    int altoMaximoLiquido = 60; 
    int anchoInterno = 74;
    // int alturaLiquido = map(porcentaje, 0, 100, 0, altoMaximoLiquido);
    int alturaLiquido = altoMaximoLiquido * porcentaje / 100;    
    int espacioVacio = altoMaximoLiquido - alturaLiquido;
    // uint16_t colorCafe = tft.color565(120, 65, 35);
    uint16_t colorCafe = MAROON;     

    if (espacioVacio > 0) ST7796_Draw_Filled_Rectangle(x + 3, y + 5, anchoInterno, espacioVacio, BLACK);
    if (alturaLiquido > 0) ST7796_Draw_Filled_Rectangle(x + 3, y + 5 + espacioVacio, anchoInterno, alturaLiquido, colorCafe);
}


void TF_Spi1_st7796_initial_video (void)
{
    Lcd_Backlight_Off();
    Lcd_Cs_Off();
    SPI1_Config();

    Wait_ms(200);

    // new init --------------
    ST7796_Init3();
    Wait_ms(200);
    // ST7796_Fill_Screen(RED);
    // ST7796_Fill_Screen(YELLOW);
    // ST7796_Fill_Screen(GREEN);
    ST7796_Fill_Screen(BLACK);
    Lcd_Backlight_On();    

    const unsigned char * ptr;
    ptr = p07;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p10;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p13;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p15;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p18;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p21;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p23;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p25;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p26;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p30;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p53;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    
    Wait_ms(1500);
    
    ptr = p77;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);

    ptr = p80;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);

    ptr = p81;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);

    ptr = p83;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);

    ptr = p85;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);

    ptr = p87;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p89;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p91;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p94;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ptr = p98;
    ST7796_Draw_Image_Monochrome ((unsigned char *) ptr, 0, 98, 105, 105 + 98);    

    ST7796_Fill_Screen(BLACK);
    
    while (1);
}


// original images
extern const unsigned char pt0[];
extern const unsigned char pt1[];
extern const unsigned char pt2[];
extern const unsigned char pt3[];
extern const unsigned char pt4[];

// inverted images
extern const unsigned char pt0i[];
void TF_Spi1_st7796_cups_video (void)
{
    Lcd_Backlight_Off();
    Lcd_Cs_Off();
    SPI1_Config();

    Wait_ms(200);

    // new init --------------
    ST7796_Init3();
    Wait_ms(200);
    // ST7796_Fill_Screen(RED);
    // ST7796_Fill_Screen(YELLOW);
    // ST7796_Fill_Screen(GREEN);        
    ST7796_Fill_Screen(BLACK);
    Lcd_Backlight_On();

    const unsigned char * ptr;
    ptr = pt0;
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 10, 100, 128, 101);
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 148, 100, 128, 101);
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 286, 100, 128, 101);    
    Wait_ms(250);
    
    ptr = pt1;
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 10, 100, 128, 101);
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 148, 100, 128, 101);
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 286, 100, 128, 101);    
    Wait_ms(250);
    
    ptr = pt2;
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 10, 100, 128, 101);
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 148, 100, 128, 101);
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 286, 100, 128, 101);    
    Wait_ms(250);
    
    ptr = pt3;
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 10, 100, 128, 101);
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 148, 100, 128, 101);
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 286, 100, 128, 101);    
    Wait_ms(250);
    
    ptr = pt4;
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 10, 100, 128, 101);
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 148, 100, 128, 101);
    ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 286, 100, 128, 101);    

    Wait_ms(5000);

    ST7796_Fill_Screen(BLACK);
    
    while (1);
}


void TF_Spi1_st7796_cups_pixel (void)
{
    Lcd_Backlight_Off();
    Lcd_Cs_Off();
    SPI1_Config();

    Wait_ms(200);

    // new init --------------
    ST7796_Init3();
    Wait_ms(200);
    // ST7796_Fill_Screen(RED);
    // ST7796_Fill_Screen(YELLOW);
    // ST7796_Fill_Screen(GREEN);        
    ST7796_Fill_Screen(BLACK);
    Lcd_Backlight_On();

    const unsigned char * ptr;
    
    ptr = pt0;
    ST7796_Draw_Image_By_Pixel ((unsigned char *)ptr, 128, 101);
    Wait_ms(100);

    ptr = pt1;
    ST7796_Draw_Image_By_Pixel ((unsigned char *)ptr, 128, 101);
    Wait_ms(100);

    ptr = pt2;
    ST7796_Draw_Image_By_Pixel ((unsigned char *)ptr, 128, 101);
    Wait_ms(100);

    ptr = pt3;
    ST7796_Draw_Image_By_Pixel ((unsigned char *)ptr, 128, 101);
    Wait_ms(100);

    ptr = pt4;
    ST7796_Draw_Image_By_Pixel ((unsigned char *)ptr, 128, 101);
    Wait_ms(10000);
    
    ST7796_Fill_Screen(BLACK);
    
    while (1);
}


void TF_Led_Dac (void)
{
    DAC_Config ();
    
    while (1)
    {
        if (LED)
        {
            LED_OFF;
            DAC_Output1 (2047);
            DAC_Output2 (4095);
        }
        else
        {
            LED_ON;
            DAC_Output1 (4095);
            DAC_Output2 (2047);
        }
        
        Wait_ms(200);
    }
}


void TF_Adc_Usart1_Tx (void)
{
    char buff [100] = { 0 };

    //-- Test ADC Multiple conversion Scanning (starts each sequence) and DMA 
    //-- DMA configuration.
    DMA1_Channel1_Config();
    DMA1_CH1_ENABLE;
    
    //Uso ADC con DMA
    AdcConfig();
    ADC_START;

    Usart1Config ();
    
    while (1)
    {
        // for (int i = 0; i < ADC_CHANNEL_QUANTITY; i++)
        //     adc_ch[i] = 0;

        Wait_ms(1000);
        Usart1Send("starting conversion with channels in:\r\n");
        sprintf(buff, "%d %d %d\r\n",
                TEMP_1,
                TEMP_2,
                TEMP_3);
        
        Usart1Send (buff);
    }
    //--- End Test ADC Multiple conversion Scanning Continuous Mode and DMA ----------------//        
}


void TF_Adc_Usart1_Voltages (void)
{
//     char buff [100] = { 0 };

//     //-- Test ADC Multiple conversion Scanning (starts each sequence) and DMA 
//     //-- DMA configuration.
//     DMAConfig();
//     DMA_ENABLE;
    
//     //Uso ADC con DMA
//     AdcConfig();
//     // ADC_START;

//     Usart1Config ();

//     int calc_int, calc_dec;
//     while (1)
//     {
//         for (int i = 0; i < ADC_CHANNEL_QUANTITY; i++)
//             adc_ch[i] = 0;

// #ifdef HARDWARE_VERSION_2_0	
//         Wait_ms(1000);
//         Usart1Send("starting conversion with channels in:\n");
//         sprintf(buff, "%d %d\n",
//                 SENSE_POWER,
//                 SENSE_MEAS);
        
//         Usart1Send (buff);

//         LED_ON;
//         ADC_START;
//         Wait_ms(100);
//         LED_OFF;
        
//         Wait_ms(900);
//         Usart1Send("conversion ended:\n");
//         sprintf(buff, "%d %d\n",
//                 SENSE_POWER,
//                 SENSE_MEAS);

//         // SENSE_POWER resistor multiplier 11
//         calc_int = SENSE_POWER * 330 * 11;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "Power: %d.%02d, ", calc_int, calc_dec);
//         Usart1Send (buff);

//         // SENSE_MEAS resistor multiplier 2
//         calc_int = SENSE_MEAS * 330 * 2;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "Meas: %d.%02d, ", calc_int, calc_dec);
//         Usart1Send (buff);
// #endif
	
// #ifdef HARDWARE_VERSION_1_0
//         Wait_ms(1000);
//         Usart1Send("starting conversion with channels in:\n");
//         sprintf(buff, "%d %d %d %d %d %d\n",
//                 SENSE_POWER,
//                 SENSE_MEAS,
//                 V_SENSE_28V,
//                 V_SENSE_25V,
//                 V_SENSE_11V,
//                 V_SENSE_8V);
        
//         Usart1Send (buff);

//         LED_ON;
//         ADC_START;
//         Wait_ms(100);
//         LED_OFF;
        
//         Wait_ms(900);
//         Usart1Send("conversion ended:\n");
//         sprintf(buff, "%d %d %d %d %d %d\n",
//                 SENSE_POWER,
//                 SENSE_MEAS,
//                 V_SENSE_28V,
//                 V_SENSE_25V,
//                 V_SENSE_11V,
//                 V_SENSE_8V);

//         // SENSE_POWER resistor multiplier 11
//         calc_int = SENSE_POWER * 330 * 11;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "Power: %d.%02d, ", calc_int, calc_dec);
//         Usart1Send (buff);

//         // SENSE_MEAS resistor multiplier 2
//         calc_int = SENSE_MEAS * 330 * 2;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "Meas: %d.%02d, ", calc_int, calc_dec);
//         Usart1Send (buff);

//         // V_SENSE_28V resistor multiplier 11
//         calc_int = V_SENSE_28V * 330 * 11;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "V28V: %d.%02d, ", calc_int, calc_dec);
//         Usart1Send (buff);

//         // V_SENSE_25V resistor multiplier 11
//         calc_int = V_SENSE_25V * 330 * 11;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "V25V: %d.%02d, ", calc_int, calc_dec);
//         Usart1Send (buff);

//         // V_SENSE_11V resistor multiplier 11
//         calc_int = V_SENSE_11V * 330 * 11;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "V11V: %d.%02d, ", calc_int, calc_dec);
//         Usart1Send (buff);

//         // V_SENSE_8V resistor multiplier 11
//         calc_int = V_SENSE_8V * 330 * 11;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "V8V: %d.%02d\n", calc_int, calc_dec);
//         Usart1Send (buff);
// #endif
                
//     }
    //--- End Test ADC Multiple conversion Scanning Continuous Mode and DMA ----------------//        
}


void TF_Usart1_Tx_String (void)
{
    Usart1Config();

    Usart1Send("Test single string send on 2 secs.\n");
    while (1)
    {
        Usart1Send("Mariano\n");
        Wait_ms(2000);
    }
}


void TF_Usart1_Tx_Rx_Int (void)
{
    // start usart1 and loop rx -> tx after 3 secs
    Usart1Config();
    char buff_local [128] = { 0 };
    unsigned char readed = 0;
    unsigned char size = 0;    

    Usart1Send("\r\nTest string loop. Answers every 3 secs.\r\n");    
    while(1)
    {
        Wait_ms(2800);
        if (Usart1HaveData())
        {
	    char mybuff [40] = { 0 };
            LED_ON;
            Usart1HaveDataReset();
            readed = Usart1ReadBuffer(buff_local, 127);

	    sprintf(mybuff, "\r\nreaded: %d", readed);
	    Usart1Send(mybuff);
	    size = strlen(buff_local);
	    sprintf(mybuff, " size: %d\r\nbuffer: ", size);
	    Usart1Send(mybuff);
	    
            *(buff_local + readed - 1) = '\r';    //cambio el '\0' por '\n' antes de enviar
            *(buff_local + readed + 0) = '\n';    //cambio el '\0' por '\n' antes de enviar	    
            *(buff_local + readed + 1) = '\0';    //ajusto el '\0'
            Usart1Send(buff_local);
            Wait_ms(200);
            LED_OFF;
        }
    }    
}


void TF_Spi2_Send_Byte (void)
{
    Mem_Ce_Off();
    Mem_Wp_Off();
    SPI2_Config();

    while (1)
    {
	LED_ON;
	SPI2_Send_Single(0x55);
	LED_OFF;
	        
        Wait_ms(200);
    }
}


void TF_Usart1_And_Memory_Only_Jedec (void)
{
    unsigned char buffer [3];
    char my_str [100];

    // SPI Init
    Mem_Ce_Off();
    Mem_Wp_Off();
    SPI2_Config();
    
    // Usart Init
    Usart1Config();
    Usart1Send("Reading Memory...\r\n");

    while (1)
    {
        Usart1Send("memory jedec: ");
        getJEDEC (buffer);
        sprintf(my_str, "0x%02x 0x%02x 0x%02x\r\n",
                buffer[0],
                buffer[1],
                buffer[2]);

        Usart1Send(my_str);
        
        Wait_ms (2000);
        
    }
}


// void TF_Usart1_And_Memory_RW (void)
// {
//     unsigned char buffer [3];
//     char my_str [100];

//     // SPI Init
//     Mem_Ce_Off();
//     Mem_Wp_Off();
//     SPI_Config();
    
//     // Usart Init
//     Usart1Config();
//     Usart1Send("Reading Memory...\n");

//     while (1)
//     {
//         Usart1Send("memory jedec: ");
//         getJEDEC (buffer);
//         sprintf(my_str, "0x%02x 0x%02x 0x%02x\n",
//                 buffer[0],
//                 buffer[1],
//                 buffer[2]);

//         Usart1Send(my_str);
        
//         Wait_ms (1000);

//         for (int i = 888; i < 1000; i++)
//         {
//             sprintf(my_str, "position %d saving: %d", i, 0x50);
//             Usart1Send(my_str);
//             if (SST_WriteCodeToMemory(i, 0x50) != 0)
//                 Usart1Send(" ok");
//             else
//                 Usart1Send(" err");
            
//             Wait_ms(100);

//             int get = SST_CheckIndexInMemory(i);
//             sprintf(my_str, " getting: %d\n", get);
//             Usart1Send(my_str);

//             Wait_ms(1000);
//         }
//     }
// }


extern void writeSPI2 (unsigned char data);
extern void writeStatus2NVM (unsigned char data);
extern void unprotectNVM (void);
void TF_Usart1_And_Memory_Debug (void)
{
    unsigned char buffer [3];
    char my_str [100];

    // SPI Init
    Mem_Ce_Off();
    Mem_Wp_Off();
    SPI2_Config();
    
    // Usart Init
    Usart1Config();
    Wait_ms(20);
    Usart1Send("\n\nmemory jedec: ");
    getJEDEC (buffer);
    sprintf(my_str, "0x%02x 0x%02x 0x%02x\n",
            buffer[0],
            buffer[1],
            buffer[2]);

    // read first jedec to initialize memory
    Usart1Send(my_str);
    Wait_ms(200);
    unsigned char send_menu = 1;
    unsigned char data = 0;
    unsigned char data1 = 0; 
    unsigned char data2 = 0;
    unsigned int pos_to_test = 0x3200;
    
    while (1)
    {
        if (send_menu)
        {
            Usart1Send("  read status register 1to3 RDSR: 1\r\n");
            Usart1Send("  enable write: 2\r\n");
            Usart1Send("  clear all mem: 3\r\n");
            Wait_ms(100);
            sprintf(my_str, "  blank 4k from 0x%04x: 4\r\n", pos_to_test);
            Usart1Send(my_str);
            sprintf(my_str, "  read position 0x%04x: 5\r\n", pos_to_test);
            Usart1Send(my_str);
            sprintf(my_str, "  write position 0x%04x: 6\r\n", pos_to_test);            
            Usart1Send(my_str);
            Wait_ms(100);
            Usart1Send("  write SR2: 7\r\n");
            Usart1Send("  unprotect: 8\r\n");
            Usart1Send("\r\n   selection: ");
            send_menu = 0;
        }

        if (Usart1HaveData())
        {
            Usart1HaveDataReset();
            Usart1ReadBuffer(my_str, sizeof(my_str));

            switch (my_str[0])
            {
            case '1':
                data = readStatusNVM();
                data1 = readStatus2NVM();
                data2 = readStatus3NVM();
                sprintf(my_str, "status 0x%02x 0x%02x 0x%02x\r\n", data, data1, data2);
                Usart1Send(my_str);
                Wait_ms(500);                
                break;

            case '2':
                NVM_On;
                writeSPI2(SST25_WREN);
                NVM_Off;
                Wait_ms(500);                
                break;

            case '3':
                clearNVM();
                Usart1Send("done!\r\n");
                Wait_ms(500);                
                break;

            case '4':
                Clear4KNVM(pos_to_test);
                Usart1Send("done!\r\n");
                Wait_ms(500);                
                break;
                
            case '5':
                readBufNVM8u(&data, 1, pos_to_test);
                sprintf(my_str, "0x%02x\r\n", data);
                Usart1Send(my_str);
                Wait_ms(500);
                break;

            case '6':
                writeNVM(0x55, pos_to_test);
                sprintf(my_str, "saving 0x55 on 0x%04x\r\n", pos_to_test);
                Usart1Send(my_str);
                Wait_ms(500);                
                break;

            case '7':
                writeStatus2NVM(0x00);
                Usart1Send("sr2 to 0x00\r\n");
                Wait_ms(500);                
                break;

            case '8':
                unprotectNVM();
                Usart1Send("memory unprotected\r\n");
                Wait_ms(500);                
                break;
                
            case '?':
                send_menu = 1;
                break;

            default:
                Usart1Send("Selection error! try ?\r\n");
                break;
            }
        }
    }
}


void TF_Usart1_And_Memory_Write_Read (void)
{
    unsigned char buffer [3];
    char my_str [100];
    unsigned char fillbuff[1024];
    unsigned char error = 0;    

    // SPI Init
    Mem_Ce_Off();
    Mem_Wp_Off();
    SPI2_Config();
    
    // Usart Init
    Usart1Config();
    Wait_ms(20);
    Usart1Send("\r\n\nmemory jedec: ");
    getJEDEC (buffer);
    sprintf(my_str, "0x%02x 0x%02x 0x%02x\r\n",
            buffer[0],
            buffer[1],
            buffer[2]);

    // read first jedec to initialize memory
    Usart1Send(my_str);
    Wait_ms(200);

    Usart1Send("Clear 4K NVM 0x1000: ");
    Clear4KNVM(0x1000);
    Usart1Send("done!\r\nChecking free: ");

    if (!error)
    {
	for (int i = 0; i < 1024; i++)
	{
	    readBufNVM8u(fillbuff, sizeof(fillbuff), 0x1000);
	}

	for (int i = 0; i < 1024; i++)
	{
	    if (fillbuff[i] != 0xff)
	    {
		sprintf(my_str, "error on byte: %d value: 0x%02x\r\n", i, fillbuff[i]);
		Usart1Send(my_str);
		error = 1;
		break;
	    }
	}

	if (!error)
	    Usart1Send("check ok\r\n");
    }    

    if (!error)
    {
	for (int i = 0; i < 1024; i++)
	{
	    fillbuff[i] = 0x55;
	}
    
	Usart1Send("Writing 1024 bytes to NVM 0x55 to address 0x1000\r\n");
	writeBufferNVM(fillbuff, sizeof(fillbuff), 0x1000);
	Usart1Send("done!\r\nReading data: ");
	for (int i = 0; i < 1024; i++)
	{
	    readBufNVM8u(fillbuff, sizeof(fillbuff), 0x1000);
	}

	Usart1Send("done!\r\nChecking data: ");
	for (int i = 0; i < 1024; i++)
	{
	    if (fillbuff[i] != 0x55)
	    {
		sprintf(my_str, "error on byte: %d value: 0x%02x\r\n", i, fillbuff[i]);
		Usart1Send(my_str);
		error = 1;
		break;
	    }
	}

	if (!error)
	    Usart1Send("check ok\r\n");
	
    }
    
    while (1);
}


void TF_Usart1_And_Memory_Write_Read_Lcd (void)
{
    unsigned char buffer [3];
    char my_str [100];

    Lcd_Backlight_Off();
    
    // SPI Init for Mem
    Mem_Ce_Off();
    Mem_Wp_Off();
    SPI2_Config();
    
    // Usart Init
    Usart1Config();
    Wait_ms(20);
    Usart1Send("\r\n\nmemory jedec: ");
    getJEDEC (buffer);
    sprintf(my_str, "0x%02x 0x%02x 0x%02x\r\n",
            buffer[0],
            buffer[1],
            buffer[2]);

    // read first jedec to initialize memory
    Usart1Send(my_str);
    Wait_ms(200);

    // Lcd Init
    Lcd_Cs_Off();
    SPI1_Config();
    ST7796_Init3();
    Wait_ms(200);
    // ST7796_Fill_Screen(RED);
    // ST7796_Fill_Screen(YELLOW);
    // ST7796_Fill_Screen(GREEN);        
    ST7796_Fill_Screen(BLACK);
    Lcd_Backlight_On();
    Usart1Send("Lcd inited\r\n");    


    // blank and save cup to memory
    Usart1Send("Clear NVM starting at 0x1000\r\n");
    unsigned int cup_size = 0;
    unsigned int mem_pages = 0;
    cup_size = 128 * 101 * 2;
    mem_pages = cup_size >> 12;
    mem_pages += 1;
    sprintf(my_str, "cup size bytes: %d mem_pages to blank: %d\r\n", cup_size, mem_pages);
    Usart1Send(my_str);

    unsigned int bytes_copy = 0;
    Usart1Send("freeing pages\r\n");
    for (int i = 0; i < mem_pages; i++)
    {
	unsigned int addr = 0;
	addr = 0x1000 + 0x1000 * i;
	Clear4KNVM(addr);
	bytes_copy += 4096;
    }
    Usart1Send("done!\r\n");
    sprintf(my_str, "bytes freeing: %d\r\n", bytes_copy);
    Usart1Send(my_str);
    
    bytes_copy = 0;
    Usart1Send("coping flash to nvm at 0x1000\r\n");    
    for (int i = 0; i < cup_size; i+= 1024)
    {
	writeBufferNVM((unsigned char *)(pt0i + i), 1024, 0x1000 + i);
	bytes_copy += 1024;
    }
    sprintf(my_str, "bytes copy: %d cups size: %d remain: %d\n", bytes_copy, cup_size, cup_size - bytes_copy);
    Usart1Send(my_str);
    Usart1Send("done!\r\n");

    Wait_ms(2000);
    Usart1Send("checking mem pictures\r\n");

    const unsigned char * ptr;

    // show saved pict in mem dma
    // ptr = (const unsigned char *) 0x1000;
    // ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 010, 100, 128, 101);
    // ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 148, 100, 128, 101);
    // ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 286, 100, 128, 101);    

    // show saved pict in mem with CS no dma
    // ptr = (const unsigned char *) 0x1000;
    // ST7796_Draw_Image_Rgb565_Mem2 ((unsigned char *) ptr, 010, 100, 128, 101);
    // ST7796_Draw_Image_Rgb565_Mem2 ((unsigned char *) ptr, 148, 100, 128, 101);
    // ST7796_Draw_Image_Rgb565_Mem2 ((unsigned char *) ptr, 286, 100, 128, 101);    

    // check mem for pixel error
    ptr = (const unsigned char *) 0x1000;
    ST7796_Check_Image_Mem3 ((unsigned char *) ptr, (unsigned char *) pt0i, 128, 101);
    ST7796_Check_Image_Mem3 ((unsigned char *) ptr, (unsigned char *) pt0i, 128, 101);
    ST7796_Check_Image_Mem3 ((unsigned char *) ptr, (unsigned char *) pt0i, 128, 101);    
    
    // show pict from flash
    // ptr = pt0;
    // ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 010, 100, 128, 101);
    // ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 148, 100, 128, 101);
    // ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 286, 100, 128, 101);    
    
    
    while (1);

}


void TF_Usart1_And_Memory_Only_Read_Lcd (void)
{
    unsigned char buffer [3];
    char my_str [100];

    Lcd_Backlight_Off();
    
    // SPI Init for Mem
    Mem_Ce_Off();
    Mem_Wp_Off();
    SPI2_Config();
    
    // Usart Init
    Usart1Config();
    Wait_ms(20);
    Usart1Send("\r\n\nmemory jedec: ");
    getJEDEC (buffer);
    sprintf(my_str, "0x%02x 0x%02x 0x%02x\r\n",
            buffer[0],
            buffer[1],
            buffer[2]);

    // read first jedec to initialize memory
    Usart1Send(my_str);
    Wait_ms(200);

    // Lcd Init
    Lcd_Cs_Off();
    SPI1_Config();
    ST7796_Init3();
    Wait_ms(200);
    // ST7796_Fill_Screen(RED);
    // ST7796_Fill_Screen(YELLOW);
    // ST7796_Fill_Screen(GREEN);        
    ST7796_Fill_Screen(BLACK);
    Lcd_Backlight_On();
    Usart1Send("Lcd inited\r\n");    

    Wait_ms(2000);
    Usart1Send("showing mem pictures\r\n");

    const unsigned char * ptr;

    // show saved pict in mem dma
    ptr = (const unsigned char *) 0x1000;
    ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 010, 100, 128, 101);
    ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 148, 100, 128, 101);
    ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 286, 100, 128, 101);    

    // show saved pict in mem with CS no dma
    // ptr = (const unsigned char *) 0x1000;
    // ST7796_Draw_Image_Rgb565_Mem2 ((unsigned char *) ptr, 010, 100, 128, 101);
    // ST7796_Draw_Image_Rgb565_Mem2 ((unsigned char *) ptr, 148, 100, 128, 101);
    // ST7796_Draw_Image_Rgb565_Mem2 ((unsigned char *) ptr, 286, 100, 128, 101);    

    // check mem for pixel error
    // ptr = (const unsigned char *) 0x1000;
    // image orig
    // ST7796_Check_Image_Mem3 ((unsigned char *) ptr, (unsigned char *) pt0, 128, 101);
    // ST7796_Check_Image_Mem3 ((unsigned char *) ptr, (unsigned char *) pt0, 128, 101);
    // ST7796_Check_Image_Mem3 ((unsigned char *) ptr, (unsigned char *) pt0, 128, 101);    
    // image inv
    // ST7796_Check_Image_Mem3 ((unsigned char *) ptr, (unsigned char *) pt0i, 128, 101);
    // ST7796_Check_Image_Mem3 ((unsigned char *) ptr, (unsigned char *) pt0i, 128, 101);
    // ST7796_Check_Image_Mem3 ((unsigned char *) ptr, (unsigned char *) pt0i, 128, 101);    
    
    // show pict from flash
    // ptr = pt0;
    // ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 010, 100, 128, 101);
    // ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 148, 100, 128, 101);
    // ST7796_Draw_Image_Rgb565 ((unsigned char *) ptr, 286, 100, 128, 101);    
    
    
    while (1);

}


void TF_Usart1_And_Memory_Only_Read_Lcd_Full (void)
{
    unsigned char buffer [3];
    char my_str [100];

    Lcd_Backlight_Off();
    
    // SPI Init for Mem
    Mem_Ce_Off();
    Mem_Wp_Off();
    SPI2_Config();
    
    // Usart Init
    Usart1Config();
    Wait_ms(20);
    Usart1Send("\r\n\nmemory jedec: ");
    getJEDEC (buffer);
    sprintf(my_str, "0x%02x 0x%02x 0x%02x\r\n",
            buffer[0],
            buffer[1],
            buffer[2]);

    // read first jedec to initialize memory
    Usart1Send(my_str);
    Wait_ms(200);

    // Lcd Init
    Lcd_Cs_Off();
    SPI1_Config();
    ST7796_Init3();
    Wait_ms(200);
    // ST7796_Fill_Screen(RED);
    // ST7796_Fill_Screen(YELLOW);
    // ST7796_Fill_Screen(GREEN);        
    ST7796_Fill_Screen(BLACK);
    Lcd_Backlight_On();
    Usart1Send("Lcd inited\r\n");    

    Wait_ms(2000);
    Usart1Send("showing mem pictures\r\n");

    const unsigned char * ptr;

    // show saved pict in mem dma
    ptr = (const unsigned char *) 0x1000;
    ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 0, 0, 480, 320);
    
    while (1)
    {
	// show cups
	Wait_ms(2000);
	ptr = (const unsigned char *) 0x4d000;
	ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 025, 100, 128, 101);
	ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 178, 100, 128, 101);
	ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 330, 100, 128, 101);	
	Wait_ms(2000);
	ptr = (const unsigned char *) 0x54000;
	ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 025, 100, 128, 101);
	ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 178, 100, 128, 101);
	ST7796_Draw_Image_Rgb565_Mem ((unsigned char *) ptr, 330, 100, 128, 101);	
	
    }

}


void TF_Usart1_And_Memory_Only_Read_Compare (void)
{
    unsigned char buffer [3];
    char my_str [100];

    Lcd_Backlight_Off();
    
    // SPI Init for Mem
    Mem_Ce_Off();
    Mem_Wp_Off();
    SPI2_Config();
    
    // Usart Init
    Usart1Config();
    Wait_ms(20);
    Usart1Send("\r\n\nmemory jedec: ");
    getJEDEC (buffer);
    sprintf(my_str, "0x%02x 0x%02x 0x%02x\r\n",
            buffer[0],
            buffer[1],
            buffer[2]);

    // read first jedec to initialize memory
    Usart1Send(my_str);
    Wait_ms(200);

    Wait_ms(2000);
    Usart1Send("checking mem pictures\r\n");

    const unsigned char * ptr;

    
    // check mem for pixel error
    ptr = (const unsigned char *) 0x1000;
    // ST7796_Check_Mem4 ((unsigned char *) ptr, (unsigned char *) pt0, 1024);
    ST7796_Check_Mem4 ((unsigned char *) ptr, (unsigned char *) pt0, 480 * 2);    
    // ST7796_Check_Image_Mem3 ((unsigned char *) ptr, (unsigned char *) pt0, 148, 100, 128, 101);
    // ST7796_Check_Image_Mem3 ((unsigned char *) ptr, (unsigned char *) pt0, 286, 100, 128, 101);    
        
    while (1);

}


void TF_Usart1_And_Memory_Managment (void)
{
    unsigned char buffer [3];
    char my_str [100];

    Lcd_Backlight_Off();
    
    // SPI Init for Mem
    Mem_Ce_Off();
    Mem_Wp_Off();
    SPI2_Config();
    
    // Usart Init
    Usart1Config();
    Wait_ms(20);
    Usart1Send("\r\n\nmemory jedec: ");
    getJEDEC (buffer);
    sprintf(my_str, "0x%02x 0x%02x 0x%02x\r\n",
            buffer[0],
            buffer[1],
            buffer[2]);

    // read first jedec to initialize memory
    Usart1Send(my_str);
    Wait_ms(200);

    Usart1Send("Going to Mem Manager\r\n");
    while (1)
    {
	Comms_Update();
    }

}

//--- end of file ---//
