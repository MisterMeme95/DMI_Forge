#include <unistd.h>
#include <stdlib.h>
#include "png.h"
#include "dmi.h"
#include <getopt.h>
#include <omp.h>

#define STDERR stdout
#define INVALID -100
#include <time.h>

void changeExtension(char* filename, const char* newExt) {
    char* extPos = strstr(filename, ".dmi");

    if (extPos) {
        strcpy(extPos, newExt);  // Replace with new extension.
    }
}

int validate_layout(char* string){
    if(strcmp("grid", string) == 0 || strcmp("1", string) == 0){
        return GRID;
    }
    else if(strcmp("horizontal", string) == 0 || strcmp("2", string) == 0) {
        return HORIZONTAL;
    }
    else if(strcmp("linear", string) == 0 || strcmp("0", string) == 0){
        return HORIZONTAL;
    }
    else
        return INVALID;
}

void print_usage() {
    printf(""
           "-l, --layout         The format that the sprite-sheet will be represented in. So far there are only two "
           "                     values: grid, horizontal.\n"

            "-w, --width         The desired width of the resultant sprite-sheet. If the provided value is insufficient "
            "                    the program will automatically set the width to the minimal needed to contain the pixels.\n"

            "-h, --height        The desired height of the resultant sprite-sheet. If the provided value is insufficient "
            "                    the program will automatically set the width to the minimal needed to contain the pixels.\n"

            "-r, --rows          Num of Rows in sprite sheet.\n"
            "-g, --gridsize      This specifies the number of icon_states that are printed out per row."

            "-c, --cols          Num of cols in sprite sheet produced.\n"

            "-p, --spacing       The amount of padding between each frame.\n"

            "-i, --icon_state    Specify the icon_state(s) that should be extracted. The list uses a comma (,) as the"
            "                    delimiter.\n"

            "-n, --name          A boolean flag determining if the name of the icon_states should be listed in the"
            "                    sprite-sheet.\n"

            "-m, --margin        The amount of space before the extraction of the frames may begin.\n"

            "-b, --background    Set background color for the sprite sheet. Only valid for non-RGBA sprites.\n"

            "-o, --overwrite     Set overwrite settings. [Yes, no, ask.]\n"

            "-f, --force         Ignores all prompts, so causing overwrite will go through automatically, no prompt "
            "                    for changing size, etc.\n"
            "-m, --metadata      Copy to a dmt file.\n"
            "-d, --directory     Where to send output file. By default, it will be relative path.\n"
    );
}


