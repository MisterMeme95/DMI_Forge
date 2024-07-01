/* The DMI structure will be used to contain all relevant information associated with a DMI.
 * The variables will be:
 *
 * - version:       The version of BYOND it should use. Either 4.0 (most current DMI format) or 3.5.
 * - width:         The width for each frame.
 * - height:        The height for each frame.
 * - icon_states:   This should be a vector containing a list of all icon_states.
 *
 * */
#include "png.h"
#include "dmi.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "PixelManip.h"


/* A basic function to initialize a DMI struct. */
void Init_DMI(DMI* dmi, int width, int height){
    dmi->num_of_states = 0;
    dmi->max_state = 15;
    dmi->has_icons = false;
    dmi->version = 4.0;
    dmi->width = width;
    dmi->height = height;
    dmi->icon_states = (icon_state*) malloc(sizeof(icon_state) * dmi->max_state);
    dmi->begin_icon_state = dmi->icon_states;
}

void Resize_IconStates(DMI* dmi, int new_size){
    icon_state *newStates = realloc(dmi->begin_icon_state, new_size * sizeof(icon_state));
    dmi->begin_icon_state = newStates;
    dmi->icon_states = dmi->begin_icon_state + dmi->num_of_states-1;

}

/** This function needs to be refactored and renamed.
 *  It's used to get the height of a spritesheet that is in HORIZONTAL_FLOW format.
 * */
png_uint_32 Get_Sheet_Size(DMI* dmi){
    dmi->icon_states = dmi->begin_icon_state;

    png_uint_32 return_total = 0;
    for(int i = 0; i < dmi->num_of_states; i++){
        return_total += dmi->icon_states->dirs;
        dmi->icon_states++;
    }
    dmi->icon_states = dmi->begin_icon_state;
    return (png_uint_32)(return_total * dmi->height);
}

Image create_sprite_sheet(Image* initial_image, SpriteSheetData* sheet_data, DMI new_icon, char* file_name){
    if(sheet_data->format == GRID){
        calculate_grid_sheet_dimensions(&new_icon, sheet_data, 3);
    }
    else {
        //run other calculations to get the sheet's dimensions.
        sheet_data->height = (int)initial_image->height;
        sheet_data->width = (int)initial_image->width;
    }

    if(sheet_data->width < sheet_data->user_input_width)
        sheet_data->width = sheet_data->user_input_width;

    if(sheet_data->height < sheet_data->user_input_height)
        sheet_data->height = sheet_data->user_input_height;


    Image new_image;
    new_image.width = sheet_data->width;
    new_image.height = sheet_data->height;
    new_image.bit_depth = initial_image->bit_depth;
    new_image.color_type = initial_image->color_type;
    initialize_image2(file_name, &new_image, NULL, NULL, NULL, NULL);



    //new_image.width = sheet_data.width + sheet_data.margin_x + (sheet_data.offset_x * sheet_data.frames_per_row);
   // new_image.height = sheet_data.height + sheet_data.margin_y + (sheet_data.offset_x * sheet_data.frames_per_col);
    //new_image.bit_depth = initial_image->bit_depth;
   // new_image.color_type = initial_image->color_type;
    //next set the new image to the height/width/bit_dep

    return new_image;
}

void calculate_horizontal_sheet_dimensions(DMI* dmi, SpriteSheetData* sheet_data, int default_width){


}

