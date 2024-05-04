
#include "data_structure.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
//Pixel_Data

void Set_Channels(Pixel_Data *pixel) {
    if(pixel->color_type != PNG_COLOR_TYPE_PALETTE) {
        Set_Red_Channel(pixel);
        Set_Green_Channel(pixel);
        Set_Blue_Channel(pixel);
    }
}
void Set_Channel_Value(Pixel_Data *pixel, png_bytep channel, int channel_offset) {
//    printf("Set = %d\n", pixel->bit_depth);
    if (pixel->bit_depth == 8) {
        *channel = pixel->byte_data[channel_offset];
    } else if (pixel->bit_depth == 16) {
        memcpy(channel, &pixel->byte_data_16[channel_offset * 2], sizeof(png_byte) * 2);
        // If your system is little-endian, you might need to swap the bytes
        // *channel = (*channel >> 8) | (*channel << 8);
    }
    else {
        *channel = pixel->byte_data[channel_offset];
    }
}

void Set_Red_Channel(Pixel_Data *pixel) {
    if (pixel->color_type == PNG_COLOR_TYPE_RGBA || pixel->color_type == PNG_COLOR_TYPE_RGB ||
        pixel->color_type == PNG_COLOR_TYPE_GRAY_ALPHA || pixel->color_type == PNG_COLOR_TYPE_GRAY) {
        Set_Channel_Value(pixel, &pixel->red, 0);
    }
}

void Set_Green_Channel(Pixel_Data *pixel) {
    if (pixel->color_type == PNG_COLOR_TYPE_RGBA || pixel->color_type == PNG_COLOR_TYPE_RGB) {
        Set_Channel_Value(pixel, &pixel->green, 1);
    } else if (pixel->color_type == PNG_COLOR_TYPE_GRAY_ALPHA || pixel->color_type == PNG_COLOR_TYPE_GRAY) {
        Set_Channel_Value(pixel, &pixel->green, 0);
    }
}

void Set_Blue_Channel(Pixel_Data *pixel) {
    if (pixel->color_type == PNG_COLOR_TYPE_RGBA || pixel->color_type == PNG_COLOR_TYPE_RGB) {
        Set_Channel_Value(pixel, &pixel->blue, 2);
    } else if (pixel->color_type == PNG_COLOR_TYPE_GRAY_ALPHA || pixel->color_type == PNG_COLOR_TYPE_GRAY) {
        Set_Channel_Value(pixel, &pixel->blue, 0);
    }
}


int Get_Red_Channel(Pixel_Data pixel){
    if(pixel.color_type == PNG_COLOR_TYPE_PALETTE){
        return pixel.color_data->red;
    }

    else if(pixel.color_type == PNG_COLOR_TYPE_RGBA || pixel.color_type == PNG_COLOR_TYPE_RGB){
        if(pixel.bit_depth == 8)
            return pixel.byte_data[0];
    }

}

int Get_Green_Channel(Pixel_Data pixel){
    if(pixel.color_type == PNG_COLOR_TYPE_PALETTE){
        return pixel.color_data->green;
    }

    else if(pixel.color_type == PNG_COLOR_TYPE_RGBA || pixel.color_type == PNG_COLOR_TYPE_RGB){
        if(pixel.bit_depth == 8)
            return pixel.byte_data[1];
    }
}

int Get_Blue_Channel(Pixel_Data pixel){
    if(pixel.color_type == PNG_COLOR_TYPE_PALETTE){
        return pixel.color_data->blue;
    }

    else if(pixel.color_type == PNG_COLOR_TYPE_RGBA || pixel.color_type == PNG_COLOR_TYPE_RGB){
        if(pixel.bit_depth == 8)
            return pixel.byte_data[2];
    }

}

int Get_Alpha_Channel(Pixel_Data pixel){
    if(pixel.color_type == PNG_COLOR_TYPE_PALETTE){
        return 255;
    }
    else if(pixel.color_type == PNG_COLOR_TYPE_RGBA || pixel.color_type == PNG_COLOR_TYPE_RGB){
        if(pixel.bit_depth == 8)
            return pixel.byte_data[3];
    }
}

int Get_Gray_Value(Pixel_Data pixel){
    int gray_value;
    if(pixel.color_type == PNG_COLOR_TYPE_PALETTE){
        gray_value = (int)ceil(0.299 * pixel.color_data->red + 0.587 * pixel.color_data->green + 0.114 * pixel.color_data->blue);
    }

    else {
        gray_value = (int)ceil(0.299 * pixel.byte_data[0] + 0.587 * pixel.byte_data[1] + 0.114 * pixel.byte_data[2]);
    }

    return gray_value;
}

