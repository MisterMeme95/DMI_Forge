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

/* A basic function to initialize a DMI struct. */
void Init_DMI(DMI* dmi, int width, int height){
    dmi->num_of_states = 0;
    dmi->has_icons = false;
    dmi->version = 4.0;
    dmi->width = width;
    dmi->height = height;
    dmi->icon_states = (icon_state*) malloc(sizeof(icon_state) * 10);
    dmi->begin_icon_state = dmi->icon_states;
}

int DMI_To_Png(DMI* dmi, int pngWidth, int pngHeight, png_bytepp orig_pointer, png_bytepp new_pointer,
               png_structp png_ptr, png_infop info_ptr){
    int DMI_HEIGHT = 144, DMI_WIDTH = dmi->width;

    int total_frames = 0;
    printf("DMI_HEIGHT = %d\n", DMI_HEIGHT);
    printf("DMI_WIDTH = %d\n", DMI_WIDTH);
    /*
     * First we define some vaaribles for the new image, row_points2.
     * We want to keep track of the rows in the row_points2
     * */
    int dmi_row = 0;
    /* This will be used to track */
    int current_frame = 1;
    int state_tracker = 0;
    int dmi_rows = 0, dmi_cols = 0;
    int row_counter = 0;
    int total_row = 0;


    //Just running a test for the first icon_state atm. There are 11 icon_states in this file
    while(state_tracker < (dmi->num_of_states)){

        //I need to run a loop for each direction
        for(int i = 0; i < dmi->begin_icon_state->dirs; i++){
            //I know that each frame is 32 row. So for each direction, I run a loop 32 times
            while(row_counter < DMI_HEIGHT){
                //This keeps track of the starting row for the original image
                int curr_row = dmi_rows + row_counter;
                //This keeps track of start column. I know that for each new direction, I need to start my column
                //32 pixels to the right
                int curr_col = dmi_cols + (i * DMI_WIDTH);
                if(curr_col > pngWidth - DMI_WIDTH){
                    curr_col = pngWidth - curr_col;
                    if(curr_col <= 0){
                        curr_col *= -1;
                    }
                    curr_row += DMI_HEIGHT;
                }

                int frame_count = 0;
                int curr_frame_row = i;

                while(frame_count < dmi->begin_icon_state->frames){
                    /* CODE BREAKS HERE*/
                    //if(strcmp(dmi->begin_icon_state->state, "\"standing\"") == 0)
                   //     return
                    //printf("Total_Row = %d, frame_count = %d, curr_row = %d, curr_col = %d\n", total_row,
                 //         frame_count * 144, curr_row, curr_col);
                 //   printf("%s\n", dmi->begin_icon_state->state);
                    if(total_row >= 4608)
                        total_row = 4607;
                    memcpy(&new_pointer[total_row][frame_count*144], &orig_pointer[curr_row][curr_col], sizeof(png_byte) * DMI_WIDTH);
                    frame_count++;

                   if(dmi->begin_icon_state->dirs)
                       curr_col += (dmi->begin_icon_state->dirs) * DMI_WIDTH;
                   if(curr_col >= pngWidth){
                       curr_col = curr_col - pngWidth; //(curr_col - 320);
                       curr_row += DMI_HEIGHT;
                   }
                }
                row_counter++;
                total_row++;
            }
            row_counter = 0;
        }//for loop end
        //return 0;
        state_tracker++;
        total_frames += (dmi->begin_icon_state->dirs) * (dmi->begin_icon_state->frames);

        //Recalculates starting row and colm based on the total number of frames read thus far.
        int row_divide = pngWidth / DMI_WIDTH;

        dmi_rows = (total_frames / row_divide) * DMI_HEIGHT;
        dmi_cols = (total_frames % row_divide) * DMI_WIDTH;
        printf("Dmi_Rows = %d\n", dmi_rows);
        printf("Dmi_cols = %d\n", dmi_cols);
        if(!strcmp(dmi->begin_icon_state->state, "\"\"")){
            printf("Dmi_Rows = %d\n", dmi_rows);
            printf("Dmi_cols = %d\n", dmi_cols);
        }
        dmi->begin_icon_state++;
    }
}

