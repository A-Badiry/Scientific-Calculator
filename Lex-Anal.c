//this file contains the RDP (recursive decsent parser)

#include "Lib.h"



//A constant array of strings to check for and evaluate mathematical functions in the expression
char *functions[]= {"cos(","exp(","log(","sin(","sqrt(","tan("};

//Array of strings for commands
char *commands[]={"exit","config","sconfig"};

//Array of strings for constants and a "long double" array for their values
char *constants[]= {"C","G","Me","Mp","Mn","E","Na","Vm","R","ANS","PI"};
long double constant_value[]= {299792458.0, 6.6741E-11, 9.1094E-31, 1.6726E-27, 1.6749E-27, 1.6022E-19, 6.0221E23, 2.2414E-2, 8.314462618, 
                               3.141592653589793238462643383279502884L};





/*################################################################ Context-Free grammar used in project #######################################################

E ––––> E+T | E-T | T
T ––––> T*F | T/F | F
F ––––> (-#) | (E) | Fct | Const | Cmd | #
F ––––> "cos()" | "sin()" | "tan()" | "exp()" | "log()" | "sqrt()"
Const ––––> "C" | "G" | "Me" | "Mp" | "Mn" | "E" | "Na" | "Vm" | "R" | "ANS" | "PI"
Cmd ––––> "exit" | "config" | "sconfig"
# ––––> {0,1,2,3,4,5,6,7,8,9}* . {0,1,2,3,4,5,6,7,8,9}*



–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––*/







/*################################################################ Functions related to parsing tree #######################################################*/


//Function that frees the tree recursively
void FreeTree(parsetree* root)
{

    if (root) {

        if ((*root).left)
            FreeTree((*root).left);
        if ((*root).right)
            FreeTree((*root).right);

        free(root);

    }
}

parsetree* CreateTree(char type, long double value, parsetree* left, parsetree* right)
{
    parsetree* expression = (parsetree*)malloc(sizeof(parsetree));

    (*expression).type = type;
    (*expression).value = value;
    (*expression).left = left;
    (*expression).right = right;

    return expression;
}


/*–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––*/








/*##################################################### Helper function for parsing fct/const/command ############################################################*/


//A function to compare a string and a linked list for recognizing functions
bool StringCmp(character **currentchar, char* functions[])
{
    char* function = *functions;
    character* copy2 = *currentchar;
    character** copy1 = currentchar;

    while((*function) != '\0')
    {   
        //If at any moment the characters are not equal return false, else, keep going
        if(*currentchar == NULL || ((*(*currentchar)).value != (*function)))
        {

            return false;
            
        }

        //Updating values for next iteration
        *currentchar = (*(*currentchar)).next;
        function += 1;

        //If  the linked list is consumed before the end of the array then they are not equal
        if (*currentchar == NULL)
        {
            return false;
        }


    }

    return true;

}

/*––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––*/





/*############################################################# Recursive descent parser functions ###############################################################*/

//Function to organize the flow of the program (Called by "main()" fct)
void ParseAndEval(character **currentchar)
{
    long double result;
    parsetree* parsetree;

    parsetree = ParseExpression(currentchar);

    //Check if we are allowed to consider the current parsing tree
    if(print == true)
    {
        result = Calculate(parsetree);

        PutAns(&result);

        Notation(&result);
    }

    //Resetting the permission to consider parsing tree
    print = true;

    //Both functions to free memory after calculation
    FreeTree(parsetree);
    DeleteLinkedList(currentchar);

    return;

}

//Function to calculate result from parsing tree recursively
long double Calculate (parsetree* parsetree)
{
    if(parsetree == NULL)
    {
        return 0;
    }

    if ((*parsetree).type == 'n')
        return (*parsetree).value;

    long double left = Calculate((*parsetree).left);
    long double right = Calculate((*parsetree).right);
    
    if ((*parsetree).type == '+')
        return left + right;
    else if ((*parsetree).type == '-')
        return left - right;
    else if ((*parsetree).type == '*')
        return left * right;
    else if ((*parsetree).type == '/')
        return (right ? (left/right) : 0);

    return 0;

}





//Function to parse expressions
parsetree* ParseExpression(character **currentchar)
{
    char type;

    //Parse a term to allow for precedence
    parsetree* left_expression = ParseTerm(currentchar);
    parsetree* right_expression;


    //To check for more addition and subtraction operators
    while(*currentchar != NULL && ((*(*currentchar)).value == '+' || (*(*currentchar)).value == '-'))
    {
        //Memorizing type for tree modification
        type = (*(*currentchar)).value;

        //Acknowledging the operator
        *currentchar = (*(*currentchar)).next;

        //Parsing for right term
        right_expression = ParseTerm(currentchar);


        //Modifying the tree to make the operator the new root of the tree
        left_expression = CreateTree (type, -1, left_expression, right_expression);
    }

    return left_expression;

}


