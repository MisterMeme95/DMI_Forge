/* The DMI structure will be used to contain all relevant information associated with a DMI.
 * The variables will be:
 *
 * - version:       The version of BYOND it should use. Either 4.0 (most current DMI format) or 3.5.
 * - width:         The width for each frame.
 * - height:        The height for each frame.
 * - icon_states:   This should be a vector containing a list of all icon_states. */
#include "png.h"
#include "dmi.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#ifdef __cplusplus
extern "C" {
#endif
unsigned long hash_string2(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = (int)*str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

int insert_state2(icon_state *iconState,  iconstate_hash *state_lookup, png_bytepp* image_data) {

    unsigned long hash_index = hash_string2(iconState->state) % 256;  // Ensure hash_pixel is defined correctly

    if (state_lookup->hash_bucket[hash_index] == NULL) {

        //create a new icon_state
        iconStateNode *new_node = (iconStateNode *) malloc(sizeof(iconStateNode));
        if (new_node == NULL) {
            fprintf(stderr, "Failed to allocate memory for new node\n");
            return -1;
        }

        // Initialize the new icon_state:

        new_node->next = NULL;
        new_node->prev = NULL;
        new_node->icon_state = iconState;//(icon_state*) malloc(sizeof(icon_state));
        new_node->icon_state->frames = (png_bytepp) malloc(sizeof(png_bytep) * 32);
        for (int i = 0; i < 32; i++) {
            // Assuming each image_data[i] is pointing to an array of png_bytep
            new_node->icon_state->frames[i] = (png_bytep) malloc(sizeof(png_byte) * 128); // Adjust size based on actual frame size
            memcpy(new_node->icon_state->frames[i], (*image_data)[i], sizeof(png_byte) * 128); // Copying the actual byte data
        }

//        printf("Checking insert! Icon data = %d\n", new_node->icon_state->frames[0][0]);
//        sleep(10);
       // new_node->icon_state->state = (char *) malloc(sizeof(char) * strlen(iconState->state));
       // strcpy(new_node->icon_state->state, iconState->state);
        new_node->icon_state->number_of_frames = 1;
        new_node->icon_state->dirs = 1;
        new_node->icon_state->delays = (int *) malloc(sizeof(int));
        *new_node->icon_state->delays = 1;
        state_lookup->hash_bucket[hash_index] = new_node;
    }
    else {
        iconStateNode *new_node = (iconStateNode*) malloc(sizeof(iconStateNode));
        if (new_node == NULL) {
            fprintf(stderr, "Failed to allocate memory for new node\n");
            return -1;
        }

        new_node->next = state_lookup->hash_bucket[hash_index];
        new_node->prev = NULL;
        //new_node->icon_state = (icon_state*) malloc(sizeof(icon_state));
        new_node->icon_state = iconState;//(icon_state*) malloc(sizeof(icon_state));
        new_node->icon_state->frames = (png_bytepp) malloc(sizeof(png_bytep) * 32);
        new_node->icon_state->frames = *image_data;
       // new_node->icon_state->state = (char *) malloc(sizeof(char) * strlen(iconState->state));
        //strcpy(new_node->icon_state->state, iconState->state);
        new_node->icon_state->number_of_frames = 1;
        new_node->icon_state->dirs = 1;
        new_node->icon_state->delays = (int *) malloc(sizeof(int));
        *new_node->icon_state->delays = 1;

        // Update the previous first node
        if (state_lookup->hash_bucket[hash_index] != NULL) {
            state_lookup->hash_bucket[hash_index]->prev = new_node;
        }

        // Set the new node as the first node in the bucket
        state_lookup->hash_bucket[hash_index] = new_node;
    }
    return 0;
}


int match_state2(icon_state *name, icon_state *other_name){

//    printf("%s  %s\n", name->state, other_name->state);
//    sleep(10);
    if(strcmp(name->state, other_name->state) != 0){
        return 0;
    }

    if(name->dirs != other_name->dirs){
        return 0;
    }

    if(name->number_of_frames != other_name->number_of_frames){
        return 0;
    }

    return 1;
}

icon_state* find_state2(const char *name, iconstate_hash *state_lookup, icon_state find_state) {

    // Assuming hash_pixel function correctly calculates a hash based on Pixel_Data
    unsigned long hash_index = hash_string2(name) % 256;  // Ensure hash_pixel is defined correctly

    iconStateNode *key_to_find = state_lookup->hash_bucket[hash_index];

    while (key_to_find != NULL) {
//        Pixel_Data other_pixel = key_to_find->pixel_info;

        if (match_state2(key_to_find->icon_state, &find_state)) {

            return key_to_find->icon_state; // Return the index of the matching node
        }

        key_to_find = key_to_find->next; // Move to next node in the list
    }

    return NULL; // Return -1 if no match is found after checking all nodes in the bucket
}




/* A basic function to initialize a DMI struct. */
void Init_DMI(DMI* dmi, int width, int height){
    dmi->num_of_states = 0;
    dmi->max_state = 15;
    dmi->has_icons = false;
    dmi->version = 4.0;
    dmi->width = width;
    dmi->height = height;
    dmi->icon_states = (icon_state*) malloc(sizeof(icon_state) * dmi->max_state);
    dmi->begin_icon_state = dmi->icon_states;
    for(int i = 0; i < 256; i++){
        dmi->iconstateHash.hash_bucket[i] = NULL;
    }
}

void add_iconstate(DMI* dmi, icon_state *iconState, png_bytepp image_data){
//    printf("State Name - %s\n", iconState->state);

    if(find_state2(iconState->state, &dmi->iconstateHash, *iconState) != NULL){
//        printf("Failed!\n");
        return;
    }
//    printf("Checking insert. . .!\n\n\n");
    insert_state2(iconState, &dmi->iconstateHash, &image_data);
//    printf("Checking insert #2. . .!\n\n\n");
    if(!dmi->has_icons){
        dmi->has_icons = true;
    }

    if(dmi->num_of_states >= dmi->max_state){
//        printf("Checking insert #3. . .!\n\n\n");
        dmi->max_state *= 2;
        icon_state *temp_pointer = realloc(dmi->icon_states, sizeof(icon_state) * dmi->max_state);
        dmi->icon_states = temp_pointer;
//        printf("Checking insert #4. . .!\n\n\n");
        dmi->icon_states[dmi->num_of_states] = *iconState;///*find_state2(iconState->state, &dmi->iconstateHash, *iconState);//*iconState;
        dmi->num_of_states++;
    }

    else{

//        printf("Died before insert. . \n");
        dmi->icon_states[dmi->num_of_states] = *iconState;
//        printf("Died after insert. . \n");

        dmi->num_of_states++;
    }

}
void Resize_IconStates(DMI* dmi, int new_size){
    icon_state *newStates = realloc(dmi->begin_icon_state, new_size * sizeof(icon_state));
    dmi->begin_icon_state = newStates;
    dmi->icon_states = dmi->begin_icon_state + dmi->num_of_states-1;

}
void get_dmi_dimensions(int *width, int* height, int icon_size, int num_of_frames){
    int W = (int)sqrt(num_of_frames);
    int H = (int)ceil((double)num_of_frames/W);

    *width = W * 32;
    *height = H * 32;

}
void create_dmi(DMI* dmi){
    Image new_image;
    int width = 32;
    int height = 32;
    int start_row = 0;
    int start_col = 0;
    get_dmi_dimensions(&width, &height, 32, dmi->num_of_states);
    initialize_image2(dmi->name, &new_image, &dmi->bit_depth, &dmi->color_type, &width, &height);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < new_image.row_bytes ; j++){
            new_image.pixel_array[i][j] = 0;
        }
    }
    char zTxt[100000] = "#BEGIN DMI\nversion = 4.0\n\twidth = 32\n\theight = 32\n";

    for(int i = 0; i < dmi->num_of_states; i++){
        for(int j = 0; j < 32; j++){
            memcpy(&new_image.pixel_array[start_row+j][start_col*4], dmi->icon_states[i].frames[j], sizeof(png_byte) * 128);
        }
        start_col += 32;
        if(start_col >= width){
            start_col = 0;
            start_row += 32;
        }
        char additional_info[256];
        sprintf(additional_info, "state = \"%s\"\n\tdirs = 1\n\tframes = 1\n\tdelay = 1\n",
                dmi->icon_states[i].state);
        strcat(zTxt, additional_info);  // Append new state info to zTxt

    }



    png_text text_chunk;
    text_chunk.compression = PNG_TEXT_COMPRESSION_zTXt;
    text_chunk.key = "Description";
    text_chunk.text = zTxt;
    text_chunk.text_length = strlen(zTxt);

    png_set_text(new_image.png_ptr, new_image.info_ptr, &text_chunk, 1);  // Correctly apply the single text_chunk

    png_write_info(new_image.png_ptr, new_image.info_ptr);
    png_write_image(new_image.png_ptr, new_image.pixel_array);
    png_write_end(new_image.png_ptr, NULL);
}
/** This function needs to be refactored and renamed.
 *  It's used to get the height of a spritesheet that is in HORIZONTAL_FLOW format. */

