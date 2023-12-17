
#include "PixelManip.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
png_bytep Get_Pixel(png_bytepp image, int x_coord, int y_coord, int color_type, int bit_depth, png_colorp palette,
                    png_bytep trans_alpha, int *num_trans){

    int pixel_index;
    png_bytep pixel_value = NULL;
    Initialize_Pixel(&pixel_value, color_type, bit_depth);

    if(bit_depth == 4){
        pixel_index = (int)floor(x_coord/2);
    }

    else if(bit_depth == 2){
        pixel_index = (int)floor(x_coord/4);
    }

    else if(bit_depth == 1){
        pixel_index = (int)floor(x_coord/8);
    }

    if(color_type == PNG_COLOR_TYPE_PALETTE){
        png_byte palette_index = image[pixel_index][y_coord];
        png_color selected_color = palette[palette_index];
        pixel_value[0] = selected_color.red;
        pixel_value[1] = selected_color.green;
        pixel_value[2] = selected_color.blue;

        if(palette_index < (*num_trans)){
            pixel_value[3] = trans_alpha[palette_index];
        }
        else
            pixel_value[3] = 255;

    }
    else {
        Copy_Pixel(pixel_value, image, x_coord, y_coord, color_type, bit_depth);

    }

    if(bit_depth == 4){
        int which_pixel = x_coord % 2;
        if(which_pixel == 0){
            //simply move 4 bits to the right
        }

        else {
            //simply cancel out the first 4 bits using AND operator
            //then store the value that remains
            //BIT4_L_OFF
        }
    }



    return pixel_value;
}


png_bytep Set_Pixel(){


}

void Copy_Pixel(png_bytep copy_destination, png_bytepp copy_source, int x_coord,
                int y_coord, int color_type, int bit_depth){

    switch(color_type){
        case PNG_COLOR_TYPE_RGB:
            if(bit_depth == 16){
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 6);
            }

            else {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 3);
            }
            break;

        case PNG_COLOR_TYPE_RGBA:
            if(bit_depth == 16) {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 8);
            }

            else {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 4);
            }
            break;

        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if(bit_depth == 16) {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 4);
            }
            else {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 2);
            }
            break;

        case PNG_COLOR_TYPE_GRAY:
            if(bit_depth == 16) {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 2);
            }

            else {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 1);
            }
            break;

        default:
            memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 1);
            break;
    }

}

