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

// void TF_Usart1_Tx (void);
void TF_Usart1_Tx_String (void);

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

    TF_Spi1_st7796_cups_video ();

    // TF_Spi1_st7796_cups_pixel ();    

    // TF_Usart1_Tx_String ();
    // TF_Adc_Usart1_Tx ();
    // TF_Adc_Usart1_Voltages ();

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


extern const unsigned char pt0[];
extern const unsigned char pt1[];
extern const unsigned char pt2[];
extern const unsigned char pt3[];
extern const unsigned char pt4[];
void TF_Spi1_st7796_cups_video (void)
{
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





void TF_Usart1_Tx (void)
{
    Usart1Config();
    
    while (1)
    {
        unsigned char snd = 'M';
        Usart1SendUnsigned(&snd, 1);
        Wait_ms(100);
    }
}


void TF_Usart1_Tx_String (void)
{
    Usart1Config();
    
    while (1)
    {
        LED_ON;
        Usart1Send("Mariano\n");
        Wait_ms(10);
        LED_OFF;
        Wait_ms(1990);
    }
}


void TF_Adc_Usart1_Tx (void)
{
    char buff [100] = { 0 };

    //-- Test ADC Multiple conversion Scanning (starts each sequence) and DMA 
    //-- DMA configuration.
    DMAConfig();
    DMA_ENABLE;
    
    //Uso ADC con DMA
    AdcConfig();
    ADC_START;

    Usart1Config ();
    
    while (1)
    {
        for (int i = 0; i < ADC_CHANNEL_QUANTITY; i++)
            adc_ch[i] = 0;

#ifdef HARDWARE_VERSION_1_0	
        Wait_ms(1000);
        Usart1Send("starting conversion with channels in:\n");
        sprintf(buff, "%d %d %d %d %d %d\n",
                SENSE_POWER,
                SENSE_MEAS,
                V_SENSE_28V,
                V_SENSE_25V,
                V_SENSE_11V,
                V_SENSE_8V);
        
        Usart1Send (buff);

        LED_ON;
        ADC_START;
        Wait_ms(100);
        LED_OFF;
        
        Wait_ms(900);
        Usart1Send("conversion ended:\n");
        sprintf(buff, "%d %d %d %d %d %d\n",
                SENSE_POWER,
                SENSE_MEAS,
                V_SENSE_28V,
                V_SENSE_25V,
                V_SENSE_11V,
                V_SENSE_8V);
        
        Usart1Send (buff);
#endif
	
    }
    //--- End Test ADC Multiple conversion Scanning Continuous Mode and DMA ----------------//        
}


void TF_Adc_Usart1_Voltages (void)
{
    char buff [100] = { 0 };

    //-- Test ADC Multiple conversion Scanning (starts each sequence) and DMA 
    //-- DMA configuration.
    DMAConfig();
    DMA_ENABLE;
    
    //Uso ADC con DMA
    AdcConfig();
    // ADC_START;

    Usart1Config ();

    int calc_int, calc_dec;
    while (1)
    {
        for (int i = 0; i < ADC_CHANNEL_QUANTITY; i++)
            adc_ch[i] = 0;

#ifdef HARDWARE_VERSION_2_0	
        Wait_ms(1000);
        Usart1Send("starting conversion with channels in:\n");
        sprintf(buff, "%d %d\n",
                SENSE_POWER,
                SENSE_MEAS);
        
        Usart1Send (buff);

        LED_ON;
        ADC_START;
        Wait_ms(100);
        LED_OFF;
        
        Wait_ms(900);
        Usart1Send("conversion ended:\n");
        sprintf(buff, "%d %d\n",
                SENSE_POWER,
                SENSE_MEAS);

        // SENSE_POWER resistor multiplier 11
        calc_int = SENSE_POWER * 330 * 11;
        calc_int >>= 12;
        calc_dec = calc_int;
        calc_int = calc_int / 100;
        calc_dec = calc_dec - calc_int * 100;
        sprintf(buff, "Power: %d.%02d, ", calc_int, calc_dec);
        Usart1Send (buff);

        // SENSE_MEAS resistor multiplier 2
        calc_int = SENSE_MEAS * 330 * 2;
        calc_int >>= 12;
        calc_dec = calc_int;
        calc_int = calc_int / 100;
        calc_dec = calc_dec - calc_int * 100;
        sprintf(buff, "Meas: %d.%02d, ", calc_int, calc_dec);
        Usart1Send (buff);
#endif
	
#ifdef HARDWARE_VERSION_1_0
        Wait_ms(1000);
        Usart1Send("starting conversion with channels in:\n");
        sprintf(buff, "%d %d %d %d %d %d\n",
                SENSE_POWER,
                SENSE_MEAS,
                V_SENSE_28V,
                V_SENSE_25V,
                V_SENSE_11V,
                V_SENSE_8V);
        
        Usart1Send (buff);

        LED_ON;
        ADC_START;
        Wait_ms(100);
        LED_OFF;
        
        Wait_ms(900);
        Usart1Send("conversion ended:\n");
        sprintf(buff, "%d %d %d %d %d %d\n",
                SENSE_POWER,
                SENSE_MEAS,
                V_SENSE_28V,
                V_SENSE_25V,
                V_SENSE_11V,
                V_SENSE_8V);

        // SENSE_POWER resistor multiplier 11
        calc_int = SENSE_POWER * 330 * 11;
        calc_int >>= 12;
        calc_dec = calc_int;
        calc_int = calc_int / 100;
        calc_dec = calc_dec - calc_int * 100;
        sprintf(buff, "Power: %d.%02d, ", calc_int, calc_dec);
        Usart1Send (buff);

        // SENSE_MEAS resistor multiplier 2
        calc_int = SENSE_MEAS * 330 * 2;
        calc_int >>= 12;
        calc_dec = calc_int;
        calc_int = calc_int / 100;
        calc_dec = calc_dec - calc_int * 100;
        sprintf(buff, "Meas: %d.%02d, ", calc_int, calc_dec);
        Usart1Send (buff);

        // V_SENSE_28V resistor multiplier 11
        calc_int = V_SENSE_28V * 330 * 11;
        calc_int >>= 12;
        calc_dec = calc_int;
        calc_int = calc_int / 100;
        calc_dec = calc_dec - calc_int * 100;
        sprintf(buff, "V28V: %d.%02d, ", calc_int, calc_dec);
        Usart1Send (buff);

        // V_SENSE_25V resistor multiplier 11
        calc_int = V_SENSE_25V * 330 * 11;
        calc_int >>= 12;
        calc_dec = calc_int;
        calc_int = calc_int / 100;
        calc_dec = calc_dec - calc_int * 100;
        sprintf(buff, "V25V: %d.%02d, ", calc_int, calc_dec);
        Usart1Send (buff);

        // V_SENSE_11V resistor multiplier 11
        calc_int = V_SENSE_11V * 330 * 11;
        calc_int >>= 12;
        calc_dec = calc_int;
        calc_int = calc_int / 100;
        calc_dec = calc_dec - calc_int * 100;
        sprintf(buff, "V11V: %d.%02d, ", calc_int, calc_dec);
        Usart1Send (buff);

        // V_SENSE_8V resistor multiplier 11
        calc_int = V_SENSE_8V * 330 * 11;
        calc_int >>= 12;
        calc_dec = calc_int;
        calc_int = calc_int / 100;
        calc_dec = calc_dec - calc_int * 100;
        sprintf(buff, "V8V: %d.%02d\n", calc_int, calc_dec);
        Usart1Send (buff);
#endif
                
    }
    //--- End Test ADC Multiple conversion Scanning Continuous Mode and DMA ----------------//        
}


//--- end of file ---//
