/* The DMI structure will be used to contain all relevant information associated with a DMI.
 * The variables will be:
 *
 * - version:       The version of BYOND it should use. Either 4.0 (most current DMI format) or 3.5.
 * - width:         The width for each frame.
 * - height:        The height for each frame.
 * - icon_states:   This should be a vector containing a list of all icon_states. */
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
 *  It's used to get the height of a spritesheet that is in HORIZONTAL_FLOW format. */

void initialize_sheet_data(SpriteSheetData* sheet_data){
    sheet_data->height = 0;
    sheet_data->width = 0;
    sheet_data->user_input_height = 0;
    sheet_data->user_input_width = 0;
    sheet_data->margin_y = 0;
    sheet_data->margin_x = 0;
    sheet_data->padding_x = 0;
    sheet_data->padding_y = 0;
    sheet_data->grid_size = 0;
    sheet_data->offset_x = 0;
    sheet_data->offset_y = 0;
    sheet_data->frame_height = 0;
    sheet_data->frame_width = 0;
    sheet_data->format = 1;
}

Image create_sprite_sheet(Image* initial_image, SpriteSheetData* sheet_data, DMI new_icon, char* file_name){
    sheet_data->frame_width = new_icon.width;
    sheet_data->frame_height = new_icon.height;
    if(sheet_data->format == GRID){

        calculate_grid_sheet_dimensions(&new_icon, sheet_data, sheet_data->grid_size);
    }
    else {
        //run other calculations to get the sheet's dimensions.
        printf("Height/Width = %d/%d\n\n\n\n\n", initial_image->height, initial_image->width);
        sheet_data->height = (int)initial_image->height;
        sheet_data->width = (int)initial_image->width;
        printf("Hrm. . .\n");
    }

    if(sheet_data->width < sheet_data->user_input_width)
        sheet_data->width = sheet_data->user_input_width;

    if(sheet_data->height < sheet_data->user_input_height)
        sheet_data->height = sheet_data->user_input_height;

    Image new_image;
    if(sheet_data->grid_size != 0){
        new_image.width = sheet_data->width + (sheet_data->margin_x * (sheet_data->grid_size - 1)) +
                (sheet_data->padding_x * sheet_data->frames_per_row);

        int num_of_rows = ceil((double)new_icon.num_of_states / sheet_data->grid_size);


        new_image.height = sheet_data->height + (sheet_data->margin_y * num_of_rows) +
                (sheet_data->padding_y * sheet_data->frames_per_col);
    }

    else {
        new_image.width = sheet_data->width;// + (sheet_data->margin_x * (sheet_data->grid_size - 1));
        new_image.height = sheet_data->height;// + (sheet_data->margin_y * num_of_rows);
    }

    printf("Width/Height = %d/%d\n", new_image.width, new_image.height);
    new_image.bit_depth = initial_image->bit_depth;
    new_image.color_type = initial_image->color_type;
    initialize_image2(file_name, &new_image, NULL, NULL, NULL, NULL);
    return new_image;
}

void calculate_grid_sheet_dimensions(DMI* dmi, SpriteSheetData* sheet_data, int icon_states_per_row){
    icon_state *iterator = dmi->begin_icon_state;
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

    fflush(stdout);
    sheet_data->width = widest_row * sheet_data->frame_width;
    sheet_data->height = height_total * sheet_data->frame_height;
    sheet_data->frames_per_row = widest_row;
    sheet_data->frames_per_col = height_total;
    sheet_data->row_count = 0;
}

void Get_Frame(png_bytepp dest_pixels, png_bytepp src_pixels, png_uint_32 dest_start_row, png_uint_32 dest_start_col,
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
    fflush((stdout));
}
int sheet2dmi(DMI* dmi, Image source_image, Image sheet_image, SpriteSheetData sheetData){


    return 0;
}

