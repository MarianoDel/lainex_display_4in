//---------------------------------------------------------------
// #### PROJECT MICRO-CURRENTS POWER-OUT F103 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### MAIN.C ##################################################
//---------------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "stm32f10x.h"
#include "hard.h"

#include "adc.h"
#include "dma.h"
#include "dac.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"

#include "comms.h"
#include "test_functions.h"
#include "st7796.h"
#include "sst25.h"
#include "pictures.h"
#include "temperatures.h"
#include "manager.h"


#include <stdio.h>
#include <string.h>


// Private Types Constants and Macros ------------------------------------------


// Externals -------------------------------------------------------------------
//--- Externals from timers
volatile unsigned short timer_standby = 0;
volatile unsigned short wait_ms_var = 0;

//--- Externals from adc
volatile unsigned short adc_ch [ADC_CHANNEL_QUANTITY];



// Globals ---------------------------------------------------------------------
// parameters_typedef * pmem = (parameters_typedef *) (unsigned int *) FLASH_PAGE_FOR_BKP;	//en flash
// parameters_typedef mem_conf;


// Module Private Functions ----------------------------------------------------
void TimingDelay_Decrement(void);
void SysTickError (void);



// Module Functions ------------------------------------------------------------
int main (void)
{
    // Gpio Configuration.
    GpioInit();
    
    // Systick Timer Activation
    if (SysTick_Config(64000))
        SysTickError();

    // Hardware Tests
    // TF_Hardware_Tests ();

    // --- main program inits. ---
    unsigned char buffer [3];
    char my_str [100];
    // init Mem and SPI2
    Lcd_Backlight_Off();
    
    // SPI Init for Mem
    Mem_Ce_Off();
    Mem_Wp_Off();
    SPI2_Config();
    
    // init Usart for debug and Management
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
    ST7796_Fill_Screen(BLACK);
    Lcd_Backlight_On();
    Usart1Send("Lcd inited\r\n");    

    // show presentation
    Wait_ms(200);
    Usart1Send("showing presentation pictures\r\n");
    Pictures_Init();

    // Init ADC with DMA    
    //-- DMA configuration.
    DMA1_Channel1_Config();
    DMA1_CH1_ENABLE;
    
    //-- ADC with DMA
    AdcConfig();
    AdcStart();


    //-- Main Loop --------------------------
    while (1)
    {
        // update comms with main
        Comms_Update ();

        // the update of led and buzzer on Treatment_Manager()
        // UpdateLed();

	// manager
	Manager();
	
	// meas always running
	Temp_Update ();

    }
}

//--- End of Main ---//


// Other Module Functions ------------------------------------------------------
void TimingDelay_Decrement(void)
{
    if (wait_ms_var)
        wait_ms_var--;

    if (timer_standby)
        timer_standby--;

    Comms_Timeouts ();

    Hard_Timeouts ();

    Temp_Timeouts ();

    Manager_Timeouts ();    

}


void SysTickError (void)
{
    //Capture systick error...
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        for (unsigned char i = 0; i < 255; i++)
        {
            asm ("nop \n\t"
                 "nop \n\t"
                 "nop \n\t" );
        }
    }
}

//--- end of file ---//