void calculate_grid_sheet_dimensions(DMI* dmi, SpriteSheetData* sheet_data, int icon_states_per_row){
    icon_state *iterator = dmi->begin_icon_state;
    //printf("Num of states = %d\n", dmi->num_of_states);
    int num_of_states = dmi->num_of_states;

    int row_size = (int)ceil(((double)dmi->num_of_states / (double)icon_states_per_row));
    int rows_filled = 0;
    sheet_data->list_of_row_sizes = (int*) malloc(sizeof(int) * row_size);

    int widest_row = 0;
    int height_total = 0;

    for(int i = 0; i < num_of_states; i+= icon_states_per_row){
        int current_count = 0;
        int max_dir_in_row = 0;
        int iterate_to;
        if(i + icon_states_per_row >= dmi->num_of_states){
            iterate_to = num_of_states - i;
        }
        else {
            iterate_to = icon_states_per_row;
        }
        for(int o = 0; o < iterate_to; o++){
            if(iterator->dirs > max_dir_in_row){
                max_dir_in_row = iterator->dirs;
            }
            current_count += iterator->frames;
            iterator++;
        }
        height_total += max_dir_in_row;
        sheet_data->list_of_row_sizes[rows_filled] = max_dir_in_row;
        rows_filled++;
        if(current_count > widest_row)
            widest_row = current_count;
    }
    printf("Widest Row = %d\n", widest_row * 32);
    fflush(stdout);
    sheet_data->width = widest_row * 32;
    sheet_data->height = height_total * 32;
    sheet_data->frames_per_row = widest_row;
    sheet_data->frames_per_col = height_total;
    sheet_data->row_count = 0;
}
png_uint_32 Get_Sheet_Width(DMI* dmi){
    png_uint_32 highest_width = 0;
    dmi->icon_states = dmi->begin_icon_state;
    for(int i = 0; i < dmi->num_of_states; i++){
        if(dmi->icon_states->frames > highest_width){
            highest_width = dmi->icon_states->frames;
        }
        dmi->icon_states++;
    }
    dmi->icon_states = dmi->begin_icon_state;

    return (png_uint_32)(highest_width * dmi->width);
}

