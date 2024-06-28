#define BEGIN_DMI "# BEGIN DMI"
#define END_DMI "# END DMI"
#define DMI_VERSION "version"
#define ICON_STATE "state"
#define DMI_WIDT "width"
#define HORIZONTAL_FLOW 0
#define LINEAR_FLOW 1
#define GRIDLOCK_FLOW 2
#define HORIZONTAL 0
#define GRID 1
#define PNG_BYTES_TO_CHECK 8
#ifndef dmi_h
#define dmi_h

#endif
#include "iconstate.h"
#include "png.h"

typedef struct DMI_Struct {
    bool has_icons;
    double version;
    int width, height, num_of_states, max_state;
    icon_state *icon_states, *begin_icon_state;
}DMI;

typedef struct Image_Struct{
    png_structp png_ptr;
    png_infop info_ptr;
    png_colorp palette;
    png_bytep trans_alpha;
    png_color_16p trans_color;

    int trans_num;
    int palette_num;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, pixels_per_byte;
    png_bytepp pixel_array;
    FILE *file_pointer;
} Image;



typedef struct SpriteSheetData{
    int FRAME_SIZE, width, height, format;
    int offset_x, offset_y, margin_x, margin_y, frames_per_row, frames_per_col;
    int user_input_width, user_input_height;
    int *list_of_row_sizes;
} SpriteSheetData;

/** @Description This function initializes SpriteSheetData with the proper dimension information for the GRID
 * spritesheet format. It also populates frames_per_row, and frames_per_col for use in other function
 * @members dmi: This is the struct containing all of the data needed for a DMI.
 * @members sheet_data:  This is a pointer to our struct containing all of the sheet data.
 * @members state_per_row: This is an input provided by the user that specifies how many icon_states
 * a user may want to be listed per row. */
void calculate_grid_sheet_dimensions(DMI* dmi, SpriteSheetData* sheet_data, int icon_states_per_row);
int create_sprite_sheet(Image* initial_image, SpriteSheetData* sheet_data, DMI new_icon);
int dmiToPng(DMI* dmi, int pngWidth, int pngHeight, png_bytepp orig_pointer, png_bytepp new_pointer,
             png_structp png_ptr, png_infop info_ptr, int ppb, int color_type, int output_flow_type,
             int input_flow_type);

void Get_Gridlock_Size(DMI *dmi, int *height, int *width);
void Resize_IconStates(DMI* dmi, int new_size);
int PNG_To_DMI();

int Get_Dest_Col(icon_state *state, int DMI_WIDTH, int iteration);
int Get_Dest_Row(icon_state* state, int DMI_HEIGHT);
void Fix_Dimension(int *dest_col, int *dest_row, int *source_col, int *source_row, int *copy_row, int *copy_col,
                   int start_row, int start_col, int *frame_tracker,int input_flow,int output_flow, int DMI_WIDTH,
                   int DMI_HEIGHT, int pngWidth, int total_frame,int outwidth, DMI *dmi);
void Init_DMI(DMI* dmi, int width, int height);

png_uint_32 Get_Sheet_Size(DMI* dmi);
png_uint_32 Get_Sheet_Width(DMI* dmi);
char *Variable_Authentication(char *string);
char *State_Authentication(char *string);
char *Value_Authentication(char *string);
void Get_Frame(png_bytepp dest_pixels, png_bytepp src_pixels, int dest_start_row,int dest_start_col,
               int src_start_row, int src_start_col, int bytes_per_frame, int cols_to_copy);
int initialize_image(png_structp *png_ptr, png_infop *png_info, FILE **fp);
int check_if_png(char *file_name, FILE **fp);
void output_pixel_values(png_structp png_ptr, png_infop info_ptr, png_bytep *row_pointers);
void Print_Variable(char *string, DMI* dmi);
char *find_newline(char **string, int *dmi_index, char *search_for);
int get_string_char(char *string, char* char_search, char doo);