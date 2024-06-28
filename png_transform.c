#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "PixelManip.h"
#include <png.h>



int authenticate_transform(int color_flag, int bit_flag, int write_flag, char** debug_report){
    int is_valid = 1;
    int index = 0;

    if(!color_flag){
        debug_report[index] = (char*) malloc(sizeof(char) * 11);
        strcpy(debug_report[index], "color_type");
        debug_report[index][10] = '\0';
        is_valid = 0;
        index++;
    }

    if(!bit_flag){
        debug_report[index] = (char*) malloc(sizeof(char) * 10);
        strcpy(debug_report[index], "bit_depth");
        debug_report[index][9] = '\0';
        is_valid = 0;
        index++;
    }

    if(!write_flag){
        debug_report[index] = (char*) malloc(sizeof(char) * 11);
        strcpy(debug_report[index], "input file");
        debug_report[index][10] = '\0';
        is_valid = 0;
    }
    return is_valid;
}
void print_usage() {
    printf("Usage: png_transform [INPUT] [COLOR_DEPTH] +OPTIONS]\n"
           "Options:\n"
           "  -i, --input <file>      Set input file name\n"
           "  -o, --output <file>     Set output file name\n"
           "  -b, --bit_depth <depth> Set bit depth (e.g., 8, 16, 32)\n"
           "-c, --color_type <type> Set color type (e.g., 0=grayscale, 2=RGB, 3=indexed, etc.)\n"
           "  -h, --help              Show this help message\n"
           "--exclude-trns            For RGB & Gray images, you can choose to have the TRNS chunk ignored.");
}

