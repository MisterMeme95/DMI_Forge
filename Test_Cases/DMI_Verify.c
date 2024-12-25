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
#include "../src/DMI/dmi.h"



int main(int argc, char **argv) {


    extern char *optarg;
    extern int optind;
    int c, err = 0;
    int mflag=0, pflag=0, fflag = 0, sflag=0;
    char *sname = "default_sname", *fname;
    static char usage[] = "usage: %s [-dmp] -f fname [-s sname] name [name ...]\n";

    while ((c = getopt(argc, argv, "f:")) != -1)
        switch (c) {

            case 'f':
                fflag = 1;
                fname = optarg;
                break;

            case '?':
                err = 1;
                break;
        }
    if (fflag == 0) {	/* -f was mandatory */
        fprintf(stderr, "%s: missing -f option\n", argv[0]);
        fprintf(stderr, usage, argv[0]);
        exit(1);
    } else if ((optind) > argc) {
        /* need at least one argument (change +1 to +2 for two, etc. as needeed) */

        printf("optind = %d, argc=%d\n", optind, argc);
        fprintf(stderr, "%s: missing name\n", argv[0]);
        fprintf(stderr, usage, argv[0]);
        exit(1);
    } else if (err) {
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    /* see what we have */


    if (optind < argc)	/* these are the arguments after the command-line options */
        for (; optind < argc; optind++)
            printf("argument: \"%s\"\n", argv[optind]);
    else {
        printf("no arguments left to process\n");
    }


    png_structp read_png_ptr;
    png_infop read_info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, pixels_per_byte;
    int run_program = 10;
    int continue_program = 1, index;
    while(run_program >= 1){
        FILE *input_fp;
        if(!check_if_png(fname,&input_fp)){
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


        printf("There are %d icon_states in this file. Select which icon_state you wish to print info on"
               "by selecting 1 - % d. \nSelect 0 if you want to end the program. \n", new_icon->num_of_states, new_icon->num_of_states);
        while(index != 0){
            printf("Select the icon_state - ");
            scanf("%d", &index);
            if(index > new_icon->num_of_states){
                printf("Nice try, this exceeds the max index!\n");
            }
            else if(index != 0){
                printf("Name - %s\nFrames - %d\nDirs - %d\n",
                       new_icon->begin_icon_state[index-1].state, new_icon->begin_icon_state[index-1].frames,
                       new_icon->begin_icon_state[index-1].dirs);
            }

            printf("\n");

        }

        fclose(input_fp);

        char c;
        printf("Do you wish to continue? (0/1)\n");
        scanf("%d", &run_program);
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return 1;
}