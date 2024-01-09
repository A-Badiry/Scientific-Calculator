#include "Lib.h"


# define PI   3.141592653589793238462643383279502884L


bool print = true;

//Function to update the configuration of the calculator
void UpdateConfig(void)
{
    char config;

    FILE* ptr;

    ptr = fopen("Config.txt","r+");
    if(ptr == NULL)
    {
        printf("Error updating configuration file !");
    }
    
    else
    {
        //Choosing angle configuration
        printf("\nPlease choose your angle configuration (R, D, G): ");
angle:
        config = getchar();

        if((config == 'R') || (config == 'D') || (config == 'G'))
        {
            fputc(config, ptr);
            printf("\nSuccessfully changed angle unit !\n");
        }

        else
        {
            printf("\nPlease choose R, D or G ! ");
            goto angle;
        }

        //Choosing the notation configuration
        printf("\nPlease choose the type of notation to be displayed (N, S): ");
notation:
        getchar();
        config = getchar();

        if((config == 'N') || (config == 'S'))
        {
            fputc(config, ptr);
            printf("\nSuccessfully changed notation !\n");
        }

        else
        {
            printf("\nPlease choose either N or S : ");
            goto notation;
        }

        //Chosing the decimal place precision
        printf("\nPlease choose the precision after the floating precision (1 - 9): ");
precision:
        getchar();
        config = getchar();
        getchar();

        if((config >= '1') && (config <= '9'))
        {
            fseek(ptr, 2, SEEK_SET);
            fputc(config, ptr);
            printf("\nSuccessfully changed the precision !\n");
            fclose(ptr);
        }

        else
        {
            printf("\nplease choose a number between 1 and 9 (inclusive) : ");
            goto precision;
        }
    }

    print = false;
}

//Function to show the actual configuration of the calculator
void ShowConfig(void)
{
    char config[4];

    FILE* ptr;

    ptr = fopen("Config.txt","r");
    if(ptr == NULL)
    {
        printf("\nError fetching the configuration !\n");
        return;
    }

    else
    {
        fgets(config, 4, ptr);
        fclose(ptr);
    }

    //Printing angle unit(default is R)
    if(*config == 'D')
    {
        printf("\nDEG, ");
    }
    else if(*config == 'G')
    {
        printf("\nGRA, ");
    }
    else
    {
        printf("\nRAD, ");
    }

    //Printing the notation type (default is S)
    if(*(config+1) == 'N')
    {
        printf("Nrm, ");
    }
    else
    {
        printf("Sci, ");
    }

    //Printing the precision number(default is 2)
    if((*(config+2)) >= '1' && (*(config+2)) <= '9')
    {
        printf("%c",*(config+2));
    }
    else
    {
        printf("2");
    }

    print = false;
    
    return;
}




//Function to update the "answer.bin" file by the new value of "ans"
void PutAns(long double* result)
{
    FILE* ptr;

    ptr = fopen("answer.bin","wb");
        if(ptr == NULL)
        {
            printf("\nError saving the last result !\n");
            return;
        }

        else
        {
            fwrite(result, sizeof(long double), 1, ptr);
            fclose(ptr);
        }
    
}

//Getting "ans" the result of the last operation
long double GetAns(void)
{
    long double ans;

    FILE* ptr;

    ptr = fopen("answer.bin","rb");
        if(ptr == NULL)
        {
            printf("\nError fetching the last result !\n");
            print = false;
            return 0.0;
        }

        else
        {
            fread(&ans, sizeof(long double), 1, ptr);
            fclose(ptr);
        }

    return ans;

}



//Function to check the angle unit for trig function conversions
char CheckAngle(void)
{

    FILE* ptr;
    char config = 'R';

    //Opening the file to read angle unit 
    ptr = fopen("Config.txt","r");

        if(ptr == NULL)
        {
            printf("\nError checking the configuration ! Calculating angles in RAD...\n");
            return config;
        }

        else
        {
            config = fgetc(ptr);
            fclose(ptr);
        }

    
        return config;

}


//Function to convert angles depending on the configuration
void Convert (long double* angle)
{
    //Here the program should check the file for configuration !
    char config = CheckAngle();

    //Based on the file content we proceed

    if (config == 'D')
    {
        (*angle)*=(PI/180);
        return;
    }

    else if (config == 'G')
    {
        (*angle)*=(PI/200);
        return;
    } 

    else if (config == 'R')
    {
        return;
    }

    else
    {
        printf("\nError checking configuration ! Calculating in radians...\n");
        return;
    } 

}

//Function that converts to scientific notation
void Notation(long double* result)
{
    char notation = 'S';
    int precision = 2;

    FILE* ptr;

    //Checking the file for notation mode
    ptr = fopen("Config.txt", "r");
        if(ptr == NULL)
        {
            printf("\nConfiguration file not found !\n");
        }

        else
        {
            fseek(ptr, 1, SEEK_SET);
            notation = fgetc(ptr);
            precision = (fgetc(ptr) - '0');
            fclose(ptr);
        }



    // Proceed according to file content
    short int exponent = 0;
    
    if(notation == 'S')
    {
        

        if((*result) == 0L)
        {
            printf("\n0");
            return;
        }



        while((*result)< 1.0L && (*result)> -1.0L)
        {
            (*result) *= 10;
            --exponent;
        }

        while((*result) > 10.0L || (*result) < -10.0L)
        {
            (*result)/= 10;
            ++exponent;
        }


        printf("\n%.*LF E %hi",precision,(*result),exponent);

    return;

    }

    else if(notation == 'N')
    {
        printf("\n%.*Lf", precision, *result);
    }

    else
    {
        printf("\nError checking configuration ! Printing in normal notation...\n");
        printf("\n%Lf",*result);
    }


    
}
