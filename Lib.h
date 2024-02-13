#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>




#ifndef lib_h
#define lib_h

//Header file including the functions defined in different modules as well as structures used by multiple files


//Linked list declaration
typedef struct character
{
    char value;
    struct character *next;

}character;





//Declaration of Parsing tree
typedef struct parsetree
{
    char type;
    long double value;
    struct parsetree* left;
    struct parsetree* right;
}parsetree;






//Pointers that point to the end and the start of the linked list
extern character* start;
extern character* head;
extern character* end;
extern character** currentchar;

//Variable to check whether to print result or not
extern bool print;


//Prototype declaration of functions in "Get-Input"
void GetInput(void);
void AddElement(char c);
void DeleteLinkedList(character** start);

//Functions declarations of functions in "Lex-Anal.c"
void ParseAndEval(character **currentchar);
parsetree* ParseExpression(character** currentchar);
parsetree* ParseTerm(character** currentchar);
parsetree* ParseFactor(character** currentchar);
parsetree* ParseFunction(character** currentchar);
parsetree* ParseConst(character** currentchar);
parsetree* ParseNumber(character** currentchar);
bool StringCmp(character **currentchar, char *functions[]);
long double Calculate (parsetree* tree);
void FreeTree(parsetree* root);
parsetree* CreateTree(char type, long double value, parsetree* left, parsetree* right);

//Function declarations of functions in "Fcts.c"
void UpdateConfig(void);
void ShowConfig(void);
void PutAns(long double* result);
long double GetAns(void);
char CheckAngle(void);
void Convert (long double* angle);
void Notation(long double* result);

#endif
