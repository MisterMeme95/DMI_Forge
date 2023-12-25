//
// Created by jonat on 12/8/2023.
//
#include "png.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../dmi.h"
#include "../PixelManip.h"
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
    int width = (int)png_get_image_width(read_ptr, read_info_ptr);
    // Copy the image data from the read structures to the write structures.
    png_set_IHDR(write_ptr, write_info_ptr, png_get_image_width(read_ptr, read_info_ptr),
                 png_get_image_height(read_ptr, read_info_ptr), 8,
                 PNG_COLOR_TYPE_GRAY_ALPHA, png_get_interlace_type(read_ptr, read_info_ptr),
                 png_get_compression_type(read_ptr, read_info_ptr), png_get_filter_type(read_ptr, read_info_ptr));

    png_colorp palette;
    int num_palette;

    if (png_get_PLTE(read_ptr, read_info_ptr, &palette, &num_palette)) {
        png_set_PLTE(write_ptr, write_info_ptr, palette, num_palette);
    }

    png_bytep trans_alpha = NULL;
    int num_trans = 0;
    png_color_16p trans_color = NULL;

    if (png_get_tRNS(read_ptr, read_info_ptr, &trans_alpha, &num_trans, &trans_color)) {
       png_set_tRNS(write_ptr, write_info_ptr, trans_alpha, num_trans, trans_color);
     }


    png_bytepp row_pointers_new = (png_bytepp)malloc(sizeof(png_bytep) * height);

    for (png_uint_32 i = 0; i < height; i++) {
        row_pointers_new[i] = (png_bytep)malloc(png_get_image_width(read_ptr, read_info_ptr) * 3);
    }

    for(png_uint_32 i = 0; i < height; i++) {
        for (png_uint_32 j = 0; j < png_get_rowbytes(write_ptr, write_info_ptr); j++) {
            row_pointers_new[i][j] = 12;
        }
    }

    printf("Died jere #1. . .\n");
    for (int i = 0; i < height; i++) {
        for (int o = 0; o < width; o++) {
            printf("(x, y)) = (%d, %d)\n", o+1, i+1);

            png_bytep isolated_pixel = Get_Pixel(row_pointers, o, i, PNG_COLOR_TYPE_PALETTE,
                                                 2,palette, trans_alpha, &num_trans);


            printf("Red Channel = %d\n", isolated_pixel[0]);
            printf("Green Channel = %d\n", isolated_pixel[1]);
            printf("Blue Channel = %d\n", isolated_pixel[2]);
            printf("Alpha Channel = %d\n", isolated_pixel[3]);
            printf("\n\n");

            /*
            if(png_get_color_type(read_ptr, read_info_ptr) == PNG_COLOR_TYPE_PALETTE){
                int index = row_pointers[i][o];
                png_color palette_color = palette[index];

                png_bytep new_pixel = Transform_Indexed_PNG(palette_color, PNG_COLOR_TYPE_GRAY_ALPHA, 8);

                memcpy(&row_pointers_new[i][o*2], new_pixel, sizeof(png_byte) * 2);
            }
            */



        }
    }

    printf("Died jere #2. . .\n");
    //png_write_info(write_ptr, write_info_ptr);

   // png_write_image(write_ptr, row_pointers_new);

    // Clean up.
    fclose(input_file);
    fclose(output_file);

    png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
    png_destroy_write_struct(&write_ptr, &write_info_ptr);

    // Free the memory associated with row_pointers
    for (png_uint_32 i = 0; i < height; i++) {
        free(row_pointers[i]);
    }
    printf("Done. . .\n");
    free(row_pointers);
}



int main(){

    printf("Starting. . .\n");
    read_and_write_png("testicon.dmi", "_output.png");
    //sleep(1000);
    printf("Done. . .\n");

    return 1;
}