Pixel_Data Pixel_Transformation(Pixel_Data pixel, int target_color_type, int target_bit_depth){
    Pixel_Data pixel_transformed;
    pixel_transformed.color_type = target_color_type;
    pixel_transformed.byte_data = NULL;
    pixel_transformed.bit_depth = target_bit_depth;

    Initialize_Pixel(&pixel_transformed.byte_data, target_color_type, target_bit_depth);
    /* If the Pixel is an indexed-pixel, then we have to run very specific operations. */
    if(pixel.color_type == PNG_COLOR_TYPE_PALETTE){
        Transform_Indexed_PNG(pixel, &pixel_transformed, target_color_type,
                              target_bit_depth);
    }
//    printf("Pixel Trans BitDepth = %d\n", pixel_transformed.bit_depth);

    Set_Channels(&pixel_transformed);
    return pixel_transformed;

}
Pixel_Data Get_Pixel(png_bytepp image, int x_coord, int y_coord, int color_type, int bit_depth, png_colorp palette,
                     png_bytep trans_alpha, int *num_trans){

    /** @Description A data structure that serves as a neat encapsulation of the various data-types that libpng
     * stores pixel information in.
     *
     * It is of the "Pixel_Data" struct. */
    Pixel_Data pixel;

    //Let's make sure to initialize it.
    pixel.bit_depth = bit_depth;
    pixel.color_type = color_type;
    pixel.byte_data = NULL;



    /** @Description pixel_index is a simple int variable that we'll use to get the proper x_coordinate.
     * Since it is possible for a pixel to be less than a byte at certain bit_depths.
     *
     * Default value is x_coord. */
    int pixel_index = x_coord;

    /** @Description This value represents the number of pixels packed into a single byte.
     * This is only relevant whenever a pixel's bit_depth is less than 8. */
    int pixels_per_byte;

    /**@Description Pixel Data is the data we extract from the image (the pixel array). After pixel_index is calculated,
     * we save the information in pixel_data, and can modify it depending on the size of the bit_depth to get
     * the useful information.
     *
     * ONLY for indexed/gray images. [Since a pixel can be less than a byte in size]
     * */
    int pixel_data = image[y_coord][pixel_index];

    /** @Description This represents the amount of bits to shift a byte by to unpack the pixel value.
     * By default, it is 0.
     *
     * Once more, only useful for Index/Gray images.*/
    int pixel_bit_shift = 0;

    /** @Description pixel_value is a pointer that we set to represent the various values in the various channels for a
     * png_byte. The size of the pixel_value depends on the color_type and bit_depth. It is set using Initialize_Pixel(). */
    //png_bytep pixel_value = NULL;

    if(pixel.color_type == PNG_COLOR_TYPE_PALETTE) {
        pixel.color_data = (png_colorp)malloc(sizeof(png_color));
    }

    else{
        Initialize_Pixel(&pixel.byte_data, color_type, bit_depth);
    }

    if(bit_depth == 16) {
        if(color_type == PNG_COLOR_TYPE_RGBA){
            pixel_index *= 8;
        }

        else if(color_type == PNG_COLOR_TYPE_RGB){
            pixel_index *= 6;
        }

        else if(color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            pixel_index *= 4;
        }

        /** We run a check here for Gray-scaled images because with 16 bit depth,
         * each pixel will be 2 bytes long. We don't need to check this for 8-bits because
         * each pixel there is 1-byte long. */
        else if(color_type == PNG_COLOR_TYPE_GRAY) {
            pixel_index *= 2;
        }
    }

    if(bit_depth == 8) {
        if(color_type == PNG_COLOR_TYPE_RGBA){
            pixel_index *= 4;
        }

        else if(color_type == PNG_COLOR_TYPE_RGB){
            pixel_index *= 3;
        }

        else if(color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            pixel_index *= 2;
        }
    }

    /* If bit_depth is 4, then we know that there are 2 pixels per byte. So we simply have and floor the value. */
    if(bit_depth == 4) {
        pixels_per_byte = 2;
        int pixel_in_byte = x_coord % pixels_per_byte;
        pixel_bit_shift = (bit_depth) * ((pixels_per_byte - 1) -pixel_in_byte);
        pixel_index = (int)floor(x_coord / pixels_per_byte);
        pixel_data = image[y_coord][pixel_index];

        if(pixel_in_byte == 0) {
            pixel_data = pixel_data & BIT4_L_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        }

        else {
            pixel_data = pixel_data & BIT4_R_ON;
        }

    }

    /** There are 4 pixels per byte, so we floor after dividing by 4. **/
    else if(bit_depth == 2){
        pixels_per_byte = 4;
        int pixel_in_byte = x_coord % pixels_per_byte;

        pixel_bit_shift = (bit_depth) * ((pixels_per_byte - 1) -pixel_in_byte);
        pixel_index = (int)floor(x_coord/pixels_per_byte);
        pixel_data = image[y_coord][pixel_index];

        if(pixel_in_byte == 0){
            pixel_data = pixel_data & BIT2_1_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        }

        else if(pixel_in_byte == 1){
            pixel_data = pixel_data & BIT2_2_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        }

        else if(pixel_in_byte == 2){
            pixel_data = pixel_data & BIT2_3_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        }
        else {
            pixel_data = pixel_data & BIT2_4_ON;
        }
    }

        /* At a bit_depth of 1 we have 8 pixels per byte. */
    else if(bit_depth == 1){
        pixels_per_byte = 8;
        int pixel_in_byte = x_coord % pixels_per_byte;

        pixel_bit_shift = (bit_depth) * ((pixels_per_byte - 1) -pixel_in_byte);
        pixel_index = (int)floor(x_coord / pixels_per_byte);
        pixel_data = image[y_coord][pixel_index];

        if(pixel_in_byte == 0){
            pixel_data = pixel_data & BIT1_1_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        }

        else if(pixel_in_byte == 1){
            pixel_data = pixel_data & BIT1_2_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        }

        else if(pixel_in_byte == 2){
            pixel_data = pixel_data & BIT1_3_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        }

        else if(pixel_in_byte == 3){
            pixel_data = pixel_data & BIT1_4_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        }

        else if(pixel_in_byte == 4){
            pixel_data = pixel_data & BIT1_5_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        }

        else if(pixel_in_byte == 5){
            pixel_data = pixel_data & BIT1_6_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        }

        else if(pixel_in_byte == 6){
            pixel_data = pixel_data & BIT1_7_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        }

        else {
            pixel_data = pixel_data & BIT1_7_ON;
        }

    }



    if(color_type == PNG_COLOR_TYPE_PALETTE){
        //printf("Num_Trans - %d\nPixel_Date = %d\n", *num_trans, pixel_data);

        /** @Description By using the pixel_data that we processed earlier, we now have the palette's index.
         * This allows us to store the data in selected_color.*/
        png_color selected_color = palette[pixel_data];


        /* From there, we simply copy the values from png_color into a png_byte array.*/
        //[0] = selected_color.red;
        //pixel.byte_data[1] = selected_color.green;
        //pixel.byte_data[2] = selected_color.blue;
      //  if(pixel_data != 0){
         //   printf("pixel_data = %d",pixel_data);
           // sleep(10);
        //}



       // printf("Selected color. .(%d, %d, %d)\n", selected_color.red, selected_color.green, selected_color.blue);
        pixel.color_data->red = selected_color.red;//100;
        pixel.color_data->green = selected_color.green;
        pixel.color_data->blue = selected_color.blue;


        /* Next up, if the index that we have in our pixel_data is less than the number of indexes in the
         * trans_alpha array, then we know that our selected color has an alpha value that is not 255.
         * Let's just copy that over. */
        if(pixel_data < (*num_trans)){
            pixel.alpha_channel = 0;
        }

            /* Otherwise, the alpha value is 255.*/
        else
            pixel.alpha_channel = 255;

    }

        /* If it is not an indexed image, then we can simply copy the pixel directly.*/
    else {
        Copy_Pixel(pixel.byte_data, image, pixel_index, y_coord, color_type, bit_depth);

    }


    /* Set up the channels before sending the pixel off. */
    Set_Channels(&pixel);
    return pixel;
}


