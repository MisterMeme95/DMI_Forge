#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

void print_rgb(png_byte *pixel) {
    printf("RGB(%d, %d, %d)\n", pixel[0], pixel[1], pixel[2]);
}


void read_png_file(const char *filename, png_bytepp *image, int *width, int *height, int *color_type, int *bit_depth) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Failed to open file %s for reading\n", filename);
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fprintf(stderr, "Failed to create png read struct\n");
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        fprintf(stderr, "Failed to create png info struct\n");
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png))) {

        fprintf(stderr, "Error during png creation\n");
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    *color_type = png_get_color_type(png, info);
    *bit_depth = png_get_bit_depth(png, info);

    if (*bit_depth == 16) {
        png_set_strip_16(png);
    }

    if (*color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
    }

    if (*color_type == PNG_COLOR_TYPE_GRAY && *bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png);
    }

    if (png_get_valid(png, info, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png);
    }

    if (*color_type == PNG_COLOR_TYPE_RGB || *color_type == PNG_COLOR_TYPE_GRAY || *color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }

    if (*color_type == PNG_COLOR_TYPE_GRAY || *color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);

    *image = (png_bytepp)malloc(sizeof(png_bytep) * (*height));
    for (int y = 0; y < *height; y++) {
        (*image)[y] = (png_bytep)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, *image);

    fclose(fp);
    png_destroy_read_struct(&png, &info, NULL);
}

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <string.h>
#include "dmi.h"
#include "PixelManip.h"

// Function declarations
Frame_Array Frame_Partition(png_bytepp image, int bit_depth, int png_width, int png_height, int frame_width,
                            int frame_height, int color_type);
void read_png_file(const char *filename, png_bytepp *image, int *width, int *height, int *color_type, int *bit_depth);

void print_frame(Frame frame, int frame_width, int frame_height) {
    for (int row = 0; row < frame_height; row++) {
        for (int col = 0; col < frame_width * 4; col++) { // Assuming RGBA
            printf("%02x ", frame.frame_data[row][col]);
        }
        printf("\n");
    }
}

int compare_frames(Frame_Array frames, png_bytepp image, int png_width, int png_height,
                   int frame_width, int frame_height, int pixel_size) {
    int is_false = 1;

    for (int y = 0; y < png_height; y++) {
        for (int x = 0; x < png_width; x++) {
            int frame_row = y / frame_height;
            int frame_col = x / frame_width;
            int frame_index = frame_row * (png_width / frame_width) + frame_col;

            int pixel_row_in_frame = y % frame_height;
            int pixel_col_in_frame = x % frame_width;

            Frame frame = frames.array[frame_index];

            png_byte *frame_pixel = &frame.frame_data[pixel_row_in_frame][pixel_col_in_frame * pixel_size];
            png_byte *image_pixel = &image[y][x * pixel_size];

            if (memcmp(frame_pixel, image_pixel, pixel_size) != 0) {
                printf("Mismatch at pixel (%d, %d): frame_pixel = ", x, y);
                print_rgb(frame_pixel);
                printf(", image_pixel = ");
                print_rgb(image_pixel);
                printf("\n");
                is_false = 0;
            }
        }
    }

    return is_false;
}


void run_tests(const char *filename) {
    // Read the PNG file
    png_bytepp image;
    int png_width, png_height, bit_depth, color_type;
    read_png_file(filename, &image, &png_width, &png_height, &color_type, &bit_depth);

    // Example frame dimensions
    int frame_width = 64;
    int frame_height = 64;
    int pixel_size = Get_Pixel_Size(bit_depth, color_type);

    // Run Frame_Partition and get the frames
    Frame_Array frames = Frame_Partition(image, bit_depth, png_width, png_height, frame_width, frame_height, color_type);

    // Verify the frames
    if (compare_frames(frames, image, png_width, png_height, frame_width, frame_height, pixel_size)) {
        printf("Test passed: Frames match the original image\n");
    } else {
        printf("Test failed: Frames do not match the original image\n");
    }

    int frame_num = calculate_frame_count(frame_width, frame_height, png_width, png_height);
    // Free allocated memory (frames and image)
    for (int i = 0; i < frame_num; i++) {
        for (int j = 0; j < frame_height; j++) {
            free(frames.array[i].frame_data[j]);
        }
        free(frames.array[i].frame_data);
    }
    free(frames.array);

    for (int i = 0; i < png_height; i++) {
        free(image[i]);
    }
    free(image);
}

int main(int argc, char *argv[]) {

    run_tests("rgb.png");

    return EXIT_SUCCESS;
}
