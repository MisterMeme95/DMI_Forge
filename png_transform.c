#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "PixelManip.h"
#include <png.h>

void print_usage() {
    printf("Usage: png_transform [OPTIONS]\n");
    printf("Options:\n");
    printf("  -i, --input <file>      Set input file name\n");
    printf("  -o, --output <file>     Set output file name\n");
    printf("  -b, --bit_depth <depth> Set bit depth (e.g., 8, 16, 32)\n");
    printf("  -c, --color_type <type> Set color type (e.g., 0=grayscale, 2=RGB, 3=indexed, etc.)\n");
    printf("  -h, --help              Show this help message\n");
}

int main(int argc, char **argv) {

    FILE *source_file;
    png_bytepp row_pointers;
    char *input_file = NULL;
    int bit_depth = 0, color_type = 0, interlace_method = 0, compression = 0, filter = 0;
    png_structp read_ptr;
    png_infop read_info_ptr;
    png_bytep trans_alpha = NULL;
    int trans_num = 0;
    png_color_16p trans_color = NULL;
    png_colorp source_palette;
    int src_palette_length = 0;
    png_uint_32 width = 0, height = 0;


    /* These are all the variables needed for my output.*/
    FILE *destination_file = NULL;
    png_bytepp image_data = NULL;
    char *output_file = NULL;
    int target_bit_depth = - 1, target_color_type = -1;
    char *color_input = NULL;
    png_structp write_ptr = NULL;
    png_infop write_info_ptr = NULL;
    png_colorp destination_palette = (png_colorp)malloc(sizeof(png_color) * 16);
    int dest_palette_length = 0;
    palette_hash new_pal;



    int pixels_per_byte;


    int overwrite_flag = 0;
    int opt;
    static struct option long_options[] = {
            {"input", required_argument, 0, 'i'},
            {"output", required_argument, 0, 'o'},
            {"bit_depth", required_argument, 0, 'b'},
            {"color_type", required_argument, 0, 'c'},
            {"help", no_argument, 0, 'h'},
            {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "i:o:b:c:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 'i':
                input_file = strdup(optarg);
                break;

            case 'o':
                output_file = strdup(optarg);
                break;

            case 'b':
                target_bit_depth = atoi(optarg);
                if(target_bit_depth != 1 && target_bit_depth != 2 && target_bit_depth != 4 &&
                   target_bit_depth != 8 && target_bit_depth != 16){
                    printf("Error: The specified bit depth '%s' is invalid. "
                           "Please choose one of the following valid values: 1, 2, 4, 8, or 16.\n", optarg);
                    return EXIT_FAILURE;
                }
                break;

            case 'c':
                color_input = strdup(optarg);
                if (color_input != NULL) {
                    if (strcmp(color_input, "gray") == 0) {
                        target_color_type = PNG_COLOR_TYPE_GRAY;
                        printf("Grayscale color type selected.\n");
                    }
                    else if (strcmp(color_input, "rgb") == 0) {
                        target_color_type = PNG_COLOR_TYPE_RGB;
                        printf("RGB color type selected.\n");
                    }
                    else if (strcmp(color_input, "indexed") == 0 || strcmp(color_input, "palette") == 0) {
                        target_color_type = PNG_COLOR_TYPE_PALETTE;
                        printf("Indexed (palette) color type selected.\n");
                    }
                    else if (strcmp(color_input, "gray_alpha") == 0) {
                        target_color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
                        printf("Grayscale with Alpha color type selected.\n");
                    }
                    else if (strcmp(color_input, "rgba") == 0) {
                        target_color_type = PNG_COLOR_TYPE_RGBA;
                        printf("RGBA color type selected.\n");
                    }
                    else {
                        printf("Invalid color type input. Please specify one of: gray, rgb, indexed, gray_alpha, rgba.\n");
                    }
                }
                break;

            case 'h':
                print_usage();
                break;

            default: // '?' is used for unrecognized options

                fprintf(stderr, "Invalid option\n");
                print_usage();
                return EXIT_FAILURE;
        }
    }


    source_file = fopen(input_file, "rb");
    if(!source_file) {
        printf("Can't open file %s for reading\n", input_file);
        return EXIT_FAILURE;
    }

    Read_PNG(&read_ptr, &read_info_ptr, source_file, &row_pointers, &height, &width);
    png_get_IHDR(read_ptr, read_info_ptr, &width, &height, &bit_depth,
                 &color_type, &interlace_method, &compression, &filter);

    if(output_file == NULL) {
        printf("Output file is invalid. Renaming to _output.png\n");
        output_file = strdup("_output.png");

    }

    destination_file = fopen(output_file, "wb");
    if(!destination_file) {
        printf("Can't open file %s for writing\n", output_file);
        return EXIT_FAILURE;
    }

    Initialize_PNG_Writer(&write_ptr,&write_info_ptr, destination_file);
    Initialize_Pixels(&image_data, height, png_get_rowbytes(read_ptr, read_info_ptr));

    for (int i = 0; i < 256; i++) {
        new_pal.hash_bucket[i] = NULL;
    }
    if(color_type == PNG_COLOR_TYPE_PALETTE){
        png_get_PLTE(read_ptr, read_info_ptr, &source_palette, &src_palette_length);
        png_get_tRNS(read_ptr, read_info_ptr, &trans_alpha, &trans_num, &trans_color);
    }
    png_set_IHDR(write_ptr, write_info_ptr, width, height, target_bit_depth,
                 target_color_type, png_get_interlace_type(read_ptr, read_info_ptr),
                 png_get_compression_type(read_ptr, read_info_ptr),
                 png_get_filter_type(read_ptr, read_info_ptr));




    for (size_t i = 0; i < height; i++) {
        for (size_t o = 0; o < width; o++) {
            Pixel_Data isolated_pixel = Get_Pixel(row_pointers, o, i, color_type,
                                                  bit_depth, source_palette, trans_alpha, &trans_num);

//            printf("isolated = %d\n", isolated_pixel.bit_depth);
            Pixel_Data transformed_pixel = Pixel_Transformation(isolated_pixel, target_color_type, target_bit_depth);

            Set_Pixel(image_data, &transformed_pixel, o, i, target_color_type,
                      target_bit_depth, destination_palette, NULL, &dest_palette_length,
                      &new_pal);
        }
    }
//    if(input_file != NULL)
//        printf("Input File: %s\n", input_file);
//    if(output_file != NULL)
//        printf("Output File: %s\n", output_file);
//    if(target_bit_depth != -1)
//        printf("Bit Depth: %d\n", target_bit_depth);

    if(target_color_type == PNG_COLOR_TYPE_PALETTE){
        png_set_PLTE(write_ptr, write_info_ptr, destination_palette, dest_palette_length);

    }
    // printf("Died jere #2. . .\n");
    png_write_info(write_ptr, write_info_ptr);

    png_write_image(write_ptr, image_data);

    // Clean up.
    fclose(source_file);
    fclose(destination_file);

    png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
    png_destroy_write_struct(&write_ptr, &write_info_ptr);

    // Free allocated memory
    free(input_file);
    free(output_file);

    return EXIT_SUCCESS;

}