void Set_Pixel(png_bytepp image, Pixel_Data* new_pixel, int x_coord,
               int y_coord, int color_type, int bit_depth, png_colorp palette,
                png_bytep trans_alpha, int *num_trans, palette_hash* pal_hash){

    /** @Description A data structure that serves as a neat encapsulation of the various data-types that libpng
     * stores pixel information in.
     *
     * It is of the "Pixel_Data" struct. */

    //printf("Set pixel#1\n");


    /** @Description pixel_index is a simple int variable that we'll use to get the proper x_coordinate.
     * Since it is possible for a pixel to be less than a byte at certain bit_depths.
     *
     * Default value is x_coord. */
    int pixel_index = x_coord;

    /** @Description This value represents the number of pixels packed into a single byte.
     * This is only relevant whenever a pixel's bit_depth is less than 8. */
    int pixels_per_byte;

    /**@Description Pixel Data is the data we extract from the image (the pixel array). After pixel_index is calculated,
     * we save the information in pixel_data, and can modify it depending on the size of the bit_depth to get
     * the useful information.
     *
     * ONLY for indexed/gray images. [Since a pixel can be less than a byte in size]
     * */
    int pixel_data = image[y_coord][pixel_index];

    /** @Description This represents the amount of bits to shift a byte by to unpack the pixel value.
     * By default, it is 0.
     *
     * Once more, only useful for Index/Gray images.*/
    int pixel_bit_shift = 0;

    /** @Description pixel_value is a pointer that we set to represent the various values in the various channels for a
     * png_byte. The size of the pixel_value depends on the color_type and bit_depth. It is set using Initialize_Pixel(). */
    //png_bytep pixel_value = NULL;

   // if(pixel.color_type == PNG_COLOR_TYPE_PALETTE) {
     //   pixel.color_data = (png_colorp)malloc(sizeof(png_color));
    //}

    //else{
      //  Initialize_Pixel(&pixel.byte_data, color_type, bit_depth);
    //}

    if(bit_depth == 16) {
        if(color_type == PNG_COLOR_TYPE_RGBA){
            pixel_index *= 8;
        }

        else if(color_type == PNG_COLOR_TYPE_RGB){
            pixel_index *= 6;
        }

        else if(color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            pixel_index *= 4;
        }

            /** We run a check here for Gray-scaled images because with 16 bit depth,
             * each pixel will be 2 bytes long. We don't need to check this for 8-bits because
             * each pixel there is 1-byte long. */
        else if(color_type == PNG_COLOR_TYPE_GRAY) {
            pixel_index *= 2;
        }
    }

    if(bit_depth == 8) {
     //   printf("Set pixel#2\n");
        if(color_type == PNG_COLOR_TYPE_RGBA){
            pixel_index *= 4;
            memcpy(&image[y_coord][pixel_index], new_pixel->byte_data, sizeof(png_byte) * 4);
        }

        else if(color_type == PNG_COLOR_TYPE_RGB){
            pixel_index *= 3;
          //  printf("(%d, %d)\n", y_coord, (int)(pixel_index/3));
            //printf("%d - %d\n", pixel_index, pixel_index+3);
            memcpy(&image[y_coord][pixel_index], new_pixel->byte_data, sizeof(png_byte) * 3);
        }

        else if(color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            pixel_index *= 2;
            memcpy(&image[y_coord][pixel_index], new_pixel->byte_data, sizeof(png_byte) * 2);
        }
        else if(color_type == PNG_COLOR_TYPE_GRAY) {
            memcpy(&image[y_coord][pixel_index], new_pixel->byte_data, sizeof(png_byte));
        }

        else if(color_type == PNG_COLOR_TYPE_PALETTE){
            int get_index = 0;
            get_index = find_key(*new_pixel, pal_hash);

            if(get_index != -1){
                image[y_coord][pixel_index] = get_index;

            }
            else {

                insert_key(*new_pixel, pal_hash, *num_trans);

                palette[*num_trans] = *(new_pixel->color_data);

                image[y_coord][pixel_index] = (png_byte)(*num_trans);
                *(num_trans)+=1;
            }
        }
    }

    /* If bit_depth is 4, then we know that there are 2 pixels per byte. So we simply have and floor the value. */
    if(bit_depth == 4) {
        pixels_per_byte = 2;
        int pixel_in_byte = x_coord % pixels_per_byte;
        pixel_bit_shift = (bit_depth) * ((pixels_per_byte - 1) -pixel_in_byte);
        pixel_index = (int)floor(x_coord / pixels_per_byte);
        //pixel_data = image[y_coord][pixel_index];

        if(pixel_in_byte == 0) {
            *new_pixel->byte_data = (*new_pixel->byte_data) << pixel_bit_shift;
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
           // pixel_data = pixel_data & BIT4_L_ON;
            //pixel_data = (pixel_data >> pixel_bit_shift);
        }

        else {
            //pixel_data = pixel_data & BIT4_R_ON;
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }

    }

        /** There are 4 pixels per byte, so we floor after dividing by 4. **/
    else if(bit_depth == 2){
        pixels_per_byte = 4;
        int pixel_in_byte = x_coord % pixels_per_byte;

        pixel_bit_shift = (bit_depth) * ((pixels_per_byte - 1) -pixel_in_byte);
        pixel_index = (int)floor(x_coord/pixels_per_byte);
       // pixel_data = image[y_coord][pixel_index];

        if(pixel_in_byte == 0){
            *new_pixel->byte_data = (*new_pixel->byte_data) << pixel_bit_shift;
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }

        else if(pixel_in_byte == 1){
            *new_pixel->byte_data = (*new_pixel->byte_data) << pixel_bit_shift;
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }

        else if(pixel_in_byte == 2){
            *new_pixel->byte_data = (*new_pixel->byte_data) << pixel_bit_shift;
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }
        else {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }
    }

        /* At a bit_depth of 1 we have 8 pixels per byte. */
    else if(bit_depth == 1){
        pixels_per_byte = 8;
        int pixel_in_byte = x_coord % pixels_per_byte;

        pixel_bit_shift = (bit_depth) * ((pixels_per_byte - 1) -pixel_in_byte);
        pixel_index = (int)floor(x_coord / pixels_per_byte);
        pixel_data = image[y_coord][pixel_index];

        if(pixel_in_byte == 0){
            *new_pixel->byte_data = (*new_pixel->byte_data) << pixel_bit_shift;
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }

        else if(pixel_in_byte == 1){
            *new_pixel->byte_data = (*new_pixel->byte_data) << pixel_bit_shift;
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }

        else if(pixel_in_byte == 2){
            *new_pixel->byte_data = (*new_pixel->byte_data) << pixel_bit_shift;
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }

        else if(pixel_in_byte == 3){
            *new_pixel->byte_data = (*new_pixel->byte_data) << pixel_bit_shift;
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }

        else if(pixel_in_byte == 4){
            *new_pixel->byte_data = (*new_pixel->byte_data) << pixel_bit_shift;
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }

        else if(pixel_in_byte == 5){
            *new_pixel->byte_data = (*new_pixel->byte_data) << pixel_bit_shift;
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }

        else if(pixel_in_byte == 6){
            *new_pixel->byte_data = (*new_pixel->byte_data) << pixel_bit_shift;
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }

        else {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index])^(*new_pixel->byte_data);
        }

    }



