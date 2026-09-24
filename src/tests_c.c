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


// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{
    // Test_Functions ();

    Test_loop ();

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




//--- end of file ---//


