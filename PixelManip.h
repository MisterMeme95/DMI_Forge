//
// Created by jonat on 11/8/2023.
//

#ifndef DMI_STRUCT_C_PIXELMANIP_H
#define DMI_STRUCT_C_PIXELMANIP_H

#endif //DMI_STRUCT_C_PIXELMANIP_H
#include "png.h"

typedef struct png_channels {
    int red;
    int green;
    int blue;
    int alpha;
}png_channels;

/** This function initializes a pixel value. Based on the color type, a different amount of space may be need to be allocated.*/
void Initialize_Pixel(png_bytepp pixel, int color_type, int bit_depth);

/** Transform a png_byte into an actual color. */
png_color Byte_To_Color(const png_byte* pixel, int color_type, png_bytep trans_byte);

/** Transform_RGBA_PNG is a function that carries a pixel transformation. Basically, it takes a pixel that is currently
 * in RGBA form, and transform it to the target_color_type specified.*/
png_bytep Transform_RBGA_PNG(const png_byte* pixel, int target_color_type, int src_bit_depth, int target_bit_depth);

/** Transforms a GA_Alpha typed pixel into any other color type. */
png_bytep Transform_GRAYALPHA_PNG(const png_byte* pixel, int target_color_type, int bit_depth);

/** Transforms GrayScaled type pixel into any other type. */
png_bytep Transform_GRAY_PNG(const png_byte* pixel, int target_color_type, int bit_depth);

/** Transforms a traditional RGB pixel into any other type. */
png_bytep Transform_RBG_PNG(const png_byte* pixel, int target_color_type, int bit_depth);

/** Transforms a color from a palette to a png_byte.*/
png_bytep Transform_Indexed_PNG(png_color pixel, int target_color_type, int bit_depth);