//Function to parse terms
parsetree* ParseTerm(character **currentchar)
{

    parsetree* left_term = NULL;
    parsetree* right_term = NULL;
    char type;

    //Parse for a factor to get the operands of the term, and also for precedence of parenthesis
    left_term = ParseFactor(currentchar);

    //Checking for *(mult) or /(div) operators
    while((*currentchar != NULL) && ((*(*currentchar)).value == '*' || (*(*currentchar)).value == '/'))
    {
        //Memorize the type for modifying the tree later
        type = (*(*currentchar)).value;

        //Acknowledge the mult or div signs
        *currentchar = (*(*currentchar)).next;

        //Parsing for right factor
        right_term = ParseFactor(currentchar);

        //Modifying the tree to account for the new operation
        left_term = CreateTree(type, -1, left_term, right_term);
    }


    
    return left_term;

}



//Function to parse factors
parsetree* ParseFactor(character **currentchar)
{
    //Creating a tree to store the factor (whatever it is !)
    parsetree* factor = NULL;

    //Checking for parenthesis to account for precedence
    if(*currentchar != NULL && (*(*currentchar)).value == '(')
    {
        //Acknowledge the right parenthesis and move forward for parsing expression
        *currentchar = (*(*currentchar)).next;

        //In this project, negative numbers can only be written as such : (-#) | This: (-# +#) for example is not allowed !!!
        if(*currentchar != NULL &&(*(*currentchar)).value == '-')
        {
            *currentchar = (*(*currentchar)).next;
            factor = ParseNumber(currentchar);
            (*factor).value = -((*factor).value);

            if(*currentchar != NULL && (*(*currentchar)).value == ')')
            {
                *currentchar = (*(*currentchar)).next;
            }
            
            else
            {
                printf("\nYou have forgot the closing parenthesis !\n");
                factor = CreateTree('n', 0.0, NULL, NULL);
                print = false;
                return factor;
            }
        }

        else
        {
            //Else, an expression could be inside the parenthesis, so we parse again for it.
            factor = ParseExpression(currentchar);

            if(*currentchar != NULL && (*(*currentchar)).value == ')')
            {
                *currentchar = (*(*currentchar)).next;
            }

            else
            {
                printf("\nYou have forgot the closing parenthesis !\n");
                factor = CreateTree('n', 0.0, NULL, NULL);
                print = false;
                return factor;
            }
        }
    }


    //Parsing for commands or functions if the character is a lower-case letter
    else if( (*currentchar != NULL) && ((*(*currentchar)).value) >= 'a' && ((*(*currentchar)).value) <= 'z' )
    {
        //If the cursor position is "head" the it might be a command or a function
        if(*currentchar == head)
        {    
            //Bringing cursor to the starting position after each check
            character* seek_start = *currentchar;
            
            //Parsing for commands
            if(StringCmp(currentchar, commands))
            {
                exit(0);
            }
            else if((*currentchar = seek_start) && StringCmp(currentchar, commands+1))
            {
                UpdateConfig();
                factor = CreateTree('n', 0.0, NULL, NULL);
                return factor;
            }
            else if((*currentchar = seek_start) && StringCmp(currentchar, commands+2))
            {
                ShowConfig();
                factor = CreateTree('n', 0.0, NULL, NULL);
                return factor;
            }

            //If the input is not a command then it is probably a function
            else
            {
                *currentchar = seek_start;
                factor = ParseFunction(currentchar);
            }

        }

        //If the position of the cursor is not "head" then the entry must be a function
        else
        {
            factor = ParseFunction(currentchar);
        }
        
    }

    //Parsing for predefined constants if the character is an upper-case letter
    else if ((*currentchar != NULL) && (*(*currentchar)).value >= 'A' && (*(*currentchar)).value <= 'Z')
    {
        factor = ParseConst(currentchar);
    }



    //Else the character must be a number
    else
    {
        factor = ParseNumber(currentchar);
    }

    return factor;

}

