#include <unistd.h>
#include <stdlib.h>
#include "png.h"
#include "dmi.h"
#include <getopt.h>

#define INVALID -100

void print_icon_states(list *iconStates) {
    if (!iconStates || !iconStates->head) {
        printf("The list is empty or NULL.\n");
        return;
    }

    node *current = iconStates->head;

    // Iterate through the linked list
    while (current != NULL) {
        // Cast the `data` field to `icon_state*`
        icon_state *iconStateData = (icon_state *)current->data;
        icon_state *tail = (icon_state*)iconStates->tail->data;

        // Ensure the casted pointer is valid before accessing
        if (iconStateData != NULL) {
            // Print the `state` field of `icon_state`
            printf("Icon State: %s\n", iconStateData->state);
        } else {
            printf("Null data in node.\n");
        }

        // Move to the next node
        current = current->next;
    }
}
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

// Function to get the head of the linked list
icon_state* get_head_of_iconStates(list* iconStates) {
    if (!iconStates || !iconStates->head) {
        fprintf(stderr, "Error: iconStates is empty.\n");
        return NULL;
    }

    return (icon_state*)iconStates->head->data; // Assuming head points to the first icon_state
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



    DMI *new_icon = (DMI*) malloc(sizeof(DMI));
    initialize_dmi_struct(new_icon, input_name);


    if(fflag == 0) {
        if (access(output_name, F_OK) == 0) {
            char overwrite;
            printf("Warning: %s already exists!\nDo you wish to overwrite it? (Y/N): ", output_name);
            fflush(stdout);
            do {
                scanf(" %c", &overwrite);
                while (getchar() != '\n');

                if (overwrite == 'Y' || overwrite == 'y' || overwrite == 'N' || overwrite == 'n') {
                    break;
                } else {
                    printf("Invalid input. Please enter 'Y' or 'N': ");
                    fflush(stdout);
                }
            } while (1);

            if (overwrite != 'Y' && overwrite != 'y') {
                int count = 1;
                char new_string[256];
                char base_string[256];
                bool found_match = false;
                for (int i = strlen(output_name) - 1; i >= 0; i--) {
                    if (output_name[i] == '.') {
                        strncpy(base_string, output_name, i);
                        base_string[i] = '\0';
                        break;
                    }
                }

                while (!found_match) {
                    sprintf(new_string, "%s (%d).png", base_string, count);
                    if (access(new_string, F_OK) != 0) {
                        output_name = realloc(output_name, strlen(new_string) + 1);
                        sprintf(output_name, "%s", new_string);
                        found_match = true;
                    }
                    count++;
                }
                printf("File will be saved as: %s\n", new_string);
            }
        }
    }
  //  create_png_from_icon_state(new_icon, "testicon.png");

   // adjust_icon_state(new_icon, GET_TAIL_ICONSTATE(new_icon));


    sheet_data.margin_x = 0;
    sheet_data.margin_y = 0;
    sheet_data.padding_y = 0;
    sheet_data.padding_x = 0;




    Image sprite_sheet = create_sprite_sheet(&sheet_data, *new_icon, output_name);
    for(int i = 0; i < sprite_sheet.height; i++){
        for(int j = 0; j < png_get_rowbytes(sprite_sheet.png_ptr, sprite_sheet.info_ptr); j++){
            sprite_sheet.pixel_array[i][j] = 0;
        }
    }

    dmi2sheet(new_icon, sprite_sheet, sheet_data);

    if (new_icon->image->color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(new_icon->image->png_ptr, new_icon->image->info_ptr, &new_icon->image->palette,
                     &new_icon->image->palette_num);
        png_set_PLTE(sprite_sheet.png_ptr, sprite_sheet.info_ptr, new_icon->image->palette, new_icon->image->palette_num);

        png_bytep trans_alpha = NULL;
        int num_trans = 0;
        png_color_16p trans_color = NULL;

        if (png_get_valid(new_icon->image->png_ptr, new_icon->image->info_ptr, PNG_INFO_tRNS)) {
            png_get_tRNS(new_icon->image->png_ptr, new_icon->image->info_ptr, &trans_alpha, &num_trans, &trans_color);
            png_set_tRNS(sprite_sheet.png_ptr, sprite_sheet.info_ptr, trans_alpha, num_trans, trans_color);
        }
    }
    png_write_info(sprite_sheet.png_ptr, sprite_sheet.info_ptr);
    png_write_image(sprite_sheet.png_ptr, sprite_sheet.pixel_array);

    png_write_end(sprite_sheet.png_ptr, NULL);
    png_destroy_write_struct(&sprite_sheet.png_ptr, &sprite_sheet.info_ptr);
  //  png_destroy_read_struct(&read_png_ptr, &read_info_ptr);
    print_icon_states(&new_icon->iconStates);

    return EXIT_SUCCESS;
}
