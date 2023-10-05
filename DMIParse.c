#define PNG_BYTES_TO_CHECK 8


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dmi.h"

/**
 * the get_fstring function is designed specifically to return the # of characters
 * that precedes either a newline, the "=" sign, or the '
 *
 * \0' character.
 * @param
 *      string The argument represents the string that is being searched. Must be a pointer.
 * @param
 *      char_search  This is the character that we are searching for. This is usually the =, newline or '\0' characters.
 * */
int get_string_char(char *string, char* char_search, char doo){
    /*
     * This variable is simply created to determine if the char_search character
     * can actually be located. If it's not, found is null, and we return 0.
     * Otherwise, found actually points to the located character, and we return the diff
     * from the beginning of a string, to the point in which the designated character is located.
     * */
    char *found = strstr(string, char_search);

    if(found){
        /* This is a simple sanity check. Basically checks to see if
         * the length of a string is 0. */
        if(doo == 'y'){
            return strlen(string);
        }
        return found - string;
    }
    return 0;
}


/**
 * This function is utilized to authenticate the type of variable that we will be examining.
 * In order to do so, we read from the start of a line, all the way to the equal sign (=) character.
 * @param
 *      string The string that we will be parsing.  * */
char *Variable_Authentication(char *string){
    // printf("Starting Variable Auth. . . \n\n\n");
    int index = 0, token_track = 0;
    int debug_tracker = 0;
    int num_of_char = get_string_char(string, "=", 'n');
    //   printf("num_of_char = %d\n", num_of_char);
    char *value_check = (char *) malloc(sizeof(char) * (num_of_char+1));
    if(num_of_char == 0){
        printf("This is not a value!\n");
        printf("%s\n", string);
    }



    //printf("string = %s\n", string);
    //printf("%s\n", string);
    //printf("Num of chars to equal sign: %d\n", num_of_char);
    //printf("value string = %s\n", value_check);
    //printf("Size of variable = %d\n", strlen(value_check));
    while(token_track < num_of_char){
        if(!isspace(*string) && !iscntrl(*string)){
            value_check[index] = *string++;
            index++;
        }

        else{
            string++;
        }
        token_track++;
    }
  //  printf("\n");
    if(strlen(value_check) > index){
        value_check[index]='\0';
    }
    value_check[num_of_char]='\0';
   // printf("Variable = %s\n", value_check);
    /*
    printf("Variable = %s\n", value_check);
    printf("Ending size of string: %d\n", strlen(value_check));
    printf("Index Size = %d\n", index);
    printf("num_of_char = %d\n", num_of_char);
    printf("------------------------\n");
     */
    return value_check;
}

char *State_Authentication(char *string){
    //We have to find the = symbol
    int index = 0, token_track = 0;

    int num_of_char = get_string_char(string, "\0", 'y');
    char *value_check = (char *) malloc(sizeof(char) * (num_of_char));
    if(num_of_char == 0){
        printf("This is not a value! (Variable Auth)\n");
        printf("num_of_char. . \n");
    }

    while(token_track < num_of_char){
        if(!isspace(*string) && !iscntrl(*string)){
            value_check[index] = *string++;
            index++;
        }
        else{
            string++;
        }
        token_track++;
    }
    value_check[num_of_char-1]='\0';
    return value_check;
}

char *Value_Authentication(char *string){
    //We have to find the = symbol
    int index = 0, token_track = 0;
    int num_of_char = get_string_char(string, "\0", 'y');
    char *value_check = (char *) malloc(sizeof(char) * (num_of_char + 1));

    if(num_of_char == 0){
        printf("This is not a value! (Variable Auth)\n");
    }

    while(token_track < num_of_char){
        if(!isspace(*string) && !iscntrl(*string)){
            value_check[index] = *string++;
            index++;
        }
        else{
            string++;
        }
        token_track++;
    }
    value_check[num_of_char-1]='\0';
    //printf("Value = %s\n", value_check);
    return value_check;
}

void Print_Variable(char *string, DMI* dmi){
    char *check_string = Variable_Authentication(string);
    char *found = strstr(string, "=");
    char *variable_value;// = Value_Authentication(found);
    int integer_value;
    if(found){
        found+= 1;
        if(strcmp(check_string, ICON_STATE) == 0){
            variable_value = State_Authentication(found);
            if(dmi->has_icons){
                dmi->icon_states++;
                dmi->num_of_states++;
                Initialize_IconState(dmi->icon_states, variable_value);
            }
            else{
                Initialize_IconState(dmi->icon_states, variable_value);
                dmi->has_icons=true;
                dmi->num_of_states++;
            }
            if(dmi->num_of_states == (dmi->max_state)-1){
                dmi->max_state += 30;
                Resize_IconStates(dmi, dmi->max_state);
            }
        }
        else {
            variable_value = Value_Authentication(found);

            //printf("Variable_Value (Before) = %s\n", variable_value);
            integer_value= atoi(variable_value);

            // printf("integer_value (After) = %d\n", integer_value);
        }
    }
    if(strcmp(check_string, "width") == 0){
        dmi->width = integer_value;
    }
    if(strcmp(check_string, "height") == 0){
        dmi->height = integer_value;
    }
    if(strcmp(check_string, "dirs") == 0){

     //   printf("For %s the dir = %d\n", dmi->icon_states->state, integer_value);
        Add_Dir(dmi->icon_states, integer_value);
    }
    if(strcmp(check_string, "frames") == 0){
        Add_Frames(dmi->icon_states, integer_value);
    }
    if(strcmp(check_string, "movement") == 0){
        if(integer_value >= 1){
            Add_Movement(dmi->icon_states);
        }
    }
    if(strcmp(check_string, "loop") == 0){
        Add_Loop(dmi->icon_states, integer_value);
    }
    if(strcmp(check_string, "rewind") == 0){
        if(integer_value >= 1){
            Add_Rewind(dmi->icon_states);
        }
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
    int num_of_char = get_string_char(*string, search_for, 'n');
    char *new_string = (char *) malloc(sizeof(char) * (num_of_char + 1));
    while(index < num_of_char){
        new_string[index] = *(*string)++;
        index++;
    }
    new_string[num_of_char]='\0';
    (*string)++;
    *dmi_index -= (num_of_char+1);
    return  new_string;
}

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