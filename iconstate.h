#include <stdbool.h>

typedef struct IconState_Struct {
    char *state;
    int dirs, frames, loop;
    int *delay;
    bool movement, rewind;
} icon_state;

void Add_Delay(icon_state* thisNode, int *delay);

void Add_Loop(icon_state* thisNode, int loop);

void Add_Frames(icon_state* thisNode, int frames);

void Add_Rewind(icon_state* thisNode);

void Add_Movement(icon_state* thisNode);

void Add_Dir(icon_state* thisNode, int dirs);

void Initialize_IconState(icon_state* thisNode, char* name);

void IconState_Create(icon_state* thisNode, char * state_name, int directions, int frames, int loop);