void initialize_sheet_data(SpriteSheetData* sheet_data){
    sheet_data->height = 0;
    sheet_data->width = 0;
    sheet_data->user_input_height = 0;
    sheet_data->user_input_width = 0;
    sheet_data->margin_y = 0;
    sheet_data->margin_x = 0;
    sheet_data->padding_x = 0;
    sheet_data->padding_y = 0;
    sheet_data->grid_size = 0;
    sheet_data->offset_x = 0;
    sheet_data->offset_y = 0;
    sheet_data->frame_height = 0;
    sheet_data->frame_width = 0;
    sheet_data->format = 1;
}

Image create_sprite_sheet(Image* initial_image, SpriteSheetData* sheet_data, DMI new_icon, char* file_name){
    sheet_data->frame_width = new_icon.width;
    sheet_data->frame_height = new_icon.height;
    if(sheet_data->format == GRID){

        calculate_grid_sheet_dimensions(&new_icon, sheet_data, sheet_data->grid_size);
    }
    else {
        //run other calculations to get the sheet's dimensions.
        printf("Height/Width = %d/%d\n\n\n\n\n", initial_image->height, initial_image->width);
        sheet_data->height = (int)initial_image->height;
        sheet_data->width = (int)initial_image->width;
        printf("Hrm. . .\n");
    }

    if(sheet_data->width < sheet_data->user_input_width)
        sheet_data->width = sheet_data->user_input_width;

    if(sheet_data->height < sheet_data->user_input_height)
        sheet_data->height = sheet_data->user_input_height;

    Image new_image;
    if(sheet_data->grid_size != 0){
        new_image.width = sheet_data->width + (sheet_data->margin_x * (sheet_data->grid_size - 1)) +
                (sheet_data->padding_x * sheet_data->frames_per_row);

        int num_of_rows = ceil((double)new_icon.num_of_states / sheet_data->grid_size);


        new_image.height = sheet_data->height + (sheet_data->margin_y * num_of_rows) +
                (sheet_data->padding_y * sheet_data->frames_per_col);
    }

    else {
        new_image.width = sheet_data->width;// + (sheet_data->margin_x * (sheet_data->grid_size - 1));
        new_image.height = sheet_data->height;// + (sheet_data->margin_y * num_of_rows);
    }

    //printf("Width/Height = %d/%d\n", new_image.width, new_image.height);
    new_image.bit_depth = initial_image->bit_depth;
    new_image.color_type = initial_image->color_type;
    initialize_image2(file_name, &new_image, NULL, NULL, NULL, NULL);
    return new_image;
}

