#include "linkedList.h"

int main(int argc, char** argv, char** envs)
{
    // Initialize an empty linked list
    NODE head = NULL;

    // Create string nodes for testing
    NODE item1 = list_string_item_new("FIRST ELEMENT");
    NODE item2 = list_string_item_new("SECOND ELEMENT");
    NODE item3 = list_string_item_new("THIRD ELEMENT");
    NODE item4 = list_string_item_new("FOURTH ELEMENT");

    // Append the nodes to the linked list
    list_append(POINTER_TO_NODE(head), item1);
    list_append(POINTER_TO_NODE(head), item2);
    list_append(POINTER_TO_NODE(head), item3);
    list_append(POINTER_TO_NODE(head), item4);
    // Print the linked list
    list_print(head);
    
    // Pop the first and last items from the linked list
    printf("The element %s has been popped from the list!\n" ,list_pop_first_item(POINTER_TO_NODE(head))->string);
    printf("The element %s has been popped from the list!\n" ,list_pop_last_item(POINTER_TO_NODE(head))->string);
    // Print the updated linked list
    list_print(head);

    // Reverse the linked list and print
    list_reverse(POINTER_TO_NODE(head));
    printf("The list has been reversed!\n");
    list_print(head);

    // Remove specific items and print the linked list
    list_remove(POINTER_TO_NODE(head), item3);
    printf("The item3 has been removed from the list!\n");
    list_print(head);
    list_remove(POINTER_TO_NODE(head), item2);
    printf("The item2 has been removed from the list!\n");
    list_print(head);

    // Free allocated memory for nodes
    free(item1);
    free(item4);

    return 0;
}

#pragma region "FUNCTIONS"
// Function to create a new string node
NODE list_string_item_new(const char* string)
{
    NODE item = malloc(sizeof(list_string_item));
    if(!item)
    {
        fprintf(stderr, "Error: Unable to allocate enough meory!\n");
        return NULL;
    }

    item->string = string;

    return item;
}

// Function to get the tail node of the linked list
NODE list_get_tail(NODE head)
{
    NODE currentNode = head;
    NODE lastNode = NULL;

    while(currentNode)
    {
        lastNode = currentNode;
        currentNode = currentNode->next;
    }

    return lastNode;
}

// Function to append a node to the linked list
NODE list_append(NODE* head, NODE item)
{
    NODE currentNode = list_get_tail(*head);
    if(!currentNode)
    {
        *head = item;
    }
    else
    {
        currentNode->next = item;
    }

    item->next = NULL;

    return item;
}

// Function to print the linked list
void list_print(NODE head)
{
    NODE currentNode = head;
    if(!currentNode)
    {
        fprintf(stderr, "Error: Trying to access an empty list!\n");
        return;
    }

    while(currentNode)
    {
        printf("Inside %p there is %s\n", currentNode, currentNode->string);
        currentNode = currentNode->next;
    }

    return;
}

// Function to pop the first item from the linked list
NODE list_pop_first_item(NODE* head)
{
    NODE currentNode = *head;
    if(!currentNode)
    {
        fprintf(stderr, "Error: Trying to pop from an empty list!\n");
        return NULL;
    }
    
    *head = currentNode->next;
    currentNode->next = NULL;

    return currentNode;
}

// Function to pop the last item from the linked list
NODE list_pop_last_item(NODE* head)
{
    NODE currentNode = *head;
    NODE previousNode = NULL;

    if(!currentNode)
    {
        fprintf(stderr, "Error: Trying to pop from an empty list!\n");
        return NULL;
    }

    if(!currentNode->next)
    {
        *head = NULL;
        return currentNode;
    }

    while(currentNode->next)
    {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    previousNode->next = NULL;

    return currentNode;
}

// Function to reverse the linked list
void list_reverse(NODE* head)
{
    NODE previousNode = NULL;
    NODE currentNode = *head;
    NODE nextNode = NULL;

    if(!currentNode)
    {
        fprintf(stderr, "Error: Trying to reverse an empty list!\n");
        return;
    }

    if(!currentNode->next)
    {
        return;
    }

    while(currentNode)
    {
        nextNode = currentNode->next;
        currentNode->next = previousNode;
        previousNode = currentNode;
        currentNode = nextNode;
    }

    *head = previousNode;

    return;
}

// Function to search for a specific item in the linked list
NODE list_search(NODE currentNode, NODE* previousNode, NODE item)
{
    while(currentNode)
    {
        if((currentNode == item) || (strcmp(currentNode->string, item->string) == 0))
        {
            return currentNode;
        }

        *previousNode = currentNode;
        currentNode = currentNode->next;
    }

    return NULL;
}

// Function to remove a specific item from the linked list
void list_remove(NODE* head, NODE item)
{
    NODE currentNode = *head;
    if(!currentNode)
    {
        fprintf(stderr, "Error: Trying to access an empty list!\n");
        return;
    }

    NODE previousNode = NULL;
    currentNode = list_search(currentNode, &previousNode, item);
    if(!currentNode)
    {
        fprintf(stderr, "Error: The specified item cannot be found!\n");
        return;
    }

    if(previousNode)
    {
        previousNode->next = currentNode->next;
    }
    else
    {
        *head = currentNode->next;
    }

    printf("%s has been removed succesfully!\n", item->string);
    free(currentNode);
    return;
}

#pragma endregion