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
 **/
typedef struct IconState_Struct {
    char *state;
    int dirs, number_of_frames;
    int *delays;
    bool movement, rewind, loop;
    png_bytepp frames;
    Vector *frame_vector;
} icon_state;

void Add_Delay(icon_state* thisNode, int *delay);
void Add_Loop(icon_state* thisNode, int loop);
void Add_Frames(icon_state* thisNode, int frames);
void Add_Rewind(icon_state* thisNode);
void Add_Movement(icon_state* thisNode);

void Add_Dir(icon_state* thisNode, int dirs);

void Initialize_IconState(icon_state* thisNode, char* name  );

void IconState_Create(icon_state* thisNode, char * state_name, int directions, int frames, int loop);

void add_image_data(icon_state* thisNode, png_bytepp image_data);
int match_icon_state(const void *key1, const void *key2);
unsigned long hash_icon_state(const icon_state *key);