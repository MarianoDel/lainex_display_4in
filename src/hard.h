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
// #define HARDWARE_VERSION_2_0    // ch1 included in main brd ver 2.0
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
// PA0 Output LCD_RS
#define TFT_RS    ((GPIOA->ODR & 0x0001) == 0)
#define TFT_RS_DATA    (GPIOA->BSRR = 0x00000001)
#define TFT_RS_CMD    (GPIOA->BSRR = 0x00010000)

// PA1 Output LCD_RST
#define TFT_RST    ((GPIOA->ODR & 0x0002) == 0)
#define TFT_RST_OFF    (GPIOA->BSRR = 0x00000002)
#define TFT_RST_ON    (GPIOA->BSRR = 0x00020000)

// PA2 Analog Channel 2 (V_SENSE_28V)
// PA3 Analog Channel 3 (V_SENSE_25V)

// PA4 Output LCD_CS
#define TFT_CS    ((GPIOA->ODR & 0x0010) == 0)
#define TFT_CS_OFF    (GPIOA->BSRR = 0x00000010)
#define TFT_CS_ON    (GPIOA->BSRR = 0x00100000)

// PA5 Alternative SPI1_SCK
// PA6 Alternative SPI1_MISO
// PA7 Alternative SPI1_MOSI

// PA8 NC

// PA9 PA10 Alternative Usart1 Tx Rx

// PA11 PA12 PA13 PA14 PA15 NC

// PB defines ----
// PB0 Out or Alternative TIM8_CH2N
#define RIGHT    ((GPIOB->ODR & 0x0001) != 0)
#define RIGHT_ON    (GPIOB->BSRR = 0x00000001)
#define RIGHT_OFF    (GPIOB->BSRR = 0x00010000)

// PB1 PB2 NC

// PB3 PB4 PB5 PB6 PB7 NC

// PB9 PB10 NC

// PB10 PB11 Alternative Usart3 Tx Rx

// PB12 NC

// PB13 Input (SYNC_IN)
#define SYNC_IN    ((GPIOB->IDR & 0x2000) != 0)

// PB14 PB15 NC

// PC defines ----
// PC0 
#define LED    ((GPIOC->ODR & 0x0001) != 0)
#define LED_ON    (GPIOC->BSRR = 0x00000001)
#define LED_OFF    (GPIOC->BSRR = 0x00010000)

// PC1 PC2 PC3 NC

// PC4 Analog Channel 14 (NTC_10K)

// PC6 NC

// PC7 Out or Alternative TIM8_CH2
#define LEFT    ((GPIOC->ODR & 0x0080) != 0)
#define LEFT_ON    (GPIOC->BSRR = 0x00000080)
#define LEFT_OFF    (GPIOC->BSRR = 0x00800000)

// PC8 PC9 NC

// PC10 PC11 PC12 NC

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
void HARD_Timeouts (void);

unsigned char Sync_Input_Is_On (void);

unsigned char Led_Is_On (void);
void Led_On (void);
void Led_Off (void);

void Hard_GetVoltages (char * buff);
void Hard_GetHardSoft (char * buff);
void Hard_GetVoltages_Complete (void);

void Lcd_Rst_On (void);
void Lcd_Rst_Off (void);

void Lcd_Rs_Cmd (void);
void Lcd_Rs_Data (void);

void Lcd_Cs_On (void);
void Lcd_Cs_Off (void);

#endif
