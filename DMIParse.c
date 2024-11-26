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
    int index = 0, token_track = 0;
    int debug_tracker = 0;
    int num_of_char = get_string_char(string, "=", 'n');
    char *value_check = (char *) malloc(sizeof(char) * (num_of_char+1));
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
    if(num_of_char > index){
        value_check[index]='\0';
    }
    value_check[num_of_char]='\0';
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

char *Value_Authentication(char *string) {
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

void Print_Variable(char *string, DMI* dmi) {
    char *check_string = Variable_Authentication(string);
    char *found = strstr(string, "=");
    char *variable_value;// = Value_Authentication(found);
    int integer_value;
    if(found){
        found+= 1;
        if(strcmp(check_string, ICON_STATE) == 0){
            variable_value = State_Authentication(found);
            if(dmi->has_icons){

                icon_state *tail_iconstate = dmi->iconStates.tail->data;
                tail_iconstate->frame_vector = (Vector*)malloc(sizeof(Vector) * tail_iconstate->dirs);
                for(int i = 0; i < tail_iconstate->dirs; i++){
                    vector_init(&tail_iconstate->frame_vector[i], sizeof(png_bytepp),
                                tail_iconstate->number_of_frames, NULL, NULL);
                }
                png_uint_32 start_col = (dmi->frame_count * dmi->icon_row_bytes) % dmi->image->row_bytes;
                png_uint_32 start_row = (dmi->frame_count * dmi->icon_row_bytes) / dmi->image->row_bytes;
                for(int i = 0; i < tail_iconstate->dirs; i++){
//                    start_row +=  dmi->icon_row_bytes;
//                    start_col += start_row / dmi->image->row_bytes;
//                    start_col += dmi->icon_row_bytes;//(start_row / dmi->image->row_bytes);
                    png_uint_32 new_col = start_col + (dmi->icon_row_bytes * i);

                    png_uint_32 new_row = new_col / dmi->image->row_bytes;

                    for(int o = 0; o < tail_iconstate->number_of_frames; o++){
                        png_uint_32 incre_amount = o * tail_iconstate->dirs * dmi->icon_row_bytes;
                        int test_val = incre_amount % dmi->image->row_bytes;
                        png_uint_32 extract_col = new_col + (incre_amount % dmi->image->row_bytes);
                        png_uint_32 extract_row = (new_row + (new_col + incre_amount) / dmi->image->row_bytes) * dmi->icon_height;

                        png_bytepp* image_data = (png_bytepp*)tail_iconstate->frame_vector[i].data;
                        image_data[o] = (png_bytepp)malloc(sizeof(png_bytep) * dmi->icon_height);

                        for (int k = 0; k < dmi->icon_height; k++) {
                            image_data[o][k] = dmi->image->pixel_array[extract_row + k] + extract_col;
                        }

                    }
                }
                dmi->frame_count += tail_iconstate->dirs * tail_iconstate->number_of_frames;


                dmi->icon_states++;
                dmi->num_of_states++;
                icon_state *new_icon_state = (icon_state*)malloc(sizeof(icon_state));
                Initialize_IconState(dmi->icon_states, variable_value);
                Initialize_IconState(new_icon_state, variable_value);

                dmi->iconStates.insert(&dmi->iconStates, dmi->iconStates.tail, new_icon_state);
            }
            else{
                Initialize_IconState(dmi->icon_states, variable_value);
                icon_state *new_icon_state = (icon_state*)malloc(sizeof(icon_state));
                Initialize_IconState(new_icon_state, variable_value);
                dmi->has_icons=true;
                dmi->num_of_states++;
                dmi->iconStates.insert(&dmi->iconStates, NULL, new_icon_state);
              //  dmi->iconStates.tail = NULL;
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
        dmi->png_width = integer_value;
    }
    if(strcmp(check_string, "height") == 0){
        dmi->png_height = integer_value;
    }
    if(strcmp(check_string, "dirs") == 0){

     //   printf("For %s the dir = %d\n", dmi->icon_states->state, integer_value);
        Add_Dir(dmi->icon_states, integer_value);
        Add_Dir(dmi->iconStates.tail->data, integer_value);

    }

    if(strcmp(check_string, "width") == 0){

        //   printf("For %s the dir = %d\n", dmi->icon_states->state, integer_value);
        //Add_Dir(dmi->icon_states, integer_value);
        //Add_Dir(dmi->iconStates.tail->data, integer_value);
        dmi->icon_width = integer_value;
        dmi->icon_row_bytes = (dmi->image->row_bytes / dmi->icon_width == 0)
                              ? dmi->image->row_bytes
                              : dmi->image->row_bytes / dmi->icon_width;
    }

    if(strcmp(check_string, "height") == 0){

        //   printf("For %s the dir = %d\n", dmi->icon_states->state, integer_value);
        //Add_Dir(dmi->icon_states, integer_value);
        //Add_Dir(dmi->iconStates.tail->data, integer_value);
        dmi->icon_height = integer_value;
    }
    if(strcmp(check_string, "frames") == 0){
        Add_Frames(dmi->icon_states, integer_value);
        Add_Frames(dmi->iconStates.tail->data, integer_value);
    }
    if(strcmp(check_string, "movement") == 0){
        if(integer_value >= 1){
            Add_Movement(dmi->icon_states);
            Add_Movement(dmi->iconStates.tail->data);

        }
    }
    if(strcmp(check_string, "loop") == 0){
        Add_Loop(dmi->icon_states, integer_value);
        Add_Loop(dmi->iconStates.tail->data, integer_value);

    }
    if(strcmp(check_string, "rewind") == 0){
        if(integer_value >= 1){
            Add_Rewind(dmi->icon_states);
            Add_Rewind(dmi->iconStates.tail->data);

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


