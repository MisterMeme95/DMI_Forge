#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defined so I can write to a file on gui/windowing platforms */
/*  #define STDERR stderr  */
#define STDERR stdout   /* For DOS */

#include "DMI_Defs.h"
#include "png.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>


/**
 * the get_string function (poorly named, ik) is designed specifically to return the # of characters
 * that precedes either a newline, the "=" sign, or the '\0' character.
 * @param string     - Example text
 *
 *string desdsd
 * */
int get_string_char(char *string, char* char_search){
    /*
     * This variable is simply created to determine if the char_search character
     * can actually be located. If it's not, found is null and we return 0.
     * Otherwise, found actually points to the located character, and we return the diff
     * from the beginning of a string, to the point in which the designated character is located.
     * */
    //printf("Get string processed. .. \n");
    char *found = strstr(string, char_search);
    //printf("Found = %s\n");
    if(found){
      //  printf("# of Char = %d\n", found - string);
        return found - string;
    }
    return 0;
}


char *Variable_Authentication(char *string){
    //We have to find the = symbol
    int index = 0, token_track = 0;
    int num_of_char = get_string_char(string, "=");
    char *value_check = (char *) malloc(sizeof(char) * (num_of_char + 1));
    if(num_of_char == 0){
        printf("This is not a value!\n");
    }
    //printf("Looping through string!\n\n");
    while(token_track < num_of_char){
       // sleep(1);
       // printf("%d ", *string);
        if(!isspace(*string) && !iscntrl(*string)){
            //printf("%c", *string);
            value_check[index] = *string++;
            index++;
        }

        else{
            string++;
        }
        token_track++;
    }
    value_check[num_of_char-1]='\0';
    //printf("\nValue = %s\n", value_check);
    return value_check;
}


void Print_Variable(char *string){
    char *check_string = Variable_Authentication(string);
    if(strcmp(check_string, "state") == 0){
        printf("This is an icon_state!\n\n");
    }

    if(strcmp(check_string, "version") == 0){
        printf("This is a version!\n\n");
    }

    if(strcmp(check_string, "frames") == 0){
        printf("This is frames!\n\n");
    }

}
/* This function is designed to locate the newline.
 *
 * Actually, I need to edit this. Make it a function that can find a designated string or character.
 * string - a pointer to a pointer. This represents the string that I am reading, so that I can permanently change
 *          its starting index.
 *
 * dmi_index - A pointer to an integer. This represents the size of the zTxt. I keep track of this to know
 *             when there is more text to read.
 *
 * search_for - This is the string that we will be searching for.
 * */
char *find_newline(char **string, int *dmi_index, char *search_for){
    int index = 0;
    int num_of_char = get_string_char(*string, search_for);
    char *new_string = (char *) malloc(sizeof(char) * (num_of_char + 1));
    while(index < num_of_char){

        new_string[index] = *(*string)++;
        index++;

    }
    new_string[num_of_char]='\0';
    (*string)++;
    *dmi_index -= (num_of_char+1);
    // printf("Dmi_index = %d\n", *dmi_index);
   // printf("dmi_index = %d\n", *dmi_index);
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

        //Read the png_ptr, and pass the text to info_ptr
        png_read_info(png_ptr, info_ptr);


        //Create a container for the text
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

            /*
             * - First I need to skip all zTxt and go to the part where it BEGIN_DMI token is found.
             * */


           // sleep(25);
            //If it does, let's begin parsing the string.
            string_parser = find_newline(&dmi_check, &dmi_length, "\n");

            //if(strcmp(string_parser, BEGIN_DMI) == 0){
                 //       printf("This is a DMI!\n");
            //}
            printf("%s\n", string_parser);
            Print_Variable(string_parser);

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