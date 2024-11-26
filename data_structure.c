#include "data_structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned long hash_string(const void *key) {
    unsigned long hash = 5381;
    const char *str = (const char*) key; // Cast void* back to const char*
    int c;
    while ((c = (int)*str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}


int init_hash_table(hash_table *table, int buckets, unsigned long  (*hash_function)(const void *key),
                    int (*match)(const void *key1, const void *key2), void (*destroy)(void *data)){

    int i;
    if((table->table = (void*) malloc(buckets * sizeof(char))) == NULL){
        return -1;
    }

    table->buckets = buckets;
    for(i = 0; i < buckets; i++){
        list_init(&table->table[i], destroy);
    }

    table->hash_function = hash_function;
    table->match = match;
    table->destroy = destroy;
    table->size = 0;
    return 0;
}

void list_init(list *list, void (*destroy)(void *data)) {
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}


int chtbl_lookup(const hash_table *hash_table, void **data) {
    node *element;
    int bucket;
/*****************************************************************************
* *
* Hash the key. *
* *
*****************************************************************************/
    bucket = (int)(hash_table->hash_function(*data) % hash_table->buckets);
/*****************************************************************************
* *
* Search for the data in the bucket. *
* *
*****************************************************************************/
    for (element = list_head(&hash_table->table[bucket]); element != NULL; element = list_next(element)) {
        if (hash_table->match(*data, list_data(element))) {
            /***********************************************************************
            * *
            * Pass back the data from the table. *
            * *
            ***********************************************************************/
            *data = list_data(element);
            return 0;
        }
    }
    return -1;
}

int chtbl_remove(hash_table *htbl, void **data) {

    node *element,*prev;
    int bucket;
/*****************************************************************************
* *
* Hash the key. *
* *
*****************************************************************************/
    bucket = (int)(htbl->hash_function(*data) % htbl->buckets);
/*****************************************************************************
* *
* Search for the data in the bucket. *
* *
*****************************************************************************/
    prev = NULL;
    for (element = list_head(&htbl->table[bucket]); element != NULL; element =
                                                                             list_next(element)) {
        if (htbl->match(*data, list_data(element))) {
            /***********************************************************************
            * *
            * Remove the data from the bucket. *
            * *
            ***********************************************************************/
            if (list_rem_next(&htbl->table[bucket], prev, data) == 0) {
                htbl->size--;
                return 0;
            }
            else {
                return -1;
            }
        }
        prev = element;
    }
    return -1;
}
int chtbl_insert(hash_table *hash_table, const void *data) {
    void *temp;
    int bucket, retval;

    temp = (void *)data;
    if (chtbl_lookup(hash_table, &temp) == 0)
        return 1;
    /*****************************************************************************
    * *
    * Hash the key. *
    * *
    *****************************************************************************/
    bucket = (int)(hash_table->hash_function(data) % hash_table->buckets);
    /*****************************************************************************
    * *
    * Insert the data into the bucket. *
    * *
    *****************************************************************************/
    if ((retval = list_ins_next(&hash_table->table[bucket], NULL, data)) == 0)
        hash_table->size++;
    return retval;
}
