#include <stdlib.h>
#include <string.h>
#include "iconstate.h"




unsigned long hash_icon_state(IconSearchParams* params) {
    unsigned long hash = 5381;
    const char *str = (const char*) params->state_name; // Cast void* back to const char*
    int c;
    while ((c = (int)*str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

int match_icon_state(IconSearchParams *params, icon_state *state_node) {

    if (strcmp(params->state_name, state_node->state) != 0) {
        return 0;
    }

    if (params->is_movement != state_node->movement) {
        return 0; // Not a match
    }

    return 1;
}


IconSearchParams *create_search_params(char *state_name, int is_movement){
    IconSearchParams *search_params = (IconSearchParams*)malloc(sizeof(IconSearchParams));
    if(!search_params)
        return NULL;
    search_params->state_name = state_name;
    search_params->is_movement = is_movement;
    return search_params;
}


unsigned long hash_icon_state2(const char* state_name) {
    unsigned long hash = 5381;
    const char *str = (const char*) state_name; // Cast void* back to const char*
    int c;
    while ((c = (int)*str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}


int match_icon_state3(char* state_name, bool is_movement, icon_state *state_node) {

    if(state_node == NULL){
        return 0;
    }
    if (strcmp(state_name, state_node->state) != 0) {
        return 0;
    }

    if (is_movement != state_node->movement) {
        return 0; // Not a match
    }

    return 1;
}

void* iconstatelookup(state_hash* table, char* state_name, bool is_movement){
    unsigned long long hash_value = hash_icon_state2(state_name) % table->buckets;
    node* node_pointer;
    for(node_pointer =  list_head(&table->table[hash_value]); node_pointer != NULL; node_pointer = list_next(node_pointer)){
        if(match_icon_state3(state_name, is_movement, node_pointer->data) != 0){
            return node_pointer->data;
        }
    }
    return NULL;
}

int init_state_hash(state_hash *table, int buckets){

    int i;
    if((table->table = (void*) malloc(buckets * sizeof(list))) == NULL){
        return -1;
    }

    table->buckets = buckets;
    for(i = 0; i < buckets; i++){
        initialize_list(&table->table[i], NULL, NULL, NULL);
    }

    return 0;
}


int insert_icon_state(state_hash* table, icon_state* state){

    /* Let us generate the hash function. Store it so we don't need to do it again. */
    unsigned long hash_value = hash_icon_state2(state->state) % table->buckets;

    /* Let's list at that bucket's index, so we can traverse and find the proper node.*/
    list* list_bucket = &table->table[hash_value];


    /* This is a temporary reference to an icon_state that we use to swap data
     * between the nodes. */
    icon_state *temp_data;

    /* This keeps track of the node reference we're currently looking through in the list. */
    node* new_node;
    unsigned long long default_length;

    for (new_node = list_head(&table->table[hash_value]); new_node != NULL; new_node = list_next(new_node)) {

        /* If we determine this is a match.*/
        if(match_icon_state3(state->state, state->movement, new_node->data) != 0){

            /* Then we store the node's data. */
            temp_data = new_node->data;

            /* Set the node's data to the state that we were trying to insert. */
            new_node->data = state;
            default_length = strlen(temp_data->state);

            int count = 1;
            snprintf(temp_data->state, default_length+30, "%s_%d", temp_data->state, count);
            while(iconstatelookup(table, temp_data->state, temp_data->movement) != NULL){
                count++;
                snprintf(temp_data->state, default_length+30, "%s_%d", state->state, count);

            }

            int new_hash = hash_icon_state2(temp_data->state) % table->buckets;
            //node* insert_node = (node*)malloc(sizeof(node));
           // insert_node->data = temp_data;
            list *tlist = &table->table[new_hash];
            list_ins_next(tlist, NULL, temp_data);
          //  table->buckets++;

            break;
        }
    }

    if(new_node == NULL){
        list_ins_next(list_bucket, NULL, state);
       // table->buckets++;
    }
    return 0;
}


icon_state *state_look_up(hash_table *table, char *state_name, bool is_movement) {
    IconSearchParams *params = create_search_params(state_name, is_movement);
    if (!params) {
        return NULL;
    }
    icon_state* result = (icon_state*)chtbl_lookup(table, (void **) &params);
    free(params);
    return result;
}


int state_insert(hash_table *table, icon_state* state) {

    if(state_look_up(table, state->state, state->movement) != NULL){
        return -1;
    }

    IconSearchParams *params = create_search_params(state->state, state->movement);
    if (!params) {
        return -1;
    }
    int retval;
    int bucket = (int)(table->hash_function(params) % table->buckets);
    if ((retval = list_ins_next(&table->table[bucket], NULL, state)) == 0)
        table->size++;

    free(params);
    return retval;
}



void Add_Delay(icon_state* thisNode, int *delay) {
    if(thisNode->delays )
        thisNode->delays = delay;
}

void Add_Loop(icon_state* thisNode, int loop) {
    thisNode->loop = loop;
}

void Add_Frames(icon_state* thisNode, int frames) {
    thisNode->number_of_frames = frames;
}

void Add_Rewind(icon_state* thisNode) {
    thisNode->rewind = true;
}

void Add_Movement(icon_state* thisNode) {
    thisNode->movement = true;
}

void Add_Dir(icon_state* thisNode, int dirs) {
    thisNode->dirs = dirs;
}



void Initialize_IconState(icon_state* thisNode, char* name) {
   // printf("Name is %s\n", name);
    thisNode->state = name;
   // printf("Icon State is set to %s\n", thisNode->state);
    thisNode->dirs = 1;
    thisNode->number_of_frames = 1;
    thisNode->loop = 1;
    thisNode->movement = false;
}

void add_image_data(icon_state* thisNode, png_bytepp image_data){
    thisNode->frames = image_data;

}

void IconState_Create(icon_state* thisNode, char * state_name, int directions, int frames, int loop) {
    thisNode->state = (char*)malloc(sizeof(strlen(state_name)));
    strcpy(thisNode->state, state_name);
    thisNode->dirs = directions;
    thisNode->number_of_frames = frames;

    //thisNode->prev = prevLoc;
}



/*
 * void read_and_write_png(const char* input_filename, const char* output_filename) {
    FILE* input_file = fopen(input_filename, "rb");
    if(!input_file) {
        printf("Can't open file %s for reading\n", input_filename);
        return;
    }

    // Initialize the reading structures.
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop read_info_ptr = png_create_info_struct(read_ptr);
    png_init_io(read_ptr, input_file);
    png_read_info(read_ptr, read_info_ptr);

    png_uint_32 height = png_get_image_height(read_ptr, read_info_ptr);
    png_bytepp row_pointers = (png_bytepp)malloc(sizeof(png_bytep) * height);

    for (png_uint_32 i = 0; i < height; i++) {
        row_pointers[i] = (png_bytep)malloc(png_get_rowbytes(read_ptr, read_info_ptr));
    }

    png_read_image(read_ptr, row_pointers);

    // Now that we've read the image, let's write it to another file.
    FILE* output_file = fopen(output_filename, "wb");
    if(!output_file) {
        printf("Can't open file %s for writing\n", output_filename);
        return;
    }

    // Initialize the writing structures.
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop write_info_ptr = png_create_info_struct(write_ptr);

    // Set up the output.
    png_init_io(write_ptr, output_file);

    // Copy the image data from the read structures to the write structures.
    png_set_IHDR(write_ptr, write_info_ptr, png_get_image_width(read_ptr, read_info_ptr),
                 png_get_image_height(read_ptr, read_info_ptr), png_get_bit_depth(read_ptr, read_info_ptr),
                 png_get_color_type(read_ptr, read_info_ptr), png_get_interlace_type(read_ptr, read_info_ptr),
                 png_get_compression_type(read_ptr, read_info_ptr), png_get_filter_type(read_ptr, read_info_ptr));

    png_colorp palette;
    int num_palette;

    if (png_get_PLTE(read_ptr, read_info_ptr, &palette, &num_palette)) {
        png_set_PLTE(write_ptr, write_info_ptr, palette, num_palette);
    }

    png_bytep trans_alpha = NULL;
    int num_trans = 0;
    png_color_16p trans_color = NULL;

    //if (png_get_tRNS(read_ptr, read_info_ptr, &trans_alpha, &num_trans, &trans_color)) {
    //   png_set_tRNS(write_ptr, write_info_ptr, trans_alpha, num_trans, trans_color);
  // }

    printf("Reading image now. . \n");
    sleep(5);
    output_pixel_values(read_ptr, read_info_ptr, row_pointers);
    printf("Done!!");

    // Set up the data in the write structure.
    png_set_rows(write_ptr, write_info_ptr, row_pointers);

    // Finally, write the image to the file.
    png_write_png(write_ptr, write_info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    // Clean up.
    fclose(input_file);
    fclose(output_file);

    png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
    png_destroy_write_struct(&write_ptr, &write_info_ptr);

    // Free the memory associated with row_pointers
    for (png_uint_32 i = 0; i < height; i++) {
        free(row_pointers[i]);
    }
    free(row_pointers);
}

 * */