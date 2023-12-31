#include "dictionary.h"

DICTIONARY set_dictionary_new(const size_t hashmapSize)
{
    if(!hashmapSize)
    {
        fprintf(stderr, "Error: Cannot create a table of 0 size!\n");
        return NULL;
    }

    DICTIONARY dictionary = malloc(sizeof(dictionary));
    if(!dictionary)
    {
        fprintf(stderr, "Error: Unable to allocate enough memory for dictionary!\n");
        return NULL;
    }

    dictionary->hashmapSize = hashmapSize;
    dictionary->items = calloc(dictionary->hashmapSize, sizeof(ITEM));
    if(!dictionary->items)
    {
        free(dictionary);
        fprintf(stderr, "Error: Unable to allocate enough memory for items!\n");
        return NULL;
    }

    dictionary->collisionAmount = 0;

    return dictionary;
}

int dictionary_calculate_index(DICTIONARY dictionary, NODE key)
{
    const size_t keyLen = strlen(key->string);
    if(!key->string || !keyLen)
    {
        return -1;
    }

    size_t hash = djb33x_hash(key->string);
    int index = hash % dictionary->hashmapSize;

    return index;
}

bool is_string(VALUE value)
{
    return (sizeof(*(char**)value) == sizeof(char*));
}
bool is_int(VALUE value)
{
    return (sizeof(*(int*)value) == sizeof(int));
}
bool is_char(VALUE value)
{
    return (sizeof(*(char*)value) == sizeof(char));
}
bool is_float(VALUE value)
{
    return (sizeof(*(float*)value) == sizeof(float));
}
bool is_double(VALUE value)
{
    return (sizeof(*(double*)value) == sizeof(double));
}
bool is_bool(VALUE value)
{
    return (sizeof(*(bool*)value) == sizeof(bool));
}

void dictionary_resize(DICTIONARY dictionary)
{
    size_t oldHashmapSize = dictionary->hashmapSize;
    dictionary->hashmapSize = (dictionary->hashmapSize * 2);

    ITEM* newItems = calloc(dictionary->hashmapSize, sizeof(ITEM));
    if(!newItems)
    {
        free(dictionary);
        fprintf(stderr, "Error: Unable to allocate enough memory for newItems!\n");
    }

    for (size_t i = 0; i < oldHashmapSize; i++)
    {
        if(dictionary->items[i] && dictionary->items[i]->key)
        {
            NODE currentNode = dictionary->items[i]->key;
            while(currentNode)
            {
                NODE poppedNode = list_pop_last_item(&currentNode);

                int index = dictionary_calculate_index(dictionary, poppedNode);
                
                if(newItems[index]->key)
                {
                    list_append(POINTER_TO_NODE(newItems[index]->key), poppedNode);
                }
                else
                {
                    newItems[index] = dictionary->items[i];
                    newItems[index]->key = poppedNode;
                    newItems[index]->value = dictionary->items[i]->value;
                }
            }
        }

    }

    
    

    //ITEM* newItems = calloc(newHashmapSize, sizeof(ITEM));
    //if (!newItems)
    //{
    //    fprintf(stderr, "Error: Unable to allocate memory for resized items!\n");
    //    return;
    //}
//
    //for (size_t i = 0; i < dictionary->hashmapSize; i++)
    //{
    //    if(dictionary->items[i] && dictionary->items[i]->key)
    //    {
    //        NODE currentNode = dictionary->items[i]->key;
    //        while(currentNode)
    //        {
    //            NODE poppedNode = list_pop_first_item(&currentNode);
//
    //            int index = dictionary_calculate_index(dictionary, poppedNode);
    //            
    //            newItems[index] = dictionary->items[i];
    //        }
    //    }
    //}
//
    //free(dictionary->items);
    //dictionary->items = newItems;
    //dictionary->hashmapSize = newHashmapSize;
    //dictionary->collisionAmount = 0;
//
    //printf("Resizing of the dictionary completed succesfully!\n");

    return;
}