png_channels Get_Channels(const png_byte* pixel, int pixel_color_type, int pixel_bit_depth){
    png_channels return_channel;
    switch(pixel_color_type){
        case PNG_COLOR_TYPE_RGBA:
            if(pixel_bit_depth == 16){
                int red_first_byte = pixel[0] << 8;
                int red_second_byte = pixel[1];
                int red_resultant = red_first_byte + red_second_byte;

                return_channel.red = red_resultant;

                int green_first_byte = pixel[2] << 8;
                int green_second_byte = pixel[3];
                int green_resultant = green_first_byte + green_second_byte;

                return_channel.green = green_resultant;



                int blue_first_byte = pixel[4] << 8;
                int blue_second_byte = pixel[5];
                int blue_resultant = blue_first_byte + blue_second_byte;
                return_channel.blue = blue_resultant;


                int alpha_first_byte = pixel[6] << 8;
                int alpha_second_byte = pixel[7];
                int alpha_resultant = alpha_first_byte + alpha_second_byte;
                return_channel.alpha = alpha_resultant;

            }

            else {
                return_channel.red = pixel[0];
                return_channel.green = pixel[1];
                return_channel.blue = pixel[2];
                return_channel.alpha = pixel[3];
            }

            break;
        case PNG_COLOR_TYPE_RGB:
            if(pixel_bit_depth == 16){
                int red_first_byte = pixel[0] << 8;
                int red_second_byte = pixel[1];
                int red_resultant = red_first_byte + red_second_byte;

                return_channel.red = red_resultant;

                int green_first_byte = pixel[2] << 8;
                int green_second_byte = pixel[3];
                int green_resultant = green_first_byte + green_second_byte;

                return_channel.green = green_resultant;



                int blue_first_byte = pixel[4] << 8;
                int blue_second_byte = pixel[5];
                int blue_resultant = blue_first_byte + blue_second_byte;
                return_channel.blue = blue_resultant;
                return_channel.alpha = 255;
            }

            else {
                return_channel.red = pixel[0];
                return_channel.green = pixel[1];
                return_channel.blue = pixel[2];
                return_channel.alpha = 255;
            }
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if(pixel_bit_depth == 16){
                int red_first_byte = pixel[0] << 8;
                int red_second_byte = pixel[1];
                int red_resultant = red_first_byte + red_second_byte;

                return_channel.red = red_resultant;
                return_channel.green = red_resultant;
                return_channel.blue = red_resultant;

                int alpha_first_byte = pixel[2] << 8;
                int alpha_second_byte = pixel[3];
                int alpha_resultant = alpha_first_byte + alpha_second_byte;
                return_channel.alpha = alpha_resultant;
            }

            else {
                return_channel.red = pixel[0];
                return_channel.green = pixel[0];
                return_channel.blue = pixel[0];
                return_channel.alpha = pixel[1];
            }
            break;
        case PNG_COLOR_TYPE_GRAY:
            if(pixel_bit_depth == 16){
                int red_first_byte = pixel[0] << 8;
                int red_second_byte = pixel[1];
                int red_resultant = red_first_byte + red_second_byte;

                return_channel.red = red_resultant;
                return_channel.green = red_resultant;
                return_channel.blue = red_resultant;
            }
            else {
                return_channel.red = pixel[0];
                return_channel.green = pixel[0];
                return_channel.blue = pixel[0];
            }

            break;

        default:
            break;
    }

    return return_channel;
}
png_color Byte_To_Color(const png_byte* pixel, int color_type, png_bytep trans_byte){
    png_color palette_pixel;
    if(color_type == PNG_COLOR_TYPE_RGBA){
        palette_pixel.red = pixel[0];
        palette_pixel.green = pixel[1];
        palette_pixel.blue = pixel[2];
        *trans_byte = pixel[3];
    }

    else if(color_type == PNG_COLOR_TYPE_RGB){
        palette_pixel.red = pixel[0];
        palette_pixel.green = pixel[1];
        palette_pixel.blue = pixel[2];
    }
    else if(color_type == PNG_COLOR_TYPE_GRAY){
        palette_pixel.red = pixel[0];
        palette_pixel.green = pixel[0];
        palette_pixel.blue = pixel[0];
    }
    else if(color_type == PNG_COLOR_TYPE_GRAY_ALPHA){
        palette_pixel.red = pixel[0];
        palette_pixel.green = pixel[0];
        palette_pixel.blue = pixel[0];
        *trans_byte = pixel[1];
    }
    else {
        palette_pixel.red = 1;
        palette_pixel.green = 1;
        palette_pixel.blue = 1;
    }
    return palette_pixel;
}