int PNG_To_DMI(){
    int dmiWidth = 32, dmiHeight = 32, iconNum = 0;
    char png_name[25], zTxt[10000] = "#BEGIN DMI\n";
    char snum[5];
    int totalFrame = 0;
    FILE *input_fp;
    char c;

    png_structp read_png_ptr;
    png_infop read_info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, pixels_per_byte;

    printf("Let's create a new DMI!\n");
    printf("Type in the name of the PNG File: ");
  //  while ((c = getchar()) != '\n' && c != EOF);
    scanf("%s", png_name);
    printf("Enter the width of your DMI: ");
   // while ((c = getchar()) != '\n' && c != EOF);
    scanf("%d", &dmiWidth);
  //  while ((c = getchar()) != '\n' && c != EOF);
    printf("width = %d\n", dmiWidth);
    printf("Enter the Height of your DMI: ");
    scanf("%d", &dmiHeight);
  //  while ((c = getchar()) != '\n' && c != EOF);
    printf("# of icon_states: ");
    scanf("%d", &iconNum);
    //while ((c = getchar()) != '\n' && c != EOF);
    DMI fake_icon;

    fake_icon.width = dmiWidth;
    fake_icon.height = dmiHeight;
    fake_icon.version = 4.0;
    fake_icon.num_of_states = iconNum;
    if(iconNum > 1){
        fake_icon.icon_states = (icon_state*) malloc(sizeof(icon_state) * iconNum);
    }

    else {
        fake_icon.icon_states = (icon_state*) malloc(sizeof(icon_state) * 1);
    }
    fake_icon.begin_icon_state = fake_icon.icon_states;

    strcat(zTxt, "version = 4.0\n");
    strcat(zTxt, "\twidth = ");
    strcat(zTxt, itoa(fake_icon.width, snum, 10));
    strcat(zTxt, "\n\theight =");
    strcat(zTxt, itoa(fake_icon.height, snum, 10));
    strcat(zTxt, "\n");


    for(int i = 0; i < iconNum; i++){
        char icon_name[25] = "";
        int dir_num = 1, frame_num = 1;
        //printf("Name For Icon_state #%d: ", i);
        //scanf("%s", icon_name);
        strcat(zTxt, "state = Lol");
        strcat(zTxt, icon_name);

        strcat(zTxt, "\n\tdirs = ");
        fake_icon.icon_states->state = icon_name;
        //printf("# of Directions for %s: ", icon_name);
       // scanf("%d", &dir_num);
        strcat(zTxt, itoa(dir_num, snum, 10));


        strcat(zTxt, "\n\tframes = ");
        fake_icon.icon_states->dirs = dir_num;
       //printf("# of Frames for %s: ", icon_name);
        //scanf("%d", &frame_num);
        strcat(zTxt, itoa(frame_num, snum, 10));
        strcat(zTxt, "\n");
        totalFrame += dir_num * frame_num;

       // strcat(zTxt, "\n\tdelay = 1");
      //  for(int o = 0; o < frame_num-1; o++){
      //      strcat(zTxt, ",1");
     //   }

        printf("Total Frame = %d\n", totalFrame);
       // strcat(zTxt, "\n");
      //  fake_icon.icon_states->frames = dir_num;
      //  if(iconNum > 1){
      //      fake_icon.icon_states++;
     //   }
        //while ((c = getchar()) != '\n' && c != EOF);
    }

    strcat(zTxt, "#END DMI\n");
    printf("%s", zTxt);
    fake_icon.icon_states = fake_icon.begin_icon_state;


    if(!check_if_png("pokemon.png",&input_fp)){
        printf("The file you are attempting to read is not a valid PNG file!\n");
        return 0;
    }

    if(initialize_image(&read_png_ptr, &read_info_ptr, &input_fp)) {
        printf("Initializing the image failed!\n");
        return 0;
    }


    png_get_IHDR(read_png_ptr, read_info_ptr, &width, &height, &bit_depth,
                 &color_type, &interlace_method, NULL, NULL);

    png_bytepp row_pointers = (png_bytepp)malloc(sizeof(png_bytep) * height);

    for (png_uint_32 i = 0; i < height; i++) {
        row_pointers[i] = (png_bytep)malloc(png_get_rowbytes(read_png_ptr, read_info_ptr));
    }
    png_read_image(read_png_ptr, row_pointers);


    FILE *output_fp = fopen("__OUTDMI.dmi", "wb");
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

    png_init_io(write_png_ptr, output_fp);


    png_colorp palette;
    int num_palette;
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(read_png_ptr, read_info_ptr, &palette, &num_palette);
        png_set_PLTE(write_png_ptr, write_info_ptr, palette, num_palette);
    }


    png_set_compression_level(write_png_ptr, 9);

    // Create the zTXt chunk data
   // png_text text_ptr;
  //  text_ptr.compression = PNG_TEXT_COMPRESSION_zTXt;
   // text_ptr.key = NULL;
   // text_ptr.text = (png_charp)zTxt;
   // text_ptr.text_length = strlen(zTxt);

    const char* keyword = "Description";
    const char* text = "This is the image description."; // Replace with your desired text

    png_text text_ptr;
    text_ptr.compression = PNG_TEXT_COMPRESSION_zTXt;
    text_ptr.key = (png_charp)keyword;
    text_ptr.text = (png_charp)zTxt;
    text_ptr.text_length = strlen(zTxt);

   // png_set_text(png_write_ptr, info_write_ptr, &text_ptr, 1);
    int write_width = ceil(sqrt(totalFrame)) * dmiWidth;
    int write_height = ceil(totalFrame / ceil(sqrt(totalFrame))) * dmiHeight;
    png_set_IHDR(write_png_ptr, write_info_ptr, (png_uint_32)write_width,
                 (png_uint_32)write_height, bit_depth,color_type,interlace_method,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_set_text(write_png_ptr, write_info_ptr, &text_ptr, 1);

    //------------------------------


    png_bytepp row_pointers_new = (png_bytepp)malloc(sizeof(png_bytep) * write_height);
    for (png_uint_32 i = 0; i < write_height; i++) {
        row_pointers_new[i] = (png_bytep)malloc(png_get_rowbytes(write_png_ptr, write_info_ptr));
    }

    for(png_uint_32 i = 0; i < write_height; i++) {
        for (png_uint_32 j = 0; j < png_get_rowbytes(write_png_ptr, write_info_ptr); j++) {
            row_pointers_new[i][j] = 0;
        }
    }

    //------------------------------

/*
    //------------------**********************------------
    int frames_per_row = floor(width / dmiWidth);
    int iterate_rows = ceil(totalFrame/frames_per_row);
    int last_row = ceil((((double)totalFrame/(double)frames_per_row) - (totalFrame/frames_per_row)) * frames_per_row);

    int total_row = 0, frame_count = 0, curr_row = 0, curr_col = 0;
    for(int i = 0; i < iterate_rows; i++){
        for(int o = 0; o < 32; o++){
            memcpy(&row_pointers_new[total_row][frame_count*DMI_WIDTH], &row_pointers[curr_row][curr_col],
            sizeof(png_byte) * dmiWidth);
        }
    }
    //----------------**********************--------------
  */
    png_write_info(write_png_ptr, write_info_ptr);
    png_write_image(write_png_ptr, row_pointers);
    printf("Write image done!\n");
    png_write_end(write_png_ptr, NULL);
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);



    return 1;

}

