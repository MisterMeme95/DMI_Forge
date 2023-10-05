#define _POSIX_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
/* Defined so I can write to a file on gui/windowing platforms */
/*  #define STDERR stderr  */
#define STDERR stdout   /* For DOS */


#include "png.h"
#include "dmi.h"



int main(int argc, char **argv)
{
  //  PNG_To_DMI();
    //return 1;
    png_structp read_png_ptr;
    png_infop read_info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, pixels_per_byte;
    int run_program = 10;
    while(run_program >= 1){
        FILE *input_fp;
        if(!check_if_png("Base_Black.dmi",&input_fp)){
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


        png_bytepp row_pointers = (png_bytepp)malloc(sizeof(png_bytep) * num_rows);
        printf("Row Bytes = %d\nHeight = %d\n", png_get_rowbytes(read_png_ptr, read_info_ptr), height);
        for (png_uint_32 i = 0; i < height; i++) {
            row_pointers[i] = (png_bytep)malloc(png_get_rowbytes(read_png_ptr, read_info_ptr));
        }

        png_read_image(read_png_ptr, row_pointers);
        //png_read_rows(read_png_ptr, row_pointers, NULL, num_rows);
        printf("Rowbytes = %d\n", png_get_rowbytes(read_png_ptr, read_info_ptr));
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

        //ALL THE ABOVE SHOULD JUST BE IN A FUNCTION
        DMI *new_icon = (DMI*) malloc(sizeof(DMI));

        Init_DMI(new_icon, 32, 32);

        //dmi_check is just a reference to the zTxt so we can read the dmi.
        char *dmi_check = text_ptr->text;
        char *string_parser;// = find_newline(dmi_check);

        if(!(dmi_check = strstr(dmi_check, BEGIN_DMI))){
            printf("There is no starting DMI token!\n");

            return 0;
        }
        int dmi_length = strlen(dmi_check);

        //We need to continuously scan the DMI from start to finish, examining by line.
        if(!(dmi_length > 0)){
            printf("There is no text to parse in this image!\n\n");
            return 0;
        }

        while(dmi_length > 0){
        /* - First I need to skip all zTxt and go to the part where it BEGIN_DMI token is found. */
            string_parser = find_newline(&dmi_check, &dmi_length, "\n");
            Print_Variable(string_parser, new_icon);
        }
        printf("while(dmi_length > 0) done \n");
        int sheet_size = Get_Sheet_Size(new_icon);
        int sheet_width = Get_Sheet_Width(new_icon);
        png_bytepp row_pointers_new = (png_bytepp)malloc(sizeof(png_bytep) * sheet_size);
        FILE *output_fp = fopen("Base_Black_output.png", "wb");
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

        png_set_IHDR(write_png_ptr, write_info_ptr, (png_uint_32)sheet_width*2, (png_uint_32)sheet_size,
                     bit_depth, color_type, interlace_method, PNG_COMPRESSION_TYPE_DEFAULT,
                     PNG_FILTER_TYPE_DEFAULT);

        for (png_uint_32 i = 0; i < sheet_size; i++) {
            row_pointers_new[i] = (png_bytep)malloc(png_get_rowbytes(write_png_ptr, write_info_ptr));
        }

        for(png_uint_32 i = 0; i < sheet_size; i++) {
            for (png_uint_32 j = 0; j < png_get_rowbytes(write_png_ptr, write_info_ptr); j++) {
                row_pointers_new[i][j] = 0;
            }
        }

        DMI_To_Png(new_icon, png_get_rowbytes(read_png_ptr, read_info_ptr), 144, row_pointers,row_pointers_new,
                   write_png_ptr, write_info_ptr, pixels_per_byte, color_type, LINEAR_FLOW, LINEAR_FLOW);

        //printf("Dmi to png complete!\n");
        png_write_info(write_png_ptr, write_info_ptr);

        png_write_image(write_png_ptr, row_pointers_new);
       // printf("Write image done!\n");
        png_write_end(write_png_ptr, NULL);
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        fclose(output_fp);

        fclose(input_fp);

        free(row_pointers_new);
        free(row_pointers);

        char c;
        printf("Do you wish to continue? (0/1)\n");
        scanf("%d", &run_program);
        while ((c = getchar()) != '\n' && c != EOF);
    }
   return 1;
}