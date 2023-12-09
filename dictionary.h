#include "set.h"

#define ITEM dictionary_item*
#define DICTIONARY dictionary*
#define VALUE void*

typedef struct dictionary_item
{
    NODE key;
    VALUE value;
} dictionary_item;

typedef struct dictionary
{
    ITEM* items;
    size_t hashmapSize;
} dictionary;