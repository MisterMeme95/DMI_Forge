#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "iconstate.h"

void Add_Delay(icon_state* thisNode, int *delay) {
    thisNode->delay = delay;
}

    void Add_Loop(icon_state* thisNode, int loop) {
    thisNode->loop = loop;
}

void Add_Frames(icon_state* thisNode, int frames) {
    thisNode->frames = frames;
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
 //   printf("Icon State is set to %s\n", thisNode->state);
    thisNode->dirs = 1;
    thisNode->frames = 1;
    thisNode->loop = 1;
    thisNode->movement = false;
}

void IconState_Create(icon_state* thisNode, char * state_name, int directions, int frames, int loop) {
    thisNode->state = (char*)malloc(sizeof(strlen(state_name)));
    strcpy(thisNode->state, state_name);
    thisNode->dirs = directions;
    thisNode->frames = frames;

    //thisNode->prev = prevLoc;
}