//    if(color_type == PNG_COLOR_TYPE_PALETTE){
//        printf("Num_Trans - %d\nPixel_Date = %d\n", *num_trans, pixel_data);
//
//        /** @Description By using the pixel_data that we processed earlier, we now have the palette's index.
//         * This allows us to store the data in selected_color.*/
//        png_color selected_color = palette[pixel_data];
//
//
//        /* From there, we simply copy the values from png_color into a png_byte array.*/
//        //[0] = selected_color.red;
//        //pixel.byte_data[1] = selected_color.green;
//        //pixel.byte_data[2] = selected_color.blue;
//
//        pixel.color_data->red = selected_color.red;
//        pixel.color_data->green = selected_color.green;
//        pixel.color_data->blue = selected_color.blue;
//
//
//        /* Next up, if the index that we have in our pixel_data is less than the number of indexes in the
//         * trans_alpha array, then we know that our selected color has an alpha value that is not 255.
//         * Let's just copy that over. */
//        if(pixel_data < (*num_trans)){
//            pixel.alpha_channel = trans_alpha[pixel_data];
//        }
//
//            /* Otherwise, the alpha value is 255.*/
//        else
//            pixel.alpha_channel = 255;
//
//    }
//
//        /* If it is not an indexed image, then we can simply copy the pixel directly.*/
//    else {
//        // pixel_data = image[yx_coord][pixel_index];
//        Copy_Pixel(pixel.byte_data, image, pixel_index, y_coord, color_type, bit_depth);
//
//    }

}