void calculate_grid_sheet_dimensions(DMI* dmi, SpriteSheetData* sheet_data, int icon_states_per_row){
    icon_state *iterator = dmi->begin_icon_state;
    int num_of_states = dmi->num_of_states;

    int row_size = (int)ceil(((double)dmi->num_of_states / (double)icon_states_per_row));
    int rows_filled = 0;
    sheet_data->list_of_row_sizes = (int*) malloc(sizeof(int) * row_size);

    int widest_row = 0;
    int height_total = 0;

    for(int i = 0; i < num_of_states; i+= icon_states_per_row){
        int current_count = 0;
        int max_dir_in_row = 0;
        int iterate_to;
        if(i + icon_states_per_row >= dmi->num_of_states){
            iterate_to = num_of_states - i;
        }
        else {
            iterate_to = icon_states_per_row;
        }
        for(int o = 0; o < iterate_to; o++){
            if(iterator->dirs > max_dir_in_row){
                max_dir_in_row = iterator->dirs;
            }
            current_count += iterator->number_of_frames;
            iterator++;
        }
        height_total += max_dir_in_row;
        sheet_data->list_of_row_sizes[rows_filled] = max_dir_in_row;
        rows_filled++;
        if(current_count > widest_row)
            widest_row = current_count;
    }

    fflush(stdout);
    sheet_data->width = widest_row * sheet_data->frame_width;
    sheet_data->height = height_total * sheet_data->frame_height;
    sheet_data->frames_per_row = widest_row;
    sheet_data->frames_per_col = height_total;
    sheet_data->row_count = 0;
}