void Get_Frame(png_bytepp dest_pixels, png_bytepp src_pixels, int dest_start_row,int dest_start_col,
               int src_start_row, int src_start_col, int bytes_per_frame, int cols_to_copy) {

    for (int i = 0; i < cols_to_copy; i++) {
        memcpy(&dest_pixels[dest_start_row + i][dest_start_col],&src_pixels[src_start_row + i][src_start_col],
               bytes_per_frame);
    }

}

void initialize_extractor(FrameExtractor *extractor) {
    extractor->starting_column = 0;
    extractor->starting_row = 0;
    extractor->current_column = 0;
    extractor->current_row = 0;
    extractor->column_offset = 0;
    extractor->row_offset = 0;
    extractor->loop_col_start = 0;
    extractor->loop_row_start = 0;
    extractor->column_tracker = 0;
    extractor->row_tracker = 0;
}

void update_current_position(FrameExtractor *extractor){
    extractor->current_column = extractor->starting_column + extractor->column_offset;
    extractor->current_row = extractor->starting_row + extractor->row_offset;
}
void printFrameExtractor(const FrameExtractor* extractor) {
    printf("Starting column: %u\n", extractor->starting_column);
    printf("Starting row: %u\n", extractor->starting_row);
    printf("Current column: %u\n", extractor->current_column);
    printf("Current row: %u\n", extractor->current_row);
    printf("Column offset: %u\n", extractor->column_offset);
    printf("Row offset: %u\n", extractor->row_offset);
}