/**
 * @INCOMPLETE_FUNCTION - Make it able to work with bit_depth values less than 8 for Gray Scaled Image, and Index
 * images.
 *
 * ------\n
 *
 * Actually I'm dumb, it already works for those images. Since when we get the pixel data, we have to push
 * the relevant pixel to the end anyway,.
 *
 * */
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


/* May be redundant. */
png_channels Get_Channels(const Pixel_Data pixel, int pixel_color_type, int pixel_bit_depth){
    png_channels return_channel;

    return_channel.red = Get_Red_Channel(pixel);
    return_channel.green = Get_Red_Channel(pixel);
    return_channel.blue = Get_Red_Channel(pixel);
    return_channel.alpha = Get_Alpha_Channel(pixel);

    return return_channel;
    /*
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
     */
}
/** I think this is to get a png_color from a png_byte. */
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
                (*pixel) = (png_bytep)calloc(6, sizeof(png_byte));
            }
            else {
                (*pixel) = (png_bytep)calloc(3, sizeof(png_byte));
            }
            break;

        case PNG_COLOR_TYPE_RGBA:
            if(bit_depth == 16) {
                (*pixel) = (png_bytep)calloc(8, sizeof(png_byte));
            }
            else {
                (*pixel) = (png_bytep)calloc(4, sizeof(png_byte));
            }
            break;

        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if(bit_depth == 16) {
                (*pixel) = (png_bytep)calloc(4, sizeof(png_byte));
            }
            else {
                (*pixel) = (png_bytep)calloc(2, sizeof(png_byte));
            }
            break;

        case PNG_COLOR_TYPE_GRAY:
            if(bit_depth == 16) {
                (*pixel) = (png_bytep)calloc(2, sizeof(png_byte));
            }
            else {
                (*pixel) = (png_bytep)calloc(1, sizeof(png_byte));
            }
            break;

        default:
            (*pixel) = (png_bytep)calloc(4, sizeof(png_byte));
            break;
    }
}