void Initialize_Pixel(png_bytepp pixel, int color_type, int bit_depth){
    switch(color_type){
        case PNG_COLOR_TYPE_RGB:
            if(bit_depth == 16){
                (*pixel) = (png_bytep)malloc(sizeof(png_byte) * 6);
            }

            else {
                (*pixel) = (png_bytep)malloc(sizeof(png_byte) * 3);
            }
            break;

        case PNG_COLOR_TYPE_RGBA:
            if(bit_depth == 16) {
                (*pixel) = (png_bytep)malloc(sizeof(png_byte) * 8);
            }

            else {
                (*pixel) = (png_bytep)malloc(sizeof(png_byte) * 4);
            }
            break;

        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if(bit_depth == 16) {
                (*pixel) = (png_bytep)malloc(sizeof(png_byte) * 4);
            }
            else {
                (*pixel) = (png_bytep)malloc(sizeof(png_byte) * 2);
            }
            break;

        case PNG_COLOR_TYPE_GRAY:
            if(bit_depth == 16) {
                (*pixel) = (png_bytep)malloc(sizeof(png_byte) * 2);
            }

            else {
                (*pixel) = (png_bytep)malloc(sizeof(png_byte) * 1);
            }
            break;

        default:
            (*pixel) = (png_bytep)malloc(sizeof(png_byte) * 4);
            break;
    }
}
void TransformPixel(png_structp index_png_ptr, png_infop index_info_ptr, int source_color_type, int target_color_type){

}
png_bytep Transform_RBGA_PNG(const png_byte* pixel, int target_color_type, int src_bit_depth, int target_bit_depth) {

    png_bytep target_pixel;
    Initialize_Pixel(&target_pixel, target_color_type, target_bit_depth);

    if(target_color_type == PNG_COLOR_TYPE_RGB) {
        /* If the bit depth for RGBA is 16, and RGB is 16.
         *      - Then I simply copy the first 6 bytes from the RGBA byte.
         *
         *  If bit_depth for RGBA is 16, and RGB is 8.
         *      - Then I have to run code to get the value of each channel.
         *      - Then I must move the value >> 8 to the right.
         *
         *  if RGBA bit_depth is 8 and RGB is 16.
         *      - Then I take the value of each channel and << to the left.
         *
         *  if RGBA bit_depth is 8 and RGB is 8.
         *      - Then I just copy the first 3 bytes.
         *
         *  */
        int depth_difference = src_bit_depth - target_bit_depth;
        if(depth_difference == 0){
            //check bit_depth. If both are 16, copy first 6 bytes to rgba.
            //otherwise both are 8, and you copy first 3 bytes
            if(src_bit_depth == 16){
                memcpy(target_pixel, pixel, sizeof(png_byte) * 6);
            }

            else {
                memcpy(target_pixel, pixel, sizeof(png_byte) * 3);
            }

        }
        else if(depth_difference > 0){
            //run code to get the total value from a channel
            //then right_shift by the amount of the depth_difference
            //Then we copy that value to the target_pixel appropriate channelo

            png_channels rgba_channels = Get_Channels(pixel, PNG_COLOR_TYPE_RGBA ,src_bit_depth);
            /* Get Red Channel.
             * Get Green Channel
             * Get Bluue Channel */

            /* Red channel >> depth_difference */
            /* Blue Channel >> depth_difference */
            /* Green channel >> depth_difference */
        }
    }

    else if(target_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        double gray_value = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
        target_pixel[0] = (png_byte)gray_value;
        target_pixel[1] = pixel[3];
    }

    else if(target_color_type == PNG_COLOR_TYPE_GRAY){
        double gray_value = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
        target_pixel[0] = (png_byte)gray_value;
    }
    else {
        target_pixel[0] = 1;
        target_pixel[1] = 1;
        target_pixel[2] = 1;
        target_pixel[3] = 255;
    }
    return target_pixel;
}

png_bytep Transform_GRAYALPHA_PNG(const png_byte* pixel, int target_color_type, int bit_depth) {
    png_bytep target_pixel;
    Initialize_Pixel(&target_pixel, target_color_type, bit_depth);

    if(target_color_type == PNG_COLOR_TYPE_RGBA) {
        target_pixel[0] = pixel[0];
        target_pixel[1] = pixel[0];
        target_pixel[2] = pixel[0];
        target_pixel[3] = pixel[1];
    }
    else if(target_color_type == PNG_COLOR_TYPE_RGB) {
        target_pixel[0] = pixel[0];
        target_pixel[1] = pixel[0];
        target_pixel[2] = pixel[0];
    }

    else if(target_color_type == PNG_COLOR_TYPE_GRAY){
        target_pixel[0] = pixel[0];
    }
    else {
        target_pixel[0] = 1;
        target_pixel[1] = 1;
        target_pixel[2] = 1;
    }
    return target_pixel;
}


png_bytep Transform_GRAY_PNG(const png_byte* pixel, int target_color_type, int bit_depth) {
    png_bytep target_pixel;
    Initialize_Pixel(&target_pixel, target_color_type, bit_depth);

    if(target_color_type == PNG_COLOR_TYPE_RGBA) {
        memcpy(target_pixel, pixel, sizeof(png_byte) * 3);
        target_pixel[3] = 255;
    }
    else if(target_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        double gray_value = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
        target_pixel[0] = (png_byte)gray_value;
        target_pixel[1] = 255;
    }

    else if(target_color_type == PNG_COLOR_TYPE_GRAY) {
        double gray_value = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
        target_pixel[0] = (png_byte)gray_value;
    }
    else {
        target_pixel[0] = 1;
        target_pixel[1] = 1;
        target_pixel[2] = 1;
    }

    return target_pixel;
}