ITEM dictionary_insert(DICTIONARY dictionary, NODE key, VALUE value)
{
    if(!dictionary)
    {
        fprintf(stderr, "Error: Trying to access an empty dictionary!\n");
        return NULL;
    }

    int index = dictionary_calculate_index(dictionary, key);

    if(index < 0)
    {
        fprintf(stderr, "Error: Trying to insert an empty key!\n");
        return NULL;
    }

    ITEM currentItem = dictionary->items[index];
    if(!currentItem)
    {
        dictionary->items[index] = malloc(sizeof(dictionary_item));
        if(!dictionary->items[index])
        {
            fprintf(stderr, "Error: Unable to allocate enough memory for item!\n");
            return NULL;
        }
        
        dictionary->items[index]->key = list_string_item_new(key->string);
        if(!dictionary->items[index]->key)
        {
            fprintf(stderr, "Error: Unable to allocate enough memory for stringItem!\n");
            free(dictionary->items[index]);
            return NULL;
        }

        dictionary->items[index]->key->string = key->string;
        
        if(is_string(value))
        {
            dictionary->items[index]->value.str_value = (char*)value;
        }
        else if(is_int(value))
        {
            dictionary->items[index]->value.int_value = *(int*)value;
        }
        else if(is_char(value))
        {
            dictionary->items[index]->value.char_value = *(char*)value;
        }
        else if(is_float(value))
        {
            dictionary->items[index]->value.float_value = *(float*)value;
        }
        else if(is_double(value))
        {
            dictionary->items[index]->value.double_value = *(double*)value;
        }
        else if(is_bool(value))
        {
            dictionary->items[index]->value.bool_value = *(bool*)value;
        }
        else
        {
            fprintf(stderr, "Error: Value type not supported!\n");
            free(dictionary->items[index]->key);
            free(dictionary->items[index]);
            return NULL;
        }

        return dictionary->items[index];
    }
    else
    {
        dictionary->collisionAmount = (dictionary->collisionAmount + 1);
    }

    if(strcmp(currentItem->key->string, key->string) == 0)
    {
        fprintf(stderr, "Error: Trying to insert an existing key: %s found at index %d!\n", key->string, index);
        return NULL;
    }
    
    if(currentItem->key->next)
    {
        NODE currentNode = currentItem->key->next;
        NODE previousNode = currentItem->key;

        currentNode = list_search(currentNode, &previousNode, key);
        if(currentNode)
        {
            fprintf(stderr, "Error: Trying to insert an existing key: %s found at index %d!\n", currentNode->string, index);
            return NULL;
        }
    }

    ITEM newItem = malloc(sizeof(dictionary_item));
    if(!newItem)
    {
        fprintf(stderr, "Error: Unable to allocate enough memory for newItem!\n");
        return NULL;
    }
    
    NODE newListStringItem = list_string_item_new(key->string);
    if(!newListStringItem)
    {
        fprintf(stderr, "Error: Unable to allocate enough memory for newListStringItem!\n");
        return NULL;
    }

    newItem->key = newListStringItem;
    newItem->key->string = key->string;
    
    if(is_string(value))
    {
        newItem->value.str_value = (char*)value;
    }
    else if(is_int(value))
    {
        newItem->value.int_value = *(int*)value;
    }
    else if(is_char(value))
    {
        newItem->value.char_value = *(char*)value;
    }
    else if(is_float(value))
    {
        newItem->value.float_value = *(float*)value;
    }
    else if(is_double(value))
    {
        newItem->value.double_value = *(double*)value;
    }
    else if(is_bool(value))
    {
        newItem->value.bool_value = *(bool*)value;
    }
    else
    {
        fprintf(stderr, "Error: Value type not supported!\n");
        free(newItem->key);
        free(newItem);
        return NULL;
    }

    list_append(POINTER_TO_NODE(currentItem->key), newListStringItem);

    return newItem;
}

void dictionary_print(DICTIONARY dictionary)
{
    if(!dictionary)
    {
        fprintf(stderr, "Error: Trying to print an empty dictionary!\n");
        return;
    }

    for(int i = 0; i < dictionary->hashmapSize; i++)
    {
        if(!dictionary->items[i])
        {
            continue;
        }

        printf("Index: %d\n", i+1);
        list_print(dictionary->items[i]->key);
    }

    return;
}

ITEM dictionary_search(DICTIONARY dictionary, ITEM item, int index)
{
    if(!dictionary)
    {
        fprintf(stderr, "Error: Trying to access an empty dictionary!\n");
        return NULL;
    }

    if(index < 0)
    {
        index = dictionary_calculate_index(dictionary, item->key);
    }

    if(index < 0)
    {
        fprintf(stderr, "Error: Trying to insert an empty key!\n");
        return NULL;
    }

    ITEM currentItem = dictionary->items[index];
    if(!currentItem)
    {
        fprintf(stderr, "Error: No item named %s found!\n", item->key->string);
        return NULL;
    }

    NODE currentNode = currentItem->key;
    if(!currentNode)
    {
        fprintf(stderr, "Error: No key named %s found!\n", item->key->string);
        return NULL;
    }

    NODE previousNode = NULL;
    currentNode = list_search(currentNode, &previousNode, item->key);
    if(currentNode)
    {
        printf("Key %s found at index %d position\n", item->key->string, index);
        return item;
    }

    fprintf(stderr, "Error: No item named %s found!\n", item->key->string);
    return NULL;
}