int main(int argc, char **argv) {


//    Image practice_image = Create_Image("goku.dmi");
//    printf("Bit_Depth = %d"
//           "\nColor_Type = %d\nHeight = %d\nWidth = %d\n", practice_image.bit_depth, practice_image.color_type, practice_image.height,
//           practice_image.width);
    /** These are flags for arguments that must be submitted in order for the program to run properly. **/
    int write_flag = 0, depth_flag = 0, color_flag = 0, required_flags = 3;

    FILE *source_file;
    char *input_file = NULL;

    PNG_INFO pngInfo;
    FILE *destination_file = NULL;
    char *output_file = NULL;

    int target_bit_depth = -1, target_color_type = -1;
    char *color_input = NULL;


    png_bytepp row_pointers;
    int bit_depth = 0, color_type = 0, interlace_method = 0, compression = 0, filter = 0;

    png_structp read_ptr; /*!< Detailed description after the member. */

    png_infop read_info_ptr;

    pngInfo.trans_alpha = NULL;
    png_bytep trans_alpha = NULL;



    png_bytep dest_trans_alpha = (png_bytep) malloc(sizeof(png_bytep) * 256);

    pngInfo.trans_num = 0;
    int trans_num = 0;

    pngInfo.trans_color = NULL;
    png_color_16p trans_color = NULL;
    png_color_16p dest_trans_color = NULL; //= (png_color_16p) malloc(sizeof(png_color_16));

    png_colorp source_palette;

    pngInfo.src_palette_length = 0;
    int src_palette_length = 0;


    png_uint_32 width = 0, height = 0;
    char *endptr;
    errno = 0;

    /* These are all the variables needed for my output.*/
    png_bytepp image_data = NULL;
    png_structp write_ptr = NULL;
    png_infop write_info_ptr = NULL;

    pngInfo.source_palette = (png_colorp)malloc(sizeof(png_color) * 256);
    png_colorp destination_palette = (png_colorp)malloc(sizeof(png_color) * 256);

    pngInfo.src_palette_length = 0;
    int dest_palette_length = 0;


    palette_hash new_pal;

    int overwrite_flag = 0;

    int opt;
    static struct option long_options[] = {
            {"input", required_argument, 0, 'i'},
            {"output", required_argument, 0, 'o'},
            {"bit_depth", required_argument, 0, 'b'},
            {"color_type", required_argument, 0, 'c'},
            {"force", no_argument, 0, 'f'},
            {"help", no_argument, 0, 'h'},
            {"exclude-trns", no_argument, 0, 200},
            {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "i:o:b:c:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 'i':
                input_file = strdup(optarg);
                write_flag = 1;
                required_flags--;
                break;

            case 'o':
                output_file = strdup(optarg);
                break;

            case 'b':
                target_bit_depth = strtol(optarg, &endptr, 10);
                if(target_bit_depth != 1 && target_bit_depth != 2 && target_bit_depth != 4 &&
                   target_bit_depth != 8 && target_bit_depth != 16) {
                    printf("Error: The specified bit depth '%s' is invalid. "
                           "Please choose one of the following valid values: 1, 2, 4, 8, or 16.\n", optarg);
                    return EXIT_FAILURE;
                }
                depth_flag = 1;
                required_flags--;
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
                color_flag = 1;
                required_flags--;
                break;

            case 'h':
                print_usage();
                break;

            case 'f':
                overwrite_flag = 1;
                break;

            default:
                fprintf(stderr, "Invalid option\n");
                print_usage();
                return EXIT_FAILURE;
        }
    }

    char **debug_text = (char**)malloc(sizeof(char*) * required_flags);
    int validation = authenticate_transform(color_flag, depth_flag, write_flag, debug_text);
    if(!validation) {
        fprintf(stderr,"Missing the following arguments: ");
        for(int i = 0; i < required_flags; i++) {
            fprintf(stderr,"%s, ", debug_text[i]);
        }
        fprintf(stderr,"\n\nFor more information, please use: png_transform --help");
        return EXIT_FAILURE;
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
    png_set_IHDR(write_ptr, write_info_ptr, width, height, target_bit_depth,
                 target_color_type, png_get_interlace_type(read_ptr, read_info_ptr),
                 png_get_compression_type(read_ptr, read_info_ptr),
                 png_get_filter_type(read_ptr, read_info_ptr));

    Initialize_Pixels(&image_data, height, png_get_rowbytes(write_ptr, write_info_ptr));

    for (int i = 0; i < 256; i++) {
        new_pal.hash_bucket[i] = NULL;
        pngInfo.palette_look_up.hash_bucket[i] = NULL;
    }
    if(color_type == PNG_COLOR_TYPE_PALETTE){
        png_get_PLTE(read_ptr, read_info_ptr, &source_palette, &src_palette_length);
        png_get_tRNS(read_ptr, read_info_ptr, &trans_alpha, &trans_num, &trans_color);
    }

    for (size_t i = 0; i < height; i++) {
        for (size_t o = 0; o < width; o++) {
            Pixel_Data isolated_pixel = Get_Pixel(row_pointers, o, i, color_type,
                                                  bit_depth, source_palette, trans_alpha, &trans_num);

            Set_Channels(&isolated_pixel);

            Pixel_Data transformed_pixel = Pixel_Transformation(isolated_pixel, target_color_type, target_bit_depth);



            if(color_type == PNG_COLOR_TYPE_PALETTE && target_color_type == PNG_COLOR_TYPE_RGB){
                if(isolated_pixel.alpha_channel == 0){
                    if(dest_trans_color == NULL){
                        dest_trans_color = (png_color_16p) malloc(sizeof(png_color_16));
                        dest_trans_color->red = Get_Red_Channel(isolated_pixel);
                        dest_trans_color->blue = Get_Blue_Channel(isolated_pixel);
                        dest_trans_color->green = Get_Green_Channel(isolated_pixel);
                    }
                }


            }
            else {
                if(dest_trans_color == NULL){
                    dest_trans_color = (png_color_16p) malloc(sizeof(png_color_16));
                    printf("Transformed Red = %d\n", Get_Red_Channel(transformed_pixel));
                    printf("Transformed Red #2 = %d\n", transformed_pixel.red);
                    dest_trans_color->gray = Get_Red_Channel(transformed_pixel);
                }
            }
            Set_Pixel(image_data, &transformed_pixel, o, i, target_color_type,
                      target_bit_depth, destination_palette, trans_alpha, &dest_palette_length,
                      &new_pal);
        }
    }

    if(target_color_type == PNG_COLOR_TYPE_PALETTE){
        png_set_PLTE(write_ptr, write_info_ptr, destination_palette, dest_palette_length);
        png_set_tRNS(write_ptr, write_info_ptr, trans_alpha, trans_num, NULL);
    }

    /* NOTE THIS CODE IS INCOMPLETE.
     *
     * TO DO: Add flag to determine if a trans_color variable ought to be ignored.
     *          By default, it should be included.
     *
     *          In addition, if the default is taken and provided for a gray image.
     *          Be sure to send out a WARNING to the command prompt that lets the user know that
     *          including the trans_color chunk at a low enough level and can some inconsistencies
     *          and make parts of the sprites that should be shown become invisible
     */
    if(target_color_type == PNG_COLOR_TYPE_RGB) {// || target_color_type == PNG_COLOR_TYPE_GRAY){
        png_set_tRNS(write_ptr, write_info_ptr, NULL, 0, dest_trans_color);
    }


    png_write_info(write_ptr, write_info_ptr);
    png_write_image(write_ptr, image_data);
    return EXIT_SUCCESS;
}
