#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include "image_manipulation/PixelManip.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

int main() {
    // Load the image
    char *file_name = "goku.dmi";
    Image image = load_image(file_name);

    // Print relevant values
    printf("Width: %u\n", image.width);
    printf("Height: %u\n", image.height);
    printf("Bit Depth: %d\n", image.bit_depth);
    printf("Color Type: %d\n", image.color_type);
    printf("Interlace Method: %d\n", image.interlace_method);
    printf("Number of Palette Colors: %d\n", image.palette_num);
    printf("Number of Transparency Values: %d\n", image.trans_num);

    // Create a new image for writing
    Image new_image;

    // Manually copy values from image to new_image
    new_image.width = image.width;
    new_image.height = image.height;
    new_image.bit_depth = image.bit_depth;
    new_image.color_type = image.color_type;
    new_image.interlace_method = image.interlace_method;
    new_image.palette_num = image.palette_num;
    new_image.trans_num = image.trans_num;

    new_image.png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    new_image.info_ptr = png_create_info_struct(new_image.png_ptr);
    new_image.file_pointer = fopen("output.png", "wb");
    png_init_io(new_image.png_ptr, new_image.file_pointer);

    // Copy palette and transparecy data
    if (image.palette) {
        new_image.palette = (png_colorp)malloc(image.palette_num * sizeof(png_color));
        memcpy(new_image.palette, image.palette, image.palette_num * sizeof(png_color));
    }

    if (image.trans_alpha) {
        new_image.trans_alpha = (png_bytep)malloc(image.trans_num * sizeof(png_byte));
        memcpy(new_image.trans_alpha, image.trans_alpha, image.trans_num * sizeof(png_byte));
    }

    if (image.trans_color) {
        new_image.trans_color = (png_color_16p)malloc(sizeof(png_color_16));
        memcpy(new_image.trans_color, image.trans_color, sizeof(png_color_16));
    }
    png_uint_32 row_bytes = png_get_rowbytes(image.png_ptr, image.info_ptr);
    new_image.pixel_array = (png_bytepp)malloc(image.height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < image.height; y++) {
        new_image.pixel_array[y] = (png_bytep)malloc(row_bytes);
    }
    for(int i = 0; i < image.height; i++){
        for(int j = 0; j < image.width; j++){
            new_image.pixel_array[i][j] = image.pixel_array[i][j];
        }
    }
    // Write the PNG header info
    png_set_IHDR(
            new_image.png_ptr,
            new_image.info_ptr,
            new_image.width,
            new_image.height,
            new_image.bit_depth,
            new_image.color_type,
            new_image.interlace_method,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
    );
    // Print relevant values for new_image
    printf("New Image:\n");
    printf("Width: %u\n", new_image.width);
    printf("Height: %u\n", new_image.height);
    printf("Bit Depth: %d\n", new_image.bit_depth);
    printf("Color Type: %d\n", new_image.color_type);
    printf("Interlace Method: %d\n", new_image.interlace_method);
    printf("Number of Palette Colors: %d\n", new_image.palette_num);
    printf("Number of Transparency Values: %d\n", new_image.trans_num);
// Write the info to the file
    png_set_PLTE(new_image.png_ptr, new_image.info_ptr, image.palette, image.palette_num);
    png_write_info(new_image.png_ptr, new_image.info_ptr);

// Write the image data
    png_write_image(new_image.png_ptr, new_image.pixel_array);
    return EXIT_SUCCESS;
}