int dmi2sheet(DMI* dmi, Image source_image, Image sheet_image, SpriteSheetData sheetData){
    int DMI_HEIGHT = dmi->height, DMI_WIDTH = (dmi->width) / source_image.pixels_per_byte;
    int icon_state_index = 0;
    int numOfStates = dmi->num_of_states;
    int total_frames = 0;

    if(source_image.color_type == PNG_COLOR_TYPE_RGBA){
        DMI_WIDTH *= 4;
    }

    sheet_image.pixels_per_byte = source_image.pixels_per_byte;
    FrameExtractor source;
    FrameExtractor destination;
    initialize_extractor(&source);
    initialize_extractor(&destination);

    while(icon_state_index < numOfStates){
        int num_of_dirs = dmi->begin_icon_state->dirs;
        int num_of_frames = dmi->begin_icon_state->frames;
        source.loop_row_start = source.row_tracker;
        source.loop_col_start = source.column_tracker;
        for(int i = 0; i < num_of_dirs; i++){
            source.starting_column = source.loop_col_start;
            source.starting_row = source.loop_row_start;
            for(int j = 0; j < num_of_frames; j++) {
                update_current_position(&source);
                update_current_position(&destination);
                if(source.current_column >= (source_image.row_bytes/source_image.pixels_per_byte)) {
                    source.row_offset += (source.current_column / (source_image.row_bytes/ source_image.pixels_per_byte)) * DMI_HEIGHT;;
                    source.starting_column = source.current_column % (source_image.row_bytes/source_image.pixels_per_byte);
                    source.column_offset = 0;
                    update_current_position(&source);
                }

                Get_Frame(sheet_image.pixel_array, source_image.pixel_array,destination.current_row,
                          destination.current_column,source.current_row,source.current_column,
                          DMI_WIDTH,DMI_HEIGHT);
                source.column_offset += (DMI_WIDTH * num_of_dirs);

                destination.column_offset += DMI_WIDTH + sheetData.padding_x;
                //printf("WIDth = %d\n Width + Pad_X = %d\n", DMI_WIDTH + sheetData.padding_x);
                update_current_position(&destination);
               // printFrameExtractor(&destination);
                if(sheetData.format != GRID){
                    if(destination.current_column >= (sheet_image.row_bytes/sheet_image.pixels_per_byte)){
                        destination.column_offset = 0;
                        destination.row_offset += DMI_HEIGHT;
                        update_current_position(&destination);
                    }
                }



            }

            if(sheetData.format == GRID){
                destination.column_offset = 0;
                destination.row_offset += DMI_HEIGHT + sheetData.padding_y;
            }
            else {
                fflush(stdout);
                if(destination.current_column >= (sheet_image.row_bytes/sheet_image.pixels_per_byte)){
                    destination.column_offset = 0;
                    destination.row_offset += (destination.current_column / (sheet_image.row_bytes/ sheet_image.pixels_per_byte)) * DMI_HEIGHT;

                }
                fflush(stdout);
            }
            fflush((stdout));
            source.column_offset = 0;
            source.row_offset = 0;
            source.loop_col_start += DMI_WIDTH;// + sheetData.padding_x;
            if(source.loop_col_start >= source_image.row_bytes){
                source.loop_row_start  += (source.loop_col_start  / source_image.row_bytes) * DMI_HEIGHT
                        + sheetData.padding_y;
                source.loop_col_start %= source_image.row_bytes;
            }
        }/*!< End of the outer loop.*/
       // return EXIT_SUCCESS;

        total_frames += dmi->begin_icon_state->frames *dmi->begin_icon_state->dirs;

        /** If in GRID format, we want to have a new starting_column that corresponds to where we'd be after parsing
         * the previous icon_state. We set our row and column offset back to 0. */
        if(sheetData.format == GRID){
            destination.column_offset = 0;
            destination.row_offset = 0;
            destination.starting_column += dmi->begin_icon_state->frames * DMI_WIDTH +
                    (sheetData.padding_x * dmi->begin_icon_state->frames);
        }

        source.column_tracker = (total_frames * DMI_WIDTH) % (source_image.row_bytes / source_image.pixels_per_byte);
        source.row_tracker = ((total_frames * DMI_WIDTH) / (source_image.row_bytes / source_image.pixels_per_byte)) * DMI_HEIGHT;
        dmi->begin_icon_state++;

        icon_state_index++;

        /** Likewise, we wan to set our starting column back to 0 once we've parsed all of the icon_states for a
         * our designated # of states per row. Only if in GRID format.*/
        if(sheetData.grid_size != 0) {
            if(icon_state_index % sheetData.grid_size == 0 && sheetData.format == GRID){
                destination.starting_row += sheetData.list_of_row_sizes[sheetData.row_count] * DMI_HEIGHT +
                        (sheetData.padding_y * sheetData.list_of_row_sizes[sheetData.row_count]);
                destination.starting_column = 0;
                sheetData.row_count++;
            }
        }

    } /*!< End Point of the while loop.*/
    return EXIT_SUCCESS;
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
