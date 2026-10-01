//---------------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMMS.C ###########################################
//---------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "comms.h"
#include "answers_defs.h"
#include "hard.h"
#include "tim.h"

#include "usart.h"

#include "sst25.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Module Private Types Constants and Macros -----------------------------------
char s_ans_ok [] = {"ok\r\n"};
char s_ans_nok [] = {"nok\r\n"};
#define SIZEOF_LOCAL_BUFF    128


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
char local_buff [SIZEOF_LOCAL_BUFF];
volatile unsigned short comms_receiv_timeout = 0;


// Module Private Functions ----------------------------------------------------
static void Comms_Messages (char * msg_str);
void Comms_ReadAddress (unsigned int addr, unsigned int size);
void Comms_WriteToMem (unsigned int addr, unsigned int bytes);
void Comms_Freeing_Pages_From_Addr (unsigned int addr, unsigned int bytes);
void Comms_UpdateCommsInTextTxRx (unsigned char * answer);


// Module Functions ------------------------------------------------------------
void Comms_Timeouts (void)
{
    if (comms_receiv_timeout)
	comms_receiv_timeout--;
}


void Comms_Update (void)
{
    if (Usart1HaveData())
    {
        Usart1HaveDataReset();
        Led_On();
        Usart1ReadBuffer(local_buff, SIZEOF_LOCAL_BUFF);
        Comms_Messages(local_buff);
        Led_Off();
    }
}


