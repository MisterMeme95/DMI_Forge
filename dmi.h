#define BEGIN_DMI "# BEGIN DMI"
#define END_DMI "# END DMI"
#define DMI_VERSION "version"
#define ICON_STATE "state"
#define DMI_WIDT "width"

#include "iconstate.h"
#include "png.h"

typedef struct DMI_Struct {
    bool has_icons;
    double version;
    int width, height, num_of_states;
    icon_state *icon_states, *begin_icon_state;
}DMI;

int DMI_To_Png(DMI* dmi, int pngWidth, int pngHeight, png_bytepp orig_pointer, png_bytepp new_pointer,
               png_structp png_ptr, png_infop info_ptr, int ppb, int color_type);
int PNG_To_DMI();

void Init_DMI(DMI* dmi, int width, int height);

png_uint_32 Get_Sheet_Size(DMI* dmi);
png_uint_32 Get_Sheet_Width(DMI* dmi);
char *Variable_Authentication(char *string);
char *State_Authentication(char *string);
char *Value_Authentication(char *string);

int initialize_image(png_structp *png_ptr, png_infop *png_info, FILE **fp);
int check_if_png(char *file_name, FILE **fp);
void output_pixel_values(png_structp png_ptr, png_infop info_ptr, png_bytep *row_pointers);
void Print_Variable(char *string, DMI* dmi);
char *find_newline(char **string, int *dmi_index, char *search_for);
int get_string_char(char *string, char* char_search, char doo);