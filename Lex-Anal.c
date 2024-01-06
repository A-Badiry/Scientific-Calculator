//This file would contain the lexical analyzer and it's functions, maybe also the expression evaluator


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>




//Prototype declaration of structures
typedef struct character
{
    char value;
    struct character* next;

}character;



//Pointers that point to the end and the start of the linked list
character* start = NULL;
character* end = NULL;
typedef character* node;






/*################################################################ Parsing Tree and its related functions#######################################################*/


//Tree structure for creating the parse tree
typedef struct parsetree
{
    char type;
    long double value;
    struct parsetree* left;
    struct parsetree* right;
}parsetree;

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






//Prototype declarations of functions 
parsetree* ParseExpression(character* currentchar);
parsetree* ParseTerm(character* currentchar);
parsetree* ParseFactor(character* currentchar);
parsetree* ParseFunction(character *currentchar);
parsetree* ParseNumber(character *currentchar);
long double Calculate (parsetree* tree);
void FreeTree(parsetree* root);


//A constant array of strings to check for and evaluate mathematical functions in the expression
const char const *functions[]={"cos(","exp(","log(","sin(","sqrt(","tan("};










/*############################################################### Helper function for ParseFunction() ############################################################*/


//A function to compare two strings for recognizing functions, one from the linked list and one from the array
bool StringCmp(character *currentchar, const char const *function)
{
    bool equal;


    while(((*function) != '\0'))
    {   
        //If at any moment the characters are not equal return false, else, keep going
        if((*currentchar).value != (*function))
        {

            return false;
            
        }

        //Updating values for next iteration
        currentchar = (*currentchar).next;
        function += 1;

        //If  the linked list is consumed before the end of the array then they are not equal
        if ( ((*function) != '\0') && (currentchar == NULL))
        {
            return false;
        }


    }

    return true;

}

/*––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––*/





/*############################################################# Recursive descent parser functions ###############################################################*/


long double ParseAndEval(character* start)
{
    parsetree* parsetree;

    parsetree = ParseExpression(start);

    return Calculate(parsetree);
}

long double Calculate (parsetree* parsetree)
{


    if ((*parsetree).type == 'n')
        return (*parsetree).value;

    int left = Calculate((*parsetree).left);
    int right = Calculate((*parsetree).right);
    
    if ((*parsetree).type == '+')
        return left + right;
    else if ((*parsetree).type == '-')
        return left - right;
    else if ((*parsetree).type == '*')
        return left * right;
    else if ((*parsetree).type == '/')
        return left / right ? right : 0;

    return 0;

}





//Function to parse expressions
parsetree* ParseExpression(character* currentchar)
{
    char type;
    //Parse a term to allow for precedence
    parsetree* left_term = ParseTerm(currentchar);
    parsetree* right_term;

    //To check for addition and subtraction operators
    while(currentchar != NULL && ((*currentchar).value == '+' || (*currentchar).value == '-'))
    {
        //Memorizing type for tree modification
        type = (*currentchar).value;

        //Acknowledging the operator
        currentchar = (*currentchar).next;

        //Parsing for right term
        right_term = ParseTerm(currentchar);
    }

    //Modifying the tree to make the operator the new root of the tree
    left_term = CreateTree (type, -1, left_term, right_term);

}


//Function to parse terms
parsetree* ParseTerm(character* currentchar)
{

    parsetree* term;
    parsetree* right_term;
    char type;
    //Parse for a factor to get the operands of the term, and also for precedence of parenthesis
    term = ParseFactor(currentchar);

    //Checking for *(mult) or /(div) operators
    while((currentchar != NULL) && ((*currentchar).value == '*' || (*currentchar).value == '/'))
    {
        //Memorize the type for modifying the tree later
        type = (*currentchar).value;

        //Acknowledge the mult or div signs
        currentchar = (*currentchar).next;

        right_term = ParseFactor(currentchar);
    }

    term = CreateTree(type, -1, term, right_term);

    return term;

}


/*🔥We will still add a (math)function option to the ParseFactor function🔥*/

//Function to parse factors
parsetree* ParseFactor(character* currentchar)
{
    //Creating a tree to store the factor (whatever it is !)
    parsetree* factor;

    //Checking for parenthesis to account for precedence
    if(currentchar != NULL && (*currentchar).value == '(')
    {
        //Acknowledge the right parenthesis and move forward for parsing expression
        currentchar = (*currentchar).next;

        //In this project, negative numbers can only be written as such : (-#) | This: (-# +#) for example is not allowed !!!
        if(currentchar != NULL &&(*currentchar).value == '-')
        {
            currentchar = (*currentchar).next;
            factor = ParseNumber(currentchar);
            (*factor).value = -((*factor).value);

            if((*currentchar).value == ')')
            {
                currentchar = (*currentchar).next;
            }
            
            else
            {
                printf("You have forgot the closing parenthesis !");
                exit(1);
            }
        }

        else
        {
            //Else, an expression could be inside the parenthesis, so we parse again for it.
            factor = ParseExpression(currentchar);

            if((*currentchar).value == ')')
            {
                currentchar = (*currentchar).next;
            }

            else
            {
                printf("You have forgot the closing parenthesis !");
            }
        }
    }

    else if((*currentchar).value >= 'a' && (*currentchar).value <= 'z' )
    {
        //If there is a character which is not a number then it is probably a function
        factor = ParseFunction(currentchar);
    }

    /*
    else if ((*currentchar).value >= 'A' && (*currentchar).value <= 'Z')
    {
        factor = ParseConst(currentchar);
    }

    */


    else
    {
        //If there are no parenthesis then we can find a number (at least for the moment, could be a function later !)
        factor = ParseNumber(currentchar);

    }

    return factor;

}