// Function to parse mathematical functions
parsetree* ParseFunction(character **currentchar)
{
    //char config = CheckAngle();
    parsetree* function;
    long double value;


    //Cos fct
    if((*(*currentchar)).value == 'c')
    {


        if (StringCmp(currentchar,functions))
        {

            function = ParseExpression(currentchar);

            value = Calculate(function);

            FreeTree(function);

            Convert (&value);

            

            if((*currentchar != NULL) && (*(*currentchar)).value == ')')
            {
                function = CreateTree('n', cos(value), NULL, NULL);
                *currentchar = (*(*currentchar)).next;
                return function;
            }

            else
            {
                printf("\nError ! No closing parenthesis for function !\n");
                function = CreateTree('n', 0.0, NULL, NULL);
                print = false;
                return function;
            }
            
        }
            
        else
        {
            printf("\nError : Uknown function detected !\n");
            function = CreateTree('n', 0.0, NULL, NULL);
            print = false;
            return function;
        }
    }

    //Exponential fct
    else if ((*(*currentchar)).value == 'e')
    {

       
        if (StringCmp(currentchar,functions+ 1))
        {

            function = ParseExpression(currentchar);

            value = Calculate(function);

            FreeTree(function);

            if((*currentchar != NULL) && (*(*currentchar)).value == ')')
            {
                function = CreateTree('n', exp(value), NULL, NULL);
                *currentchar = (*(*currentchar)).next;
                return function;
            }

            else
            {
                printf("\nError ! No closing parenthesis for function !\n");
                function = CreateTree('n', 0.0, NULL, NULL);
                print = false;
                return function;
            }
           

        }

        else
        {
            printf("\nError : Uknown function detected !\n");
           function = CreateTree('n', 0.0, NULL, NULL);
           print = false;
           return function;

        }
    }

    
    //Detecting the "log" function
    else if ((*(*currentchar)).value == 'l')
    {

       
        if (StringCmp(currentchar,functions+2))
        {

            function = ParseExpression(currentchar);

            value = Calculate(function);

            FreeTree(function);

            if((*currentchar != NULL) && (*(*currentchar)).value == ')')
            {
                function = CreateTree('n', log(value), NULL, NULL);
                *currentchar = (*(*currentchar)).next;
                return function;
            }

            else
            {
                printf("\nError ! No closing parenthesis for function !\n");
                function = CreateTree('n', 0.0, NULL, NULL);
                print = false;
                return function;
            }
           

        }

        else
        {
            printf("\nError : Uknown function detected !\n");
            function = CreateTree('n', 0.0, NULL, NULL);
            print = false;
            return function;
        }
    }


    //Sqrt or sin function
    else if ((*(*currentchar)).value == 's')
    {
        //Could have used threads ! but maybe I'll improve later :)

        character* currentchar2p = *currentchar;

        character** currentchar2 = &currentchar2p ;

        //comparing with the sin() function
        if (StringCmp(currentchar,functions+3))
        {

            function = ParseExpression(currentchar);

            value = Calculate(function);

            FreeTree(function);

            Convert (&value);

            if((*currentchar != NULL) && (*(*currentchar)).value == ')')
            {
                function = CreateTree('n', sin(value), NULL, NULL);
                *currentchar = (*(*currentchar)).next;
                return function;
            }

            else
            {
                printf("\nError ! No closing parenthesis for function !\n");
                function = CreateTree('n', 0.0, NULL, NULL);
                print = false;
                return function;
            }
           
        }

        //Comparing with the sqrt() function
        if (StringCmp(currentchar2,functions+4))
        {

            function = ParseExpression(currentchar2);

            value = Calculate(function);

            FreeTree(function);

            if((*currentchar2 != NULL) && (*(*currentchar2)).value == ')')
            {
                function = CreateTree('n', sqrt(value), NULL, NULL);
                *currentchar2 = (*(*currentchar2)).next;
                return function;
            }

            else
            {
                printf("\nError ! No closing parenthesis for function !\n");
                function = CreateTree('n', 0.0, NULL, NULL);
                print = false;
                return function;

            }
           
        }

        else
        {
            printf("\nError : Uknown function detected !\n");
            function = CreateTree('n', 0.0, NULL, NULL);
            print = false;
            return function;
        }
    }

    //Tan fct
    else if((*(*currentchar)).value == 't')
    {
        
        if (StringCmp(currentchar,functions+5))
        {

            function = ParseExpression(currentchar);

            value = Calculate(function);

            FreeTree(function);

            Convert (&value);

            if((*currentchar != NULL) && (*(*currentchar)).value == ')')
            {
                function = CreateTree('n', tan(value), NULL, NULL);
                *currentchar = (*(*currentchar)).next;
                return function;
            }

            else
            {
                printf("\nError ! No closing parenthesis for function !\n");
                function = CreateTree('n', 0.0, NULL, NULL);
                print = false;
                return function;

            }
           
        }
            else
            {
                printf("\nError : Uknown function detected !\n");
                function = CreateTree('n', 0.0, NULL, NULL);
                print = false;
                return function;
            }
    }

    else
    {
        printf("\nError : Uknown function detected !\n");
        function = CreateTree('n', 0.0, NULL, NULL);
        print = false;
        return function;
    }
    

}