void Get_Frame(png_bytepp dest_pixels, png_bytepp src_pixels, png_uint_32 dest_start_row, png_uint_32 dest_start_col,
               int src_start_row, int src_start_col, int bytes_per_frame, int cols_to_copy) {

    for (int i = 0; i < cols_to_copy; i++) {
        memcpy(&dest_pixels[dest_start_row + i][dest_start_col],&src_pixels[src_start_row + i][src_start_col],
               bytes_per_frame);
    }

}

void Get_Frame2(png_bytepp dest_pixels, png_bytepp src_pixels, png_uint_32 dest_start_row, png_uint_32 dest_start_col,
               int src_start_row, int src_start_col, int bytes_per_frame, int cols_to_copy, int row_bytes) {

    for (int i = 0; i < cols_to_copy; i++) {
        memcpy(&dest_pixels[dest_start_row + i][dest_start_col],&src_pixels[src_start_row + i][src_start_col],
               bytes_per_frame);
    }

}

void initialize_extractor(FrameExtractor *extractor) {
    extractor->starting_column = 0;
    extractor->starting_row = 0;
    extractor->current_column = 0;
    extractor->current_row = 0;
    extractor->column_offset = 0;
    extractor->row_offset = 0;
    extractor->loop_col_start = 0;
    extractor->loop_row_start = 0;
    extractor->column_tracker = 0;
    extractor->row_tracker = 0;
}

void update_current_position(FrameExtractor *extractor){
    extractor->current_column = extractor->starting_column + extractor->column_offset;
    extractor->current_row = extractor->starting_row + extractor->row_offset;
}
void printFrameExtractor(const FrameExtractor* extractor) {
    printf("Starting column: %u\n", extractor->starting_column);
    printf("Starting row: %u\n", extractor->starting_row);
    printf("Current column: %u\n", extractor->current_column);
    printf("Current row: %u\n", extractor->current_row);
    printf("Column offset: %u\n", extractor->column_offset);
    printf("Row offset: %u\n", extractor->row_offset);
    fflush((stdout));
}
int sheet2dmi(DMI* dmi, Image source_image, Image sheet_image, SpriteSheetData sheetData){


    return 0;
}