void output_pixel_values(png_structp png_ptr, png_infop info_ptr, png_bytep *row_pointers) {
    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_colorp palette = NULL;
    int num_palette = 0;
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);
    }
    for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        //printf("Height = %d\n", height);
        //    sleep(10);
        for (int x = 0; x < width; x++) {
            if (color_type == PNG_COLOR_TYPE_RGB) {
                png_bytep px = &(row[x * 3]);
                // printf("Pixel at (%d, %d): R=%d, G=%d, B=%d\n", x, y, px[0], px[1], px[2]);
            } else if (color_type == PNG_COLOR_TYPE_RGBA) {
                png_bytep px = &(row[x * 4]);
                //    printf("Pixel at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", x, y, px[0], px[1], px[2], px[3]);
            } else if (color_type == PNG_COLOR_TYPE_GRAY) {
                int num_bytes = bit_depth == 8 ? 1 : 2;
                png_bytep px = &(row[x * num_bytes]);
                int gray_value = num_bytes == 1 ? px[0] : (px[0] << 8) + px[1];
                //    printf("Pixel at (%d, %d): Gray=%d\n", x, y, gray_value);
            } else if (color_type == PNG_COLOR_TYPE_PALETTE) {
                int index = row[x];
                if (index < num_palette) {
                    png_color palette_color = palette[index];
                    if(y >= 32 && y<= 96){
                        printf("Pixel at (%d, %d): Palette Index=%d, R=%d, G=%d, B=%d\n", x, y, index, palette_color.red, palette_color.green, palette_color.blue);

                    }

                } else {
                    if(y >= 32 && y<= 96){
                        printf("Index = %d\n", index);
                        printf("Pixel at (%d, %d): Palette index out of range\n", x, y);
                    }

                }
            }

            else {
                if(y >= 32 && y<= 96){
                    printf("Index = %d", row[x]);
                    printf("Pixel at (%d, %d): Color type not supported\n", x, y);
                }
            }
        }
    }
}


void output_pixel_values2(png_structp png_ptr, png_infop info_ptr, png_bytep *row_pointers, int start_row, int between1, int between2, char type) {
    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_colorp palette = NULL;
    int num_palette = 0;
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);
    }
    for (int y = start_row; y < start_row+1; y++) {
        png_bytep row = row_pointers[y];
        //printf("Height = %d\n", height);
        //    sleep(10);
        for (int x = 0; x < width; x++) {
            if (color_type == PNG_COLOR_TYPE_RGB) {
                png_bytep px = &(row[x * 3]);
                // printf("Pixel at (%d, %d): R=%d, G=%d, B=%d\n", x, y, px[0], px[1], px[2]);
            } else if (color_type == PNG_COLOR_TYPE_RGBA) {
                png_bytep px = &(row[x * 4]);
                //    printf("Pixel at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", x, y, px[0], px[1], px[2], px[3]);
            } else if (color_type == PNG_COLOR_TYPE_GRAY) {
                int num_bytes = bit_depth == 8 ? 1 : 2;
                png_bytep px = &(row[x * num_bytes]);
                int gray_value = num_bytes == 1 ? px[0] : (px[0] << 8) + px[1];
                //    printf("Pixel at (%d, %d): Gray=%d\n", x, y, gray_value);
            } else if (color_type == PNG_COLOR_TYPE_PALETTE) {
                int index = row[x];
                if (index < num_palette) {
                    png_color palette_color = palette[index];
                    if(y >= between1 && y<= between2){
                        printf("%c - Pixel at (%d, %d): Palette Index=%d, R=%d, G=%d, B=%d\n",type, x, y, index, palette_color.red, palette_color.green, palette_color.blue);

                    }

                } else {
                    if(y >= between1 && y<= between2){
                        printf("%c - Index = %d\n",type, index);
                        printf("%c - Pixel at (%d, %d): Palette index out of range\n",type, x, y);
                    }

                }
            }

            else {
                if(y >= between1 && y<= between2){
                    printf("%c - Index = %d",type, row[x]);
                    printf("%c - Pixel at (%d, %d): Color type not supported\n",type, x, y);
                }
            }
        }
    }
}