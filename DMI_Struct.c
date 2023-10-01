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

/* A basic function to initialize a DMI struct. */
void Init_DMI(DMI* dmi, int width, int height){
    dmi->num_of_states = 0;
    dmi->has_icons = false;
    dmi->version = 4.0;
    dmi->width = width;
    dmi->height = height;
    dmi->icon_states = (icon_state*) malloc(sizeof(icon_state) * 15);
    dmi->begin_icon_state = dmi->icon_states;
}
png_uint_32 Get_Sheet_Size(DMI* dmi){
    dmi->icon_states = dmi->begin_icon_state;
    png_uint_32 return_total = 0;
    for(int i = 0; i < dmi->num_of_states; i++){
        //printf("Dirs = %d\n", dmi->begin_icon_state->dirs);
        return_total += dmi->icon_states->dirs;
        dmi->icon_states++;
    }
    dmi->icon_states = dmi->begin_icon_state;

    return (png_uint_32)(return_total * dmi->height);
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

int DMI_To_Png(DMI* dmi, int pngWidth, int pngHeight, png_bytepp orig_pointer, png_bytepp new_pointer,
                 png_structp png_ptr, png_infop info_ptr, int ppb, int color_type){
    int DMI_HEIGHT = dmi->height, DMI_WIDTH = (dmi->width) / ppb;
    int total_frames = 0;
    int state_tracker = 0;
    if(color_type == PNG_COLOR_TYPE_RGB_ALPHA || color_type == PNG_COLOR_TYPE_GRAY_ALPHA){
        DMI_WIDTH *= 4;
    }

    if(color_type == PNG_COLOR_TYPE_RGB){
        DMI_WIDTH *= 3;
    }
    int frames_in_state, frame_tracker = 1;
    int dest_row = 0, dest_col = 0, source_row = 0, source_col = 0;
    int start_col = 0, start_row = 0;
    int row_offset = 0, col_offset = 0;
    while(state_tracker < (dmi->num_of_states)) {
        frames_in_state = dmi->begin_icon_state->dirs * dmi->begin_icon_state->frames;
        for(int i = 0; i < frames_in_state; i++){
            int copy_col = start_col + source_col + col_offset;
            int copy_row = start_row + source_row + row_offset;
            if(copy_col >= pngWidth){
                copy_row += (copy_col/pngWidth) * DMI_HEIGHT;
                copy_col = copy_col % pngWidth;
            }
            Get_Frame(new_pointer, orig_pointer, dest_row, dest_col,
                      copy_row, copy_col, DMI_WIDTH, DMI_HEIGHT);
            dest_col += DMI_WIDTH;
            if(dmi->begin_icon_state->dirs > 1){
                source_col += DMI_WIDTH * dmi->begin_icon_state->frames;
            }
            else {
                source_col += DMI_WIDTH;
            }
            if(source_col >= pngWidth){
                source_row += DMI_HEIGHT;
                source_col = (source_col % pngWidth);
            }
            if(frame_tracker >= dmi->begin_icon_state->frames){
                frame_tracker = 1;
                dest_row += DMI_HEIGHT;
                dest_col = 0;
                col_offset += DMI_WIDTH;
                source_row = 0;
                source_col = 0;
                if(col_offset >= pngWidth){
                    col_offset = (col_offset % pngWidth);
                    row_offset += DMI_HEIGHT;
                }
            }
            else
                frame_tracker++;
        }
        total_frames += frames_in_state;
        dmi->begin_icon_state++;
        state_tracker++;
        col_offset = 0;
        row_offset = 0;
        source_col = 0;
        source_row = 0;
        start_col = (total_frames * DMI_WIDTH) % pngWidth;
        start_row = ((total_frames * DMI_WIDTH) / pngWidth) * DMI_HEIGHT;
    }
    return 1;
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
             //   int index = row[x];

                int index, ppb = 8/bit_depth;    // pixels per byte
                switch (bit_depth){
                    default: printf("bit depth %d not implemented\n", bit_depth); exit(1);
                    case 1:
                    case 2:
                    case 4:
                    case 8: index = row[x/ppb]>>8-(x%ppb+1)*bit_depth&255>>8-bit_depth;
                }
                /* First, get the width.
                 * Divide width by the pixels per bit.
                 * Shift
                 * */

                printf("8-bit_depth = %d\n", 8-bit_depth);
                printf("Index = %d\n", index);
                if(y >= 5){
                    sleep(10);
                }
                //sleep(10);
                if (index < num_palette) {
                    png_color palette_color = palette[index];
                    printf(" >>> Pixel at (%d, %d): Palette Index=%d, R=%d, G=%d, B=%d\n", x, y, index,
                           palette_color.red, palette_color.green, palette_color.blue);

                } else {
                    printf("Index = %d\n", index);
                    printf("Pixel at (%d, %d): Palette index out of range\n", x, y);
                    //row[x] = 0;
                    sleep(10);


               }
            }

            else {
                printf("Index = %d", row[x]);
                printf("Pixel at (%d, %d): Color type not supported\n", x, y);
            }
        }
    }
}


void output_pixel_values2(png_structp png_ptr, png_infop info_ptr, png_bytep *row_pointers, int start_row, int between1,
                          int between2, char type) {
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
                        printf("%c - Pixel at (%d, %d): Palette Index=%d, R=%d, G=%d, B=%d\n",type, x, y, index,
                               palette_color.red, palette_color.green, palette_color.blue);

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