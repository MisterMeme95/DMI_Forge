#include "data_structure.h"
#include <stdio.h>
#include <stdlib.h>

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
                    int (*match)(const void *key1, const void *key2), void (*look_up)(), void(*insert)(),
                    void (*destroy)(void *data)){

    int i;
    if((table->table = (void*) malloc(buckets * sizeof(list))) == NULL){
        return -1;
    }

    table->buckets = buckets;
    for(i = 0; i < buckets; i++){
        initialize_list(&table->table[i], NULL, NULL, NULL);
    }

    table->hash_function = hash_function;
    table->match = match;
    table->destroy = destroy;
    table->size = 0;
    return 0;
}



void *chtbl_lookup(const hash_table *hash_table, void **data) {
    node *element;
    int bucket;
    bucket = (int)(hash_table->hash_function(*data) % hash_table->buckets); /*< Find the buckets. */
    for (element = list_head(&hash_table->table[bucket]); element != NULL; element = list_next(element)) {
        if (hash_table->match(*data, list_data(element))) {
            return list_data(element);
        }
    }
    return NULL;
}

void* get_hash(){

}

int chtbl_remove(hash_table *htbl, void **data) {

    node *element,*prev;
    int bucket;

    bucket = (int)(htbl->hash_function(*data) % htbl->buckets);
    prev = NULL;
    for (element = list_head(&htbl->table[bucket]); element != NULL; element =
                                                                             list_next(element)) {
        if (htbl->match(*data, list_data(element))) {

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
    if (chtbl_lookup(hash_table, &temp) != NULL)
        return 1;
    bucket = (int)(hash_table->hash_function(data) % hash_table->buckets);
    if ((retval = list_ins_next(&hash_table->table[bucket], NULL, data)) == 0)
        hash_table->size++;
    return retval;
}
