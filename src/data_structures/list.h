#ifndef DMI_STRUCT_C_LIST_H
#define DMI_STRUCT_C_LIST_H
#endif //DMI_STRUCT_C_LIST_H




#ifdef __cplusplus
extern "C" {
#endif
#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)



typedef struct Node {
    void *data;
    struct Node *next;
    struct Node *prev;
}node;


typedef struct Linked_List {
    struct Node *head;
    struct Node *tail;
    int size;
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    int (*insert)(struct Linked_List *list, node *element, const void *data);

}list;

void list_init(list *list, void (*destroy)(void *data));
void initialize_list(list *list, void (*destroy)(void *data), int (*match)(const void *key1, const void *key2),
                     int (*insert)(struct Linked_List *list, node *element, const void *data));
void list_destroy(list *list);
int list_ins_next(list *list, node *element, const void *data);
int list_rem_next(list *list, node *element, void **data);
int list_insert_end();
#ifdef __cplusplus
}
#endif