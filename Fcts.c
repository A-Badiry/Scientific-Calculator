#include <stdio.h>

# define PI   3.141592653589793238462643383279502884L


void UpdateConfig(void)
{
    char config;

    FILE* ptr;

    ptr = fopen("configuration.txt","r+");

    //Choosing angle configuration
    Printf("Please choose your angle configuration (R, D, G): ");
    config = getchar();

    if(config = 'R')
    {
        Printf("");
    }

}

//Function to update the "answer.bin" file by the new value of "ans"
void PutAns(long double* result)
{
    FILE* ptr;

    ptr = fopen("answer.bin","wb");
        if(ptr == NULL)
        {
            printf("Error fetching the last result");
        }

        else
        {
            fwrite(result, sizeof(long double), 1, ptr);
        }
    
}

//This function is used to get the result of the last operation from a binary file called "answer"
long double GetAns(void)
{
    long double ans;

    FILE* ptr;

    ptr = fopen("answer.bin","rb");
        if(ptr == NULL)
        {
            printf("Error fetching the last result");
        }

        else
        {
            fread(&ans, sizeof(long double), 1, ptr);
        }

    return ans;

}



//Function to check the configuration of the calculator and display results accordingly
char CheckAngle(void)
{

    FILE* ptr;
    char config = "R";

    //Opening the file to read configuration
    ptr = fopen("configuration.txt","r");

        if(ptr == NULL)
        {
            printf("Error ! Failed to check the configuration, default : angles in Rad and 2 decimal places notation");
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
long double *Convert (long double* x)
{
    //Here the program should check the file for configuration !
    char config = CheckAngle();

    //Based on the file content we proceed

    if (config == 'D')
    {
        (*x)*=(PI/180);
        return x;
    }

    else if (config == 'G')
    {
        (*x)*=(PI/200);
        return x;
    } 

    else if (config == 'R')
    {
        return x;
    }

    else
    {
        printf("Unknown angle unit ! Calculating in radians...\n");
        return x;
    } 

}

//Function that converts to scientific notation. I MUST CREATE A STRUCTURE SOLUTION RATHER THAN PRINTF !!!
void Notation(long double* result)
{
    char output[100];

    char* notation = "S2";

    FILE* ptr;

    //Checking the file for notation mode
    ptr = fopen("configuration", "r");
        if(ptr == NULL)
        {
            printf("File not found !");
        }

        else
        {
            fseek(ptr, 1, SEEK_SET);
            fgets(notation, 3, ptr);
            fclose(ptr);
        }



    // Proceed according to file content
    short int exponent = 0;
    
    if((*notation) == 'S')
    {
        

        if((*result) == 0L)
        {
            printf("Result : 0");
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

        sprintf(output,"%%.%dLf E %d",(*notation)+1,(*result),exponent);
        printf("%s",output);
    return;

    }

    else if((*notation) == 'N')
    {
        sprintf(output, "%%.%dLf", (*notation)+1, (*result));
        printf("%s",output);
    }

    else
    {
        printf("Error !");
        sprintf(output, "%%.%dLf", (*notation)+1, (*result));
        printf("%s",output);

    }


    
}


//Coming to it at the end
void ErrorLog(int n)
{

}





int main (void)
{

    short int y= 123456789234573245;

    long double x=123.456;
    long double* xptr = &x;
   
    printf("%hd\n",y);

    printf("%LE\n",x);

    SciNotation(xptr); 

}
