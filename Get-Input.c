/**
 * @file Get-Input.c
 * @author Achraf Badiry (achraf.badiry@edu.bme.hu)
 * @brief 
 * @version 0.1
 * @date 2024-02-14
 * 
 * @copyright GNU General Public License
 * 
 */

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
    (*temp).next = NULL;

    //If the start is null = if linked list does not exist yet
    if(start == NULL)
    {
        start = temp;
        head = temp;
        end = temp;
        (*start).value = character;
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
    (*start) = head;

    if ((*start) == NULL)
    {
        return;
    }
    while((*start) != NULL)
    {
        block = (*start);
        (*start) = (**start).next;
        free(block);
    }

    head = NULL;
    *start = NULL;
    end = NULL;
    start = NULL;
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