void dictionary_remove(DICTIONARY dictionary, ITEM item)
{
    const int index = dictionary_calculate_index(dictionary, item->key);

    if(index < 0)
    {
        fprintf(stderr, "Error: Trying to insert an empty key!\n");
        return;
    }

    ITEM itemToRemove = dictionary_search(dictionary, item, index);
    if(itemToRemove)
    {
        list_remove(POINTER_TO_NODE(dictionary->items[index]->key), itemToRemove->key);

        free(itemToRemove);
    }

    return;
}

value_item* dictionary_get_value(DICTIONARY dictionary, ITEM item)
{
    ITEM itemFound = dictionary_search(dictionary, item, -1);
    if(itemFound)
    {
        return &(item->value);
    }

    return NULL;
}

int main(int argc, char** argv, char** envs)
{
    DICTIONARY dictionary = set_dictionary_new(3);

    NODE key1 = list_string_item_new("PIPPO");
    NODE key2 = list_string_item_new("PLUTO");
    NODE key3 = list_string_item_new("FEDERICO");
    NODE key4 = list_string_item_new("RICCARDO");
    NODE key5 = list_string_item_new("ANASTASIA");
    NODE key6 = list_string_item_new("TEST");

    printf("---FILL---\n");

    ITEM item1 = dictionary_insert(dictionary, key1, (VALUE)"NUCLEO");
    ITEM item2 = dictionary_insert(dictionary, key2, (VALUE)600000);
    ITEM item3 = dictionary_insert(dictionary, key3, (VALUE)'h');
    ITEM item4 = dictionary_insert(dictionary, key4, (VALUE)-10);
    ITEM item5 = dictionary_insert(dictionary, key5, (VALUE)-50000);
    ITEM item6 = dictionary_insert(dictionary, key6, (VALUE)false);
    dictionary_print(dictionary);

    dictionary_resize(dictionary);
    
    //dictionary_print(dictionary);



    //printf("---INSERT_ERROR---\n");
//
    //dictionary_insert(dictionary, key1, (VALUE)10);
    //dictionary_insert(dictionary, key5, (VALUE)65);
    //dictionary_insert(dictionary, key3, (VALUE)123);
    //dictionary_insert(dictionary, key4, (VALUE)-156);
//
    //printf("---SEARCH---\n");
//
    //dictionary_search(dictionary, item3, -1);
    //dictionary_search(dictionary, item1, -1);
    //dictionary_search(dictionary, item5, -1);
    //dictionary_search(dictionary, item4, -1);
    //dictionary_search(dictionary, item6, -1);
    //
    //printf("---GET_VALUE---\n");
//
    //printf("%s->%s\n",item1->key->string, *(char**)dictionary_get_value(dictionary, item1));
    //printf("%s->%d\n",item2->key->string, *(int*)dictionary_get_value(dictionary, item2));
    //printf("%s->%c\n",item3->key->string, *(char*)dictionary_get_value(dictionary, item3));
    //printf("%s->%d\n",item4->key->string, *(int*)dictionary_get_value(dictionary, item4));
    //printf("%s->%d\n",item5->key->string, *(int*)dictionary_get_value(dictionary, item5));
    //printf("%s->%d\n",item6->key->string, *(int*)dictionary_get_value(dictionary, item6));
//
    //printf("---REMOVE---\n");
//
    //dictionary_remove(dictionary, item6);
    //dictionary_remove(dictionary, item5);
    //dictionary_remove(dictionary, item1);
    //dictionary_remove(dictionary, item2);
    //dictionary_remove(dictionary, item4);
    //dictionary_remove(dictionary, item3);
    //dictionary_remove(dictionary, item6);
//
    //printf("---GET_VALUE_ERROR---\n");
//
    //VALUE valueTest1 = dictionary_get_value(dictionary, item3);
    //VALUE valueTest2 = dictionary_get_value(dictionary, item4);

    return 0;
}