//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.H #################################
//---------------------------------------------

#ifndef HARD_H_
#define HARD_H_


//----------- Defines For Configuration -------------

//----- Board Configuration -------------------//
//--- Hardware ------------------//
#define HARDWARE_VERSION_1_0    // first prototype


//--- Software ------------------//
#define FIRMWARE_VERSION_1_0    // init version


//-------- Type of Program (depending on software version) ----------------


//--- Serial Number / Device Id Bytes length ----------
#define USE_DEVICE_ID_4BYTES
// #define USE_DEVICE_ID_12BYTES


//-------- Oscillator and Crystal selection (Freq in startup_clocks.h) ---
#define HSI_INTERNAL_RC
// #define HSE_CRYSTAL_OSC

#ifdef HSE_CRYSTAL_OSC
// #define CRYSTAL_8MHZ
#define CRYSTAL_12MHZ
#endif

#ifdef HSE_CRYSTAL_OSC
// #define SYSCLK_FREQ_72MHz
#define SYSCLK_FREQ_8MHz
#endif

#ifdef HSI_INTERNAL_RC
#define SYSCLK_FREQ_64MHz
// #define SYSCLK_FREQ_8MHz
#endif

//-------- End Of Defines For Configuration ------




//--- Hardware & Software Messages ------------------//
#ifdef HARDWARE_VERSION_1_0
#define HARD "Hardware Version: 1.0"
#endif
#ifdef FIRMWARE_VERSION_1_0
#define SOFT "Firmware Version: 1.0"
#endif
//--- End of Hardware & Software Messages ------------------//



// Exported Types --------------------------------------------------------------
#ifdef HARDWARE_VERSION_1_0

// PA defines ----
// PA0 NC

// PA1 Output #LCD_RST
#define TFT_RST    ((GPIOA->ODR & 0x0002) == 0)
#define TFT_RST_OFF    (GPIOA->BSRR = 0x00000002)
#define TFT_RST_ON    (GPIOA->BSRR = 0x00020000)

// PA2  Output LCD_RS
#define TFT_RS    ((GPIOA->ODR & 0x0004) == 0)
#define TFT_RS_DATA    (GPIOA->BSRR = 0x00000004)
#define TFT_RS_CMD    (GPIOA->BSRR = 0x00040000)

// PA3 Output #LCD_CS
#define TFT_CS    ((GPIOA->ODR & 0x0008) == 0)
#define TFT_CS_OFF    (GPIOA->BSRR = 0x00000008)
#define TFT_CS_ON    (GPIOA->BSRR = 0x00080000)

// PA4 NC

// PA5 Alternative SPI1_SCK
// PA6 Alternative SPI1_MISO
// PA7 Alternative SPI1_MOSI

// PA8 NC

// PA9 PA10 Alternative Usart1 Tx Rx

// PA11 PA12 PA13 PA14 PA15 NC

// PB defines ----
// PB0 input pullup
#define DET_AC3    ((GPIOB->IDR & 0x0001) == 0)

// PB1 PB2 NC

// PB3 PB4 PB5 PB6 NC

// PB7 CTP_RST

// PB8 PB9 PB10 NC

// PB10 PB11 Alternative i2c CTP_SCL CTP_SDA

// PB12 LED
#define LED    ((GPIOB->ODR & 0x1000) != 0)
#define LED_ON    (GPIOB->BSRR = 0x00001000)
#define LED_OFF    (GPIOB->BSRR = 0x10000000)

// PB13 PB14 PB15 alternative SPI2 SPI2_SCK SPI2_MISO SPI2_MOSI


// PC defines ----
// PC0 Analog Channel 10 (NTC_10K) TEMP_1

// PC1 Analog Channel 11 (NTC_10K) TEMP_2

// PC2 Analog Channel 12 (NTC_10K) TEMP_3

// PC3 NC

// PC4 input pullup
#define DET_AC1    ((GPIOC->IDR & 0x0010) == 0)

// PC5 input pullup
#define DET_AC2    ((GPIOC->IDR & 0x0020) == 0)

// PC6 #CE
#define CE    ((GPIOC->ODR & 0x0040) == 0)
#define CE_OFF    (GPIOC->BSRR = 0x00000040)
#define CE_ON    (GPIOC->BSRR = 0x00400000)

// PC7 #WP
#define WP    ((GPIOC->ODR & 0x0080) == 0)
#define WP_OFF    (GPIOC->BSRR = 0x00000080)
#define WP_ON    (GPIOC->BSRR = 0x00800000)

// PC8 PC9 NC

// PC10 #SD_CS
#define SD_CS    ((GPIOC->ODR & 0x0400) == 0)
#define SD_CS_OFF    (GPIOC->BSRR = 0x00000400)
#define SD_CS_ON    (GPIOC->BSRR = 0x04000000)

// PC11 NC

// PC12 BACKLIGHT
#define TFT_BACKLIGHT    ((GPIOC->ODR & 0x1000) == 0)
#define TFT_BACKLIGHT_ON    (GPIOC->BSRR = 0x00001000)
#define TFT_BACKLIGHT_OFF    (GPIOC->BSRR = 0x10000000)

// PC13 PC14 PC15 NC

// PD defines ----
// PD0 PD1 PD2 NC

#endif //HARDWARE_VERSION_1_0



// LED states (how many blinks)
#define LED_NO_BLINKING    0     
#define LED_TREATMENT_STANDBY    1
#define LED_TREATMENT_SQUARE_RUNNING    2
#define LED_TREATMENT_SINE_RUNNING    3



//--- Exported Module Functions ----
void ChangeLed (unsigned char how_many);
void ChangeLed_With_Timer (unsigned char how_many, unsigned short led_timer_off);
void UpdateLed (void);
void Hard_Timeouts (void);

unsigned char Led_Is_On (void);
void Led_On (void);
void Led_Off (void);

void Hard_GetHardSoft (char * buff);

void Lcd_Rst_On (void);
void Lcd_Rst_Off (void);

void Lcd_Rs_Cmd (void);
void Lcd_Rs_Data (void);

void Lcd_Cs_On (void);
void Lcd_Cs_Off (void);

void Lcd_Backlight_On (void);
void Lcd_Backlight_Off (void);

void Mem_Ce_On (void);
void Mem_Ce_Off (void);
void Mem_Wp_On (void);
void Mem_Wp_Off (void);

#endif
