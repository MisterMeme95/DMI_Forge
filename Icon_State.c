#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
typedef struct IconState_Struct {
    char *state;
    int dirs, frames, loop;
    int *delay;
    bool movement, rewind;

    //Add chunks of data that represent images of frames as well.
} icon_state;

void Initialize_IconState(icon_state* thisNode){
    thisNode->state = NULL;
    thisNode->dirs = 1;
    thisNode->frames = 1;
    thisNode->loop = 1;


}

void IconState_Create(icon_state* thisNode, char * state_name, int directions, int frames, int loop) {
    thisNode->state = (char*)malloc(sizeof(strlen(state_name)));
    strcpy(thisNode->state, state_name);
    thisNode->dirs = directions;
    thisNode->frames = frames;

    //thisNode->prev = prevLoc;
}