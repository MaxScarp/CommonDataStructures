#include "misc.h"

// Define NODE as a pointer to list_string_item
#define NODE list_string_item*
// Macro to get a pointer to a NODE
#define POINTER_TO_NODE(x) &x

// Definition of the list_string_item structure representing a node in the linked list
typedef struct list_string_item
{
    struct list_string_item* previous;
    struct list_string_item* next;
    const char* string;
} list_string_item;

// Function to create a new string node with the given string
NODE list_string_item_new(const char* string);
// Function to print the linked list
void list_print(NODE head);
// Function to get the tail node of the linked list
NODE list_get_tail(NODE head);
// Function to append a node to the linked list
void list_append(NODE* head, NODE* tail, NODE item);
// Function to pop the first item from the linked list
NODE list_pop_first_item(NODE* head, NODE* tail)
// Function to pop the last item from the linked list;
NODE list_pop_last_item(NODE* head, NODE* tail);
// Function to remove a specific item from the linked list
void list_remove(NODE* head, NODE item);
// Function to insert an item after a specific item in the linked list
void list_insert_after(NODE* head, NODE itemToFind, NODE itemToInsert);
// Function to insert an item before a specific item in the linked list
void list_insert_before(NODE* head, NODE itemToFind, NODE itemToInsert);
// Function to shuffle the linked list
void list_shuffle(NODE* head);