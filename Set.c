#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

int main(int argc, char** argv, char** envs)
{
    TABLE table = set_table_new(3);

    NODE item1 = list_string_item_new("PIPPO");
    NODE item2 = list_string_item_new("PLUTO");
    NODE item3 = list_string_item_new("FEDERICO");
    NODE item4 = list_string_item_new("RICCARDO");
    NODE item5 = list_string_item_new("ANASTASIA");
    NODE item6 = list_string_item_new("TEST");

    printf("---FILL---\n");

    set_print_table(table);
    set_insert(table, item1);
    set_insert(table, item2);
    set_insert(table, item3);
    set_insert(table, item4);
    set_insert(table, item5);
    set_print_table(table);

    printf("---INSERT_ERROR---\n");

    set_insert(table, item1);
    set_insert(table, item5);
    set_insert(table, item3);
    set_insert(table, item4);

    printf("---SEARCH---\n");

    set_search(table, item3);
    set_search(table, item1);
    set_search(table, item5);
    set_search(table, item4);
    set_search(table, item6);

    printf("---REMOVE---\n");

    set_remove(table, item6);
    set_remove(table, item5);
    set_remove(table, item1);
    set_remove(table, item2);
    set_remove(table, item4);
    set_remove(table, item3);
    set_remove(table, item6);

    return 0;
}

#pragma region "FUNCTIONS"
size_t djb33x_hash(const char* key)
{
    size_t hash = 5381;

    for(size_t i = 0; i < strlen(key); i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }

    return hash;
}

TABLE set_table_new(const size_t hashmapSize)
{
    if(!hashmapSize)
    {
        fprintf(stderr, "Error: Cannot create a table of 0 size!\n");
        return NULL;
    }

    TABLE table = malloc(sizeof(set_table));
    if(!table)
    {
        fprintf(stderr, "Error: Unable to allocate enough memory for table!\n");
        return NULL;
    }

    table->hashmapSize = hashmapSize;
    table->nodes = calloc(table->hashmapSize, sizeof(NODE));
    if(!table->nodes)
    {
        free(table);
        fprintf(stderr, "Error: Unable to allocate enough memory for node!\n");
        return NULL;
    }

    return table;
}

NODE set_insert(TABLE table, NODE item)
{
    if(!table)
    {
        fprintf(stderr, "Error: Trying to access an empty table!\n");
        return NULL;
    }

    const size_t keyLen = strlen(item->string);
    if(!item->string || !keyLen)
    {
        fprintf(stderr, "Error: Trying to insert an empty value!\n");
        return NULL;
    }

    size_t hash = djb33x_hash(item->string);
    size_t index = hash % table->hashmapSize;

    NODE head = table->nodes[index];
    if(!head)
    {
        table->nodes[index] = malloc(sizeof(list_string_item));
        if(!table->nodes[index])
        {
            fprintf(stderr, "Error: Unable to allocate enough memory for node!\n");
            return NULL;
        }

        table->nodes[index]->string = item->string;
        table->nodes[index]->next = NULL;

        return table->nodes[index];
    }

    if(strcmp(head->string, item->string) == 0)
    {
        fprintf(stderr, "Error: Trying to insert an existing key: %s found at index %zu!\n", item->string, index);
        return NULL;
    }
    
    if(head->next)
    {
        NODE currentNode = head->next;
        NODE previousNode = head;

        currentNode = list_search(currentNode, &previousNode, item);
        if(currentNode)
        {
            fprintf(stderr, "Error: Trying to insert an existing key: %s found at index %zu!\n", item->string, index);
            return NULL;
        }
    }

    list_append(POINTER_TO_NODE(head), item);

    return item;
}

void set_print_table(TABLE table)
{
    if(!table)
    {
        fprintf(stderr, "Error: Trying to print an empty table!\n");
        return;
    }

    for(int i = 0; i < table->hashmapSize; i++)
    {
        if(!table->nodes[i])
        {
            continue;
        }

        list_print(table->nodes[i]);
    }

    return;
}

NODE set_search(TABLE table, NODE item)
{
    if(!table)
    {
        fprintf(stderr, "Error: Trying to access an empty table!\n");
        return NULL;
    }

    const size_t keyLen = strlen(item->string);
    if(!item->string || !keyLen)
    {
        fprintf(stderr, "Error: Trying to search for empty value!\n");
        return NULL;
    }

    const size_t hash = djb33x_hash(item->string);
    const size_t index = hash % table->hashmapSize;

    NODE currentNode = table->nodes[index];
    if(!currentNode)
    {
        fprintf(stderr, "Error: No item named %s found!\n", item->string);
        return NULL;
    }

    NODE previousNode = NULL;
    currentNode = list_search(currentNode, &previousNode, item);
    if(currentNode)
    {
        printf("Key %s found at index %zu position\n", item->string, index);
        return currentNode;
    }

    fprintf(stderr, "Error: No item named %s found!\n", item->string);
    return NULL;
}

void set_remove(TABLE table, NODE item)
{
    if(!table)
    {
        fprintf(stderr, "Error: Trying to remove from an empty table!\n");
        return;
    }

    const size_t keyLen = strlen(item->string);
    if(!item->string || !keyLen)
    {
        fprintf(stderr, "Error: Trying to remove an empty value!\n");
        return;
    }

    const size_t hash = djb33x_hash(item->string);
    const size_t index = hash % table->hashmapSize;

    NODE currentNode = table->nodes[index];
    if(!currentNode)
    {
        fprintf(stderr, "Error: No item named %s found!\n", item->string);
        return;
    }

    list_remove(POINTER_TO_NODE(table->nodes[index]), item);

    return;
}
#pragma endregion