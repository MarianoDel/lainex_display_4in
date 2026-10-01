//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### SPI.C #################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "spi.h"
#include "stm32f10x.h"
// #include "hard.h"
#include "dma.h"

#include <stdio.h>
#include <string.h>


// Module Private Types Constants and Macros -----------------------------------
//Clock Peripherals
#define RCC_SPI1_CLK    (RCC->APB2ENR & 0x00001000)
#define RCC_SPI1_CLK_ON    (RCC->APB2ENR |= 0x00001000)
#define RCC_SPI1_CLK_OFF    (RCC->APB2ENR &= ~0x00001000)

#define RCC_SPI2_CLK    (RCC->APB1ENR & 0x00004000)
#define RCC_SPI2_CLK_ON    (RCC->APB1ENR |= 0x00004000)
#define RCC_SPI2_CLK_OFF    (RCC->APB1ENR &= ~0x00004000)

#define RCC_SPI3_CLK    (RCC->APB1ENR & 0x00008000)
#define RCC_SPI3_CLK_ON    (RCC->APB1ENR |= 0x00008000)
#define RCC_SPI3_CLK_OFF    (RCC->APB1ENR &= ~0x00008000)


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------
unsigned char SPI1_DMA_Check_Free (void);


// Module Exported Functions ---------------------------------------------------
////////////////////
// SPI1 Functions //
////////////////////
void SPI1_Config(void)
{
    if (!RCC_SPI1_CLK)
        RCC_SPI1_CLK_ON;

    //Configuracion SPI
    SPI1->CR1 = 0;

#if defined SPI_MASTER
    //SPI speed; clk / 256; master
    // SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2;
    //SPI speed; clk / 16; master
    // SPI1->CR1 |=  SPI_CR1_BR_1 | SPI_CR1_BR_0;
    //SPI speed; clk / 8; master
    // SPI1->CR1 |=  SPI_CR1_BR_1;
    //SPI speed; clk / 4; master
    SPI1->CR1 |=  SPI_CR1_MSTR | SPI_CR1_BR_0;
#elif defined SPI_SLAVE
    //SPI speed; clk / 256; slave
    SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 |SPI_CR1_SSM;
#else
#error "Select Peripheral use on spi.h"
#endif

#ifdef SPI_DATA_VALID_ON_FALLING_CLK
    //CPOL High; CPHA first clock
    SPI1->CR1 |= SPI_CR1_CPOL | SPI_CR1_SSM | SPI_CR1_SSI;
#endif
#ifdef SPI_DATA_VALID_ON_RISING_CLK
    //CPOL High; CPHA second clock
    SPI1->CR1 |= SPI_CR1_CPOL | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_CPHA;
#endif
#ifdef SPI_DATA_VALID_ON_RISING_CLK_DIRECT_POLARITY
    //CPOL Low; CPHA on riding
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
#endif

    // peripheral enable
    SPI1->CR1 |= SPI_CR1_SPE;    
}


unsigned char SPI1_Send_Receive (unsigned char a)
{
    // wait for transmit buffer empty
    while ((SPI1->SR & SPI_SR_TXE) == 0);

    *(__IO uint8_t *) ((uint32_t)SPI1 + (uint32_t)0x0C) = a;    // compiler problems

    // wait for received data
    while ((SPI1->SR & SPI_SR_RXNE) == 0);

    return (SPI1->DR & 0x00FF);
}


void SPI1_Wait_Clk256 (void)
{
    for (unsigned char i = 0; i < 15; i++)
        asm ("nop \n\t");

}


void SPI1_Busy_Wait (void)
{
    // wait for complete transfer
    while ((SPI1->SR & SPI_SR_BSY) != 0);
}


void SPI1_Send_Array (unsigned char * data, unsigned short size)
{
    for (int i = 0; i < size; i++)
	SPI1_Send_Multiple(*(data + i));

    SPI1_Busy_Wait ();
}


void SPI1_Send_Multiple (unsigned char a)
{
    //espero que haya lugar en el buffer
    while ((SPI1->SR & SPI_SR_TXE) == 0);

    //*(__IO uint8_t *) SPI1->DR = a;
    *(__IO uint8_t *) ((uint32_t)SPI1 + (uint32_t)0x0C) = a; //evito enviar 16bits problemas de compilador

}


void SPI1_Send_Single (unsigned char a)
{
    //espero que se libere el buffer
    while ((SPI1->SR & SPI_SR_TXE) == 0);

    //tengo espacio
    //SPI1->DR = a;
    //SPI1->DR = a;
    *(__IO uint8_t *) ((uint32_t)SPI1 + (uint32_t)0x0C) = a; //evito enviar 16bits problemas de compilador

    //espero que se transfiera el dato
    while ((SPI1->SR & SPI_SR_BSY) != 0);
}


unsigned char SPI1_Receive_Single (void)
{
    unsigned char dummy;

    //espero que se libere el buffer
    while (((SPI1->SR & SPI_SR_TXE) == 0) || ((SPI1->SR & SPI_SR_BSY) != 0));

    //limpio buffer RxFIFO
    while ((SPI1->SR & SPI_SR_RXNE) != 0)
        dummy = SPI1->DR;

    *(__IO uint8_t *) ((uint32_t)SPI1 + (uint32_t)0x0C) = 0xff; //evito enviar 16bits problemas de compilador

    //espero que se transfiera el dato
    while ((SPI1->SR & SPI_SR_BSY) != 0);

    dummy = (unsigned char) SPI1->DR;
    return dummy;
}


