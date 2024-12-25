//
// Created by jonat on 10/17/2023.
//

#define STDERR stdout

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "src/DMI/dmi.h"

void get_frame_coordinates(int frame_num, int *copy_row, int *copy_col, int DMI_HEIGHT, int DMI_WIDTH) {
    // With 14 frames per row, the row of the frame is calculated by dividing the frame number by 14
    *copy_row = (frame_num / 14) * DMI_HEIGHT;
    // The column is calculated based on the remainder
    *copy_col = (frame_num % 14) * DMI_WIDTH;
}


void changeExtension(char* filename, const char* newExt) {
    char* extPos = strstr(filename, "png");
    if (extPos) {
        strcpy(extPos, newExt);  // Replace with new extension.
    }
}

int main(int argc, char **argv){

    char *input_name = NULL; //= malloc(sizeof(char) * strlen(argv[optind]) + 1);
    char *output_name = NULL; //= malloc(sizeof(char) * strlen(argv[optind]) + 1);

    //printf("Printf test" "is fucking cool"
    //     "sdedsfdf ""LOLOLOL""Ilike eggs, nigga!\n\n\n");
    if (optind < argc)
    {
        printf ("non-option ARGV-elements: ");
        int start_value = optind;
        while(optind < argc){
            if(optind - start_value == 0){
                input_name = malloc(sizeof(char) * strlen(argv[optind]) + 1);
                strcpy(input_name, argv[optind++]);
            }

            else if(optind - start_value == 1){
                output_name = malloc(sizeof(char) * strlen(argv[optind]) + 1);
                strcpy(output_name, argv[optind++]);
            }

            else {
                printf ("%s ", argv[optind++])     ;
            }
        }
    }

    if(input_name == NULL){
        printf("The command MUST have an input file specified!\nProgram ends in failure. . .\n");
        return 0;
    }

    if(output_name == NULL){
        output_name = malloc(sizeof(char) * strlen(input_name) + 1);
        sprintf(output_name, "%s", input_name);
        changeExtension(output_name, "dmi");
    }

    printf("Input_Name = %s\nOut_Name=%s\n\n", input_name, output_name);
    png_structp read_png_ptr;
    png_infop read_info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, pixels_per_byte;

    FILE *input_fp;
    if(!check_if_png(input_name,&input_fp)){
        printf("The file you are attempting to read is not a valid PNG file!\n");
        return 0;
    }

    if(initialize_image(&read_png_ptr, &read_info_ptr, &input_fp)) {
        printf("Initializing the image failed!\n");
        return 0;
    }

    png_get_IHDR(read_png_ptr, read_info_ptr, &width, &height, &bit_depth,
                 &color_type, &interlace_method, NULL, NULL);

    png_uint_32 num_rows = height;

    switch (bit_depth) {
        case 1:
            pixels_per_byte = 8;
            break;
        case 2:
            pixels_per_byte = 4;
            break;
        case 4:
            pixels_per_byte = 2;
            break;
        case 8:
            pixels_per_byte = 1;
            break;
        default:
            pixels_per_byte = 1;
            break;
    }


    int DMI_HEIGHT = 62, DMI_WIDTH = 62 / pixels_per_byte;


    if(color_type == PNG_COLOR_TYPE_RGB_ALPHA || color_type == PNG_COLOR_TYPE_GRAY_ALPHA){
        DMI_WIDTH *= 4;
    }

    if(color_type == PNG_COLOR_TYPE_RGB){
        DMI_WIDTH *= 3;
    }

    png_bytepp row_pointers = (png_bytepp)malloc(sizeof(png_bytep) * num_rows);
    //printf("Row Bytes = %d\nHeight = %d\n", png_get_rowbytes(read_png_ptr, read_info_ptr), height);
    for (png_uint_32 i = 0; i < height; i++) {
        row_pointers[i] = (png_bytep)malloc(png_get_rowbytes(read_png_ptr, read_info_ptr));
    }

    png_read_image(read_png_ptr, row_pointers);
    png_textp text_ptr;
    int num_text;
    if (png_get_text(read_png_ptr, read_info_ptr, &text_ptr, &num_text) > 0) {
        png_uint_32 i;
        fprintf(STDERR,"\n");
        for (i=0; i<num_text; i++){
            fprintf(STDERR,"Text compression[%d]=%d\n",
                    i, text_ptr[i].compression);
        }
    }

 //   int row_bytes = (int)png_get_rowbytes(png_ptr, info_ptr);


    png_bytepp row_pointers_new = (png_bytepp)malloc(sizeof(png_bytep) * num_rows);
    for (png_uint_32 i = 0; i < height; i++) {
        row_pointers_new[i] = (png_bytep)malloc(png_get_rowbytes(read_png_ptr, read_info_ptr));
    }


    FILE *output_fp = fopen(output_name, "wb");


    if (!output_fp) {
        printf("Error opening output file\n");
        return 1;
    }

    png_structp write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!write_png_ptr) {
        printf("Error creating write struct\n");
        fclose(output_fp);
        return 1;
    }

    png_infop write_info_ptr = png_create_info_struct(write_png_ptr);
    if (!write_info_ptr) {
        printf("Error creating write info struct\n");
        png_destroy_write_struct(&write_png_ptr, NULL);
        fclose(output_fp);
        return 1;
    }

    if (setjmp(png_jmpbuf(write_png_ptr))) {
        printf("Error during write\n");
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        fclose(output_fp);
        return 1;
    }
    png_colorp palette;
    int num_palette;
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(read_png_ptr, read_info_ptr, &palette, &num_palette);
        png_set_PLTE(write_png_ptr, write_info_ptr, palette, num_palette);
    }
    png_init_io(write_png_ptr, output_fp);
    png_color_16p trans_color;
    png_bytep trans_alpha;
    int num_trans;
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_tRNS(read_png_ptr, read_info_ptr, &trans_alpha, &num_trans, &trans_color);
        png_set_tRNS(write_png_ptr, write_info_ptr, trans_alpha, num_trans, trans_color);
    }

    //png_set_tRNS(write_png_ptr, write_info_ptr, trans_alpha, num_trans, trans_color);

    png_set_IHDR(write_png_ptr, write_info_ptr, (png_uint_32)width, (png_uint_32)height,
                 bit_depth, color_type, interlace_method, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    for (png_uint_32 i = 0; i < height; i++) {
        row_pointers_new[i] = (png_bytep)malloc(png_get_rowbytes(write_png_ptr, write_info_ptr));
    }

    for(png_uint_32 i = 0; i < height; i++) {
        for (png_uint_32 j = 0; j < png_get_rowbytes(write_png_ptr, write_info_ptr); j++) {
            row_pointers_new[i][j] = 0;
        }
    }

    int dest_row = 0, dest_col = 0, copy_row = 0, copy_col = 0;
    int dest_frame = 0;
    int source_frames[] = {0, 6, 4, 2, //Idle animations

                            //Aura animations
              //  /           4, 20, 39, 36,
                  //         5, 21, 40, 37,
                  //         6, 22, 41, 38,

                           8, 37, 66, 69,
                           9, 38, 67, 70,
                           10, 39, 68, 71,
                            //blocking
//                            //7, 23, 43, 42,
                           11, 40, 43, 42,

                            //Charging
                           //8, 24, 45, 44,
                           13, 42, 78, 76,

                            //Releasing_1
                           //9, 25, 47, 46,
                           15, 44, 82, 80,

                            //B_Releasing
                           //10, 26, 49, 48,

                           17, 46, 86, 84,

                            //Getting hit
                           //11, 27, 52, 50,
                           //12, 28, 53, 51,

                           19, 48, 92, 88,
                           21,50, 94, 90,

                            //Knockback animation
                           //13, 29, 55, 54,
                           23, 52, 98, 96,

                            //Dashing_1
                            //14, 30, 57, 56,

                            25,54, 102, 100,

                            //Dashing_2
                           //15, 31, 59, 58,
                           27, 56, 106, 104,

                            //Dashing_3
                           //16, 32, 61, 60,

                           29, 58, 110, 108,

                           /* BEYOND HERE MAY NEED SOME EDITING*/


                            //B_Charging
                           //17, 33, 63, 62,
                           31, 60, 114, 112,

                            //Driving
                           //18, 34, 65, 64,
                           33, 62, 118, 116,

                            //Sitting
                           //19, 35, 67, 66,
                           35, 64, 122, 120,

                            //Dragon_1
                           //74, 68, 84, 82,
                           //75, 69, 85, 83,

                           130, 124, 140, 138,
                           131, 125, 141, 139,


                            //Walking
                           //76, 70, 90, 86,
                           //77, 71, 91, 87,
                           //78, 72, 92, 88,
                           //79, 73, 93, 89,
                           132, 126, 146, 142,
                           133, 127, 147, 143,
                           134, 128, 148, 144,
                           135, 129, 149, 145,


                           //Tiger 1
                           //94, 80, 107, 105,
                          // 95, 81, 108, 106,

                          150, 136, 164, 162,
                          151, 137, 165, 163,


                           //Running
                         //  96, 101, 113, 109,
                          // 97, 102, 114, 110,
                        //   98, 103, 115, 111,
                          // 99, 104, 116, 112,

                          152, 158, 170, 166,
                          153, 159, 171, 167,
                          154, 160, 172, 168,
                          155, 161, 173, 169,

                            //Crane
                           //100, 117, 121, 120,

                           174, 156, 180, 178,

                           //Sword_1
                          // 134, 118, 150, 148,
                           //135, 119, 151, 149,
                           194, 176, 210, 208,
                           195, 177, 211, 209,

                           //Punch_1
                           //136, 142, 124, 122,
                           // 137, 143, 125, 123,

                           196, 202,184, 182,
                           197, 203, 185, 183,



                            //Punch_2
                          // 138, 144, 128, 126,
                          // 139, 145, 129, 127,

                          198, 204, 188, 186,
                          199, 205, 189, 187,

                           //kicking
                  //         140, 146, 132, 130,
                    //       141, 147, 133, 131,

                            200, 206, 192, 190,
                            201, 207, 193, 191,

                           //Slash
                           //158, 161, 155, 152,
                           //159, 162, 156, 153,
                           //160, 163, 157, 154,

                           224, 230, 218, 212,
                           226, 232, 220, 214,
                           228, 234, 222, 216,



                           //Ki_Blasts
                           //172, 176, 168, 164,
                           //173, 177, 169, 165,
                           //174, 178, 170, 166,
                           //175, 179, 171, 167,

                           244, 248, 240, 236,
                           245, 249, 241, 237,
                           246, 250, 242, 238,
                           247, 251, 243, 239,

                           //Hover
                           //181, 182, 183, 184,
                           252, 254, 258, 256,

                           //Fall
                           //185, 186, 187, 188,
                           260, 262, 266, 264,

                           //Fly_Blast
                           //189, 190, 191, 192
                           268, 270, 274, 272


    };


    int num_frames = sizeof(source_frames) / sizeof(source_frames[0]);

    for (int i = 0; i < num_frames; i++) {

        get_frame_coordinates(source_frames[i], &copy_row, &copy_col, DMI_HEIGHT, DMI_WIDTH);
        get_frame_coordinates(dest_frame++, &dest_row, &dest_col, DMI_HEIGHT, DMI_WIDTH);
        Get_Frame(row_pointers_new, row_pointers, dest_row, dest_col,
                  copy_row, copy_col, DMI_WIDTH, DMI_HEIGHT);
    }
    const char* keyword = "Description";
    char zTxt[10000] = "#BEGIN DMI\nversion = 4.0\n\twidth = 62\n\theight = 62\n"
                       "state = \"Idle\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Aura\"\n\tdirs = 4\n\tframes = 3\n\tdelay = 1, 1, 1\n"
                       "state = \"Block\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Charge_1\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Release_1\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"BRelease_1\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Hit\"\n\tdirs = 4\n\tframes = 2\n\tdelay = 1, 1\n"
                       "state = \"Knockback\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Dash_1\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Dash_2\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Dash_3\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"

                        /* BEYOND HERE MAY NEED SOME EDITING*/
                       "state = \"B_Charging\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Driving\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Sitting\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Dragon_1\"\n\tdirs = 4\n\tframes = 2\n\tdelay = 1, 1\n"
                       "state = \"Walking\"\n\tdirs = 4\n\tframes = 4\n\tdelay = 1, 1, 1, 1\n"
                       "state = \"Tiger_1\"\n\tdirs = 4\n\tframes = 2\n\tdelay = 1, 1\n"
                       "state = \"Running\"\n\tdirs = 4\n\tframes = 4\n\tdelay = 1, 1, 1, 1\n"
                       "state = \"Crane\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Sword_1\"\n\tdirs = 4\n\tframes = 2\n\tdelay = 1, 1\n"
                       "state = \"Punch_1\"\n\tdirs = 4\n\tframes = 2\n\tdelay = 1, 1\n"
                       "state = \"Punch_2\"\n\tdirs = 4\n\tframes = 2\n\tdelay = 1, 1\n"
                       "state = \"Kick\"\n\tdirs = 4\n\tframes = 2\n\tdelay = 1, 1\n"
                       "state = \"Slashing\"\n\tdirs = 4\n\tframes = 3\n\tdelay = 1, 1, 1\n"
                       "state = \"Ki_Blast\"\n\tdirs = 4\n\tframes = 4\n\tdelay = 1, 1, 1, 1\n"
                       "state = \"Hover\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Fall\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       "state = \"Fly_Blast\"\n\tdirs = 4\n\tframes = 1\n\tdelay = 1\n"
                       ;
    png_text text_pt_new;
    text_pt_new.compression = PNG_TEXT_COMPRESSION_zTXt;
    text_pt_new.key = (png_charp)keyword;
    text_pt_new.text = (png_charp)zTxt;
    text_pt_new.text_length = strlen(zTxt);
    png_set_text(write_png_ptr, write_info_ptr, &text_pt_new, 1);
    png_write_info(write_png_ptr, write_info_ptr);
    png_write_image(write_png_ptr, row_pointers_new);
    // printf("Write image done!\n");
    png_write_end(write_png_ptr, NULL);
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    fclose(output_fp);

    fclose(input_fp);

    return 1;
}