#include "set.h"

int main(int argc, char** argv, char** envs)
{
    // Create a new set table with a specified hashmap size of 3
    TABLE table = set_table_new(3);

    // Create new nodes with string values
    NODE item1 = list_string_item_new("PIPPO");
    NODE item2 = list_string_item_new("PLUTO");
    NODE item3 = list_string_item_new("FEDERICO");
    NODE item4 = list_string_item_new("RICCARDO");
    NODE item5 = list_string_item_new("ANASTASIA");
    NODE item6 = list_string_item_new("TEST");

    printf("---FILL---\n");

    // Print the contents of the set table (initially empty)
    set_print_table(table);
    // Insert items into the set table
    set_insert(table, item1);
    set_insert(table, item2);
    set_insert(table, item3);
    set_insert(table, item4);
    set_insert(table, item5);
    // Print the contents of the set table after insertion
    set_print_table(table);

    // Attempt to insert duplicate items and display error messages
    printf("---INSERT_ERROR---\n");

    set_insert(table, item1);
    set_insert(table, item5);
    set_insert(table, item3);
    set_insert(table, item4);

    // Search for items in the set table and display messages
    printf("---SEARCH---\n");

    set_search(table, item3);
    set_search(table, item1);
    set_search(table, item5);
    set_search(table, item4);
    set_search(table, item6);

    // Remove items from the set table and display messages
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
// Function to calculate the DJB33X hash value for a given key
size_t djb33x_hash(const char* key)
{
    size_t hash = 5381;

    for(size_t i = 0; i < strlen(key); i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }

    return hash;
}

// Function to create a new set table with the specified hashmap size
TABLE set_table_new(const size_t hashmapSize)
{
    // Validate the hashmap size
    if(!hashmapSize)
    {
        fprintf(stderr, "Error: Cannot create a table of 0 size!\n");
        return NULL;
    }

    // Allocate memory for the table structure
    TABLE table = malloc(sizeof(set_table));
    if(!table)
    {
        fprintf(stderr, "Error: Unable to allocate enough memory for table!\n");
        return NULL;
    }

    // Allocate memory for the array of nodes in the table
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

// Function to insert an item into the set table
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

    // Calculate the hash value and index for the item
    size_t hash = djb33x_hash(item->string);
    size_t index = hash % table->hashmapSize;

    // Retrieve the head of the linked list at the calculated index
    NODE head = table->nodes[index];
    // If the list is empty at this index, create a new node and set it as the head
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

    // If the head contains the same key, report an error as duplicate keys are not allowed
    if(strcmp(head->string, item->string) == 0)
    {
        fprintf(stderr, "Error: Trying to insert an existing key: %s found at index %zu!\n", item->string, index);
        return NULL;
    }

    // If the head has a next node, check the rest of the list for duplicates
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

     // If no duplicates were found, append the item to the end of the linked list
    list_append(POINTER_TO_NODE(head), item);

    return item;
}

// Function to print the contents of the set table
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

// Function to search for an item in the set table
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

// Function to remove an item from the set table
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