static void Comms_Messages (char * msg_str)
{
    resp_e resp;
    char buff[100];
    
    char * msg = msg_str;
    
    if (!strncmp(msg, "read_from_mem", sizeof("read_from_mem") - 1))
    {
        memset(buff, 0, sizeof(buff));
	msg += sizeof("read_from_mem");		//normalizo al payload, hay un espacio
	
        //aca llega el address que quieren leer
        for (unsigned char i = 0; i < sizeof(buff); i++)
        {
            if (*(msg + i) != ' ')
                buff[i] = *(msg + i);
            else
            {
                msg += i + 1;    //hay un espacio
                i = sizeof(buff);
            }
        }

	// int address = atoi(buff);
	unsigned int address = (unsigned int) strtol(buff, NULL, 0);	
        // sprintf(b_vect, "a: %x ", address);
        // Usart1Send(b_vect);
        
        memset(buff, 0, sizeof(buff));

        //aca llega la cantidad de bytes a leer
        for (unsigned char i = 0; i < sizeof(buff); i++)
        {
            if (*(msg + i) != ' ')
                buff[i] = *(msg + i);
            else
            {
                msg += i + 1;    //hay un espacio
                i = sizeof(buff);
            }
        }
	
        unsigned int bytes_to_read = atoi(buff);
        // sprintf(b_vect, "b: %x\n", bytes_to_read);
        // Usart1Send(b_vect);
        
        if ((address + bytes_to_read) < SST25_SIZE)    // 16Mbit device
        {
            Comms_ReadAddress(address, bytes_to_read);
            resp = resp_ok;
        }

        if (resp == resp_ok)
            Usart1Send (s_ans_ok);
        else
            Usart1Send (s_ans_nok);
    }

    //-- Write Actions
    else if (strncmp(msg, "write_file_to_mem", sizeof("write_file_to_mem") - 1) == 0)
    {
        memset(buff, 0, sizeof(buff));
	msg += sizeof("write_file_to_mem");		//normalizo al payload, hay un espacio
	
        // address to write
        for (unsigned char i = 0; i < sizeof(buff); i++)
        {
            if (*(msg + i) != ' ')
                buff[i] = *(msg + i);
            else
            {
                msg += i + 1;    //hay un espacio
                i = sizeof(buff);
            }
        }

	// int address = atoi(buff);
	unsigned int address = (unsigned int) strtol(buff, NULL, 0);	
        // sprintf(b_vect, "a: %x ", address);
        // Usart1Send(b_vect);
        
        memset(buff, 0, sizeof(buff));

        //aca llega la cantidad de bytes a guardar
        for (unsigned char i = 0; i < sizeof(buff); i++)
        {
            if (*(msg + i) != ' ')
                buff[i] = *(msg + i);
            else
            {
                msg += i + 1;    //hay un espacio
                i = sizeof(buff);
            }
        }
	
        unsigned int bytes_to_write = atoi(buff);
        // sprintf(b_vect, "b: %x\n", bytes_to_read);
        // Usart1Send(b_vect);
        
        if ((address + bytes_to_write) < SST25_SIZE)    // 16Mbit device
        {
	    Comms_Freeing_Pages_From_Addr(address, bytes_to_write);
            Comms_WriteToMem(address, bytes_to_write);
            resp = resp_ok;
        }

        if (resp == resp_ok)
            Usart1Send (s_ans_ok);
        else
            Usart1Send (s_ans_nok);


	// Usart1Send("go to binary\n");
        // resp = COMM_WriteAllMem();
        // if (resp == resp_timeout)
        //     Usart1Send("Timeout\n");
        
    }
    
    // else if (!strncmp(msg, "square frequency", sizeof("square frequency") - 1))
    // {
    //     resp = Treatment_SetFrequency_Str (MODE_SQUARE, msg + sizeof("square frequency"));
    //     if (resp == resp_ok)
    //         Usart1Send (s_ans_ok);
    //     else
    //         Usart1Send (s_ans_nok);
    // }
    
    // else if (!strncmp(msg, "sine intensity", sizeof("sine intensity") - 1))
    // {
    //     resp = Treatment_SetIntensity_Str (MODE_SINE, msg + sizeof("sine intensity"));
    //     if (resp == resp_ok)
    //         Usart1Send (s_ans_ok);
    //     else
    //         Usart1Send (s_ans_nok);
    // }

    // else if (!strncmp(msg, "square intensity", sizeof("square intensity") - 1))
    // {
    //     resp = Treatment_SetIntensity_Str (MODE_SQUARE, msg + sizeof("square intensity"));
    //     if (resp == resp_ok)
    //         Usart1Send (s_ans_ok);
    //     else
    //         Usart1Send (s_ans_nok);
    // }
    
    // else if (!strncmp(msg, "polarity", sizeof("polarity") - 1))
    // {
    //     resp = Treatment_SetPolarity_Str (msg + sizeof("polarity"));
    //     if (resp == resp_ok)
    //         Usart1Send (s_ans_ok);
    //     else
    //         Usart1Send (s_ans_nok);
    // }

    // // config messages for channel setup
    // // else if (!strncmp(msg, "mode", sizeof("mode") - 1))
    // // {
    // //     resp = Treatment_SetMode_Str (msg + sizeof("mode"));
    // //     if (resp == resp_ok)
    // //         Usart1Send (s_ans_ok);
    // //     else
    // //         Usart1Send (s_ans_nok);
    // // }

    // else if (!strncmp(msg, "threshold", sizeof("threshold") - 1))
    // {
    //     resp = Treatment_SetThreshold_Str (msg + sizeof("threshold"));
    //     if (resp == resp_ok)
    //         Usart1Send (s_ans_ok);
    //     else
    //         Usart1Send (s_ans_nok);
    // }

    // // -- operation messages --
    // else if (!strncmp(msg, "square start", sizeof("square start") - 1))
    // {
    //     Treatment_Start (MODE_SQUARE);
    //     Usart1Send (s_ans_ok);
    // }

    // else if (!strncmp(msg, "sine start", sizeof("sine start") - 1))
    // {
    //     Treatment_Start (MODE_SINE);
    //     Usart1Send (s_ans_ok);
    // }

    // else if (!strncmp(msg, "stop", sizeof("stop") - 1))
    // {
    //     Treatment_Stop ();
    //     Usart1Send (s_ans_ok);
    // }

    // // -- measures messages --
    // if (!strncmp(msg, "set_gain", sizeof("set_gain") - 1))
    // {
    //     resp = Treatment_SetGain_Str (msg + sizeof("set_gain"));
    //     if (resp == resp_ok)
    //         Usart1Send (s_ans_ok);
    //     else
    //         Usart1Send (s_ans_nok);
    // }

    // if (!strncmp(msg, "get_gain", sizeof("get_gain") - 1))
    // {
    //     char buff [50];
    //     unsigned char gain;
    //     gain = Treatment_GetGain ();
    //     sprintf(buff, "gain: %d\n", gain);
    //     Usart1Send(buff);
    // }

    // // -- board messages --
    // if (!strncmp(msg, "voltages", sizeof("voltages") - 1))
    // {
    //     // char buff [50];
    //     // Hard_GetVoltages (buff);
    //     // Usart1Send(buff);

    //     Hard_GetVoltages_Complete ();
    // }

    // if (!strncmp(msg, "hard_soft", sizeof("hard_soft") - 1))
    // {
    //     char buff [50];
    //     Hard_GetHardSoft (buff);
    //     Usart1Send(buff);
    // }

}


// Comms Utils Functions ------------------------------------------------------
#define SIZEOF_MEM_BUFFER    16
void Comms_ReadAddress (unsigned int addr, unsigned int bytes)
{
    unsigned char data [SIZEOF_MEM_BUFFER] = { 0 };
    char string_data [100] = { 0 };
    unsigned int orig_addr = addr;
    
    Wait_ms(300);

    do {
	readBufNVM8u(data, SIZEOF_MEM_BUFFER, addr);
        // for (unsigned char i = 0; i < SIZEOF_MEM_BUFFER; i++)
        // {
        //     data[i] = MEM_ReadByte(addr + i);
        // }

        sprintf(string_data, "addr: %d data: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n",
                addr,
                data[0],
                data[1],
                data[2],
                data[3],
                data[4],
                data[5],
                data[6],
                data[7],
                data[8],
                data[9],
                data[10],
                data[11],
                data[12],
                data[13],
                data[14],
                data[15]);
                
        Usart1Send(string_data);
        addr += SIZEOF_MEM_BUFFER;
        Wait_ms(100);

    } while (addr < (orig_addr + bytes));
}


