#include <stdio.h>
#include <stdlib.h>
#define BEGIN_DMI "# BEGIN DMI"
#define END_DMI "# END DMI"
#define DMI_VERSION "version"
#define ICON_STATE "state"
#define HORIZONTAL 0
#define GRID 1

#ifndef dmi_h
#define dmi_h
#endif


#ifndef ICONSTATE_H
#define ICONSTATE_H

#include "iconstate.h"
#endif // ICONSTATE_H



#include "png.h"
#include "PixelManip.h"

#ifdef __cplusplus
extern "C" {
#endif
/** @brief This struct represents a basic PNG image.
 * @param png_ptr - A pointer linking to the PNG file being processed. It handles the reading and writing operations.
 * @param png_infop - This contains all of the relevant metadata, like height, bit_depth, etc.
 * @param trans_color - This a png_color value that is used for RGB and Gray images to specify transparency color.
 * @param palette - This is an array of png_color that stores valid entries for a paletted image.
 * @param palette_num - This is the number of entries in the palette array.
 * @param trans_alpha - Trans_alpha is a png_byte array that contains list of alpha pixel indexes for paletted images.
 * @param trans_num - This species the number of transparent pixel entries in a palette.
 * @param width - This represents the width.
 * @param height - This represents the height.
 * @param bit_depth - This represents bit_depth, which is used to specify the size of each color channel.
 * @param color_type - This represents the type of color format for the image. [RGB, RGBA, Gray, Gray & Alpha, Indexed]
 * @param file_pointer - This is the PNG file that is being read or written to.
 * @param pixel_array - This contains the array of pixels that the image is made up of.
 * @param interlace_method - ???
 **/
typedef struct iconstate_node {
    int index;
    icon_state *icon_state;
    struct iconstate_node *next;
    struct iconstate_node *prev;
} iconStateNode;


typedef struct iconstate_lookup_table {
    int MAX_VALUE;
    iconStateNode *hash_bucket[256];
} iconstate_hash;

typedef struct DMI_Struct {

    int bit_depth;
    int color_type;
    double bytes_per_pixel;

    char *name;
    bool has_icons;
    double version;
    int width, height, num_of_states, max_state;
    icon_state *icon_states, *begin_icon_state;

    /* Hash Table of icon_states*/
    iconstate_hash iconstateHash;
} DMI;


typedef struct FrameExtractHandler {
    png_uint_32 starting_column;
    png_uint_32 starting_row;

    png_uint_32 current_column;
    png_uint_32 current_row;

    png_uint_32 column_offset;
    png_uint_32 row_offset;

    png_uint_32 loop_row_start;
    png_uint_32 loop_col_start;

    png_uint_32 column_tracker;
    png_uint_32 row_tracker;

} FrameExtractor;

void add_iconstate(DMI* dmi, icon_state *iconState, png_bytepp image_data);
void Resize_IconStates(DMI *dmi, int new_size);
void create_dmi(DMI* dmi);
int PNG_To_DMI();

void Init_DMI(DMI *dmi, int width, int height);

char *Variable_Authentication(char *string);

char *State_Authentication(char *string);

char *Value_Authentication(char *string);

void Print_Variable(char *string, DMI *dmi);

char *find_newline(char **string, int *dmi_index, char *search_for);

int get_string_char(char *string, char *char_search, char doo);


unsigned long hash_string2(const char *str);

int insert_state2(icon_state *iconState,  iconstate_hash *state_lookup, png_bytepp* image_data);

int match_state2(icon_state *name, icon_state *other_name);

icon_state *find_state2(const char *name, iconstate_hash *state_lookup, icon_state find_state);

typedef struct SpriteSheetData {
    int FRAME_SIZE, width, height;

    int format;

    int offset_x, offset_y; /*!< These variables are used to denote the x/y offset from the top of image.*/

    int padding_x, padding_y; /*!< These variables represent the space between each individual frame within an icon_state.*/

    int margin_x, margin_y; /*!< These variables represent the space between icon_states. */

    int frames_per_row, frames_per_col;
    int user_input_width, user_input_height;
    int *list_of_row_sizes;
    int row_count;
    int grid_size;
    int frame_width;
    int frame_height;

} SpriteSheetData;

/** @Description This function initializes SpriteSheetData with the proper dimension information for the GRID
 * spritesheet format. It also populates frames_per_row, and frames_per_col for use in other function
 * @members dmi: This is the struct containing all of the data needed for a DMI.
 * @members sheet_data:  This is a pointer to our struct containing all of the sheet data.
 * @members state_per_row: This is an input provided by the user that specifies how many icon_states
 * a user may want to be listed per row. */
void calculate_grid_sheet_dimensions(DMI *dmi, SpriteSheetData *sheet_data, int icon_states_per_row);

/** @Description A simple initialization function for SpriteSheetData. It basically sets all relevant values to 0 just in case
 * users don't specify a value for important variables. */
void initialize_sheet_data(SpriteSheetData *sheet_data);

Image create_sprite_sheet(Image *initial_image, SpriteSheetData *sheet_data, DMI new_icon, char *file_name);

/** @Description This function handles conversion from a DMI image into a spritesheet. All sprite-sheet specifications,
 * like the format, margins, padding -- are stored in the sheetData argument.
 * */
int dmi2sheet(DMI *dmi, Image source_image, Image sheet_image, SpriteSheetData sheetData);

int dmi2sheet2(DMI *dmi, Image source_image, Image sheet_image, SpriteSheetData sheetData);

#ifdef __cplusplus
}
#endif