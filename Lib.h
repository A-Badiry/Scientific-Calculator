/**
 * @file Lib.h
 * @author Achraf Badiry (achraf.badiry@edu.bme.hu)
 * @brief 
 * @version 0.1
 * @date 2024-02-14
 * 
 * @copyright GNU General Public License
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>




#ifndef lib_h
#define lib_h


/**
 * @brief Structure storing the character inputs by the user to a linked list
 * 
 */
typedef struct character
{
    char value;
    struct character *next;

}character;





/**
 * @brief Structure that stores elements of the parse tree
 * 
 */
typedef struct parsetree
{
    char type;
    long double value;
    struct parsetree* left;
    struct parsetree* right;
}parsetree;






/**
 * @brief Pointer used to parse through the linked list to generate the parsetree
 * 
 */
extern character* start;
/**
 * @brief Pointer that points to the start of the linked list and is used for deletion
 * 
 */
extern character* head;
/**
 * @brief Pointer that points to the end of the linked list to add new elements to it
 * 
 */
extern character* end;
/**
 * @brief Pointer to pointer that helps modifying the value of the pointer @ref start while passing through multiple functions
 * 
 */
extern character** currentchar;

/**
 * @brief A variable to enable/disable printing of the final result
 * @example When a user types the command "config" the parser actually generates and evaluates the parsing tree to 0.0 but this value should not be printed !
 * 
 */
extern bool print;


//Prototype declaration of functions in "Get-Input"
/**
 * @brief Function to read the input from the "stdin" file and stores it in the linked list by calling @ref AddElement()
 * 
 */
void GetInput(void);
/**
 * @brief Function to allocate the memory for an instance of the struct "character" and assigns it a value
 * 
 * @param c Value to be assigned to an instance
 */
void AddElement(char c);
/**
 * @brief Function to delete the linked list
 * 
 * @param start Pointer to the pointer to the start of the linked list
 */
void DeleteLinkedList(character** start);

//Functions declarations of functions in "Lex-Anal.c"
/**
 * @brief Function that drives the flow of the program
 * 
 * @param currentchar Pointer to pointer to first element of the linked list
 */
void ParseAndEval(character** currentchar);
/**
 * @brief Function to parse expression
 * 
 * @param currentchar Pointer to pointer to the current element pointed to on the linked list
 * @return parsetree* Modifies and returns the parse tree 
 */
parsetree* ParseExpression(character** currentchar);
/**
 * @brief Function to parse a term
 * 
 * @param currentchar Pointer to pointer to the current element pointed to on the linked list
 * @return parsetree* Modifies and returns the parse tree
 */
parsetree* ParseTerm(character** currentchar);
/**
 * @brief Function to parse a factor
 * 
 * @param currentchar Pointer to pointer to the current element pointed to on the linked list
 * @return parsetree* Modifies and returns the parse tree
 */
parsetree* ParseFactor(character** currentchar);
/**
 * @brief Function to parse a mathematical function
 * 
 * @param currentchar Pointer to pointer to the current element pointed to on the linked list
 * @return parsetree* Modifies and returns the parse tree
 */
parsetree* ParseFunction(character** currentchar);
/**
 * @brief Function to parse a constant
 * 
 * @param currentchar Pointer to pointer to the current element pointed to on the linked list
 * @return parsetree* Modifies and returns the parse tree
 */
parsetree* ParseConst(character** currentchar);
/**
 * @brief Function to parse a number
 * 
 * @param currentchar Pointer to pointer to the current element pointed to on the linked list
 * @return parsetree* Modifies and returns the parse tree
 */
parsetree* ParseNumber(character** currentchar);
/**
 * @brief Function to compare the functions, constants and commands input by the user with supported built-in functions
 * 
 * @param currentchar Pointer to pointer to the current element pointed to on the linked list
 * @param functions Array of character pointer pointing to the functions, constants or commands supported
 * @return true If input by the user is supported
 * @return false If input by the user is unsupported
 */
bool StringCmp(character **currentchar, char *functions[]);
/**
 * @brief Function to recursively evaluate the parsing tree
 * 
 * @param tree Pointer to the root of the parsing tree
 * @return long double Result of the calculation
 */
long double Calculate (parsetree* tree);
/**
 * @brief Function to recursively free the tree
 * 
 * @param root Pointer to the root of the tree
 */
void FreeTree(parsetree* root);
/**
 * @brief Function that creates a node in the tree
 * 
 * @param type Type of character to be inserted (number or operator)
 * @param value Value to be inserted
 * @param left Pointer to the left child
 * @param right Pointer to the right child
 * @return parsetree* 
 */
parsetree* CreateTree(char type, long double value, parsetree* left, parsetree* right);

//Function declarations of functions in "Fcts.c"
/**
 * @brief Function to update the configuration of the calculator
 * 
 */
void UpdateConfig(void);
/**
 * @brief Function to fetch the configuration if the calculator and display it by calling @ref Notation()
 * 
 */
void ShowConfig(void);
/**
 * @brief Function that stores the result of the last calulator to a binary file 
 * 
 * @param result result of the calculation to be stored
 */
void PutAns(long double* result);
/**
 * @brief Function to fetch the result of the last calculation and display it by calling @ref Notation()
 * 
 * @return long double 
 */
long double GetAns(void);
/**
 * @brief Function to fetch the angle unit used so that it can convert to radians and feed values to built-in math library functions by calling @ref Convert()
 * 
 * @return char 
 */
char CheckAngle(void);
/**
 * @brief Function to convert the value of angles into radians if the calculator is not already set to radians
 * 
 * @param angle The angle unit : D for degrees, R for randians and G for grandians
 */
void Convert (long double* angle);
/**
 * @brief Function to display the result in the configured notation
 * 
 * @param result result of current calculation
 */
void Notation(long double* result);

#endif
