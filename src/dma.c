//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### DMA.C #################################
//---------------------------------------------

#include "dma.h"
#include "stm32f10x.h"

#include "adc.h"


// Module Private Types Constants and Macros -----------------------------------
#define RCC_DMA1_CLK    (RCC->AHBENR & RCC_AHBENR_DMA1EN)
#define RCC_DMA1_CLK_ON    (RCC->AHBENR |= RCC_AHBENR_DMA1EN)
#define RCC_DMA1_CLK_OFF    (RCC->AHBENR &= ~RCC_AHBENR_DMA1EN)

// #define RCC_DMA2_CLK    (RCC->AHBENR & RCC_AHBENR_DMA2EN)
// #define RCC_DMA2_CLK_ON    (RCC->AHBENR |= RCC_AHBENR_DMA2EN)
// #define RCC_DMA2_CLK_OFF    (RCC->AHBENR &= ~RCC_AHBENR_DMA2EN)


/* Externals variables ---------------------------------------------------------*/
extern volatile unsigned short adc_ch [];

/* Global variables ---------------------------------------------------------*/


/* Module Definitions ---------------------------------------------------------*/


/* Module functions ---------------------------------------------------------*/
void DMA1_Channel1_Config (void)
{
    /* DMA1 clock enable */
    if (!RCC_DMA1_CLK)
        RCC_DMA1_CLK_ON;

    //Configuro el control del DMA CH1
    DMA1_Channel1->CCR = 0;
    //priority low
    //memory halfword
    //peripheral halfword
    //increment memory
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_0 | DMA_CCR1_MINC;
    //DMA1_Channel1->CCR |= DMA_Mode_Circular | DMA_CCR_TCIE;
    //cicular mode
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;

    //Tamaño del buffer a transmitir
    DMA1_Channel1->CNDTR = ADC_CHANNEL_QUANTITY;

    //Address del periferico
    DMA1_Channel1->CPAR = (uint32_t) &ADC1->DR;

    //Address en memoria
    DMA1_Channel1->CMAR = (uint32_t) &adc_ch[0];

    //Enable
    //DMA1_Channel1->CCR |= DMA_CCR_EN;
#ifdef DMA_WITH_INTERRUPT
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    NVIC_SetPriority(DMA1_Channel1_IRQn, 5);
#endif
}

void DMAEnableInterrupt (void)
{
    DMA1_Channel1->CCR |= DMA_CCR1_TCIE;
}

void DMADisableInterrupt (void)
{
    DMA1_Channel1->CCR &= ~DMA_CCR1_TCIE;
}

#ifdef DMA_WITH_INTERRUPT
void DMA1_Channel1_IRQHandler (void)
{
    if (sequence_ready)
    {
        // Clear DMA TC flag
        sequence_ready_reset;
        
    }
}
#endif


void DMA1_Channel3_Config (unsigned char * pmem, unsigned short mem_size)
{
    // DMA1 clock enable
    if (!RCC_DMA1_CLK)
        RCC_DMA1_CLK_ON;

    //Configuro el control del DMA SPI1 Tx
    DMA1_Channel3->CCR = 0;
    // priority very high
    // memory byte
    // peripheral byte
    // increment memory
    // no increment peripheral
    // memory to peripheral
    // DMA1_Channel3->CCR |= DMA_CCR1_PL_1 |
    //     DMA_CCR1_PL_0 |
    //     DMA_CCR1_MINC |
    //     DMA_CCR1_DIR;

    // priority low
    DMA1_Channel3->CCR |= DMA_CCR1_MINC |
        DMA_CCR1_DIR;
    
    //Tamaño del buffer a transmitir
    DMA1_Channel3->CNDTR = mem_size;

    //Address del periferico
    DMA1_Channel3->CPAR = (uint32_t) &SPI1->DR;

    //Address en memoria
    DMA1_Channel3->CMAR = (uint32_t) pmem;

    //Enable
    DMA1_Channel3->CCR |= DMA_CCR1_EN;
}


void DMA1_Channel4_5_Config (unsigned char * pmem, unsigned char * pdummy, unsigned short mem_size)
{
    // DMA1 clock enable
    if (!RCC_DMA1_CLK)
        RCC_DMA1_CLK_ON;

    //Configuro el control del DMA SPI2 Rx
    DMA1_Channel4->CCR = 0;
    // priority very high
    // memory byte
    // peripheral byte
    // increment memory
    // no increment peripheral
    // peripheral memory
    DMA1_Channel4->CCR |= DMA_CCR1_PL_1 | DMA_CCR1_PL_0 |
        DMA_CCR1_MINC;

    //Tamaño del buffer a recibir
    DMA1_Channel4->CNDTR = mem_size;

    //Address del periferico
    DMA1_Channel4->CPAR = (uint32_t) &SPI2->DR;

    //Address en memoria
    DMA1_Channel4->CMAR = (uint32_t) pmem;
    
    //Enable
    DMA1_Channel4->CCR |= DMA_CCR1_EN;

    //Configuro el control del DMA SPI2 Tx
    DMA1_Channel5->CCR = 0;
    // priority medium
    // memory byte
    // peripheral byte
    // no increment memory
    // no increment peripheral
    // memory to peripheral
    DMA1_Channel5->CCR |= DMA_CCR1_PL_0 |
        DMA_CCR1_DIR;

    //Tamaño del buffer a transmitir
    DMA1_Channel5->CNDTR = mem_size;

    //Address del periferico
    DMA1_Channel5->CPAR = (uint32_t) &SPI2->DR;

    //Address en memoria
    DMA1_Channel5->CMAR = (uint32_t) pdummy;

    //Enable
    DMA1_Channel5->CCR |= DMA_CCR1_EN;
    
}

//---- end of file ----//
