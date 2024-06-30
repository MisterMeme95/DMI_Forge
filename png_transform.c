#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "PixelManip.h"
#include <png.h>
#include <math.h>
#include <unistd.h>

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
    /** These are flags for arguments that must be submitted in order for the program to run properly. **/
    int write_flag = 0, depth_flag = 0, color_flag = 0, required_flags = 3;

    /* These store very important values. */
    char *input_file = NULL;
    char *output_file = NULL;
    char *color_input = NULL;

    char *endptr;
    errno = 0;
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

    int target_color_type, target_bit_depth;
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

    if(output_file == NULL) {
        printf("Output file is invalid. Renaming to _output.png\n");
        output_file = strdup("_output.png");
    }

    Image original_image = load_image(input_file);
    Image transformed_image;

    transformed_image.color_type = target_color_type;
    transformed_image.bit_depth = target_bit_depth;
    transformed_image.height = original_image.height;
    transformed_image.width = original_image.width;
    int palette_maximum_size = (int)pow(2.0, (double)transformed_image.bit_depth);
    //transformed_image.palette = (png_colorp)malloc(sizeof(png_color) * palette_maximum_size);
    //transformed_image.palette = (png_colorp)malloc(sizeof(png_color) * palette_maximum_size);
    transformed_image.palette_num = 0;
//    if(new_image->color_type == PNG_COLOR_TYPE_PALETTE){
//        int palette_maximum_size = (int)pow(2.0, (double)new_image->bit_depth);
//        printf("Max = %d\n", palette_maximum_size);
//        new_image->palette = (png_colorp)malloc(sizeof(png_color) * palette_maximum_size);
//    }

    initialize_image2(output_file, &transformed_image);

    for (int i = 0; i < 256; i++) {
        new_pal.hash_bucket[i] = NULL;
    }


    for (size_t i = 0; i < original_image.height; i++) {
        for (size_t o = 0; o < original_image.width; o++) {
            Pixel_Data isolated_pixel = Get_Pixel(original_image.pixel_array, o, i,
                                                  original_image.color_type,original_image.bit_depth,
                                                  original_image.palette, original_image.trans_alpha,&original_image.trans_num);

            Set_Channels(&isolated_pixel);
            Pixel_Data transformed_pixel = Pixel_Transformation(isolated_pixel, target_color_type, target_bit_depth);
            //Set_Channels(&transformed_pixel);
            if(original_image.color_type == PNG_COLOR_TYPE_PALETTE && target_color_type == PNG_COLOR_TYPE_RGB){
                if(isolated_pixel.alpha_channel == 0){
                    if(transformed_image.trans_color == NULL){
                        transformed_image.trans_color = (png_color_16p) malloc(sizeof(png_color_16));
                        transformed_image.trans_color->red = Get_Red_Channel(isolated_pixel);
                        transformed_image.trans_color->blue = Get_Blue_Channel(isolated_pixel);
                        transformed_image.trans_color->green = Get_Green_Channel(isolated_pixel);
                    }
                }
            }
            else {
                if(transformed_image.trans_color == NULL){
                    transformed_image.trans_color = (png_color_16p) malloc(sizeof(png_color_16));
                    transformed_image.trans_color->gray = Get_Red_Channel(transformed_pixel);
                }
            }
            Set_Pixel(transformed_image.pixel_array, &transformed_pixel,
                      o, i, transformed_image.color_type,transformed_image.bit_depth,
                      transformed_image.palette, transformed_image.trans_alpha,&transformed_image.palette_num,
                      &new_pal);
        }
    }

    if(target_color_type == PNG_COLOR_TYPE_PALETTE){
        png_set_PLTE(transformed_image.png_ptr, transformed_image.info_ptr,
                     original_image.palette, original_image.palette_num);
        png_set_tRNS(transformed_image.png_ptr, transformed_image.info_ptr,
                     original_image.trans_alpha, original_image.trans_num, NULL);
    }

    if(target_color_type == PNG_COLOR_TYPE_RGB) {// || target_color_type == PNG_COLOR_TYPE_GRAY){
        png_set_tRNS(transformed_image.png_ptr, transformed_image.info_ptr, NULL, 1, transformed_image.trans_color);
    }
    for(int i = 0; i < transformed_image.height; i++){
        for(int j = 0; j < transformed_image.width; j++){
            //printf("%d ", transformed_image.pixel_array[i][j]);
            if(transformed_image.pixel_array[i][j] != original_image.pixel_array[i][j]){
//                printf("(%d, %d) - Origin [%d] | transformed [%d] \n", i, j, transformed_image.pixel_array[i][j],
//                       original_image.pixel_array[i][j]);
            }
        }
        //printf("\n");
        //sleep(10);
    }
    png_write_info(transformed_image.png_ptr, transformed_image.info_ptr);
    png_write_image(transformed_image.png_ptr, transformed_image.pixel_array);
    return EXIT_SUCCESS;
}