int dmi2sheet(DMI* dmi, Image source_image, Image sheet_image, SpriteSheetData sheetData){

    int DMI_HEIGHT = dmi->height, DMI_WIDTH = (dmi->width) / source_image.pixels_per_byte;
    int icon_state_index = 0;
    int numOfStates = dmi->num_of_states;
    int total_frames = 0;
    FrameExtractor source;
    FrameExtractor destination;
    initialize_extractor(&source);
    initialize_extractor(&destination);

    while(icon_state_index < numOfStates){
        printf("Died here#3\n");
        fflush(stdout);
        int num_of_dirs = dmi->begin_icon_state->dirs;
        int num_of_frames = dmi->begin_icon_state->frames;

        source.loop_row_start = source.row_tracker;
        source.loop_col_start = source.column_tracker;
        for(int i = 0; i < num_of_dirs; i++){
            printf("Died here#4\n");
            fflush(stdout);
            source.starting_column = source.loop_col_start;
            source.starting_row = source.loop_row_start;
            for(int j = 0; j < num_of_frames; j++) {
                update_current_position(&source);
                update_current_position(&destination);
                if(source.current_column >= (source_image.width/source_image.pixels_per_byte)) {
                    source.row_offset += (source.current_column / (source_image.width/ source_image.pixels_per_byte)) * DMI_HEIGHT;;
                    source.starting_column = source.current_column % (source_image.width/source_image.pixels_per_byte);
                    source.column_offset = 0;
                    update_current_position(&source);
                }

                printFrameExtractor(&source);
                Get_Frame(sheet_image.pixel_array, source_image.pixel_array,destination.current_row,
                          destination.current_column,source.current_row,source.current_column,
                          DMI_WIDTH,DMI_HEIGHT);

                source.column_offset += (DMI_WIDTH * num_of_dirs);
                destination.column_offset += DMI_WIDTH;
                update_current_position(&destination);
            }

            destination.column_offset = 0;
            destination.row_offset += DMI_HEIGHT;
            source.column_offset = 0;
            source.row_offset = 0;

            source.loop_col_start += DMI_WIDTH;
            if(source.loop_col_start >= source_image.width){
                source.loop_row_start  += (source.starting_column / source_image.width) * DMI_HEIGHT;
                source.loop_col_start %= source_image.width;
            }
        }/*!< End of the outer loop.*/

        total_frames += dmi->begin_icon_state->frames *dmi->begin_icon_state->dirs;
        destination.column_offset = 0;
        destination.row_offset = 0;
        destination.starting_column += dmi->begin_icon_state->frames * DMI_WIDTH;

        source.column_tracker = (total_frames * DMI_WIDTH) % (source_image.width / source_image.pixels_per_byte);
        source.row_tracker = ((total_frames * DMI_WIDTH) / (source_image.width / source_image.pixels_per_byte)) * DMI_HEIGHT;
        dmi->begin_icon_state++;

        icon_state_index++;

        if(icon_state_index % 3 == 0){
            destination.starting_row += sheetData.list_of_row_sizes[sheetData.row_count] * DMI_HEIGHT;
            destination.starting_column = 0;
        }
    } /*!< End Point of the while loop.*/
    return EXIT_SUCCESS;
}

void Get_Gridlock_Size(DMI *dmi, int *height, int *width){
    icon_state *state = dmi->begin_icon_state;
    int current_frame = 0;
    int total_width = 0;
    int greatest_width = 0, greatest_height = 0, total_height = 0;
    while(current_frame < dmi->num_of_states){

        total_width += state->frames;
        //printf("Total_Width #%d = %d\n", current_frame, total_width);
      //  fflush(stdout);
        if(state->dirs > greatest_height){
            greatest_height = state->dirs;
        }
        if((current_frame + 1) % 3 == 0){
            if(total_width > greatest_width){
                greatest_width = total_width;
            }
            total_height += greatest_height;
            greatest_height = 0;
            total_width = 0;
        }
        current_frame++;
        state++;
    }

    if(total_width > greatest_width){
        greatest_width = total_width;
    }
    total_height += greatest_height;

    *height = total_height * dmi->height;
    *width = greatest_width * dmi->width;
}

int Get_Dest_Col(icon_state* state, int DMI_WIDTH, int iteration){
    if(iteration % 3 == 0){
        return 0;
    }
    return state->frames * DMI_WIDTH;
}

int Get_Dest_Row(icon_state* state, int DMI_HEIGHT){
    int biggest_dir = 1;
    for(int i = 1; i <= 3; i++){
        if(state->dirs > biggest_dir){
            biggest_dir = state->dirs;
        }
        state--;
    }
    return biggest_dir * DMI_HEIGHT;
}

