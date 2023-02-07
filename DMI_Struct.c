/* The DMI structure will be used to contain all relevant information associated with a DMI.
 * The variables will be:
 *
 * - version:       The version of BYOND it should use. Either 4.0 (most current DMI format) or 3.5.
 * - width:         The width for each frame.
 * - height:        The height for each frame.
 * - icon_states:   This should be a vector containing a list of all icon_states.
 * */

#include "Icon_state.c"

typedef struct DMI_Struct {
    double version;
    int width, height;
    icon_state *icon_states;
}DMI;