Pixel_Data Transform_RBGA_PNG2(Pixel_Data pixel, int target_color_type, int src_bit_depth, int target_bit_depth) {

    Pixel_Data target_pixel;
    Initialize_Pixel(&target_pixel.byte_data, target_color_type, target_bit_depth);

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
                memcpy(target_pixel.byte_data, pixel.byte_data, sizeof(png_byte) * 6);
            }

            else {
                memcpy(target_pixel.byte_data, pixel.byte_data, sizeof(png_byte) * 3);
            }

        }
        else {
            if(src_bit_depth == 16){
                target_pixel.byte_data[0] = target_pixel.byte_data[0] >> 8;
                target_pixel.byte_data[2] = target_pixel.byte_data[2] >> 8;
                target_pixel.byte_data[4] = target_pixel.byte_data[4] >> 8;
                memcpy(target_pixel.byte_data, pixel.byte_data, sizeof(png_byte) * 3);
             }

            else {
                target_pixel.byte_data[0] = target_pixel.byte_data[1] << 8;
                target_pixel.byte_data[1] = target_pixel.byte_data[2] << 8;
                target_pixel.byte_data[2] = target_pixel.byte_data[3] << 8;
                memcpy(target_pixel.byte_data, pixel.byte_data, sizeof(png_byte) * 6);
            }
        }
    }

    else if(target_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        //double gray_value = 0.299 * pixel.byte_data[0] + 0.587 * pixel.byte_data[1] + 0.114 * pixel.byte_data[2];
        int gray_value = Get_Gray_Value(pixel);
        target_pixel.byte_data[0] = (png_byte)gray_value;
        target_pixel.byte_data[1] = pixel.byte_data[3];
    }

    else if(target_color_type == PNG_COLOR_TYPE_GRAY){
        //double gray_value = 0.299 * pixel.byte_data[0] + 0.587 * pixel.byte_data[1] + 0.114 * pixel.byte_data[2];
        int gray_value = Get_Gray_Value(pixel);
        target_pixel.byte_data[0] = (png_byte)gray_value;
    }
    else {
        target_pixel.byte_data[0] = 1;
        target_pixel.byte_data[1] = 1;
        target_pixel.byte_data[2] = 1;
        target_pixel.byte_data[3] = 255;
    }
    return target_pixel;
}



