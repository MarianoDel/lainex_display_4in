//----------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEMPERATURES.C ###############################
//----------------------------------------------------

// Includes --------------------------------------------------------------------
#include "temperatures.h"
#include "dsp.h"
#include "adc.h"


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch[];


// Globals ---------------------------------------------------------------------
volatile unsigned short temp_timeout = 0;
ma32_u16_data_obj_t temp1_filter;
ma32_u16_data_obj_t temp2_filter;
ma32_u16_data_obj_t temp3_filter;
unsigned short temp1_filtered = 0;
unsigned short temp2_filtered = 0;
unsigned short temp3_filtered = 0;


// Module Private Types & Macros -----------------------------------------------
enum Temp_States {
    TEMP_INIT,
    TEMP_WAITING_1,
    TEMP_WAITING_2,
    TEMP_WAITING_3    

};
    

// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Temp_Timeouts (void)
{
    if (temp_timeout)
	temp_timeout--;
}

enum Temp_States temp_state = TEMP_INIT;
void Temp_Update (void)
{
    switch (temp_state)
    {
    case TEMP_INIT:
	MA32_U16Circular_Reset (&temp1_filter);
	MA32_U16Circular_Reset (&temp2_filter);
	MA32_U16Circular_Reset (&temp3_filter);

	for (int i = 0; i < 32; i++)
	{
	    MA32_U16Circular (&temp1_filter, Sense_Temp_1);
	    MA32_U16Circular (&temp2_filter, Sense_Temp_2);
	    MA32_U16Circular (&temp3_filter, Sense_Temp_3);
	}

	temp_state++;
	break;

    case TEMP_WAITING_1:
	if (temp_timeout)
	    break;

	temp1_filtered = MA32_U16Circular (&temp1_filter, Sense_Temp_1);
	temp_timeout = 300;
	temp_state++;
	break;

    case TEMP_WAITING_2:
	if (temp_timeout)
	    break;

	temp2_filtered = MA32_U16Circular (&temp2_filter, Sense_Temp_2);
	temp_timeout = 300;
	temp_state++;
	break;

    case TEMP_WAITING_3:
	if (temp_timeout)
	    break;

	temp3_filtered = MA32_U16Circular (&temp3_filter, Sense_Temp_3);
	temp_timeout = 300;
	temp_state = TEMP_WAITING_1;
	break;

    default:
	temp_state = TEMP_INIT;
	break;
    }
}


unsigned short Temp_Temp1 (void)
{
    return temp1_filtered;
}


unsigned short Temp_Temp2 (void)
{
    return temp2_filtered;
}


unsigned short Temp_Temp3 (void)
{
    return temp3_filtered;
}


// unsigned char Temp_TempToDegrees (unsigned short temp)
// {
// #if (defined TEMP_SENSOR_LM335)
//     if (temp < TEMP_IN_MIN)
//         return TEMP_DEG_MIN;

//     if (temp > TEMP_IN_MAX)
//         return TEMP_DEG_MAX;
// #elif (defined TEMP_SENSOR_NTC1K)
//     if (temp > TEMP_IN_MIN)
//         return TEMP_DEG_MIN;

//     if (temp < TEMP_IN_MAX)
//         return TEMP_DEG_MAX;
// #else
// #error "No sensor selected on temperatures.h"
// #endif
    
//     int calc = 0;
//     short dx = TEMP_IN_MAX - TEMP_IN_MIN;
//     short dy = TEMP_DEG_MAX - TEMP_DEG_MIN;

//     calc = temp * dy;
//     calc = calc / dx;

//     calc = calc - TEMP_DEG_OFFSET;

//     return (unsigned char) calc;
    
// }


// unsigned char Temp_TempToDegreesExtended (unsigned short temp)
// {
// #if (defined TEMP_SENSOR_LM335)

//     int calc = 0;
//     short dx = TEMP_IN_85 - TEMP_IN_30;
//     short dy = 85 - 30;

//     calc = temp * dy;
//     calc = calc / dx;

//     if (calc > (TEMP_DEG_OFFSET - 13))    //seria mayor a 0 grados
//         calc = calc - TEMP_DEG_OFFSET + 13;    //prob +13 es la dif entre LM335 y LM35
//     else
//         calc = 0;

// #elif (defined TEMP_SENSOR_NTC1K)

//     int calc = 0;
//     short dx = TEMP_IN_MAX - TEMP_IN_MIN;
//     short dy = TEMP_DEG_MAX - TEMP_DEG_MIN;

//     calc = temp * dy;
//     calc = calc / dx;

//     calc = calc - TEMP_DEG_OFFSET;
    
// #else
// #error "No sensor selected on temperatures.h"
// #endif

//     return (unsigned char) calc;
// }


// unsigned short Temp_DegreesToTemp (unsigned char deg)
// {
//     if (deg < TEMP_DEG_MIN)
//         return TEMP_IN_MIN;

//     if (deg > TEMP_DEG_MAX)
//         return TEMP_IN_MAX;
    
//     int calc = 0;
//     short dx = TEMP_DEG_MAX - TEMP_DEG_MIN;
//     short dy = TEMP_IN_MAX - TEMP_IN_MIN;

//     calc = deg * dy;
//     calc = calc / dx;

//     calc = calc + TEMP_IN_OFFSET;
        
//     return (unsigned short) calc;
    
// }

//--- end of file ---//
