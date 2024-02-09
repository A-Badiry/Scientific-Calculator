// this file would contain the data structure and the functions that gather the data entered by the user

#include "Lib.h"


//Pointers that point to the end and the start of the linked list
character* start = NULL;
character* head = NULL;
character* end = NULL;
character** currentchar;
typedef character* node;



//Function to dynamically add an element to the linked list 
void AddElement(char character)
{
    //Allocate mem.
    node temp;
    temp =(node)malloc(sizeof(character));

    //If the start is null = if linked list does not exist yet
    if(start == NULL)
    {
        start = temp;
        (*start).value = character;
        head = start;
        end = start;
        currentchar = &start;
        return;
    }

    //Else if the linked list already have some elements
    else
    {
        (*end).next = temp;
        end = temp;
        (*end).value = character;
    }

}

//Function to delete all elements form linked list and free the memory
void DeleteLinkedList(character** start)
{
    node block;
    *start = head;

    if (*start == NULL)
    {
        return;
    }
    while(*start != NULL)
    {
        block = *start;
        *start = (**start).next;
        free(block);
    }

    start = NULL;
    head = NULL;
    end = NULL;
}



void GetInput(void)
{
    char inputchar;

    while ((inputchar = getchar()) != '\n' && inputchar != EOF) 
    {
        //Ignoring the space character
        if(inputchar != ' ')
        {
            AddElement(inputchar);
        }
    }

}


