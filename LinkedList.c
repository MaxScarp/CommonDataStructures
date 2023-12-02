#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#define NODE list_string_item*
#define POINTER_TO_NODE(x) &x

typedef struct list_string_item
{
    struct list_string_item* next;
    const char* string;
} list_string_item;

NODE list_string_item_new(const char* string);
NODE list_get_tail(NODE head);
NODE list_append(NODE* head, NODE item);
void list_print(NODE head);
NODE list_pop_first_item(NODE* head);
NODE list_pop_last_item(NODE* head);
void list_reverse(NODE* head);
void list_remove(NODE* head, NODE item);

int main(int argc, char** argv, char** envs)
{
    NODE head = NULL;

    NODE item1 = list_string_item_new("FIRST ELEMENT");
    NODE item2 = list_string_item_new("SECOND ELEMENT");
    NODE item3 = list_string_item_new("THIRD ELEMENT");
    NODE item4 = list_string_item_new("FOURTH ELEMENT");

    list_append(POINTER_TO_NODE(head), item1);
    list_append(POINTER_TO_NODE(head), item2);
    list_append(POINTER_TO_NODE(head), item3);
    list_append(POINTER_TO_NODE(head), item4);
    list_print(head);
    
    printf("The element %s has been popped from the list!\n" ,list_pop_first_item(POINTER_TO_NODE(head))->string);
    printf("The element %s has been popped from the list!\n" ,list_pop_last_item(POINTER_TO_NODE(head))->string);
    list_print(head);

    list_reverse(POINTER_TO_NODE(head));
    printf("The list has been reversed!\n");
    list_print(head);

    list_remove(POINTER_TO_NODE(head), item3);
    printf("The item3 has been removed from the list!\n");
    list_print(head);
    list_remove(POINTER_TO_NODE(head), item2);
    printf("The item2 has been removed from the list!\n");
    list_print(head);

    free(item1);
    free(item4);

    return 0;
}

#pragma region "FUNCTIONS"
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

void list_remove(NODE* head, NODE item)
{
    NODE currentNode = *head;
    NODE previousNode = NULL;
    if(!currentNode)
    {
        fprintf(stderr, "Error: Trying to access an empty list!\n");
        return;
    }

    while(currentNode)
    {
        if(currentNode == item)
        {
            if(previousNode)
            {
                previousNode->next = currentNode->next;
            }
            else
            {
                *head = currentNode->next;
            }

            free(currentNode);
            return;
        }

        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    fprintf(stderr, "Error: The specified item cannot be found!\n");
    return;
}

#pragma endregion