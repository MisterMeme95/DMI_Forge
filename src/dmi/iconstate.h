#ifndef DMI_STRUCT_C_DATA_STRUCTURE_H
#define DMI_STRUCT_C_DATA_STRUCTURE_H
#endif

#ifndef DMI_STRUCT_C_LIST_H
#include "../data_structures/list.h"
#endif

#ifndef DMI_STRUCT_C_VECTOR_H
#include "../data_structures/vector.h"
#endif

#include <stdbool.h>
#include "png.h"
#include "../data_structures/data_structure.h"


#ifdef __cplusplus
extern "C"{
#endif

typedef struct IconSearchParams {
    char *state_name;
    bool is_movement;
} IconSearchParams;

/**
 * @Description
 * The icon_state structure represents a unique animation state within BYOND,
 * encapsulating all the necessary data to define how a specific animation is rendered and behaves.
 *
 * @param state
 *   The name of the animation state (e.g., "idle", "running", "attacking").
 *
 * @param dirs
 *   The number of directional frames available for this state. Possible values: [1, 4, 8]
 *
 * @param number_of_frames
 *   Number of frames for each direction.
 *
 * @param delays
 *   An array representing the delay (in deciseconds) between frames.
 *
 * @param movement
 *   Indicates whether the state corresponds to a movement-related animation (e.g., walking or running).
 *
 * @param rewind
 *   Determines if the animation should rewind after completion, playing frames in reverse order.
 *
 * @param loop
 *   Specifies if the animation should loop continuously upon reaching the final frame.
 *
 * @param frames
 *   A pointer to an array of image data representing the individual frames of the animation.
 *
 * @param frame_vector
 *   A vector container that stores additional frame information or references, allowing dynamic
 *   resizing and manipulation of frame data.
 */
typedef struct IconState_Struct {
    char *state;
    int dirs, number_of_frames;
    int *delays;
    bool movement, rewind, loop;
    png_bytepp frames;
    Vector *frame_vector;
} icon_state;

void Add_Delay(icon_state *thisNode, int *delay);

void Add_Loop(icon_state *thisNode, int loop);

void Add_Frames(icon_state *thisNode, int frames);

void Add_Rewind(icon_state *thisNode);

void Add_Movement(icon_state *thisNode);

void Add_Dir(icon_state *thisNode, int dirs);

void Initialize_IconState(icon_state *thisNode, char *name);

int match_icon_state2(char *state_name, const icon_state *state, bool is_movement);

void IconState_Create(icon_state *thisNode, char *state_name, int directions, int frames, int loop);

void add_image_data(icon_state *thisNode, png_bytepp image_data);

int match_icon_state(IconSearchParams *params, icon_state *state_node);

unsigned long hash_icon_state(IconSearchParams* params);
icon_state *state_look_up(hash_table *table, char *state_name, bool is_movement);
int state_insert(hash_table *table, icon_state* state);
IconSearchParams *create_search_params(char *state_name, int is_movement);

#ifdef __cplusplus
}
#endif