void SPI1_DMA_Send_Array (unsigned char * data, unsigned short size)
{
    if (!SPI1_DMA_Check_Free())
	return;

    // activate DMA
    DMA1_Channel3_Config (data, size);

    // activate DMA Tx bit transfer
    SPI1->CR2 |= SPI_CR2_TXDMAEN;
    
}


unsigned char SPI1_DMA_Check_Free (void)
{
    if ((SPI1->SR & SPI_SR_TXE) &&
	!(SPI1->SR & SPI_SR_BSY))
	return 1;

    return 0;
}


void SPI1_DMA_Disable (void)
{
    // deactivate DMA Tx bit transfer
    SPI1->CR2 &= ~(SPI_CR2_TXDMAEN);
}


////////////////////
// SPI2 Functions //
////////////////////
void SPI2_Config(void)
{
    //Habilitar Clk
    if (!RCC_SPI2_CLK)
        RCC_SPI2_CLK_ON;

    //Configuracion SPI
    SPI2->CR1 = 0;    

    //SPI speed; clk / 32; master
    // SPI2->CR1 |=  SPI_CR1_MSTR | SPI_CR1_BR_2;

    //SPI speed; clk / 16; master
    // SPI2->CR1 |=  SPI_CR1_MSTR | SPI_CR1_BR_1 | SPI_CR1_BR_0;
    
    //SPI speed; clk / 8; master
    // SPI2->CR1 |=  SPI_CR1_MSTR | SPI_CR1_BR_1;
    
    //SPI speed; clk / 4; master
    // SPI2->CR1 |=  SPI_CR1_MSTR | SPI_CR1_BR_0;
    
    //SPI speed; clk / 2; master
    SPI2->CR1 |=  SPI_CR1_MSTR;

    //CPOL High; CPHA second clock (original)
    SPI2->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_SSM | SPI_CR1_SSI;

    SPI2->CR1 |= SPI_CR1_SPE;		//habilito periferico
}


void SPI2_Send_Single (unsigned char a)
{
    //espero que se libere el buffer
    while ((SPI2->SR & SPI_SR_TXE) == 0);

    *(__IO uint8_t *) ((uint32_t)SPI2 + (uint32_t)0x0C) = a; //evito enviar 16bits problemas de compilador

    //espero que se transfiera el dato
    while ((SPI2->SR & SPI_SR_BSY) != 0);
}


// unsigned char SPI_Send_Receive (unsigned char a)
// {
//     unsigned char dummy;

//     //primero limpio buffer rx spi
//     while ((SPI1->SR & SPI_SR_RXNE) == 1)
//     {
//         dummy = SPI1->DR & 0x0F;
//     }

//     //espero que haya lugar en el buffer
//     while ((SPI1->SR & SPI_SR_TXE) == 0);

//     *(__IO uint8_t *) ((uint32_t)SPI1 + (uint32_t)0x0C) = a; //evito enviar 16bits problemas de compilador

//     //espero tener el dato en RX
//     for (unsigned char j = 0; j < 150; j++)
//     {
//     	asm("nop");
//     }

//     dummy = SPI1->DR & 0x0F;
//     return dummy;
// }


// void SPI_Busy_Wait (void)
// {
//     //espero que se transfiera el dato
//     while ((SPI1->SR & SPI_SR_BSY) != 0);
// }


// void SPI_Send_Multiple (unsigned char a)
// {
//     //espero que haya lugar en el buffer
//     while ((SPI1->SR & SPI_SR_TXE) == 0);

//     //*(__IO uint8_t *) SPI1->DR = a;
//     *(__IO uint8_t *) ((uint32_t)SPI1 + (uint32_t)0x0C) = a; //evito enviar 16bits problemas de compilador

// }




unsigned char SPI2_Receive_Single (void)
{
    unsigned char dummy;

    //espero que se libere el buffer
    while (((SPI2->SR & SPI_SR_TXE) == 0) || ((SPI2->SR & SPI_SR_BSY) != 0));

    //limpio buffer RxFIFO
    while ((SPI2->SR & SPI_SR_RXNE) != 0)
        dummy = SPI2->DR;

    *(__IO uint8_t *) ((uint32_t)SPI2 + (uint32_t)0x0C) = 0xff; //evito enviar 16bits problemas de compilador

    //espero que se transfiera el dato
    while ((SPI2->SR & SPI_SR_BSY) != 0);

    dummy = (unsigned char) SPI2->DR;
    return dummy;
}


volatile unsigned char dummy = 0;
void SPI2_DMA_Rx_Array (unsigned char * data, unsigned short size)
{
    // empty rx
    dummy = (unsigned char) SPI2->DR;
    
    while (!SPI2_DMA_Rx_Check_Free());

    // activate DMA for Rx on SPI2
    DMA1_Channel4_5_Config (data,
			    (unsigned char *) &dummy, size);

    // activate DMA Rx bit transfer
    SPI2->CR2 |= SPI_CR2_RXDMAEN;

    // activate DMA Tx bit transfer
    SPI2->CR2 |= SPI_CR2_TXDMAEN;
    
}


unsigned char SPI2_DMA_Rx_Check_Free (void)
{
    if ((SPI2->SR & SPI_SR_TXE) &&
	!(SPI2->SR & SPI_SR_BSY) &&
	!(SPI2->SR & SPI_SR_RXNE))
	return 1;

    return 0;
}


void SPI2_DMA_RxDisable (void)
{
    // deactivate DMA Tx bit transfer
    SPI2->CR2 &= ~(SPI_CR2_TXDMAEN);

    // deactivate DMA Rx bit transfer
    SPI2->CR2 &= ~(SPI_CR2_RXDMAEN);
    
}


//---- End of File ----//
