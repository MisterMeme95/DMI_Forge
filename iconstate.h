#include <stdbool.h>


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
typedef struct IconState_Struct {
    char *state;
    int dirs, frames;
    int *delays;
    bool movement, rewind, loop;
} icon_state;

void Add_Delay(icon_state* thisNode, int *delay);
void Add_Loop(icon_state* thisNode, int loop);
void Add_Frames(icon_state* thisNode, int frames);
void Add_Rewind(icon_state* thisNode);
void Add_Movement(icon_state* thisNode);

void Add_Dir(icon_state* thisNode, int dirs);

void Initialize_IconState(icon_state* thisNode, char* name);

void IconState_Create(icon_state* thisNode, char * state_name, int directions, int frames, int loop);