//png_bytep Transform_RBGA_PNG(const png_byte* pixel, int target_color_type, int src_bit_depth, int target_bit_depth) {
//
//    png_bytep target_pixel;
//    Initialize_Pixel(&target_pixel, target_color_type, target_bit_depth);
//
//    if(target_color_type == PNG_COLOR_TYPE_RGB) {
//        /* If the bit depth for RGBA is 16, and RGB is 16.
//         *      - Then I simply copy the first 6 bytes from the RGBA byte.
//         *
//         *  If bit_depth for RGBA is 16, and RGB is 8.
//         *      - Then I have to run code to get the value of each channel.
//         *      - Then I must move the value >> 8 to the right.
//         *
//         *  if RGBA bit_depth is 8 and RGB is 16.
//         *      - Then I take the value of each channel and << to the left.
//         *
//         *  if RGBA bit_depth is 8 and RGB is 8.
//         *      - Then I just copy the first 3 bytes.
//         *
//         *  */
//        int depth_difference = src_bit_depth - target_bit_depth;
//        if(depth_difference == 0){
//            //check bit_depth. If both are 16, copy first 6 bytes to rgba.
//            //otherwise both are 8, and you copy first 3 bytes
//            if(src_bit_depth == 16){
//                memcpy(target_pixel, pixel, sizeof(png_byte) * 6);
//            }
//
//            else {
//                memcpy(target_pixel, pixel, sizeof(png_byte) * 3);
//            }
//
//        }
//        else if(depth_difference > 0){
//            //run code to get the total value from a channel
//            //then right_shift by the amount of the depth_difference
//            //Then we copy that value to the target_pixel appropriate channelo
//
//            png_channels rgba_channels = Get_Channels(pixel, PNG_COLOR_TYPE_RGBA ,src_bit_depth);
//            /* Get Red Channel.
//             * Get Green Channel
//             * Get Bluue Channel */
//
//            /* Red channel >> depth_difference
//            Blue Channel >> depth_difference
//            Green channel >> depth_difference */
//        }
//    }
//
//    else if(target_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
//        double gray_value = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
//        target_pixel[0] = (png_byte)gray_value;
//        target_pixel[1] = pixel[3];
//    }
//
//    else if(target_color_type == PNG_COLOR_TYPE_GRAY){
//        double gray_value = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
//        target_pixel[0] = (png_byte)gray_value;
//    }
//    else {
//        target_pixel[0] = 1;
//        target_pixel[1] = 1;
//        target_pixel[2] = 1;
//        target_pixel[3] = 255;
//    }
//    return target_pixel;
//}

