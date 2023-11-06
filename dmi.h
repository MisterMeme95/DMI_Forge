#define BEGIN_DMI "# BEGIN DMI"
#define END_DMI "# END DMI"
#define DMI_VERSION "version"
#define ICON_STATE "state"
#define DMI_WIDT "width"
#define HORIZONTAL_FLOW 0
#define LINEAR_FLOW 1
#define GRIDLOCK_FLOW 2

#include "iconstate.h"
#include "png.h"

typedef struct DMI_Struct {
    bool has_icons;
    double version;
    int width, height, num_of_states, max_state;
    icon_state *icon_states, *begin_icon_state;
}DMI;

int DMI_To_Png(DMI* dmi, int pngWidth, int pngHeight, png_bytepp orig_pointer, png_bytepp new_pointer,
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