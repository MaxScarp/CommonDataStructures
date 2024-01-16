#include "misc.h"

// Define NODE as a pointer to list_string_item
#define NODE list_string_item*
// Define POINTER_TO_NODE macro to get the address of a variable
#define POINTER_TO_NODE(x) &x

// Definition of a linked list node structure
typedef struct list_string_item
{
    struct list_string_item* next;
    const char* string;
} list_string_item;

// Function to create a new string node with the given string
NODE list_string_item_new(const char* string);
// Function to get the tail node of the linked list
NODE list_get_tail(NODE head);
// Function to append a node to the linked list
NODE list_append(NODE* head, NODE item);
// Function to print the linked list
void list_print(NODE head);
// Function to pop the first item from the linked list
NODE list_pop_first_item(NODE* head);
// Function to pop the last item from the linked list
NODE list_pop_last_item(NODE* head);
// Function to reverse the linked list
void list_reverse(NODE* head);
// Function to search for a specific item in the linked list
NODE list_search(NODE currentNode, NODE* previousNode, NODE item);
// Function to remove a specific item from the linked list
void list_remove(NODE* head, NODE item);