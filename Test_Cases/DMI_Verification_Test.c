#include <stdio.h>
#include <unistd.h>
/*
int main(int argc, char **argv){
    printf("LOl!\n");
    sleep(10);
}
*/

#include <stdio.h>
#include "png.h"
#include "../src/DMI/dmi.h"
#include <stdlib.h>
#include <string.h>
#define STDERR stdout


void read_and_write_png(const char* input_filename, const char* output_filename) {
    FILE* input_file = fopen(input_filename, "rb");
    if(!input_file) {
        printf("Can't open file %s for reading\n", input_filename);
        return;
    }

    // Initialize the reading structures.
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop read_info_ptr = png_create_info_struct(read_ptr);
    png_init_io(read_ptr, input_file);
    png_read_info(read_ptr, read_info_ptr);

    png_uint_32 height = png_get_image_height(read_ptr, read_info_ptr);
    png_bytepp row_pointers = (png_bytepp)malloc(sizeof(png_bytep) * height);

    for (png_uint_32 i = 0; i < height; i++) {
        row_pointers[i] = (png_bytep)malloc(png_get_rowbytes(read_ptr, read_info_ptr));
    }

    png_read_image(read_ptr, row_pointers);

    // Now that we've read the image, let's write it to another file.
    FILE* output_file = fopen(output_filename, "wb");
    if(!output_file) {
        printf("Can't open file %s for writing\n", output_filename);
        return;
    }

    // Initialize the writing structures.
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop write_info_ptr = png_create_info_struct(write_ptr);

    // Set up the output.
    png_init_io(write_ptr, output_file);

    // Copy the image data from the read structures to the write structures.
    png_set_IHDR(write_ptr, write_info_ptr, png_get_image_width(read_ptr, read_info_ptr),
                 png_get_image_height(read_ptr, read_info_ptr), png_get_bit_depth(read_ptr, read_info_ptr),
                 png_get_color_type(read_ptr, read_info_ptr), png_get_interlace_type(read_ptr, read_info_ptr),
                 png_get_compression_type(read_ptr, read_info_ptr), png_get_filter_type(read_ptr, read_info_ptr));

    png_colorp palette;
    int num_palette;

    if (png_get_PLTE(read_ptr, read_info_ptr, &palette, &num_palette)) {
        png_set_PLTE(write_ptr, write_info_ptr, palette, num_palette);
    }

    png_bytep trans_alpha = NULL;
    int num_trans = 0;
    png_color_16p trans_color = NULL;

    //if (png_get_tRNS(read_ptr, read_info_ptr, &trans_alpha, &num_trans, &trans_color)) {
    //   png_set_tRNS(write_ptr, write_info_ptr, trans_alpha, num_trans, trans_color);
    // }

    printf("Reading image now. . \n");
    sleep(5);
    output_pixel_values(read_ptr, read_info_ptr, row_pointers);
    printf("Done!!ee");

    // Set up the data in the write structure.
    png_set_rows(write_ptr, write_info_ptr, row_pointers);

    // Finally, write the image to the file.
    png_write_png(write_ptr, write_info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    // Clean up.
    fclose(input_file);
    fclose(output_file);

    png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
    png_destroy_write_struct(&write_ptr, &write_info_ptr);

    // Free the memory associated with row_pointers
    for (png_uint_32 i = 0; i < height; i++) {
        free(row_pointers[i]);
    }
    free(row_pointers);
}

int main(int argc, char **argv){
    read_and_write_png("Tremor.dmi", "_output.png");
}