typedef enum {
    RECEIV_STATE_INIT,
    RECEIV_STATE_WAITING_NEXT,
    RECEIV_STATE_GETTING

} receiv_state_t;

#define NEXT 0x01
#define STOP 0x02
#define ENDED 0x04
#define TT_BINARY    30000
unsigned char membuff[SST25_SIZE_1K];
void Comms_WriteToMem (unsigned int addr, unsigned int bytes)
{
    resp_e resp = resp_continue;
    unsigned int getted = 0;
    unsigned char comms_receiv_answer = 0;
    receiv_state_t comms_receiv_state = RECEIV_STATE_INIT;
    char s_ended [40] = { 0 };

    //wait for receiv_next or timeout
    while (resp == resp_continue)
    {
        switch (comms_receiv_state)
        {
        case RECEIV_STATE_INIT:
	    Usart1Send("go to binary\n");
            comms_receiv_timeout = TT_BINARY;
            comms_receiv_state = RECEIV_STATE_WAITING_NEXT;
            break;

        case RECEIV_STATE_WAITING_NEXT:
            
            comms_receiv_answer = 0;
            Comms_UpdateCommsInTextTxRx(&comms_receiv_answer);

            if (comms_receiv_answer & NEXT)
            {
                Usart1Send("n\n");
                Usart1ToBinary(membuff, SST25_SIZE_1K);
                comms_receiv_timeout = TT_BINARY;
                comms_receiv_state = RECEIV_STATE_GETTING;
            }

            if (comms_receiv_answer & ENDED)
                resp = resp_ok;

            if (!comms_receiv_timeout)
	    {
		Usart1Send("timeout on waiting next\r\n");
                resp = resp_timeout;
	    }
            break;

        case RECEIV_STATE_GETTING:
	    if (Usart1HaveData())
	    {
                //tengo todo el membuff
		Usart1HaveDataReset();

		writeBufferNVM(membuff, SST25_SIZE_1K, addr);
            
		addr += SST25_SIZE_1K;
		getted += SST25_SIZE_1K;
		char mybuff [50];
		sprintf(mybuff, "getted: %d accum: %d\r\n", UsartIntPtrPos(), getted);
		Usart1ToText();
		Usart1Send(mybuff);
		// Usart1ToText();
		Usart1Send(".\n");
		comms_receiv_timeout = TT_BINARY;
		comms_receiv_state = RECEIV_STATE_WAITING_NEXT;
	    }

            if (!comms_receiv_timeout)
	    {
		char mybuff [50];
		sprintf(mybuff, "timeout on getting bytes: %d\r\n", UsartIntPtrPos());
		Usart1ToText();
		Usart1Send(mybuff);
                resp = resp_timeout;
	    }

            break;

        default:
            comms_receiv_state = RECEIV_STATE_INIT;
            break;
        }

    }    //end while resp == resp_continue

    Usart1ToText();
    if (resp == resp_timeout)
    {
	Usart1Send("Timeout on binary reception\r\n");
    }
    else
    {
	sprintf(s_ended, "Rx: %d Svd: %d\r\n", getted, getted);
	Usart1Send(s_ended);	
    }
}


void Comms_Freeing_Pages_From_Addr (unsigned int addr, unsigned int bytes)
{
    char my_str [100];
    unsigned int mem_pages = 0;

    mem_pages = bytes >> 12;
    mem_pages += 1;
    
    sprintf(my_str, "freeing pages at addr: 0x%06x size ask: %d pages: %d\r\n",
	    addr,
	    bytes,
	    mem_pages);
    Usart1Send(my_str);

    unsigned int bytes_copy = 0;
    for (int i = 0; i < mem_pages; i++)
    {
	unsigned int a = 0;
	a = addr + SST25_SIZE_4K * i;
	Clear4KNVM(a);
	bytes_copy += SST25_SIZE_4K;
    }
    sprintf(my_str, "bytes cleared hex: 0x%06x bytes: %d\r\n",
	    bytes_copy,
	    bytes_copy);

    Usart1Send(my_str);
    Usart1Send("done!\r\n");
}


void Comms_UpdateCommsInTextTxRx (unsigned char * answer)
{
    if (Usart1HaveData())
    {
	unsigned char read = 0;
	Usart1HaveDataReset();
        read = Usart1ReadBuffer(local_buff, SIZEOF_LOCAL_BUFF);

        //espero solo un next ended o un stop
        char * pStr = local_buff;
        if (strncmp(pStr, "next", sizeof("next") - 1) == 0)
        {
            *answer |= NEXT;
        }

        else if (strncmp(pStr, "stop", sizeof("stop") - 1) == 0)
        {
            *answer |= STOP;
        }

        else if (strncmp(pStr, "ended", sizeof("ended") - 1) == 0)
        {
            *answer |= ENDED;
        }
	
	else
	{
	    sprintf(local_buff, "some alignment error bytes: %d\r\n", read);
	    Usart1Send(local_buff);
	    // Usart1Send(pStr);
	}
    }
}

//---- End of File ----//
