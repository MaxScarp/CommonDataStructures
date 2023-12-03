#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

int main(int argc, char** argv, char** envs)
{
    TABLE table = set_table_new(3);

    printf("---FILL---\n");

    set_print_table(table);
    set_insert(table, "PIPPO");
    set_insert(table, "PLUTO");
    set_insert(table, "FEDERICO");
    set_insert(table, "RICCARDO");
    set_insert(table, "ANASTASIA");
    set_print_table(table);

    printf("---INSERT_ERROR---\n");

    set_insert(table, "PLUTO");
    set_insert(table, "ANASTASIA");
    set_insert(table, "FEDERICO");
    set_insert(table, "RICCARDO");

    printf("---SEARCH---\n");

    set_search(table, "FEDERICO");
    set_search(table, "PIPPO");
    set_search(table, "ANASTASIA");
    set_search(table, "RICCARDO");
    set_search(table, "TEST");

    printf("---REMOVE---\n");

    set_remove(table, "TEST");
    set_remove(table, "ANASTASIA");
    set_remove(table, "PIPPO");
    set_remove(table, "PLUTO");
    set_print_table(table);
    set_remove(table, "RICCARDO");
    set_remove(table, "FEDERICO");
    set_remove(table, "TEST");
    set_print_table(table);

    return 0;
}

#pragma region "FUNCTIONS"
size_t djb33x_hash(const char *key)
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

NODE set_insert(TABLE table, const char* key)
{
    if(!table)
    {
        fprintf(stderr, "Error: Trying to access an empty table!\n");
        return NULL;
    }

    const size_t keyLen = strlen(key);
    if(!key || !keyLen)
    {
        fprintf(stderr, "Error: Trying to insert an empty value!\n");
        return NULL;
    }

    size_t hash = djb33x_hash(key);
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

        table->nodes[index]->string = key;
        table->nodes[index]->next = NULL;

        return table->nodes[index];
    }

    if(strcmp(head->string, key) == 0)
    {
        fprintf(stderr, "Error: Trying to insert an existing key: %s found at index %zu position %d!\n", key, index, 1);
        return NULL;
    }
    
    if(head->next)
    {
        NODE currentNode = head->next;
        int position = 0;
        while(currentNode)
        {
            position++;

            if(strcmp(currentNode->string, key) == 0)
            {
                fprintf(stderr, "Error: Trying to insert an existing key: %s found at index %zu position %d!\n", key, index, position);
                return NULL;
            }

            currentNode = currentNode->next;
        }
    }

    NODE newNode = malloc(sizeof(list_string_item));
    if(!newNode)
    {
        fprintf(stderr, "Error: Unable to allocate enough memory for node!\n");
        return NULL;
    }

    newNode->string = key;
    newNode->next = NULL;

    NODE currentNode = head;
    while(currentNode->next)
    {
        currentNode = currentNode->next;
    }
    currentNode->next = newNode;

    return newNode;
}

void set_print_nodes(NODE head, const int index)
{
    NODE currentNode = head;
    while(currentNode)
    {
        printf("Index %d: %s\n", index, currentNode->string);
        currentNode = currentNode->next;
    }

    return;
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
        set_print_nodes(table->nodes[i], i);
    }

    return;
}

NODE set_search(TABLE table, const char* key)
{
    if(!table)
    {
        fprintf(stderr, "Error: Trying to access an empty table!\n");
        return NULL;
    }

    const size_t keyLen = strlen(key);
    if(!key || !keyLen)
    {
        fprintf(stderr, "Error: Trying to search for empty value!\n");
        return NULL;
    }

    const size_t hash = djb33x_hash(key);
    const size_t index = hash % table->hashmapSize;

    NODE currentNode = table->nodes[index];
    if(!currentNode)
    {
        fprintf(stderr, "Error: No item named %s found!\n", key);
        return NULL;
    }

    int position = 0;
    while(currentNode)
    {
        position++;

        if(strcmp(currentNode->string, key) == 0)
        {
            printf("Key %s found at index %zu position %d\n", key, index, position);
            return currentNode;
        }

        currentNode = currentNode->next;
    }

    fprintf(stderr, "Error: No item named %s found!\n", key);
    return NULL;
}

void set_remove(TABLE table, const char* key)
{
    if(!table)
    {
        fprintf(stderr, "Error: Trying to remove from an empty table!\n");
        return;
    }

    const size_t keyLen = strlen(key);
    if(!key || !keyLen)
    {
        fprintf(stderr, "Error: Trying to remove an empty value!\n");
        return;
    }

    const size_t hash = djb33x_hash(key);
    const size_t index = hash % table->hashmapSize;

    NODE currentNode = table->nodes[index];
    if(!currentNode)
    {
        fprintf(stderr, "Error: No item named %s found!\n", key);
        return;
    }

    NODE previousNode = NULL;
    while(currentNode)
    {
        if(strcmp(currentNode->string, key) == 0)
        {
            if(!previousNode)
            {
                table->nodes[index] = currentNode->next;
            }
            else
            {
                previousNode->next = currentNode->next;
            }

            free(currentNode);
            return;
        }

        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    fprintf(stderr, "Error: No item named %s found!\n", key);
    return;
}
#pragma endregion