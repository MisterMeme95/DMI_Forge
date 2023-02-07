#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defined so I can write to a file on gui/windowing platforms */
/*  #define STDERR stderr  */
#define STDERR stdout   /* For DOS */

#include "png.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


/**
 * the get_string function (poorly named, ik) is designed specifically to return the # of characters
 * that precedes either a newline, the "=" sign, or the '\0' character.
 * @param string     - Example text
 *
 *string desdsd
 * */
int get_string_char(char *string, const char char_search){
    /*
     * This variable is simply created to determine if the char_search character
     * can actually be located. If it's not, found is null and we return 0.
     * Otherwise, found actually points to the located character, and we return the diff
     * from the beginning of a string, to the point in which the designated character is located.
     * */
    char const *found = strchr(string, char_search);
    if(found){
        return found - string;
    }
    return 0;
}
#define DMI_TOKEN "# BEGIN DMI"
#define IS_ICON_STATE "version"

char Variable_Authentication(char *string){



}

char *find_newline(char **string, int *dmi_index){
    int index = 0;
    int num_of_char = get_string_char(*string, '\n');
    char *new_string = (char *) malloc(sizeof(char) * (num_of_char + 1));
    while(index < num_of_char){

        new_string[index] = *(*string)++;
        index++;

    }
    new_string[num_of_char]='\0';
    (*string)++;
    *dmi_index -= (num_of_char+1);
    // printf("Dmi_index = %d\n", *dmi_index);
    return  new_string;
}

int main(int argc, char **argv)
{
    unsigned char buffer[100];
    int is_png;
    int run_program = 0;
    while(run_program != 1){
        //FILE *fp = fopen("Base_Black.dmi", "rb");
        FILE *fp = fopen("Base_Black.dmi", "rb");
        if (fread(buffer, 8, 1, fp) != 1){
            printf("Too many bytes!\n");
        }

        is_png = !png_sig_cmp(buffer, 0, 1);
        if (!is_png)
        {
            printf("This is not a png!\n");
        }

        else {
            printf("This IS a png file!\n");
        }

        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
                                                     NULL);
        if (!png_ptr)
            return 0;

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            png_destroy_read_struct(&png_ptr,
                                    (png_infopp)NULL, (png_infopp)NULL);
            return 0;
        }

        if (setjmp(png_jmpbuf(png_ptr)))
        {
            png_destroy_read_struct(&png_ptr, &info_ptr,
                                    (png_infopp)NULL);
            fclose(fp);
            return 0;
        }
        /*This is where you initialize the png pointer to be the file you read. */
        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);


        png_textp text_ptr;
        int num_text;

        if (png_get_text(png_ptr, info_ptr, &text_ptr, &num_text) > 0)
        {
           int i;
           fprintf(STDERR,"\n");
           for (i=0; i<num_text; i++)
           {
               fprintf(STDERR,"Text compression[%d]=%d\n",
                       i, text_ptr[i].compression);
           }
        }

        char *dmi_check = text_ptr->text;
        char *string_parser;// = find_newline(dmi_check);

        int dmi_length = strlen(text_ptr->text);

        while(dmi_length > 0){
            string_parser = find_newline(&dmi_check, &dmi_length);
            if(strcmp(string_parser, DMI_TOKEN) == 0){
                printf("This is a DMI!\n");
            }
            printf("%s\n", string_parser);
        }
        /*
        printf("\n\n\n");
        printf("state =\"Attack\"\n");
        printf("\t dir = 4\n");
*/
        printf("Do you wish to continue? (0/1)\n");
        scanf("%d", &run_program);
    }
   return 0;
}