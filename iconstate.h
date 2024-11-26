#ifndef DMI_STRUCT_C_DATA_STRUCTURE_H
#define DMI_STRUCT_C_DATA_STRUCTURE_H
#endif

#ifndef DMI_STRUCT_C_LIST_H
#include "list.h"
#endif

#ifndef DMI_STRUCT_C_VECTOR_H
#include "vector.h"
#endif

#include <stdbool.h>
#include "png.h"

/** @Description icon_state is a data structure that we use to record all of the relevant information for a unique
 * animation state in BYOND.
 *
 * @state A string representing the name of the icon_state.
 * @dirs Number of directions in the icon_state. Must be 1, 4, or 8.
 * @frames Number of frames per direction.
 * @delays A list of integer representing the detail for a frame. Size of list is equal to "frames" value.
 * @movement It is set to one if the icon_state is a movement state.
 * @rewind If the animation should rewind.
 * @loop If the animation should loop.
 **/

/**
 * TO DO: Make a doubly linked_list structure to represent the frames.
 * prev/next does what you expect.
 *
 * the data will be a png_bytepp* that points to the beginning of a frame for each direction.
 *
 * png_bytepp = (png_bytepp*)malloc(sizeof(png_bytepp) * frames);
 * for(int i = 0; i < dirs; i ++)
 *  frames = frames.next
 * **/
typedef struct IconState_Struct {
    char *state;
    int dirs, number_of_frames;
    int *delays;
    bool movement, rewind, loop;
    png_bytepp frames;
    Vector *frame_vector;
} icon_state;


/*
 *
 * png_bytepp frames = (*png_bytepp) malloc(sizeof(png_bytepp) * 8)
 * for(int i = 0; i < 8; i++)
 *  frames[i] = (png_bytepp) malloc(sizeof(png_bytep) * num_of_frames)
 *  for (int o = 0; o < number of frame; o++)
 *      frames[i][o] = get_proper_frame_location()
 *
 *  FUTURE CHANGE: make it a list of vectors rather.
 *      * So it will be a dynamically allocated array of 1, 4, or 8 vectors of png_bytepp.
 *      So instead it should be:
 *          vector *frames = (*vector) malloc(sizeof(vector) * num_of_directions);
 *          for(int i = 0; i < num_of_directions; i++)
 *              initialize_vector(&frames[i],
 *
 *              vector_init(&frames[i], sizeof(png_bytepp), number_of_frames,
                 void PUSH_BACK_FUNCTION,
                 void POP_FUNCTION);
 *
 * */

void Add_Delay(icon_state* thisNode, int *delay);
void Add_Loop(icon_state* thisNode, int loop);
void Add_Frames(icon_state* thisNode, int frames);
void Add_Rewind(icon_state* thisNode);
void Add_Movement(icon_state* thisNode);

void Add_Dir(icon_state* thisNode, int dirs);

void Initialize_IconState(icon_state* thisNode, char* name  );

void IconState_Create(icon_state* thisNode, char * state_name, int directions, int frames, int loop);

void add_image_data(icon_state* thisNode, png_bytepp image_data);