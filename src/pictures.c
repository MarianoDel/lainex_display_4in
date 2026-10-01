//---------------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### PICTURES.C ########################################
//---------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "pictures.h"
#include "tim.h"
#include "usart.h"

#include "st7796.h"
#include "sst25.h"

// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>


// Module Private Types Constants and Macros -----------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
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
void Pictures_Init (void)
{
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
}

//---- End of File ----//