int main(int argc, char **argv){
    SpriteSheetData sheet_data;
    initialize_sheet_data(&sheet_data);
    char *input_name = NULL;
    char *output_name = NULL;

    static char usage[] = "Usage: dmi2sheet [OPTIONS]. . .[INPUT_FILE] [OUTPUT_FILE]\nTransforms a DMI input"
                          " file into a sprite sheet.\n\n";

    static int fflag = 0;
    static struct option long_options[] =
            {
                   {"overwrite",     required_argument,       0, 201},
                   {"output",     required_argument,       0, 'o'},
                   {"input",     required_argument,       0, 'i'},
                   {"exclude",  required_argument, 0, 'e'},
                   {"force",    no_argument, 0, 'f'},
                   {"layout", required_argument, 0, 200},
                   {"margins", required_argument, 0, 'm'},
                   {"help", required_argument, 0, 'm'},
                   {"width", required_argument, 0, 'w'},
                   {"height", required_argument, 0, 'h'}
            };


        int opt;
        while ((opt = getopt_long(argc, argv, "i:o:b:c:h:w:g:", long_options, NULL)) != -1) {
            switch (opt) {
                case 'i':
                    input_name = strdup(optarg);
                    break;

                case 'o':
                    output_name = strdup(optarg);
                    break;

                case 'b':
                    break;

                case 'c':
                    break;

                case 'h':
                    sheet_data.user_input_height = atoi(optarg);
                    break;

                case 'w':
                    sheet_data.user_input_width = atoi(optarg);
                    break;

                case 'f':
                    break;
                case 'g':
                    if(sheet_data.format == GRID){
                        sheet_data.grid_size = atoi(optarg);
                    }

                    else
                        sheet_data.grid_size = 0;
                    /** TO DO:
                     * make this send a warning to the client if their format is not in GRID.
                     * This way they know of any potential error.
                     * */
                    break;
                case 200:
                    sheet_data.format = validate_layout(optarg);
                    if(sheet_data.format != GRID){
                        sheet_data.grid_size = 0;
                    }
                    break;
                default:
                    //fprintf(stderr, "Invalid option\n");
                    //print_usage();
                    return EXIT_FAILURE;
            }
        }

    if(input_name == NULL){
        printf("The command MUST have an input file specified!\nProgram ends in failure. . .\n");
        return 0;
    }

    if(output_name == NULL){
        output_name = malloc(sizeof(char) * strlen(input_name) + 8);
        sprintf(output_name, "output_%s", input_name);
        changeExtension(output_name, ".png");
    }

    Image image = load_image(input_name);
    switch (image.bit_depth) {
        case 1:
            image.pixels_per_byte = 8;
            break;
        case 2:
            image.pixels_per_byte = 4;
            break;
        case 4:
            image.pixels_per_byte = 2;
            break;

        default:
            image.pixels_per_byte = 1;
            break;
    }

    png_textp text_ptr;
    int num_text;
    if (png_get_text(image.png_ptr, image.info_ptr, &text_ptr, &num_text) > 0) {
        png_uint_32 i;
       // fprintf(STDERR,"\n");
        for (i=0; i<num_text; i++){
          //  fprintf(STDERR,"Text compression[%d]=%d\n",
                 //   i, text_ptr[i].compression);
        }
    }
    DMI *new_icon = (DMI*) malloc(sizeof(DMI));
    Init_DMI(new_icon, 32, 32);

    char *dmi_check = text_ptr->text;
    char *string_parser;// = find_newline(dmi_check);

    if(!(dmi_check = strstr(dmi_check, BEGIN_DMI))){
        printf("There is no starting DMI token!\n");

        return 0;
    }
    int dmi_length = strlen(dmi_check);

    if(dmi_length <= 0){
        printf("There is no text to parse in this image!\n\n");
        return 0;
    }

    while(dmi_length > 0){
        /* - First I need to skip all zTxt and go to the part where it BEGIN_DMI token is found. */
        string_parser = find_newline(&dmi_check, &dmi_length, "\n");
        Print_Variable(string_parser, new_icon);
    }

//    if(fflag == 0) {
//        if (access(output_name, F_OK) == 0) {
//            char overwrite;
//            printf("Warning: %s already exists!\nDo you wish to overwrite it? (Y/N): ", output_name);
//            fflush(stdout);
//            do {
//                scanf(" %c", &overwrite); // space before %c consumes any whitespace characters, including 'enter'
//                while (getchar() != '\n'); // clean the buffer
//
//                if (overwrite == 'Y' || overwrite == 'y' || overwrite == 'N' || overwrite == 'n') {
//                    break; // Exit the loop if a valid input is given
//                } else {
//                    printf("Invalid input. Please enter 'Y' or 'N': ");
//                    fflush(stdout);
//                }
//            } while (1); // Keep looping until a valid input is provided
//
//            if (overwrite != 'Y' && oveQQrwrite != 'y') {
//                int count = 1;
//                char new_string[256];
//                char base_string[256];
//                bool found_match = false;
//                for (int i = strlen(output_name) - 1; i >= 0; i--) {
//                    if (output_name[i] == '.') {
//                        strncpy(base_string, output_name, i);
//                        base_string[i] = '\0';
//                        break;
//                    }
//                }
//
//                while (!found_match) {
//                    sprintf(new_string, "%s (%d).png", base_string, count);
//                    if (access(new_string, F_OK) != 0) {
//                        output_name = realloc(output_name, strlen(new_string) + 1);
//                        sprintf(output_name, "%s", new_string);
//                        found_match = true;
//                    }
//                    count++;
//                }
//                printf("File will be saved as: %s\n", new_string);
//            }
//        }
//    }

   // sheet_data.format = GRID;
    sheet_data.margin_x = 32;
    sheet_data.margin_y = 32;
    sheet_data.padding_y = 0;
    sheet_data.padding_x = 0;

    Image sprite_sheet = create_sprite_sheet(&image, &sheet_data, *new_icon, output_name);
    for(int i = 0; i < sprite_sheet.height; i++){
        for(int j = 0; j < png_get_rowbytes(sprite_sheet.png_ptr, sprite_sheet.info_ptr); j++){
            sprite_sheet.pixel_array[i][j] = 0;
        }
    }

    dmi2sheet(new_icon, image, sprite_sheet, sheet_data);
   // dmi2sheet2(new_icon, image, sprite_sheet, sheet_data);

    if (image.color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(image.png_ptr, image.info_ptr, &image.palette, &image.palette_num);
        png_set_PLTE(sprite_sheet.png_ptr, sprite_sheet.info_ptr, image.palette, image.palette_num);
    }
    png_write_info(sprite_sheet.png_ptr, sprite_sheet.info_ptr);
    png_write_image(sprite_sheet.png_ptr, sprite_sheet.pixel_array);

    png_write_end(sprite_sheet.png_ptr, NULL);
    //png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    //png_destroy_read_struct(&read_png_ptr, &read_info_ptr);
    return EXIT_SUCCESS;
}
