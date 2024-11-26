#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialize_list(list *list,void (*destroy)(void *data), int (*match)(const void *key1, const void *key2),
        int (*insert)(struct Linked_List *list, node *element, const void *data)
) {
    if (list == NULL) return; // Safety check

    list->size = 0;
    list->destroy = destroy;
    list->match = match;
    list->insert = insert;
    list->head = NULL;
    list->tail = NULL;
}
void list_destroy(list *list1) {
    void *data;
    while (list_size(list1) > 0) {
        if (list_rem_next(list1, NULL, (void **)&data) == 0 && list1->destroy != NULL) {
            list1->destroy(data);
        }
    }

    memset(list1, 0, sizeof(list));
}


void destroy_list(list *list){
    void *data;
    while(list_size(list) > 0){
        if(list_rem_next(list, NULL, (void**) &data) == 0 && list->destroy != NULL){
            list->destroy(data);
        }
    }
    memset(list, 0, sizeof(struct Linked_List));
}

int list_ins_next(list *list, node *element, const void *data){
    node *new_element;
    if((new_element = (node*)malloc(sizeof(node))) == NULL){
        return -1;
    }

    new_element->data = (void*)data;
    if(element == NULL){
        if(list_size(list) == 0){
            list->tail = new_element;
        }
        new_element->next = list->head;
        list->head = new_element;
    }
    else {
        if(element->next == NULL){
            list->tail = new_element;
        }

        //new_element->next = element->next;
        element->next = new_element;
        new_element->next = NULL;
    }

    list->size++;
    return 0;
}

int list_rem_next(struct Linked_List *list, node *element, void **data) {
    node *old_element;
    //Can't remove empty list
    if(list_size(list) == 0){
        return -1;
    }

    if(element == NULL){
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;
        if (list_size(list) == 1)
            list->tail = NULL;
    }

    else {
        if (element->next == NULL)
            return -1;
        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;
        if (element->next == NULL)
            list->tail = element;
    }
    free(old_element);
    list->size--;
    return 0;
}