png_bytep Transform_RBG_PNG(const png_byte* pixel, int target_color_type, int bit_depth) {
    png_bytep target_pixel;
    Initialize_Pixel(&target_pixel, target_color_type, bit_depth);

    if(target_color_type == PNG_COLOR_TYPE_RGBA) {
        memcpy(target_pixel, pixel, sizeof(png_byte) * 3);
        target_pixel[3] = 255;
    }
    else if(target_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        double gray_value = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
        target_pixel[0] = (png_byte)gray_value;
        target_pixel[1] = 255;
    }

    else if(target_color_type == PNG_COLOR_TYPE_GRAY){
        double gray_value = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
        target_pixel[0] = (png_byte)gray_value;
    }
    else {
        target_pixel[0] = 1;
        target_pixel[1] = 1;
        target_pixel[2] = 1;
    }
    return target_pixel;
}

png_bytep Transform_Indexed_PNG(png_color pixel, int target_color_type, int bit_depth){
    png_bytep target_pixel;
    Initialize_Pixel(&target_pixel, target_color_type, bit_depth);

    if(target_color_type == PNG_COLOR_TYPE_RGB) {
        target_pixel[0] = pixel.red;
        target_pixel[1] = pixel.green;
        target_pixel[2] = pixel.blue;
    }
    else if(target_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        double gray_value = 0.299 * pixel.red + 0.587 * pixel.green + 0.114 * pixel.blue;
        target_pixel[0] = (png_byte)gray_value; // Grayscale value
        target_pixel[1] = 150; // Alpha value
    }

    else if(target_color_type == PNG_COLOR_TYPE_RGBA) {
        target_pixel[0] = pixel.red;
        target_pixel[1] = pixel.green;
        target_pixel[2] = pixel.blue;
        target_pixel[3] = 255;
    }

    else if(target_color_type == PNG_COLOR_TYPE_GRAY){
        double gray_value = 0.299 * pixel.red + 0.587 * pixel.green + 0.114 * pixel.blue;
        target_pixel[0] = (png_byte)gray_value;
    }
    else {
        target_pixel[0] = 1;
        target_pixel[1] = 1;
        target_pixel[2] = 1;
    }
    return target_pixel;
}
void Combine_Pixels(int color_type){
    /*
    PNG_COLOR_TYPE_PALETTE;
    PNG_COLOR_TYPE_RGBA;
    PNG_COLOR_TYPE_RGB;
    PNG_COLOR_TYPE_GRAY_ALPHA;
    PNG_COLOR_TYPE_GRAY;
    */

    /*This will be a function that will be used to combine pixels from */
    if(color_type == PNG_COLOR_TYPE_PALETTE){
        /* If BOTH pixels are from palette image:
         *      A) png_get_tRNS(read_png_ptr, read_info_ptr, &trans_alpha, &num_trans, &trans_color); -- For both images
         *          1) We run a check on each increment along the pixel_array to see if a pixel is a tRNS bit.
         *              if it is, then we know that if the next or preceding pixel is NOT trns, then it's given
         *              precedence.
         *      B) If we confirm that in either the images, the current pixel is not a tRNS. Then we take the pixel
         *          from prime_image
         * */

        /*
         * All pixels will follow the hierarchy. If there's a mix,. then the image is converted with the following
         * scheme:
         *      A) INDEX_IMAGE + TrueColour(RGB)
         *          - If Index Image has an alpha value, then the new image will True Colour with Alpha (RGBA)
         *          - Otherwise, new image will be TrueColour (RGB)
         *      B) INDEX_IMAGE + INDEX_IMAGE
         *          - If the combined # of unique colors exceeds 256, then this becomes either:
         *              - Truecolor [RGB] (In the case that there are no transparent colors)
         *              - True Colour with Alpha [RGBA] in the cases where one or more colors are partially
         *                  transparent
         *          - If not, then the resulting color will simply be an INDEX image as well.
         *              - In this scenario, we copy the trNS chunk from the image going on top.
         *
         *      C) INDEX + True Colour with Alpha(RGBA)
         *          - Convert to RGBA no matter what.
         *
         *      D) INDEX + GREY SCALE WITH ALPHA
         *          - Convert to RGBA (we presume an Index Image is indexed only when required)
         *
         *      E) INDEX + GREYSCALE
         *          - Convert to RGBA
         * */
    }
}