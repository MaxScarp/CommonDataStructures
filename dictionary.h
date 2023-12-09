#include "set.h"
#include <stdbool.h>

#define ITEM dictionary_item*
#define DICTIONARY dictionary*
#define VALUE void*
#define COLLISION_THRESHOLD 3

typedef union value_item {
    char* str_value;
    int int_value;
    char char_value;
    float float_value;
    double double_value;
    bool bool_value;
} value_item;

typedef struct dictionary_item
{
    NODE key;
    value_item value;
} dictionary_item;

typedef struct dictionary
{
    ITEM* items;
    size_t hashmapSize;
    size_t collisionAmount;
} dictionary;