int dmi2sheet2(DMI* dmi, Image source_image, Image sheet_image, SpriteSheetData sheetData){
    int DMI_HEIGHT = dmi->height, DMI_WIDTH = (dmi->width) / source_image.pixels_per_byte;
    int icon_state_index = 0;
    int numOfStates = dmi->num_of_states;
    int total_frames = 0;

    if(source_image.color_type == PNG_COLOR_TYPE_RGBA){
        DMI_WIDTH *= 4;
    }

    sheet_image.pixels_per_byte = source_image.pixels_per_byte;
    FrameExtractor source;
    FrameExtractor destination;
    initialize_extractor(&source);
    initialize_extractor(&destination);

    while(icon_state_index < numOfStates){
        int num_of_dirs = dmi->begin_icon_state->dirs;
        int num_of_frames = dmi->begin_icon_state->number_of_frames;
        source.loop_row_start = source.row_tracker;
        source.loop_col_start = source.column_tracker;
        for(int i = 0; i < num_of_dirs; i++){
            source.starting_column = source.loop_col_start;
            source.starting_row = source.loop_row_start;
            for(int j = 0; j < num_of_frames; j++) {
                update_current_position(&source);
                update_current_position(&destination);


                Get_Frame(sheet_image.pixel_array, source_image.pixel_array,destination.current_row,
                          destination.current_column,source.current_row,source.current_column,
                          DMI_WIDTH,DMI_HEIGHT);

                source.column_offset += (DMI_WIDTH * num_of_dirs);

                destination.column_offset += DMI_WIDTH + sheetData.padding_x;
                update_current_position(&destination);
                if(sheetData.format != GRID){
                    if(destination.current_column >= (sheet_image.row_bytes/sheet_image.pixels_per_byte)){
                        destination.column_offset = 0;
                        destination.row_offset += DMI_HEIGHT;
                        update_current_position(&destination);
                    }
                }
            }
            if(sheetData.format == GRID){
                destination.column_offset = 0;
                destination.row_offset += DMI_HEIGHT + sheetData.padding_y;
            }
            else {
                fflush(stdout);
                if(destination.current_column >= (sheet_image.row_bytes/sheet_image.pixels_per_byte)){
                    destination.column_offset = 0;
                    destination.row_offset += (destination.current_column / (sheet_image.row_bytes/ sheet_image.pixels_per_byte)) * DMI_HEIGHT;

                }
                fflush(stdout);
            }
            fflush((stdout));
            source.column_offset = 0;
            source.row_offset = 0;
            source.loop_col_start += DMI_WIDTH;// + sheetData.padding_x;
//            if(source.loop_col_start >= source_image.row_bytes){
//                source.loop_row_start  += (source.loop_col_start  / source_image.row_bytes) * DMI_HEIGHT
//                                          + sheetData.padding_y;
//                source.loop_col_start %= source_image.row_bytes;
//            }
        }/*!< End of the outer loop.*/
        // return EXIT_SUCCESS;

        total_frames += dmi->begin_icon_state->number_of_frames * dmi->begin_icon_state->dirs;

        /** If in GRID format, we want to have a new starting_column that corresponds to where we'd be after parsing
         * the previous icon_state. We set our row and column offset back to 0. */
        if(sheetData.format == GRID){
            destination.column_offset = 0;
            destination.row_offset = 0;
            destination.starting_column += dmi->begin_icon_state->number_of_frames * DMI_WIDTH +
                                           (sheetData.padding_x * dmi->begin_icon_state->number_of_frames);
        }

        source.column_tracker = (total_frames * DMI_WIDTH) % (source_image.row_bytes / source_image.pixels_per_byte);
        source.row_tracker = ((total_frames * DMI_WIDTH) / (source_image.row_bytes / source_image.pixels_per_byte)) * DMI_HEIGHT;
        dmi->begin_icon_state++;

        icon_state_index++;

        /** Likewise, we wan to set our starting column back to 0 once we've parsed all of the icon_states for a
         * our designated # of states per row. Only if in GRID format.*/
        if(sheetData.grid_size != 0) {
            if(icon_state_index % sheetData.grid_size == 0 && sheetData.format == GRID){
                destination.starting_row += sheetData.list_of_row_sizes[sheetData.row_count] * DMI_HEIGHT +
                                            (sheetData.padding_y * sheetData.list_of_row_sizes[sheetData.row_count]);
                destination.starting_column = 0;
                sheetData.row_count++;
            }
        }
    } /*!< End Point of the while loop.*/
    return EXIT_SUCCESS;
}