void output_image_pixels(Image image){
    png_uint_32 width = image.width;
    png_uint_32 height = image.height;
    int color_type = image.color_type;
    int bit_depth = image.bit_depth;
    png_colorp palette = NULL;
    int num_palette = 0;
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(image.png_ptr, image.info_ptr, &palette, &num_palette);
    }
    for (int y = 0; y < height; y++) {
        png_bytep row = image.pixel_array[y];
        for (int x = 0; x < width; x++) {
            if (color_type == PNG_COLOR_TYPE_RGB) {
                png_bytep px = &(row[x * 3]);
                printf("Pixel at (%d, %d): R=%d, G=%d, B=%d\n", x, y, px[0], px[1], px[2]);
            } else if (color_type == PNG_COLOR_TYPE_RGBA) {
                png_bytep px = &(row[x * 4]);
                printf("Pixel at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", x, y, px[0], px[1], px[2], px[3]);
            } else if (color_type == PNG_COLOR_TYPE_GRAY) {
                int num_bytes = bit_depth == 8 ? 1 : 2;
                png_bytep px = &(row[x * num_bytes]);
                int gray_value = num_bytes == 1 ? px[0] : (px[0] << 8) + px[1];
                printf("Pixel at (%d, %d): Gray=%d\n", x, y, gray_value);
            } else if (color_type == PNG_COLOR_TYPE_PALETTE) {
                int index, ppb = 8/bit_depth;    // pixels per byte
                switch (bit_depth){
                    default: printf("bit depth %d not implemented\n", bit_depth); exit(1);
                    case 1:
                    case 2:
                    case 4:
                    case 8: index = row[x/ppb]>>8-(x%ppb+1)*bit_depth&255>>8-bit_depth;
                }
                printf("Index = %d\n", index);

                if (index < num_palette) {
                    png_color palette_color = palette[index];
                    printf(" >>> Pixel at (%d, %d): Palette Index=%d, R=%d, G=%d, B=%d\n", x, y, index,
                           palette_color.red, palette_color.green, palette_color.blue);
                } else {
                    printf("Index = %d\n", index);
                    printf("Pixel at (%d, %d): Palette index out of range\n", x, y);
                }
            }
            else {
                printf("Index = %d", row[x]);
                printf("Pixel at (%d, %d): Color type not supported\n", x, y);
            }
        }
    }


}
void output_pixel_values(png_structp png_ptr, png_infop info_ptr, png_bytep *row_pointers) {
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_colorp palette = NULL;
    int num_palette = 0;
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);
    }
    for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            if (color_type == PNG_COLOR_TYPE_RGB) {
                png_bytep px = &(row[x * 3]);
                printf("Pixel at (%d, %d): R=%d, G=%d, B=%d\n", x, y, px[0], px[1], px[2]);
            } else if (color_type == PNG_COLOR_TYPE_RGBA) {
                png_bytep px = &(row[x * 4]);
                printf("Pixel at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", x, y, px[0], px[1], px[2], px[3]);
            } else if (color_type == PNG_COLOR_TYPE_GRAY) {
                int num_bytes = bit_depth == 8 ? 1 : 2;
                png_bytep px = &(row[x * num_bytes]);
                int gray_value = num_bytes == 1 ? px[0] : (px[0] << 8) + px[1];
                printf("Pixel at (%d, %d): Gray=%d\n", x, y, gray_value);
            } else if (color_type == PNG_COLOR_TYPE_PALETTE) {
                int index, ppb = 8/bit_depth;    // pixels per byte
                switch (bit_depth){
                    default: printf("bit depth %d not implemented\n", bit_depth); exit(1);
                    case 1:
                    case 2:
                    case 4:
                    case 8: index = row[x/ppb]>>8-(x%ppb+1)*bit_depth&255>>8-bit_depth;
                }
                printf("8-bit_depth = %d\n", 8-bit_depth);
                printf("Index = %d\n", index);
                if(y >= 5){
                    sleep(10);
                }
                if (index < num_palette) {
                    png_color palette_color = palette[index];
                    printf(" >>> Pixel at (%d, %d): Palette Index=%d, R=%d, G=%d, B=%d\n", x, y, index,
                           palette_color.red, palette_color.green, palette_color.blue);
                } else {
                    printf("Index = %d\n", index);
                    printf("Pixel at (%d, %d): Palette index out of range\n", x, y);
               }
            }
            else {
                printf("Index = %d", row[x]);
                printf("Pixel at (%d, %d): Color type not supported\n", x, y);
            }
        }
    }
}