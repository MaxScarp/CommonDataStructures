#include "linkedList.h"

#define TABLE set_table*

typedef struct set_table
{
    NODE* nodes;
    size_t hashmapSize;
} set_table;

size_t djb33x_hash(const char *key);
TABLE set_table_new(const size_t hashmapSize);
NODE set_search(TABLE table, NODE item);
NODE set_insert(TABLE table, NODE item);
void set_print_table(TABLE table);
void set_remove(TABLE table, NODE item);