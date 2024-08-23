#ifndef DMI_STRUCT_C_DATA_STRUCTURE_H
#define DMI_STRUCT_C_DATA_STRUCTURE_H

#endif //DMI_STRUCT_C_DATA_STRUCTURE_H
#include "png.h"

#include "dmi.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct dmi_node {
    int index;
    DMI icon;
    struct dmi_node *next;
    struct dmi_node *prev;
} dmi_node;


typedef struct DMI_HASH{
    int MAX_VALUE;
    dmi_node *hash_bucket[256];
}dmi_hash;

typedef struct dmilist {
    DMI **array;
    int max_capacity;
    int currently_filled;
    dmi_hash hash_table;
} dmi_list;

void initialize_dmi_vector(dmi_list *look_up);

unsigned int hash_pixel(Pixel_Data pixel);

int insert_key(Pixel_Data pixel, palette_hash *paletteHash, png_byte new_index);
int find_key(Pixel_Data pixel, palette_hash* paletteHash);
int match_node(Pixel_Data pixel, Pixel_Data other_pixel);
DMI* find_dmi(const char *name, dmi_hash *state_lookup, DMI* find_state);
int insert_dmi(const char *name,  dmi_hash *state_lookup);
#ifdef __cplusplus
}
#endif