//Function to parse Constants
parsetree* ParseConst(character** currentchar)
{
    parsetree* constant;
    character* copy = start;

    if(StringCmp(currentchar,constants))
    {
        constant = CreateTree('n', constant_value[0], NULL, NULL);
        return constant;
    }

    else if(StringCmp(currentchar,constants+1))
    {
        constant = CreateTree('n', constant_value[1], NULL, NULL);
        return constant;
    }

    else if(StringCmp(currentchar,constants+2))
    {
        constant = CreateTree('n', constant_value[2], NULL, NULL);
        return constant;
    }


    //Returning Cursor to the start of the contant

    else if((start = copy) && StringCmp(currentchar,constants+3))
    {
        constant = CreateTree('n', constant_value[3], NULL, NULL);
        return constant;
    }
	
    //Returning Cursor to the start of the contant

    else if((start = copy) && StringCmp(currentchar,constants+4))
    {
        constant = CreateTree('n', constant_value[4], NULL, NULL);
        return constant;
    }

    else if((start = copy) && StringCmp(currentchar,constants+5))
    {
        constant = CreateTree('n', constant_value[5], NULL, NULL);
        return constant;
    }

    else if(StringCmp(currentchar,constants+6))
    {
        constant = CreateTree('n', constant_value[6], NULL, NULL);
        return constant;
    }

    else if(StringCmp(currentchar,constants+7))
    {
        constant = CreateTree('n', constant_value[7], NULL, NULL);
        return constant;
    }

    else if(StringCmp(currentchar,constants+8))
    {
        constant = CreateTree('n', constant_value[8], NULL, NULL);
        return constant;
    }

    //Constant ANS is used to get the result of the last operation from the file anwer.bin
    else if(StringCmp(currentchar,constants+9))
    {
        long double ans;

        ans = GetAns();

        constant = CreateTree('n', ans, NULL, NULL);

        return constant;
    }

    else if(StringCmp(currentchar,constants+10))
    {
        constant = CreateTree('n', constant_value[9], NULL, NULL);
        return constant;
    }

    else
    {
        printf("\nError ! Uknown constant detected, please check input !\n");
        constant = CreateTree('n', 0.0, NULL, NULL);
        print = false;
        return constant;

    }

}



//Function to parse a number and turn it into a double for evaluation
parsetree* ParseNumber(character **currentchar)
{
    parsetree* num;

    //To check whether the current character is actually a number
    if ((*currentchar == NULL)|| (*(*currentchar)).value > '9' || (*(*currentchar)).value < '0')
    {
        printf("\nError parsing number, please check input !\n");
        num = CreateTree('n', 0.0, NULL, NULL);
        print = false;
        return num;
    }

    long double number = 0.0;
    unsigned long int mantissa = 10;

    //Transforming the charactersitic part into a number
    while((*currentchar != NULL) && (*(*currentchar)).value >= '0' && (*(*currentchar)).value<= '9')
    {
        number = number*10 + ((*(*currentchar)).value - '0');
        *currentchar = (*(*currentchar)).next;
    }

    //If we detect the "decimal point" character we should add the mantissa
    if (*currentchar != NULL && (*(*currentchar)).value == '.')
    {
        *currentchar = (*(*currentchar)).next;

        //If the user forgets tha mantissa we cannot allow it (e.g 225.____ is not a number ! however .225 is interpreted as 0.225)
        if ((*currentchar == NULL) || (*(*currentchar)).value > '9' || (*(*currentchar)).value < '0')
        {
            printf("\nMissing mantissa ! (%i. <––––)\n",(int)number);
            num = CreateTree('n', 0.0, NULL, NULL);
            print = false;
            return num;
        }
        
        //Adding the mantissa to the original number
        while((*currentchar != NULL) && (*(*currentchar)).value >= '0' && (*(*currentchar)).value <= '9')
        {
            number = number + (long double)((*(*currentchar)).value - '0')/mantissa;
            mantissa *= 10;
            *currentchar = (*(*currentchar)).next;
        }

    }

        num = CreateTree('n', number, NULL, NULL);

    return num;

}


/*––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––*/



