#include <unistd.h>
#include <stdlib.h>
#include "png.h"
#include "dmi.h"
#include <getopt.h>


#define STDERR stdout
#define INVALID -100

void changeExtension(char* filename, const char* newExt) {
    char* extPos = strstr(filename, "dmi");
    if (extPos) {
        strcpy(extPos, newExt);  // Replace with new extension.
    }
}

int validate_layout(char* string){
    if(strcmp("grid", string) == 0 || strcmp("1", string) == 0){
        return GRID;
    }
    else if(strcmp("horizontal", string) == 0 || strcmp("0", string) == 0){
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

    png_structp read_png_ptr;
    png_infop read_info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, pixels_per_byte;
    int overwrite_flag = 0;
    char cwd[PATH_MAX];
    int c;

    char *input_name = NULL; //= malloc(sizeof(char) * strlen(argv[optind]) + 1);
    char *output_name = NULL; //= malloc(sizeof(char) * strlen(argv[optind]) + 1);

    static char usage[] = "Usage: dmi2sheet [OPTIONS]. . .[INPUT_FILE] [OUTPUT_FILE]\nTransforms a DMI input"
                          " file into a sprite sheet.\n\n";



    static int fflag = 0;
    int sheet_format = 0;

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

        int option_index = 0;

        int opt;
        while ((opt = getopt_long(argc, argv, "i:o:b:c:h:w", long_options, NULL)) != -1) {
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
                    overwrite_flag = 1;
                    break;

                case 200:
                    sheet_data.format = validate_layout(optarg);
                    break;
                default:
                    //fprintf(stderr, "Invalid option\n");
                    //print_usage();
                    return EXIT_FAILURE;
            }
        }


//    if (optind < argc)
//    {
//        printf ("non-option ARGV-elements: ");
//        int start_value = optind;
//        while(optind < argc){
//            if(optind - start_value == 0){
//                input_name = malloc(sizeof(char) * strlen(argv[optind]) + 1);
//                strcpy(input_name, argv[optind++]);
//            }
//
//            else if(optind - start_value == 1){
//                output_name = malloc(sizeof(char) * strlen(argv[optind]) + 1);
//                strcpy(output_name, argv[optind++]);
//            }
//
//            else {
//                printf ("%s ", argv[optind++])     ;
//            }
//       }
//    }

    if(input_name == NULL){
        printf("The command MUST have an input file specified!\nProgram ends in failure. . .\n");
        return 0;
    }

    if(output_name == NULL){
        output_name = malloc(sizeof(char) * strlen(input_name) + 8);
        sprintf(output_name, "output_%s", input_name);
        changeExtension(output_name, "png");
    }
    printf("\ninput name = %s\noutput_name = %s\n", input_name, output_name);

    /* Check to make sure that the output_name does not already exist. I*/

    FILE *input_fp;

    if(!check_if_png(input_name,&input_fp)){
        printf("The file you are attempting to read is not a valid PNG file!\n");
        return 0;
    }

    if(initialize_image(&read_png_ptr, &read_info_ptr, &input_fp)) {
        printf("Initializing the image failed!\n");
        return 0;
    }

    png_get_IHDR(read_png_ptr, read_info_ptr, &width, &height, &bit_depth,
                 &color_type, &interlace_method, NULL, NULL);

    png_uint_32 num_rows = height;

    switch (bit_depth) {
        case 1:
            pixels_per_byte = 8;
            break;
        case 2:
            pixels_per_byte = 4;
            break;
        case 4:
            pixels_per_byte = 2;
            break;
 //       case 8:
    //        pixels_per_byte = 1;
      //      break;
        default:
            pixels_per_byte = 1;
            break;
    }

    //  png_get_x_offset_pixels()
    png_bytepp row_pointers = (png_bytepp)malloc(sizeof(png_bytep) * num_rows);
    //printf("Row Bytes = %d\nHeight = %d\n", png_get_rowbytes(read_png_ptr, read_info_ptr), height);
    for (png_uint_32 i = 0; i < height; i++) {
        row_pointers[i] = (png_bytep)malloc(png_get_rowbytes(read_png_ptr, read_info_ptr));
    }

    png_read_image(read_png_ptr, row_pointers);


    png_textp text_ptr;
    int num_text;
    if (png_get_text(read_png_ptr, read_info_ptr, &text_ptr, &num_text) > 0) {
        png_uint_32 i;
        fprintf(STDERR,"\n");
        for (i=0; i<num_text; i++){
            fprintf(STDERR,"Text compression[%d]=%d\n",
                    i, text_ptr[i].compression);
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

    if(fflag == 0) {
        if (access(output_name, F_OK) == 0) {
            char overwrite;
            printf("Warning: %s already exists!\nDo you wish to overwrite it? (Y/N): ", output_name);
            fflush(stdout);
            do {
                scanf(" %c", &overwrite); // space before %c consumes any whitespace characters, including 'enter'
                while (getchar() != '\n'); // clean the buffer

                if (overwrite == 'Y' || overwrite == 'y' || overwrite == 'N' || overwrite == 'n') {
                    break; // Exit the loop if a valid input is given
                } else {
                    printf("Invalid input. Please enter 'Y' or 'N': ");
                    fflush(stdout);
                }
            } while (1); // Keep looping until a valid input is provided

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
    FILE *output_fp = fopen(output_name, "wb");
    if (!output_fp) {
        printf("Error opening output file\n");
        return 1;
    }

    png_structp write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!write_png_ptr) {
        printf("Error creating write struct\n");
        fclose(output_fp);
        return 1;
    }

    png_infop write_info_ptr = png_create_info_struct(write_png_ptr);
    if (!write_info_ptr) {
        printf("Error creating write info struct\n");
        png_destroy_write_struct(&write_png_ptr, NULL);
        fclose(output_fp);
        return 1;
    }

    if (setjmp(png_jmpbuf(write_png_ptr))) {
        printf("Error during write\n");
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        fclose(output_fp);
        return 1;
    }

    png_colorp palette;
    int num_palette;
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(read_png_ptr, read_info_ptr, &palette, &num_palette);
        png_set_PLTE(write_png_ptr, write_info_ptr, palette, num_palette);
    }

    png_init_io(write_png_ptr, output_fp);
    png_color_16p trans_color;
    png_bytep trans_alpha;
    int num_trans;
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_tRNS(read_png_ptr, read_info_ptr, &trans_alpha, &num_trans, &trans_color);
        png_set_tRNS(write_png_ptr, write_info_ptr, trans_alpha, num_trans, trans_color);
    }
    int new_height = 0;
    int new_width = 0;
    sheet_data.format = GRID;
    create_sprite_sheet(NULL, &sheet_data, *new_icon);

    //calculate_grid_sheet_dimensions(new_icon, &sheet_data, 3);
    printf("Height - %d\n"
           "Width - %d\n", sheet_data.height, sheet_data.width);

    if(sheet_format == LINEAR_FLOW){
        new_height = (int)height;
        new_width = (int)width;
    }
    else if(sheet_format == HORIZONTAL_FLOW){
        new_height = (int)Get_Sheet_Size(new_icon);
        new_width = (int)Get_Sheet_Width(new_icon);
    }
    else if(sheet_format == GRIDLOCK_FLOW){
        Get_Gridlock_Size(new_icon, &new_height, &new_width);
    }

    png_bytepp row_pointers_new = (png_bytepp)malloc(sizeof(png_bytep) * new_height);

    png_set_IHDR(write_png_ptr, write_info_ptr, (png_uint_32)new_width, (png_uint_32)new_height,
                 bit_depth, color_type, interlace_method, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    for (png_uint_32 i = 0; i < new_height; i++) {
        row_pointers_new[i] = (png_bytep)malloc(png_get_rowbytes(write_png_ptr, write_info_ptr));
    }

    for(png_uint_32 i = 0; i < new_height; i++) {
        for (png_uint_32 j = 0; j < png_get_rowbytes(write_png_ptr, write_info_ptr); j++) {
            row_pointers_new[i][j] = 0;
        }
    }
    dmiToPng(new_icon, png_get_rowbytes(read_png_ptr, read_info_ptr), 144,
             row_pointers, row_pointers_new, write_png_ptr, write_info_ptr,
             pixels_per_byte, color_type, GRIDLOCK_FLOW, LINEAR_FLOW);

    png_write_info(write_png_ptr, write_info_ptr);
    png_write_image(write_png_ptr, row_pointers_new);

    png_write_end(write_png_ptr, NULL);
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    //png_destroy_read_struct(&read_png_ptr, &read_info_ptr);
    fclose(output_fp);
    fclose(input_fp);

    free(row_pointers_new);
    free(row_pointers);
    return 1;

}
