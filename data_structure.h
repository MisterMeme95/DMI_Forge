#ifndef DMI_STRUCT_C_DATA_STRUCTURE_H
#define DMI_STRUCT_C_DATA_STRUCTURE_H

#endif //DMI_STRUCT_C_DATA_STRUCTURE_H
#include "png.h"
#include "PixelManip.h"



unsigned int hash_pixel(Pixel_Data pixel);

int insert_key(Pixel_Data pixel, palette_hash *paletteHash, png_byte new_index);
int find_key(Pixel_Data pixel, palette_hash* paletteHash);
int match_node(Pixel_Data pixel, Pixel_Data other_pixel);