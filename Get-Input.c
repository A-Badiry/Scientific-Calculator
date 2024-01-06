// this file would contain the data structure and the functions that gather the data entered by the user

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



//Linked list declaration
typedef struct character
{
    char value;
    struct character *next;

}character;

//Pointers that point to the end and the start of the linked list
character* start = NULL;
character* end = NULL;
typedef character* node;

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

    struct character *print  = start;

    while(printptr != NULL)
    {
        printf("%c",(*printptr).value);
        printptr = (*printptr).next;
    } 


    return 0;
}