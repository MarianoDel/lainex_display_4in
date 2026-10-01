//---------------------------------------------
// Simple C functions tests
// #### TESTS_C.C #############################
//---------------------------------------------


#include <stdio.h>
// #include <math.h>

// Types Constants and Macros --------------------------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------
void Test_loop (void);
void Test_Mem_Copy (void);


// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{
    // Test_Functions ();

    // Test_loop ();

    Test_Mem_Copy ();

    return 0;
}


void Test_loop (void)
{
    // for (int i = 319; i >= 0; i--)
    for (int i = 0; i < 320; i++)	
    {
	printf("%d\n", i);
    }
}


void Test_Mem_Copy (void)
{
    unsigned int cup_size = 0;
    unsigned int mem_pages = 0;
    cup_size = 128 * 101 * 2;
    mem_pages = cup_size >> 12;
    mem_pages += 1;
    printf("cup size bytes: %d mem_pages to blank: %d\n", cup_size, mem_pages);

    int bytes_copy = 0;
    printf("freeing pages\n");
    for (int i = 0; i < mem_pages; i++)
    {
	unsigned int addr = 0;
	addr = 0x1000 + 0x1000 * i;
	printf("freeng addr: 0x%06x\n",addr);
	bytes_copy += 4096;
    }
    printf("done!\n");
    printf("bytes freeing: %d\n", bytes_copy);

    bytes_copy = 0;
    printf("coping flash to nvm at 0x1000\n");    
    for (int i = 0; i < cup_size; i+= 1024)
    {
	printf("copy pt0 src addr: 0x%06x len: %d dst addr: 0x%06x\n", i, 1024, 0x1000 + i);

	bytes_copy += 1024;
    }
    printf("bytes copy: %d cups size: %d remain: %d\n", bytes_copy, cup_size, cup_size - bytes_copy);
    printf("done!\n");
    

}

//--- end of file ---//


