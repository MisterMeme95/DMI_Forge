#define _POSIX_SOURCE 1
#define PNG_BYTES_TO_CHECK 8
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defined so I can write to a file on gui/windowing platforms */
/*  #define STDERR stderr  */
#define STDERR stdout   /* For DOS */

#include "DMI_Defs.h"
#include "DMI_Struct.c"
#include "png.h"
#include "DMIParse.c"
int initialize_image(png_structp *png_ptr, png_infop *png_info, FILE **fp){
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!(*png_ptr)) {
        printf("Error creating read struct\n");
        fclose(*fp);
        return 1;
    }
    png_set_sig_bytes(*png_ptr, PNG_BYTES_TO_CHECK);
    *png_info = png_create_info_struct(*png_ptr);
    if (!(*png_info)) {
        printf("Error creating read info struct\n");
        png_destroy_read_struct(png_ptr, NULL, NULL);
        fclose(*fp);
        return 1;
    }

    if (setjmp(png_jmpbuf(*png_ptr))) {
        printf("Error during read\n");
        png_destroy_read_struct(png_ptr, png_info, NULL);
        fclose(*fp);
        return 1;
    }
    png_init_io(*png_ptr, *fp);
    png_read_info(*png_ptr, *png_info);

    return 0;

}

int check_if_png(char *file_name, FILE **fp)
{
    char buf[PNG_BYTES_TO_CHECK];

    /* Open the prospective PNG file. */
    if ((*fp = fopen(file_name, "rb")) == NULL) {
        printf("Error opening input file\n");
        return 0;
    }

    /* Read in some of the signature bytes. */
    if (fread(buf, 1, PNG_BYTES_TO_CHECK, *fp) != PNG_BYTES_TO_CHECK){
        printf("Not enough bytes read!\n");
        return 0;
    }


    /* Compare the first PNG_BYTES_TO_CHECK bytes of the signature.
     * Return nonzero (true) if they match.
     */
    return(!png_sig_cmp(buf, 0, PNG_BYTES_TO_CHECK));
}

int main(int argc, char **argv)
{
    png_structp read_png_ptr;
    png_infop read_info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type;
    int run_program = 0;
    while(run_program != 1){
        FILE *input_fp;
        if(!check_if_png("Base_Black.dmi",&input_fp)){
            printf("The file you are attempting to read is not a valid PNG file!\n");
            return 0;
        }

        if(initialize_image(&read_png_ptr, &read_info_ptr, &input_fp)) {
            printf("Initializing the image failed!\n");
            return 0;
        }


        png_get_IHDR(read_png_ptr, read_info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

        png_uint_32 num_rows = (height > 32) ? height  : height; // Read only the first 32 rows or the entire image if it has fewer than 32 rows
        png_bytepp row_pointers = (png_bytepp)malloc(sizeof(png_bytep) * num_rows);
        png_bytepp row_pointers_new = (png_bytepp)malloc(sizeof(png_bytep) * num_rows);
        for (png_uint_32 i = 0; i < num_rows; i++) {
            row_pointers[i] = (png_bytep)malloc(png_get_rowbytes(read_png_ptr, read_info_ptr));
            row_pointers_new[i] = (png_bytep)malloc(png_get_rowbytes(read_png_ptr, read_info_ptr));
        }

        png_read_rows(read_png_ptr, row_pointers, NULL, num_rows);
        for(int i = 0; i < height; i++){

        }
        png_textp text_ptr;
        int num_text;
        if (png_get_text(read_png_ptr, read_info_ptr, &text_ptr, &num_text) > 0) {
            int i;
            fprintf(STDERR,"\n");
            for (i=0; i<num_text; i++){
                fprintf(STDERR,"Text compression[%d]=%d\n",
                        i, text_ptr[i].compression);
            }
        }
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


        /*
        char checker = 'y';
        int new_options = 0;
        I only have this to run tests on whether an icon worked or not.
        while(checker == 'y'){
            printf("Choose a number!\n");
            scanf("%d", &new_options);
            printf("You selected %d", new_options);
            printf("\n");
            if(new_options > 0 && new_options <= 11){
                new_options-= 1;
                printf("Name = %s\n",new_icon->begin_icon_state[new_options].state);
                printf("# of Dir = %d\n",new_icon->begin_icon_state[new_options].dirs);
                printf("# of Frames = %d\n",new_icon->begin_icon_state[new_options].frames);
            }
            else
                checker = 'n';
        }
        */
        //ADD HERE
        fclose(input_fp);

        FILE *output_fp = fopen("Lol.png", "wb");
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
        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            png_colorp palette;
            int num_palette;
            png_get_PLTE(read_png_ptr, read_info_ptr, &palette, &num_palette);
            png_set_PLTE(write_png_ptr, write_info_ptr, palette, num_palette);
        }
        png_init_io(write_png_ptr, output_fp);

        png_set_IHDR(write_png_ptr, write_info_ptr, width, num_rows, bit_depth, color_type,
                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        png_write_info(write_png_ptr, write_info_ptr);

        png_write_rows(write_png_ptr, row_pointers, num_rows);

        png_write_end(write_png_ptr, NULL);

        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);


        fclose(output_fp);

// Free row memory and clean up
        for (png_uint_32 i = 0; i < num_rows; i++) {
            free(row_pointers[i]);
        }
        free(row_pointers);

        printf("Do you wish to continue? (0/1)\n");
        scanf("%d", &run_program);
    }
   return 0;
}