// Function to parse mathematical functions
parsetree* ParseFunction(character *currentchar)
{

    parsetree* function;
    long double value;

    if((*currentchar).value == 'c')
    {


        if (StringCmp(currentchar,functions))
        {

            function = ParseExpression(currentchar);

            value = Calculate(function);

            if((*currentchar).value == ')')
            {
                function = CreateTree('n', cos(value), NULL, NULL);
                return function;
            }

            else
            {
                printf("Error ! No closing parenthesis for function !");
            }
            
        }
            
        else
        {   //Might add the Error column (log) later :)
            printf("Error : Uknown function detected !");
            exit(1);
        }
    }

    else if ((*currentchar).value == 'e')
    {
       
        if (StringCmp(currentchar,functions+1))
        {

            function = ParseExpression(currentchar);

            value = Calculate(function);

             if((*currentchar).value == ')')
            {
                function = CreateTree('n', exp(value), NULL, NULL);
                return function;
            }

            else
            {
                printf("Error ! No closing parenthesis for function !");
            }
           

        }

        else
        {   //Might add the Error column later :)
            printf("Error : Uknown function detected !");
            exit(1);
        }
    }

    else if ((*currentchar).value == 's')
    {
        //Could have used threads !
        character* currentchar2 = currentchar;

        //comparing with the sin functions
        if (StringCmp(currentchar,functions+2))
        {

            function = ParseExpression(currentchar);

            value = Calculate(function);

            if((*currentchar).value == ')')
            {
                function = CreateTree('n', sin(value), NULL, NULL);
                return function;
            }

            else
            {
                printf("Error ! No closing parenthesis for function !");
            }
           
        }

        //Comparing with the sqrt() function
        if (StringCmp(currentchar2,functions+3))
        {

            function = ParseExpression(currentchar);

            value = Calculate(function);

            if((*currentchar).value == ')')
            {
                function = CreateTree('n', sqrt(value), NULL, NULL);
                return function;
            }

            else
            {
                printf("Error ! No closing parenthesis for function !");
            }
           
        }

        else
        {   //Might add the Error column later :)
            printf("Error : Uknown function detected !");
            exit(1);
        }
        }

    
    else if((*currentchar).value == 't')
    {
        
        if (StringCmp(currentchar,functions+4))
        {

            function = ParseExpression(currentchar);

            value = Calculate(function);

            if((*currentchar).value == ')')
            {
                function = CreateTree('n', tan(value), NULL, NULL);
                return function;
            }

            else
            {
                printf("Error ! No closing parenthesis for function !");
            }
           
        }
            else
            {   //Might add the Error column later :)
                printf("Error : Uknown function detected !");
                exit(1);
            }
    }
    

}



//Function to parse a number and turn it into a double for evaluation
parsetree* ParseNumber(character *currentchar)
{

    if ((*currentchar).value > '9' || (*currentchar).value < '0')
    {
        printf("*Error* Could not parse number, Please check grammar !");
        exit(1);
    }

    long double number = 0;
    unsigned long int mantissa = 10;

    //Transforming the charactersitic part into a number
    while((*currentchar).value >= '0' && (*currentchar).value<= '9')
    {
        number = number*10 + ((*currentchar).value - '0');
        currentchar = (*currentchar).next;
    }

    //If we detect the "decimal point" character we should add the mantissa
    if ((*currentchar).value == '.')
    {
        currentchar = (*currentchar).next;

        //If the user forgets tha mantissa we cannot allow it (e.g 225.____ is not a number ! however .225 is interpreted as 0.225)
        if ((*currentchar).value > '9' || (*currentchar).value < '0')
        {
            printf("Missing mantissa ! (%i. <––––)",(int)number);
            exit(1);
        }
        
        //Adding the mantissa to the original number
        while((*currentchar).value >= '0' && (*currentchar).value <= '9')
        {
            number = number + ((*currentchar).value - '0')/mantissa;
            mantissa *= 10;
            currentchar = (*currentchar).next;
        }

    }

    parsetree* num = CreateTree('n', number, NULL, NULL);

}


/*––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––*/






//Function to dynamically add an element to the linked list 
void AddElement(char c){
    node temp;
    temp =(node)malloc(sizeof(character));

    if(start == NULL)
    {
        start = temp;
        (*start).value = c;
        end = start;
        return;
    }

    else
    {
        (*end).next = temp;
        end = temp;
        (*end).value = c;
    }

}

//Function to delete all elements form linked list and free the memory
void DeleteLinkedList(character *start)
{
    node block;

    if (start == NULL)
    {
        return;
    }
    while(start != NULL)
    {
        block = start;
        start = (*start).next;
        free(block);
    }
}



void GetInput(void)
{
    char inputchar;

    while ((inputchar = getchar()) != '\n' && inputchar != EOF) 
    {
        if(inputchar != ' ')
        {
            AddElement(inputchar);
        }
    }
}


// Mainly for testing the functions written !
int main (void)
{
    
    GetInput();

    ParseAndEval(start);


}

