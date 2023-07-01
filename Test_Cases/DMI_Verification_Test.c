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
#include "../dmi.h"
#include <stdlib.h>
#include <string.h>
#define STDERR stdout

int main(int argc, char **argv){
    png_structp read_png_ptr;
    png_infop read_info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type;
    int run_program = 10;
    while(run_program >= 1) {
        FILE *input_fp;
        if (!check_if_png("Base_Black.dmi", &input_fp)) {
            printf("The file you are attempting to read is not a valid PNG file!\n");
            return 0;
        }

        if (initialize_image(&read_png_ptr, &read_info_ptr, &input_fp)) {
            printf("Initializing the image failed!\n");
            return 0;
        }

        png_get_IHDR(read_png_ptr, read_info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

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
        char checker = 'y';
        int new_options = 0;
        while (checker == 'y') {
            printf("Choose a number!\n");
            scanf("%d", &new_options);
            printf("You selected %d", new_options);
            printf("\n");
            if (new_options > 0 && new_options <= 11) {
                new_options -= 1;
                printf("Name = %s\n", new_icon->begin_icon_state[new_options].state);
                printf("# of Dir = %d\n", new_icon->begin_icon_state[new_options].dirs);
                printf("# of Frames = %d\n", new_icon->begin_icon_state[new_options].frames);
            } else
                checker = 'n';
        }
        return 1;
    }
}

