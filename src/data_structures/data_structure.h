#ifndef DMI_STRUCT_C_DATA_STRUCTURE_H
#define DMI_STRUCT_C_DATA_STRUCTURE_H
#endif //DMI_STRUCT_C_DATA_STRUCTURE_H

#ifndef DMI_STRUCT_C_LIST_H
#include "list.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif



typedef struct HASH_TABLE{
    unsigned long buckets;
    unsigned long (*hash_function)(const void *key);
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    int (*insert)(struct HASH_TABLE *hash_table, const void *data);
    int size;
    list* table;
}hash_table;





int init_hash_table(hash_table *table, int buckets, unsigned long  (*hash_function)(const void *key),
                    int (*match)(const void *key1, const void *key2), void (*look_up)(), void(*insert)(),
                    void (*destroy)(void *data));

void destroy_hash_table(hash_table *table);
int chtbl_insert(hash_table *hash_table, const void *data);
unsigned long hash_string(const void *str);
int chtbl_remove(hash_table *htbl, void **data);
void * chtbl_lookup(const hash_table *hash_table, void **data);
#define chtbl_size(htbl) ((htbl)->size)
#ifdef __cplusplus
}
#endif