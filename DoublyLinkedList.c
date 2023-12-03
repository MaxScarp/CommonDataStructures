#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "doublyLinkedList.h"

int main(int argc, char** argv, char** envs)
{
    printf("----------HEAD1----------\n");

    NODE head1 = NULL;

    NODE item1 = list_string_item_new("FIRST ELEMENT");
    NODE item2 = list_string_item_new("SECOND ELEMENT");
    NODE item3 = list_string_item_new("THIRD ELEMENT");
    NODE item4 = list_string_item_new("FOURTH ELEMENT");

    list_append(POINTER_TO_NODE(head1), item1);
    list_append(POINTER_TO_NODE(head1), item2);
    list_append(POINTER_TO_NODE(head1), item3);
    list_append(POINTER_TO_NODE(head1), item4);
    list_print(head1);

    NODE poppedItem1 = list_pop_first_item(POINTER_TO_NODE(head1));
    printf("The element %s has been popped from the list!\n", poppedItem1->string);
    NODE poppedItem2 = list_pop_last_item(POINTER_TO_NODE(head1));
    printf("The element %s has been popped from the list!\n", poppedItem2->string);
    list_print(head1);

    list_remove(POINTER_TO_NODE(head1), item3);
    printf("The item3 has been removed from the list!\n");
    list_print(head1);
    list_remove(POINTER_TO_NODE(head1), item2);
    printf("The item2 has been removed from the list!\n");
    list_print(head1);

    printf("----------HEAD2----------\n");

    NODE head2 = NULL;

    NODE item5 = list_string_item_new("FIFTH ELEMENT");
    NODE item6 = list_string_item_new("SIXTH ELEMENT");
    NODE item7 = list_string_item_new("SEVENTH ELEMENT");

    list_append(POINTER_TO_NODE(head2), poppedItem1);
    list_append(POINTER_TO_NODE(head2), poppedItem2);
    list_print(head2);
    
    printf("---INSERT_AFTER---\n");

    list_insert_after(POINTER_TO_NODE(head2), poppedItem1, item5);
    printf("The item5 has been inserted after poppedItem1\n");
    list_print(head2);
    list_insert_after(POINTER_TO_NODE(head2), item5, item6);
    printf("The item6 has been inserted after item5\n");
    list_print(head2);
    list_insert_after(POINTER_TO_NODE(head2), poppedItem2, item7);
    printf("The item7 has been inserted after poppedItem2\n");
    list_print(head2);

    printf("---INSERT_BEFORE---\n");

    list_insert_before(POINTER_TO_NODE(head2), item6, list_pop_first_item(POINTER_TO_NODE(head2)));
    printf("The first item of the list has been inserted before item6\n");
    list_print(head2);
    list_insert_before(POINTER_TO_NODE(head2), item5, list_pop_last_item(POINTER_TO_NODE(head2)));
    printf("The last item of the list has been inserted before item5\n");
    list_print(head2);
    list_insert_before(POINTER_TO_NODE(head2), item6, list_pop_last_item(POINTER_TO_NODE(head2)));
    printf("The new last item of the list has been inserted before item6\n");
    list_print(head2);

    printf("----SHUFFLE---\n");

    list_shuffle(POINTER_TO_NODE(head2));
    list_print(head2);

    free(item1);
    free(item4);
    free(item5);
    free(item6);
    free(item7);

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

void list_print(NODE head)
{
    NODE currentNode = head;
    if(!currentNode)
    {
        fprintf(stderr, "Error: Trying to read an empty list!\n");
        return;
    }

    while(currentNode)
    {
        printf("Inside %p there is %s\n", currentNode, currentNode->string);
        currentNode = currentNode->next;
    }

    return;
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

void list_append(NODE* head, NODE item)
{
    if(!item)
    {
        fprintf(stderr, "Error: Trying to insert a NULL item!\n");
        return;
    }

    NODE currentNode = list_get_tail(*head);

    if(!currentNode)
    {
        *head = item;
    }
    else
    {
        currentNode->next = item;
    }

    item->previous = currentNode;
    item->next = NULL;

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
    currentNode->previous = NULL;
    currentNode->next = NULL;

    return currentNode;
}

NODE list_pop_last_item(NODE* head)
{
    NODE currentNode = list_get_tail(*head);
    if(!currentNode)
    {
        fprintf(stderr, "Error: Trying to pop from an empty list!\n");
        return NULL;
    }

    currentNode->previous->next = NULL;
    currentNode->previous = NULL;
    currentNode->next = NULL;

    return currentNode;
}

void list_remove(NODE* head, NODE item)
{
    if(!item)
    {
        fprintf(stderr, "Error: Trying to insert a NULL item!\n");
        return;
    }

    NODE currentNode = *head;
    if(!currentNode)
    {
        fprintf(stderr, "Error: Trying to access an empty list!\n");
        return;
    }

    if(currentNode == item)
    {
        *head = currentNode->next;
        if(*head)
        {
            (*head)->previous = NULL;
        }

        free(currentNode);
        return;
    }

    while(currentNode)
    {
        if(currentNode == item)
        {
            currentNode->previous->next = currentNode->next;
            if(currentNode->next)
            {
                currentNode->next->previous = currentNode->previous;
            }

            free(currentNode);
            return;
        }

        currentNode = currentNode->next;
    }

    fprintf(stderr, "Error: The specified item cannot be found!\n");
    return;
}

void list_insert_after(NODE* head, NODE itemToFind, NODE itemToInsert)
{
    if(!itemToFind)
    {
        fprintf(stderr, "Error: Trying to find a NULL item!\n");
        return;
    }

    if(!itemToInsert)
    {
        fprintf(stderr, "Error: Trying to insert a NULL item!\n");
        return;
    }

    NODE currentNode = *head;
    if(!currentNode)
    {
        fprintf(stderr, "Error: Trying to insert into an empty list!\n");
        return;
    }

    while(currentNode)
    {
        if(currentNode == itemToFind)
        {
            if(currentNode->next)
            {
                currentNode->next->previous = itemToInsert;
                itemToInsert->next = currentNode->next;
                itemToInsert->previous = currentNode;
                currentNode->next = itemToInsert;
            }
            else
            {
                currentNode->next = itemToInsert;
                itemToInsert->previous = currentNode;
                itemToInsert->next = NULL;
            }

            return;
        }

        currentNode = currentNode->next;
    }

    fprintf(stderr, "Error: The specified item cannot be found!\n");
    return;
}

void list_insert_before(NODE* head, NODE itemToFind, NODE itemToInsert)
{
    if(!itemToFind)
    {
        fprintf(stderr, "Error: Trying to find a NULL item!\n");
        return;
    }

    if(!itemToInsert)
    {
        fprintf(stderr, "Error: Trying to insert a NULL item!\n");
        return;
    }

    NODE currentNode = *head;
    if(!currentNode)
    {
        fprintf(stderr, "Error: Trying to insert into an empty list!\n");
        return;
    }

    if(currentNode == itemToFind)
    {
        itemToInsert->previous = NULL;
        itemToInsert->next = currentNode;
        currentNode->previous = itemToInsert;
        *head = itemToInsert;
        return;
    }

    while(currentNode)
    {
        if(currentNode == itemToFind)
        {
            itemToInsert->previous = currentNode->previous;
            itemToInsert->next = currentNode;
            currentNode->previous->next = itemToInsert;
            currentNode->previous = itemToInsert;

            return;
        }

        currentNode = currentNode->next;
    }

    fprintf(stderr, "Error: The specified item cannot be found!\n");
    return;
}

void list_shuffle(NODE* head)
{
    NODE currentNode = *head;
    if(!currentNode)
    {
        fprintf(stderr, "Error: Trying to access an empty list!\n");
        return;
    }

    srand((unsigned int)time(NULL));
    int randomNumber = rand();

    int itemAmount = 0;
    while(currentNode)
    {
        itemAmount++;
        currentNode = currentNode->next;
    }

    NODE* array = malloc(itemAmount * sizeof(NODE));
    if(!array)
    {
        fprintf(stderr, "Error: Unable to allocate enough meory!\n");
        return;
    }

    currentNode = *head;
    for(int i = 0; i < itemAmount; i++)
    {
        array[i] = currentNode;
        currentNode = currentNode->next;
    }

    for(int i = itemAmount - 1; i >= 0; i--)
    {
        int j = rand() % itemAmount;
        NODE tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }

    *head = array[0];
    array[0]->previous = NULL;
    for(int i = 0; i < itemAmount - 1; i++)
    {
        array[i]->next = array[i+1];
        array[i+1]->previous = array[i];
    }
    array[itemAmount - 1]->next = NULL;

    free(array);
}
#pragma endregion