int dmi2sheet(DMI* dmi, Image source_image, Image sheet_image, SpriteSheetData sheetData){
    int DMI_HEIGHT = dmi->height, DMI_WIDTH = (dmi->width) / source_image.pixels_per_byte;
    int icon_state_index = 0;
    int numOfStates = dmi->num_of_states;
    int total_frames = 0;

    if(source_image.color_type == PNG_COLOR_TYPE_RGBA){
        DMI_WIDTH *= 4;
    }

    sheet_image.pixels_per_byte = source_image.pixels_per_byte;
    FrameExtractor source;
    FrameExtractor destination;
    initialize_extractor(&source);
    initialize_extractor(&destination);

    while(icon_state_index < numOfStates){
        int num_of_dirs = dmi->begin_icon_state->dirs;
        int num_of_frames = dmi->begin_icon_state->number_of_frames;
        source.loop_row_start = source.row_tracker;
        source.loop_col_start = source.column_tracker;
        for(int i = 0; i < num_of_dirs; i++){
            source.starting_column = source.loop_col_start;
            source.starting_row = source.loop_row_start;
            for(int j = 0; j < num_of_frames; j++) {
                update_current_position(&source);
                update_current_position(&destination);
                if(source.current_column >= (source_image.row_bytes/source_image.pixels_per_byte)) {
                    source.row_offset += (source.current_column / (source_image.row_bytes/ source_image.pixels_per_byte)) * DMI_HEIGHT;;
                    source.starting_column = source.current_column % (source_image.row_bytes/source_image.pixels_per_byte);
                    source.column_offset = 0;
                    update_current_position(&source);
                }

                Get_Frame(sheet_image.pixel_array, source_image.pixel_array,destination.current_row,
                          destination.current_column,source.current_row,source.current_column,
                          DMI_WIDTH,DMI_HEIGHT);
                source.column_offset += (DMI_WIDTH * num_of_dirs);

                destination.column_offset += DMI_WIDTH + sheetData.padding_x;
                //printf("WIDth = %d\n Width + Pad_X = %d\n", DMI_WIDTH + sheetData.padding_x);
                update_current_position(&destination);
               // printFrameExtractor(&destination);
                if(sheetData.format != GRID){
                    if(destination.current_column >= (sheet_image.row_bytes/sheet_image.pixels_per_byte)){
                        destination.column_offset = 0;
                        destination.row_offset += DMI_HEIGHT;
                        update_current_position(&destination);
                    }
                }
            }
            if(sheetData.format == GRID){
                destination.column_offset = 0;
                destination.row_offset += DMI_HEIGHT + sheetData.padding_y;
            }
            else {
                fflush(stdout);
                if(destination.current_column >= (sheet_image.row_bytes/sheet_image.pixels_per_byte)){
                    destination.column_offset = 0;
                    destination.row_offset += (destination.current_column / (sheet_image.row_bytes/ sheet_image.pixels_per_byte)) * DMI_HEIGHT;

                }
                fflush(stdout);
            }
            fflush((stdout));
            source.column_offset = 0;
            source.row_offset = 0;
            source.loop_col_start += DMI_WIDTH;// + sheetData.padding_x;
            if(source.loop_col_start >= source_image.row_bytes){
                source.loop_row_start  += (source.loop_col_start  / source_image.row_bytes) * DMI_HEIGHT
                        + sheetData.padding_y;
                source.loop_col_start %= source_image.row_bytes;
            }
        }/*!< End of the outer loop.*/
       // return EXIT_SUCCESS;

        total_frames += dmi->begin_icon_state->number_of_frames * dmi->begin_icon_state->dirs;

        /** If in GRID format, we want to have a new starting_column that corresponds to where we'd be after parsing
         * the previous icon_state. We set our row and column offset back to 0. */
        if(sheetData.format == GRID){
            destination.column_offset = 0;
            destination.row_offset = 0;
            destination.starting_column += dmi->begin_icon_state->number_of_frames * DMI_WIDTH +
                                           (sheetData.padding_x * dmi->begin_icon_state->number_of_frames) + sheetData.margin_x;


        }

        source.column_tracker = (total_frames * DMI_WIDTH) % (source_image.row_bytes / source_image.pixels_per_byte);
        source.row_tracker = ((total_frames * DMI_WIDTH) / (source_image.row_bytes / source_image.pixels_per_byte)) * DMI_HEIGHT;
        dmi->begin_icon_state++;

        icon_state_index++;

        /** Likewise, we wan to set our starting column back to 0 once we've parsed all of the icon_states for a
         * our designated # of states per row. Only if in GRID format.*/
        if(sheetData.grid_size != 0) {
            if(icon_state_index % sheetData.grid_size == 0 && sheetData.format == GRID){
                destination.starting_row += sheetData.list_of_row_sizes[sheetData.row_count] * DMI_HEIGHT +
                        (sheetData.padding_y * sheetData.list_of_row_sizes[sheetData.row_count]) + sheetData.margin_y;
                destination.starting_column = 0;
                sheetData.row_count++;
            }
        }
    } /*!< End Point of the while loop.*/
    return EXIT_SUCCESS;
}

#ifdef __cplusplus
}
#endif