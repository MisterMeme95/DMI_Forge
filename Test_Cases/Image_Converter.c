//
// Created by jonat on 12/8/2023.
//
#include "png.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../dmi.h"
#include "../data_structure.h"

void read_and_write_png(const char* input_filename, const char* output_filename) {
    FILE* input_file = fopen(input_filename, "rb");
    if(!input_file) {
        printf("Can't open file %s for reading\n", input_filename);
        return;
    }

    png_structp read_ptr;
    png_infop read_info_ptr;
    png_bytepp row_pointers;

    png_uint_32 height = png_get_image_height(read_ptr, read_info_ptr);
    png_uint_32 width1 = png_get_image_width(read_ptr, read_info_ptr);
    Read_PNG(&read_ptr, &read_info_ptr, input_file, &row_pointers, &height, &width1);


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

    png_set_IHDR(write_ptr, write_info_ptr, png_get_image_width(read_ptr, read_info_ptr),
                 png_get_image_height(read_ptr, read_info_ptr), 8,
                 PNG_COLOR_TYPE_PALETTE, png_get_interlace_type(read_ptr, read_info_ptr),
                 png_get_compression_type(read_ptr, read_info_ptr), png_get_filter_type(read_ptr, read_info_ptr));

    png_colorp palette;
    png_colorp new_palette = (png_colorp)malloc(sizeof(png_color) * 256);

    int num_palette = 0;
    int new_num_palette = 0;
    palette_hash new_pal;
    for (int i = 0; i < 256; i++) {
        new_pal.hash_bucket[i] = NULL;
    }
    png_get_PLTE(read_ptr, read_info_ptr, &palette, &num_palette);
    png_set_PLTE(write_ptr, write_info_ptr, palette, num_palette);
//    if (png_get_PLTE(read_ptr, read_info_ptr, &palette, &num_palette)) {
 //       png_set_PLTE(write_ptr, write_info_ptr, palette, num_palette);
  //x  }

    png_bytep trans_alpha = NULL;
    int num_trans = 0;
    png_color_16p trans_color = NULL;
    png_get_tRNS(read_ptr, read_info_ptr, &trans_alpha, &num_trans, &trans_color);
 //   png_set_tRNS(write_ptr, write_info_ptr, trans_alpha, num_trans, trans_color);
 //   if (png_get_tRNS(read_ptr, read_info_ptr, &trans_alpha, &num_trans, &trans_color)) {
   //    png_set_tRNS(write_ptr, write_info_ptr, trans_alpha, num_trans, trans_color);
    //}



    png_bytepp row_pointers_new = (png_bytepp)malloc(sizeof(png_bytep) * height);
    Initialize_Pixels(&row_pointers_new, height, png_get_rowbytes(read_ptr, read_info_ptr));


    for (size_t i = 0; i < height; i++) {
        for (size_t o = 0; o < width; o++) {
            Pixel_Data isolated_pixel = Get_Pixel(row_pointers, o, i, PNG_COLOR_TYPE_PALETTE,
                                                  8, palette, trans_alpha, &num_trans);

            Pixel_Data transformed_pixel = Pixel_Transformation(isolated_pixel,
                                                                PNG_COLOR_TYPE_PALETTE, 8);

            Set_Pixel(row_pointers_new, &transformed_pixel, o, i, PNG_COLOR_TYPE_PALETTE,
                      8, new_palette, NULL, &new_num_palette, &new_pal);
        }
    }


    printf("new_num_pal = %d\n", new_num_palette);
    png_set_PLTE(write_ptr, write_info_ptr, new_palette, new_num_palette);
   // printf("Died jere #2. . .\n");
    png_write_info(write_ptr, write_info_ptr);

    png_write_image(write_ptr, row_pointers_new);

    // Clean up.
    fclose(input_file);
    fclose(output_file);

    png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
    png_destroy_write_struct(&write_ptr, &write_info_ptr);

    // Free the memory associated with row_pointers
    for (png_uint_32 i = 0; i < height; i++) {
        free(row_pointers[i]);
        free(row_pointers_new[i]);
    }
   // printf("Done. . .\n");
    free(row_pointers);
    free(row_pointers_new);
}



int main(){

    printf("Starting. . .\n");
    read_and_write_png("goku.dmi", "_output.png");
    //sleep(1000);
  //  printf("Done. . .\n");

    return EXIT_SUCCESS;
}