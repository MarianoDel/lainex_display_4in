//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### DMA.H #################################
//---------------------------------------------

#ifndef _DMA_H_
#define _DMA_H_

//--- Defines for configuration ----------------
// #define DMA_WITH_INTERRUPT

//--- Exported Macros ---//
#define sequence_ready         (DMA1->ISR & DMA_ISR_TCIF1)
#define sequence_ready_reset   (DMA1->IFCR = DMA_ISR_TCIF1)

#define DMA1_CH1_ENABLE    (DMA1_Channel1->CCR |= DMA_CCR1_EN)

#define DMA1_CH3_ENABLE    (DMA1_Channel3->CCR |= DMA_CCR1_EN)
#define DMA1_CH3_DISABLE    (DMA1_Channel3->CCR &= ~DMA_CCR1_EN)

//--- Exported constants ---//


//--- Module Functions ---//
void DMA1_Channel1_Config(void);
void DMAEnableInterrupt (void);
void DMADisableInterrupt (void);
void DMA1_Channel1_IRQHandler (void);

// void DMA1_Channel3_Config (void);
void DMA1_Channel3_Config (unsigned char * pmem, unsigned short mem_size);
void DMA1_Channel4_5_Config (unsigned char * pmem, unsigned char * pdummy, unsigned short mem_size);


#endif /* _DMA_H_ */