png_bytep Transform_GRAYALPHA_PNG(const png_byte* pixel, int target_color_type, int target_bit_depth) {
    png_bytep target_pixel;
    Initialize_Pixel(&target_pixel, target_color_type, target_bit_depth);

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


void Transform_Indexed_PNG(Pixel_Data pixel, Pixel_Data* new_pixel, int target_color_type, int target_bit_depth){
    png_bytep target_pixel;
    Initialize_Pixel(&target_pixel, target_color_type, target_bit_depth);
    if(target_color_type == PNG_COLOR_TYPE_RGB) {
        if(target_bit_depth == 8){
            target_pixel[0] = pixel.color_data->red;
            target_pixel[1] = pixel.color_data->green;
            target_pixel[2] = pixel.color_data->blue;
            memcpy(new_pixel->byte_data, target_pixel, sizeof(png_byte) * 3);
        }
        else {
            target_pixel[0] = pixel.color_data->red;
            target_pixel[2] = pixel.color_data->green;
            target_pixel[4] = pixel.color_data->blue;
            memcpy(new_pixel->byte_data, target_pixel, sizeof(png_byte) * 6);
        }

        new_pixel->color_type = PNG_COLOR_TYPE_RGB;

        //new_pixel->byte_data = target_pixel;
    }

    else if(target_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        //double gray_value = 0.299 * pixel.color_data->red + 0.587 * pixel.color_data->green + 0.114 * pixel.color_data->blue;
        int gray_value = Get_Gray_Value(pixel);
        if(target_bit_depth == 8){
            target_pixel[0] = (png_byte)gray_value; // Grayscale value
            target_pixel[1] = pixel.alpha_channel; // Alpha value
        }

        else {
            target_pixel[0] = (png_byte)gray_value; // Grayscale value
            target_pixel[2] = pixel.alpha_channel; // Alpha value
        }

        new_pixel->color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
        new_pixel->byte_data = target_pixel;
    }

    else if(target_color_type == PNG_COLOR_TYPE_RGBA) {

        if(target_bit_depth == 8){
            target_pixel[0] = pixel.color_data->red;
            target_pixel[1] = pixel.color_data->green;
            target_pixel[2] = pixel.color_data->blue;
            target_pixel[3] = pixel.alpha_channel;
        }

        else {
            target_pixel[0] = pixel.color_data->red;
            target_pixel[2] = pixel.color_data->green;
            target_pixel[4] = pixel.color_data->blue;
            target_pixel[6] = pixel.alpha_channel;
        }

        new_pixel->color_type = PNG_COLOR_TYPE_RGBA;
        new_pixel->byte_data = target_pixel;

    }

    else if(target_color_type == PNG_COLOR_TYPE_GRAY){
        /*Color transformation. Then copy that value to first entry. */
        double gray_value = 0.299 * pixel.color_data->red + 0.587 * pixel.color_data->green + 0.114 * pixel.color_data->blue;
        if(target_bit_depth == 8){
      //      printf("Gray value = %f\n", gray_value);
            target_pixel[0] = (png_byte)((int)gray_value);
        }

        else {
            target_pixel[0] = (png_byte)gray_value;
        }

        new_pixel->color_type = PNG_COLOR_TYPE_GRAY;
        new_pixel->byte_data = target_pixel;

    }

    else if(target_color_type == PNG_COLOR_TYPE_PALETTE){
        /* new_pixel needs to copy pixel.
         * */
        memcpy(new_pixel, &pixel, sizeof(Pixel_Data));

    }
    else {
        target_pixel[0] = 0;
        target_pixel[1] = 0;
        target_pixel[2] = 0;
    }
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

void Initialize_PNG(png_structp* read_ptr, png_infop* read_info_ptr, FILE* input_file){
    *read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    *read_info_ptr = png_create_info_struct(*read_ptr);

    png_init_io(*read_ptr, input_file);


}
void Read_PNG(png_structp* read_ptr, png_infop* read_info_ptr, FILE* input_file, png_bytepp* row_pointers,
              png_uint_32 *height, png_uint_32* width) {
    Initialize_PNG_Reader(read_ptr, read_info_ptr, input_file);

    png_read_info((*read_ptr), (*read_info_ptr));

    *width = png_get_image_width((*read_ptr), (*read_info_ptr));
    *height = png_get_image_height((*read_ptr), (*read_info_ptr));
    *row_pointers = (png_bytepp)malloc(sizeof(png_bytep) * (*height));
    if (*row_pointers == NULL) {
        fprintf(stderr, "Memory allocation failed for row pointers.\n");
        exit(EXIT_FAILURE); // Or handle the error appropriately
    }

    for (png_uint_32 i = 0; i < *height; i++) {
        (*row_pointers)[i] = (png_bytep)malloc(png_get_rowbytes(*read_ptr, *read_info_ptr));
        if ((*row_pointers)[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for row %u.\n", i);
            // Proper cleanup for already allocated memory
            while (i-- > 0) {
                free((*row_pointers)[i]);
            }
            free(*row_pointers);
            png_destroy_read_struct(read_ptr, read_info_ptr, NULL);
            fclose(input_file);
            exit(EXIT_FAILURE); // Or handle the error appropriately
        }
    }

    png_read_image(*read_ptr, *row_pointers);
}


void Write_PNG(png_structp* write_ptr, png_infop* write_info_ptr, FILE* output_file, png_bytepp row_pointers,
               png_uint_32 height, png_uint_32 width, int bit_depth, int color_type) {
    Initialize_PNG_Writer(write_ptr, write_info_ptr, output_file);

    // Set image attributes
    png_set_IHDR(*write_ptr, *write_info_ptr, width, height, bit_depth,
                 color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Write header
    png_write_info(*write_ptr, *write_info_ptr);

    // Write image data
    if (row_pointers != NULL) {
        png_write_image(*write_ptr, row_pointers);
    }

    // Finish writing
    png_write_end(*write_ptr, NULL);

    // Clean up
    png_destroy_write_struct(write_ptr, write_info_ptr);
    fclose(output_file);
    // Note: Caller should free row_pointers if allocated dynamically
}

void Initialize_Pixels(png_bytepp *pixel_array, int height, size_t bytes_per_row){

    *pixel_array = (png_bytepp)malloc(sizeof(png_bytep) * height);
    for (size_t i = 0; i < height; i++) {
        (*pixel_array)[i] = (png_bytep)malloc(bytes_per_row);
    }
    for(size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < bytes_per_row; j++) {
            (*pixel_array)[i][j] = 0;
        }
    }
}

void Initialize_PNG_Reader(png_structp* read_ptr, png_infop* read_info_ptr, FILE* input_file){
    *read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!*read_ptr) {
        fprintf(stderr, "Failed to create PNG read struct.\n");
        return;
    }

    *read_info_ptr = png_create_info_struct(*read_ptr);
    if (!*read_info_ptr) {
        png_destroy_read_struct(read_ptr, NULL, NULL);
        fprintf(stderr, "Failed to create PNG info struct.\n");
        return;
    }

    png_init_io(*read_ptr, input_file);
}

void Initialize_PNG_Writer(png_structp* write_ptr, png_infop* write_info_ptr, FILE* output_file){
    *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!*write_ptr) {
        fprintf(stderr, "Failed to create PNG write struct.\n");
        return;
    }

    *write_info_ptr = png_create_info_struct(*write_ptr);
    if (!*write_info_ptr) {
        png_destroy_write_struct(write_ptr, NULL);
        fprintf(stderr, "Failed to create PNG info struct for writing.\n");
        return;
    }

    png_init_io(*write_ptr, output_file);
}
