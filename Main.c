
#include "Lib.h"



//Main function
int main(void)
{
    while(true)
    {

        printf("\n\n>> ");
        
        GetInput();

        ParseAndEval(currentchar);

    }
}