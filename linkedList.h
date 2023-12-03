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
NODE list_search(NODE currentNode, NODE* previousNode, NODE item);
void list_remove(NODE* head, NODE item);