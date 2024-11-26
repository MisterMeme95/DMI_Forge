#include "data_structure.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "PixelManip.h"
#ifdef __cplusplus
extern "C" {
#endif

//Pixel_Data
void Set_Channels(Pixel_Data *pixel) {
    if (pixel->color_type != PNG_COLOR_TYPE_PALETTE) {
        Set_Red_Channel(pixel);
        Set_Green_Channel(pixel);
        Set_Blue_Channel(pixel);
        //Set_Alpha_Channel(pixel);
    }
}

void Set_Channel_Value(Pixel_Data *pixel, png_bytep channel, int channel_offset) {
    if (pixel->bit_depth == 8) {
        *channel = pixel->byte_data[channel_offset];
    } else if (pixel->bit_depth == 16) {
        memcpy(channel, &pixel->byte_data_16[channel_offset * 2], sizeof(png_byte) * 2);
        // If your system is little-endian, you might need to swap the bytes
        // *channel = (*channel >> 8) | (*channel << 8);
    } else {
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


int Get_Red_Channel(Pixel_Data pixel) {
    if (pixel.color_type == PNG_COLOR_TYPE_PALETTE) {
        return pixel.color_data->red;
    } else if (pixel.color_type == PNG_COLOR_TYPE_RGBA || pixel.color_type == PNG_COLOR_TYPE_RGB) {
        if (pixel.bit_depth == 8)
            return pixel.byte_data[0];
    } else if (pixel.color_type == PNG_COLOR_TYPE_GRAY || pixel.color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        return pixel.color_data->red;

    }

    return -1;
}

int Get_Green_Channel(Pixel_Data pixel) {
    if (pixel.color_type == PNG_COLOR_TYPE_PALETTE) {
        return pixel.color_data->green;
    } else if (pixel.color_type == PNG_COLOR_TYPE_RGBA || pixel.color_type == PNG_COLOR_TYPE_RGB) {
        if (pixel.bit_depth == 8)
            return pixel.byte_data[1];
    }
    return -1;
}

int Get_Blue_Channel(Pixel_Data pixel) {
    if (pixel.color_type == PNG_COLOR_TYPE_PALETTE) {
        return pixel.color_data->blue;
    } else if (pixel.color_type == PNG_COLOR_TYPE_RGBA || pixel.color_type == PNG_COLOR_TYPE_RGB) {
        if (pixel.bit_depth == 8)
            return pixel.byte_data[2];
    }
    return -1;
}

int Get_Alpha_Channel(Pixel_Data pixel) {
    if (pixel.color_type == PNG_COLOR_TYPE_PALETTE) {
        return 255;
    } else if (pixel.color_type == PNG_COLOR_TYPE_RGBA || pixel.color_type == PNG_COLOR_TYPE_RGB) {
        if (pixel.bit_depth == 8)
            return pixel.byte_data[3];
    }
}

int Get_Gray_Value(Pixel_Data pixel) {
    int gray_value;
    if (pixel.color_type == PNG_COLOR_TYPE_PALETTE) {
        gray_value = (int) ceil(
                0.299 * pixel.color_data->red + 0.587 * pixel.color_data->green + 0.114 * pixel.color_data->blue);
    } else {
        gray_value = (int) ceil(0.299 * pixel.byte_data[0] + 0.587 * pixel.byte_data[1] + 0.114 * pixel.byte_data[2]);
    }

    return gray_value;
}

Pixel_Data Pixel_Transformation(Pixel_Data pixel, int target_color_type, int target_bit_depth) {
    Pixel_Data pixel_transformed;
    pixel_transformed.color_type = target_color_type;
    pixel_transformed.byte_data = NULL;
    pixel_transformed.color_data = NULL;
    pixel_transformed.bit_depth = target_bit_depth;
    //Initialize_Pixel(&pixel_transformed.byte_data, target_color_type, target_bit_depth);
    Initialize_Pixel2(&pixel_transformed, target_color_type, target_bit_depth);
    if (pixel.color_type == PNG_COLOR_TYPE_PALETTE) {
        Transform_Indexed_PNG(pixel, &pixel_transformed, target_color_type,
                              target_bit_depth);
    } else if (pixel.color_type == PNG_COLOR_TYPE_RGBA) {
        pixel_transformed = Transform_RGBA_PNG2(pixel, target_color_type, pixel.bit_depth, target_bit_depth);
    } else if (pixel.color_type == PNG_COLOR_TYPE_RGB) {
        pixel_transformed = Transform_RGB_PNG(&pixel, target_color_type, target_bit_depth);

    }

    //Set_Channels(&pixel_transformed);
    return pixel_transformed;

}

Pixel_Data Get_Pixel(Image image, int x_coord, int y_coord) {
    Pixel_Data pixel;
    pixel.bit_depth = image.bit_depth;
    pixel.color_type = image.color_type;
    pixel.byte_data = NULL;
    pixel.color_data = NULL;


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
    int pixel_data = image.pixel_array[y_coord][pixel_index];

    /** @Description This represents the amount of bits to shift a byte by to unpack the pixel value.
     * By default, it is 0.
     *
     * Once more, only useful for Index/Gray images.*/
    int pixel_bit_shift = 0;

    /** @Description pixel_value is a pointer that we set to represent the various values in the various channels for a
     * png_byte. The size of the pixel_value depends on the color_type and bit_depth. It is set using Initialize_Pixel(). */
    //png_bytep pixel_value = NULL;

    if (pixel.color_type == PNG_COLOR_TYPE_PALETTE) {
        pixel.color_data = (png_colorp) malloc(sizeof(png_color));
    } else {
        Initialize_Pixel(&pixel.byte_data, image.color_type, image.bit_depth);
    }

    if (image.bit_depth == 16) {
        if (image.color_type == PNG_COLOR_TYPE_RGBA) {
            pixel_index *= 8;
        } else if (image.color_type == PNG_COLOR_TYPE_RGB) {
            pixel_index *= 6;
        } else if (image.color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            pixel_index *= 4;
        }

            /** We run a check here for Gray-scaled images because with 16 bit depth,
             * each pixel will be 2 bytes long. We don't need to check this for 8-bits because
             * each pixel there is 1-byte long. */
        else if (image.color_type == PNG_COLOR_TYPE_GRAY) {
            pixel_index *= 2;
        }
    }

    if (image.bit_depth == 8) {
        if (image.color_type == PNG_COLOR_TYPE_RGBA) {
            pixel_index *= 4;
        } else if (image.color_type == PNG_COLOR_TYPE_RGB) {
            pixel_index *= 3;
        } else if (image.color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            pixel_index *= 2;
        }
    }

    /* If bit_depth is 4, then we know that there are 2 pixels per byte. So we simply have and floor the value. */
    if (image.bit_depth == 4) {
        pixels_per_byte = 2;
        int pixel_in_byte = x_coord % pixels_per_byte;
        pixel_bit_shift = (image.bit_depth) * ((pixels_per_byte - 1) - pixel_in_byte);
        pixel_index = (int) floor(x_coord / pixels_per_byte);
        pixel_data = image.pixel_array[y_coord][pixel_index];

        if (pixel_in_byte == 0) {
            pixel_data = pixel_data & BIT4_L_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        } else {
            pixel_data = pixel_data & BIT4_R_ON;
        }

    }

        /** There are 4 pixels per byte, so we floor after dividing by 4. **/
    else if (image.bit_depth == 2) {
        pixels_per_byte = 4;
        int pixel_in_byte = x_coord % pixels_per_byte;

        pixel_bit_shift = (image.bit_depth) * ((pixels_per_byte - 1) - pixel_in_byte);
        pixel_index = (int) floor(x_coord / pixels_per_byte);
        pixel_data = image.pixel_array[y_coord][pixel_index];

        if (pixel_in_byte == 0) {
            pixel_data = pixel_data & BIT2_1_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        } else if (pixel_in_byte == 1) {
            pixel_data = pixel_data & BIT2_2_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        } else if (pixel_in_byte == 2) {
            pixel_data = pixel_data & BIT2_3_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        } else {
            pixel_data = pixel_data & BIT2_4_ON;
        }
    }

        /* At a bit_depth of 1 we have 8 pixels per byte. */
    else if (image.bit_depth == 1) {
        pixels_per_byte = 8;
        int pixel_in_byte = x_coord % pixels_per_byte;

        pixel_bit_shift = (image.bit_depth) * ((pixels_per_byte - 1) - pixel_in_byte);
        pixel_index = (int) floor(x_coord / pixels_per_byte);
        pixel_data = image.pixel_array[y_coord][pixel_index];

        if (pixel_in_byte == 0) {
            pixel_data = pixel_data & BIT1_1_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        } else if (pixel_in_byte == 1) {
            pixel_data = pixel_data & BIT1_2_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        } else if (pixel_in_byte == 2) {
            pixel_data = pixel_data & BIT1_3_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        } else if (pixel_in_byte == 3) {
            pixel_data = pixel_data & BIT1_4_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        } else if (pixel_in_byte == 4) {
            pixel_data = pixel_data & BIT1_5_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        } else if (pixel_in_byte == 5) {
            pixel_data = pixel_data & BIT1_6_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        } else if (pixel_in_byte == 6) {
            pixel_data = pixel_data & BIT1_7_ON;
            pixel_data = (pixel_data >> pixel_bit_shift);
        } else {
            pixel_data = pixel_data & BIT1_7_ON;
        }
    }
    if (image.color_type == PNG_COLOR_TYPE_PALETTE) {
        //printf("Num_Trans - %d\nPixel_Date = %d\n", *num_trans, pixel_data);

        /** @Description By using the pixel_data that we processed earlier, we now have the palette's index.
         * This allows us to store the data in selected_color.*/
        png_color selected_color = image.palette[pixel_data];

        pixel.color_data->red = selected_color.red;//100;
        pixel.color_data->green = selected_color.green;
        pixel.color_data->blue = selected_color.blue;

        /* Next up, if the index that we have in our pixel_data is less than the number of indexes in the
         * trans_alpha array, then we know that our selected color has an alpha value that is not 255.
         * Let's just copy that over. */

        if (pixel_data < (image.trans_num)) {
            pixel.alpha_channel = image.trans_alpha[pixel_data];

        }

            /* Otherwise, the alpha value is 255.*/
        else
            pixel.alpha_channel = 255;

    }

        /* If it is not an indexed image, then we can simply copy the pixel directly.*/
    else {
        Copy_Pixel(pixel.byte_data, image.pixel_array, pixel_index, y_coord,
                   image.color_type, image.bit_depth);

    }


    /* Set up the channels before sending the pixel off. */
    Set_Channels(&pixel);
    return pixel;
}


void Set_Pixel(png_bytepp image, Pixel_Data *new_pixel, int x_coord, int y_coord, int color_type, int bit_depth,
               png_colorp palette, png_bytep trans_alpha, int *num_trans, palette_hash *pal_hash) {
    /** @Description pixel_index is a simple int variable that we'll use to get the proper x_coordinate.
     * Since it is possible for a pixel to be less than a byte at certain bit_depths.
     *
     * Default value is x_coord. */
    int pixel_index = x_coord;
    /** @Description This value represents the number of pixels packed into a single byte.
     * This is only relevant whenever a pixel's bit_depth is less than 8. */
    int pixels_per_byte = 0;
    /** @Description This represents the amount of bits to shift a byte by to unpack the pixel value.
     * By default, it is 0.
     *
     * Once more, only useful for Index/Gray images.*/
    int pixel_bit_shift = 0;
    int insert_index = 0;
    double bpp = 0.0;
    int pixel_in_byte = 0;
    switch (bit_depth) {
        case 16:
            bpp = 2;
            break;
        case 8:
            bpp = 1;
            break;
        case 4:
            bpp = 0.5;
            pixel_in_byte = x_coord % 2;
            break;
        case 2:
            bpp = 0.25;
            pixel_in_byte = x_coord % 4;
            break;
        case 1:
            bpp = 0.125;
            pixel_in_byte = x_coord % 8;
            break;
        default:
            break;
    }
    pixels_per_byte = (int) (1 / bpp);
    pixel_bit_shift = (bit_depth) * ((pixels_per_byte - 1) - pixel_in_byte);
    switch (color_type) {
        case PNG_COLOR_TYPE_RGBA:
            bpp *= 4;
            break;
        case PNG_COLOR_TYPE_RGB:
            bpp *= 3;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            bpp *= 2;
            break;
        case PNG_COLOR_TYPE_PALETTE: {
            int get_index = find_key(*new_pixel, pal_hash);
            if (get_index == -1) {
                insert_key(*new_pixel, pal_hash, *num_trans);
                palette[*num_trans] = *(new_pixel->color_data);
                insert_index = *num_trans;
                *(num_trans) += 1;
            } else {
                insert_index = get_index;
            }
        }
            break;
        default:
            *new_pixel->byte_data = (*new_pixel->byte_data) << pixel_bit_shift;
            bpp *= 1;
            break;
    }
    if (bit_depth == 4) {
        pixel_index = (int) floor((x_coord / pixels_per_byte));
        if (pixel_in_byte == 0) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT4_R_ON;
        } else if (pixel_in_byte == 1) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT4_L_ON;
        }
        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            insert_index = (insert_index << (pixel_bit_shift));
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) ^ insert_index;
        } else {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) ^ (*new_pixel->byte_data);
        }
    }

    if (bit_depth == 2) {
        pixel_index = (int) floor((x_coord / pixels_per_byte));
        if (pixel_in_byte == 0) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT2_1_OFF;
        } else if (pixel_in_byte == 1) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT2_2_OFF;
        } else if (pixel_in_byte == 2) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT2_3_OFF;
        } else if (pixel_in_byte == 3) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT2_4_OFF;
        }
        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            insert_index = (insert_index << (pixel_bit_shift));
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) ^ insert_index;
        } else {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) ^ (*new_pixel->byte_data);
        }
    }

    if (bit_depth == 1) {
        pixel_index = (int) floor((x_coord / pixels_per_byte));
        if (pixel_in_byte == 0) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT1_1_OFF;
        } else if (pixel_in_byte == 1) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT1_2_OFF;
        } else if (pixel_in_byte == 2) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT1_3_OFF;
        } else if (pixel_in_byte == 3) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT1_4_OFF;
        } else if (pixel_in_byte == 4) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT1_5_OFF;
        } else if (pixel_in_byte == 5) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT1_6_OFF;
        } else if (pixel_in_byte == 6) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT1_7_OFF;
        } else if (pixel_in_byte == 7) {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) & BIT1_8_OFF;
        }
        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            insert_index = (insert_index << (pixel_bit_shift));
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) ^ insert_index;
        } else {
            image[y_coord][pixel_index] = (image[y_coord][pixel_index]) ^ (*new_pixel->byte_data);
        }
    } else {
        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            image[y_coord][pixel_index] = (png_byte) insert_index;
        } else {
            pixel_index = (int) (pixel_index * bpp);
            size_t bytes_to_copy = (size_t) (sizeof(png_byte) * bpp);
            memcpy(&image[y_coord][pixel_index], new_pixel->byte_data, bytes_to_copy);
        }
    }
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
                int y_coord, int color_type, int bit_depth) {

    switch (color_type) {
        case PNG_COLOR_TYPE_RGB:
            if (bit_depth == 16) {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 6);
            } else {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 3);
            }
            break;

        case PNG_COLOR_TYPE_RGBA:
            if (bit_depth == 16) {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 8);
            } else {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 4);
            }
            break;

        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if (bit_depth == 16) {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 4);
            } else {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 2);
            }
            break;

        case PNG_COLOR_TYPE_GRAY:
            if (bit_depth == 16) {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 2);
            } else {
                memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 1);
            }
            break;

        default:
            memcpy(copy_destination, &copy_source[y_coord][x_coord], sizeof(png_byte) * 1);
            break;
    }

}


/* May be redundant. */
png_channels Get_Channels(const Pixel_Data pixel, int pixel_color_type, int pixel_bit_depth) {
    png_channels return_channel;

    return_channel.red = Get_Red_Channel(pixel);
    return_channel.green = Get_Red_Channel(pixel);
    return_channel.blue = Get_Red_Channel(pixel);
    return_channel.alpha = Get_Alpha_Channel(pixel);

    return return_channel;
}

/** I think this is to get a png_color from a png_byte. */
png_color Byte_To_Color(const png_byte *pixel, int color_type, png_bytep trans_byte) {
    png_color palette_pixel;
    if (color_type == PNG_COLOR_TYPE_RGBA) {
        palette_pixel.red = pixel[0];
        palette_pixel.green = pixel[1];
        palette_pixel.blue = pixel[2];
        *trans_byte = pixel[3];
    } else if (color_type == PNG_COLOR_TYPE_RGB) {
        palette_pixel.red = pixel[0];
        palette_pixel.green = pixel[1];
        palette_pixel.blue = pixel[2];
    } else if (color_type == PNG_COLOR_TYPE_GRAY) {
        palette_pixel.red = pixel[0];
        palette_pixel.green = pixel[0];
        palette_pixel.blue = pixel[0];
    } else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        palette_pixel.red = pixel[0];
        palette_pixel.green = pixel[0];
        palette_pixel.blue = pixel[0];
        *trans_byte = pixel[1];
    } else {
        palette_pixel.red = 1;
        palette_pixel.green = 1;
        palette_pixel.blue = 1;
    }
    return palette_pixel;
}

int calculate_frame_count(int frame_width, int frame_height, int png_width, int png_height) {
    int frames_per_row = png_width / frame_width;
    int frames_per_column = png_height / frame_height;
    return frames_per_row * frames_per_column;
}


int Get_Pixel_Size(int bit_depth, int color_type) {
    int channels;
    switch (color_type) {
        case PNG_COLOR_TYPE_GRAY:
            channels = 1;
            break;
        case PNG_COLOR_TYPE_RGB:
            channels = 3;
            break;
        case PNG_COLOR_TYPE_PALETTE:
            channels = 1;  // Palette entries are indexed, typically 1 byte per pixel
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            channels = 2;
            break;
        case PNG_COLOR_TYPE_RGBA:
            channels = 4;
            break;
        default:
            fprintf(stderr, "Unsupported color type\n");
            return 0;
    }
    return channels * (bit_depth / 8);  // Calculate the pixel size in bytes
}

size_t Get_Bytes_Per_Row(int bit_depth, int color_type, int image_width) {
    int channels = Get_Pixel_Size(bit_depth, color_type);
    size_t row_bits = image_width * channels * bit_depth;
    size_t row_bytes = (row_bits + 7) / 8;  // Equivalent to ceil(row_bits / 8.0)
    return row_bytes;
}

//void Initialize_Frames(Frame_Array array, png_structp png_ptr, png_infop info_ptr){
//    png_get_PLTE(png_ptr, info_ptr, array.palette, NULL);
//}

Frame_Array Frame_Partition(png_bytepp image, int bit_depth, int png_width, int png_height, int frame_width,
                            int frame_height, int color_type) {

    /* Calculate # of frames in the image. Malloc new_frames.array to # of frames. */
    int num_of_frames = calculate_frame_count(frame_width, frame_height, png_width, png_height);
    int current_frame = 0;


    size_t row_bytes = Get_Bytes_Per_Row(bit_depth, color_type, frame_width);
    Frame_Array new_frames;
    new_frames.array = (Frame *) malloc(sizeof(Frame) * num_of_frames);

    if (new_frames.array == NULL) {
        fprintf(stderr, "Failed to allocate memory for frames array\n");
        exit(EXIT_FAILURE);
    }

    new_frames.color_type = color_type;
    new_frames.bit_depth = bit_depth;
    int pixel_size = Get_Pixel_Size(bit_depth, color_type);

    for (int o = 0; o < png_height; o += frame_height) {
        for (int i = 0; i < png_width; i += frame_width) {
            Frame new_frame;
            new_frame.frame_data = (png_bytepp) malloc(sizeof(png_bytep) * frame_height);
            for (int row = 0; row < frame_height; row++) {
                new_frame.frame_data[row] = (png_bytep) malloc(row_bytes);
            }

            Get_Frame(new_frame.frame_data, image, 0, 0,
                      o, i * pixel_size, (int) row_bytes, frame_height);
            new_frames.array[current_frame] = new_frame;
            current_frame++;
        }
    }

    return new_frames;
}

void Initialize_Pixel2(Pixel_Data *pixel, int color_type, int bit_depth) {
    switch (color_type) {
        case PNG_COLOR_TYPE_RGB:
            if (bit_depth == 16) {
                (pixel->byte_data) = (png_bytep) calloc(6, sizeof(png_byte));
            } else {
                (pixel->byte_data) = (png_bytep) calloc(3, sizeof(png_byte));
            }
            break;

        case PNG_COLOR_TYPE_RGBA:
            if (bit_depth == 16) {
                (pixel->byte_data) = (png_bytep) calloc(8, sizeof(png_byte));
            } else {
                //    printf("RGBA!\n");
                (pixel->byte_data) = (png_bytep) calloc(4, sizeof(png_byte));
            }
            break;

        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if (bit_depth == 16) {
                (pixel->byte_data) = (png_bytep) calloc(4, sizeof(png_byte));
            } else {
                (pixel->byte_data) = (png_bytep) calloc(2, sizeof(png_byte));
            }
            break;

        case PNG_COLOR_TYPE_GRAY:
            if (bit_depth == 16) {
                (pixel->byte_data) = (png_bytep) calloc(2, sizeof(png_byte));
            } else {
                (pixel->byte_data) = (png_bytep) calloc(1, sizeof(png_byte));
            }
            break;

        case PNG_COLOR_TYPE_PALETTE:
            (pixel->color_data) = (png_colorp) malloc(sizeof(png_colorp));//(png_colorp)calloc(1, sizeof(png_color));
            break;

        default:
            (pixel->byte_data) = (png_bytep) calloc(4, sizeof(png_byte));
            break;
    }
    pixel->color_type = color_type;
    pixel->bit_depth = bit_depth;
    Set_Channels(pixel);
}

void Initialize_Pixel(png_bytepp pixel, int color_type, int bit_depth) {
    switch (color_type) {
        case PNG_COLOR_TYPE_RGB:
            if (bit_depth == 16) {
                (*pixel) = (png_bytep) calloc(6, sizeof(png_byte));
            } else {
                (*pixel) = (png_bytep) calloc(3, sizeof(png_byte));
            }
            break;

        case PNG_COLOR_TYPE_RGBA:
            if (bit_depth == 16) {
                (*pixel) = (png_bytep) calloc(8, sizeof(png_byte));
            } else {
                (*pixel) = (png_bytep) calloc(4, sizeof(png_byte));
            }
            break;

        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if (bit_depth == 16) {
                (*pixel) = (png_bytep) calloc(4, sizeof(png_byte));
            } else {
                (*pixel) = (png_bytep) calloc(2, sizeof(png_byte));
            }
            break;

        case PNG_COLOR_TYPE_GRAY:
            if (bit_depth == 16) {
                (*pixel) = (png_bytep) calloc(2, sizeof(png_byte));
            } else {
                (*pixel) = (png_bytep) calloc(1, sizeof(png_byte));
            }
            break;

        case PNG_COLOR_TYPE_PALETTE:

        default:
            (*pixel) = (png_bytep) calloc(4, sizeof(png_byte));
            break;
    }
}

Pixel_Data Transform_RGBA_PNG2(Pixel_Data pixel, int target_color_type, int src_bit_depth, int target_bit_depth) {

    Pixel_Data target_pixel;
    Initialize_Pixel2(&target_pixel, target_color_type, target_bit_depth);

    if (target_color_type == PNG_COLOR_TYPE_RGB) {
        int depth_difference = src_bit_depth - target_bit_depth;
        if (depth_difference == 0) {
            if (src_bit_depth == 16) {
                memcpy(target_pixel.byte_data, pixel.byte_data, sizeof(png_byte) * 6);
            } else {
                memcpy(target_pixel.byte_data, pixel.byte_data, sizeof(png_byte) * 3);
            }

        } else {
            if (src_bit_depth == 16) {
                target_pixel.byte_data[0] = target_pixel.byte_data[0] >> 8;
                target_pixel.byte_data[2] = target_pixel.byte_data[2] >> 8;
                target_pixel.byte_data[4] = target_pixel.byte_data[4] >> 8;
                memcpy(target_pixel.byte_data, pixel.byte_data, sizeof(png_byte) * 3);
            } else {
                target_pixel.byte_data[0] = target_pixel.byte_data[1] << 8;
                target_pixel.byte_data[1] = target_pixel.byte_data[2] << 8;
                target_pixel.byte_data[2] = target_pixel.byte_data[3] << 8;
                memcpy(target_pixel.byte_data, pixel.byte_data, sizeof(png_byte) * 6);
            }
        }
    } else if (target_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        int gray_value = Get_Gray_Value(pixel);
        target_pixel.byte_data[0] = (png_byte) gray_value;
        target_pixel.byte_data[1] = pixel.byte_data[3];
    } else if (target_color_type == PNG_COLOR_TYPE_GRAY) {
        int gray_value = Get_Gray_Value(pixel);
        target_pixel.byte_data[0] = (png_byte) gray_value;
    } else if (target_color_type == PNG_COLOR_TYPE_PALETTE) {
        target_pixel.color_data->red = pixel.byte_data[0];
        target_pixel.color_data->green = pixel.byte_data[1];
        target_pixel.color_data->blue = pixel.byte_data[2];
    } else if (target_color_type == PNG_COLOR_TYPE_RGBA) {
        memcpy(target_pixel.byte_data, pixel.byte_data, sizeof(png_byte) * 8);

    } else {
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

png_bytep Transform_GRAYALPHA_PNG(const png_byte *pixel, int target_color_type, int target_bit_depth) {
    png_bytep target_pixel;
    Initialize_Pixel(&target_pixel, target_color_type, target_bit_depth);

    if (target_color_type == PNG_COLOR_TYPE_RGBA) {
        target_pixel[0] = pixel[0];
        target_pixel[1] = pixel[0];
        target_pixel[2] = pixel[0];
        target_pixel[3] = pixel[1];
    } else if (target_color_type == PNG_COLOR_TYPE_RGB) {
        target_pixel[0] = pixel[0];
        target_pixel[1] = pixel[0];
        target_pixel[2] = pixel[0];
    } else if (target_color_type == PNG_COLOR_TYPE_GRAY) {
        target_pixel[0] = pixel[0];
    } else {
        target_pixel[0] = 1;
        target_pixel[1] = 1;
        target_pixel[2] = 1;
    }
    return target_pixel;
}


png_bytep Transform_GRAY_PNG(const png_byte *pixel, int target_color_type, int bit_depth) {
    png_bytep target_pixel;
    Initialize_Pixel(&target_pixel, target_color_type, bit_depth);

    if (target_color_type == PNG_COLOR_TYPE_RGBA) {
        memcpy(target_pixel, pixel, sizeof(png_byte) * 3);
        target_pixel[3] = 255;
    } else if (target_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        double gray_value = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
        target_pixel[0] = (png_byte) gray_value;
        target_pixel[1] = 255;
    } else if (target_color_type == PNG_COLOR_TYPE_GRAY) {
        double gray_value = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
        target_pixel[0] = (png_byte) gray_value;
    } else {
        target_pixel[0] = 1;
        target_pixel[1] = 1;
        target_pixel[2] = 1;
    }

    return target_pixel;
}

Pixel_Data Transform_RGB_PNG(Pixel_Data *pixel, int target_color_type, int bit_depth) {
    Pixel_Data target_pixel;
    Initialize_Pixel2(&target_pixel, target_color_type, bit_depth);

    /* NOTE if this has a trans chunk, make sure that this pixel is not the one. If it is, set alpha level to 0. */
    if (target_color_type == PNG_COLOR_TYPE_RGBA) {
        memcpy(target_pixel.byte_data, pixel->byte_data, sizeof(png_byte) * 3);
        target_pixel.byte_data[3] = 255;
    }
        /* NOTE if this has a trans chunk, make sure that this pixel is not the one. If it is, set alpha level to 0. */
    else if (target_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        double gray_value = 0.299 * pixel->byte_data[0] + 0.587 * pixel->byte_data[1] + 0.114 * pixel->byte_data[2];
        target_pixel.byte_data[0] = (png_byte) gray_value;
        target_pixel.byte_data[1] = 255;
    } else if (target_color_type == PNG_COLOR_TYPE_GRAY) {
        double gray_value = 0.299 * pixel->byte_data[0] + 0.587 * pixel->byte_data[1] + 0.114 * pixel->byte_data[2];
        int shift_amount = pixel->bit_depth - bit_depth;
        target_pixel.byte_data[0] = ((png_byte) gray_value >> shift_amount);
    } else if (target_color_type == PNG_COLOR_TYPE_RGB) {
        memcpy(target_pixel.byte_data, pixel->byte_data, sizeof(png_byte) * 3);
    } else if (target_color_type == PNG_COLOR_TYPE_PALETTE) {
        target_pixel.color_data->red = pixel->byte_data[0];
        target_pixel.color_data->green = pixel->byte_data[1];
        target_pixel.color_data->blue = pixel->byte_data[2];
    } else {
        target_pixel.byte_data[0] = 1;
        target_pixel.byte_data[1] = 1;
        target_pixel.byte_data[2] = 1;
    }
    return target_pixel;
}


void Transform_Indexed_PNG(Pixel_Data pixel, Pixel_Data *new_pixel, int target_color_type, int target_bit_depth) {
    png_bytep target_pixel;
    Initialize_Pixel(&target_pixel, target_color_type, target_bit_depth);
    if (target_color_type == PNG_COLOR_TYPE_RGB) {
        if (target_bit_depth == 8) {
            target_pixel[0] = pixel.color_data->red;
            target_pixel[1] = pixel.color_data->green;
            target_pixel[2] = pixel.color_data->blue;
            memcpy(new_pixel->byte_data, target_pixel, sizeof(png_byte) * 3);
            Set_Channels(new_pixel);
        } else {
            target_pixel[0] = pixel.color_data->red;
            target_pixel[2] = pixel.color_data->green;
            target_pixel[4] = pixel.color_data->blue;
            memcpy(new_pixel->byte_data, target_pixel, sizeof(png_byte) * 6);
        }

        new_pixel->color_type = PNG_COLOR_TYPE_RGB;
    } else if (target_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        int gray_value = Get_Gray_Value(pixel);
        if (target_bit_depth == 8) {
            target_pixel[0] = (png_byte) gray_value; // Grayscale value
            target_pixel[1] = pixel.alpha_channel; // Alpha value
        } else {
            target_pixel[0] = (png_byte) gray_value; // Grayscale value
            target_pixel[2] = pixel.alpha_channel; // Alpha value
        }
        new_pixel->color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
        new_pixel->byte_data = target_pixel;
    } else if (target_color_type == PNG_COLOR_TYPE_RGBA) {
        if (target_bit_depth == 8) {
            target_pixel[0] = pixel.color_data->red;
            target_pixel[1] = pixel.color_data->green;
            target_pixel[2] = pixel.color_data->blue;
            target_pixel[3] = pixel.alpha_channel;
            memcpy(new_pixel->byte_data, target_pixel, sizeof(png_byte) * 4);
        } else {
            target_pixel[0] = pixel.color_data->red;
            target_pixel[2] = pixel.color_data->green;
            target_pixel[4] = pixel.color_data->blue;
            target_pixel[6] = pixel.alpha_channel;
            memcpy(new_pixel->byte_data, target_pixel, sizeof(png_byte) * 8);
        }
        new_pixel->color_type = PNG_COLOR_TYPE_RGBA;

        //new_pixel->byte_data = target_pixel;
    } else if (target_color_type == PNG_COLOR_TYPE_GRAY) {
        /*Color transformation. Then copy that value to first entry. */
        double gray_value =
                0.299 * pixel.color_data->red + 0.587 * pixel.color_data->green + 0.114 * pixel.color_data->blue;
        if (target_bit_depth == 8) {
            target_pixel[0] = (png_byte) ((int) gray_value);
        } else if (target_bit_depth == 4) {
            target_pixel[0] = ((png_byte) ((int) gray_value) >> 4);
        } else if (target_bit_depth == 2) {
            target_pixel[0] = ((png_byte) ((int) gray_value) >> 6);
        } else if (target_bit_depth == 1) {
            target_pixel[0] = ((png_byte) ((int) gray_value) >> 7);
        } else {
            target_pixel[0] = (png_byte) gray_value;
        }
        new_pixel->color_type = PNG_COLOR_TYPE_GRAY;
        new_pixel->byte_data = target_pixel;
        Set_Channels(new_pixel);
    } else if (target_color_type == PNG_COLOR_TYPE_PALETTE) {
        new_pixel->color_data->red = pixel.color_data->red;
        new_pixel->color_data->green = pixel.color_data->green;
        new_pixel->color_data->blue = pixel.color_data->blue;
        new_pixel->alpha_channel = pixel.alpha_channel;
        //memcpy(new_pixel->color_data, pixel.color_data, sizeof(png_color));
    } else {
        target_pixel[0] = 0;
        target_pixel[1] = 0;
        target_pixel[2] = 0;
    }
}

Pixel_Data Combine_Pixels(Pixel_Data foreground, Pixel_Data background, int color_type) {
    /*
    PNG_COLOR_TYPE_PALETTE;      // Palette-based images are not handled here because they do not directly contain color values.
    PNG_COLOR_TYPE_RGBA;
    PNG_COLOR_TYPE_RGB;
    PNG_COLOR_TYPE_GRAY_ALPHA;
    PNG_COLOR_TYPE_GRAY;
    */
    Pixel_Data new_pixel;
    Initialize_Pixel2(&new_pixel, color_type, 8);


    double divided_alpha = (double) Get_Alpha_Channel(foreground) / 255;
    double alpha_f = (double) Get_Alpha_Channel(foreground) / 255.0;
    double alpha_b = (double) Get_Alpha_Channel(background) / 255.0;

    png_byte new_red, new_green, new_blue, new_alpha;

    new_red = (png_byte) (divided_alpha * Get_Red_Channel(foreground) +
                          (1 - divided_alpha) * Get_Red_Channel(background));
    new_green = (png_byte) (divided_alpha * Get_Green_Channel(foreground) +
                            (1 - divided_alpha) * Get_Green_Channel(background));
    new_blue = (png_byte) (divided_alpha * Get_Blue_Channel(foreground) +
                           (1 - divided_alpha) * Get_Blue_Channel(background));
    new_alpha = (png_byte) ((alpha_f + alpha_b * (1 - alpha_f)) * 255);
    switch (color_type) {
        case PNG_COLOR_TYPE_PALETTE:
            new_pixel.color_data->red = new_red;
            new_pixel.color_data->green = new_green;
            new_pixel.color_data->blue = new_blue;
            new_pixel.alpha_channel = new_alpha;
            break;

        case PNG_COLOR_TYPE_RGBA:
            new_pixel.byte_data[0] = new_red;
            new_pixel.byte_data[1] = new_green;
            new_pixel.byte_data[2] = new_blue;
            new_pixel.byte_data[3] = new_alpha;
            break;

        case PNG_COLOR_TYPE_RGB:
            new_pixel.byte_data[0] = new_red;
            new_pixel.byte_data[1] = new_green;
            new_pixel.byte_data[2] = new_blue;
            break;

        default:
            new_pixel.byte_data[0] = new_red;
            new_pixel.byte_data[1] = new_green;
            new_pixel.byte_data[2] = new_blue;
            new_pixel.byte_data[0] = new_red;
            break;
    }

    Set_Channels(&new_pixel);

    return new_pixel;
    // Output or use the new blended pixel values
    printf("Blended Pixel RGBA(%d, %d, %d, %d)\n", new_red, new_green, new_blue, new_alpha);
}

void Initialize_PNG(png_structp *read_ptr, png_infop *read_info_ptr, FILE *input_file) {
    *read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    *read_info_ptr = png_create_info_struct(*read_ptr);

    png_init_io(*read_ptr, input_file);


}

void Read_PNG(png_structp *read_ptr, png_infop *read_info_ptr, FILE *input_file, png_bytepp *row_pointers,
              png_uint_32 *height, png_uint_32 *width) {
    Initialize_PNG_Reader(read_ptr, read_info_ptr, input_file);

    png_read_info((*read_ptr), (*read_info_ptr));

    *width = png_get_image_width((*read_ptr), (*read_info_ptr));
    *height = png_get_image_height((*read_ptr), (*read_info_ptr));
    *row_pointers = (png_bytepp) malloc(sizeof(png_bytep) * (*height));
    if (*row_pointers == NULL) {
        fprintf(stderr, "Memory allocation failed for row pointers.\n");
        exit(EXIT_FAILURE); // Or handle the error appropriately
    }

    for (png_uint_32 i = 0; i < *height; i++) {
        (*row_pointers)[i] = (png_bytep) malloc(png_get_rowbytes(*read_ptr, *read_info_ptr));
        if ((*row_pointers)[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for row %u.\n", i);
            // Cleanup previously allocated memory
            while (i > 0) {
                free((*row_pointers)[--i]);
            }
            free(*row_pointers);
            png_destroy_read_struct(read_ptr, read_info_ptr, NULL);
            fclose(input_file);
            exit(EXIT_FAILURE); // Consider a more graceful exit or handling strategy
        }
    }

    png_read_image(*read_ptr, *row_pointers);
}


void Write_PNG(png_structp *write_ptr, png_infop *write_info_ptr, FILE *output_file, png_bytepp row_pointers,
               png_uint_32 height, png_uint_32 width, int bit_depth, int color_type) {
    Initialize_PNG_Writer(write_ptr, write_info_ptr, output_file);

    // Set image attributes
    png_set_IHDR(*write_ptr, *write_info_ptr, width, height, bit_depth,
                 color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

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


void initialize_image2(char *file_name, Image *new_image, int *bit_depth, const int *color_type, int *width, int *height) {
    new_image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!new_image->png_ptr) {
        fprintf(stderr, "Failed to create PNG write struct.\n");
        exit(EXIT_FAILURE);
    }

    new_image->info_ptr = png_create_info_struct(new_image->png_ptr);

    if (!new_image->info_ptr) {
        png_destroy_write_struct(&new_image->png_ptr, &new_image->info_ptr);
        fprintf(stderr, "Failed to create PNG info struct for writing.\n");
        exit(EXIT_FAILURE);
    }

    if ((new_image->file_pointer = fopen(file_name, "wb")) == NULL) {
        printf("Error opening input file\n");
        exit(EXIT_FAILURE);
    }
    png_init_io(new_image->png_ptr, new_image->file_pointer);
    if(width != NULL){
        new_image->width = *width;
    }
    //new_image->width = *width;
    if(height != NULL){
        new_image->height = *height;
    }

    if(bit_depth != NULL){
        new_image->bit_depth = *bit_depth;
    }

    if(color_type != NULL){
        new_image->color_type = *color_type;
    }
  //  new_image->height = *height;
    png_set_IHDR(new_image->png_ptr, new_image->info_ptr, new_image->width, new_image->height,
                 new_image->bit_depth, new_image->color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);


    new_image->trans_color = NULL;

    new_image->row_bytes = png_get_rowbytes(new_image->png_ptr, new_image->info_ptr);
    new_image->pixel_array = (png_bytepp) malloc(sizeof(png_bytep) * new_image->height);
    if (new_image->pixel_array == NULL) {
        fprintf(stderr, "Memory allocation failed for row pointers.\n");
        exit(EXIT_FAILURE);
    }
    png_bytep fake_array = (png_bytep) malloc(sizeof(png_byte) * new_image->height * new_image->row_bytes);

//    for (png_uint_32 pix_index = 0; pix_index < new_image->height; pix_index++) {
//        new_image->pixel_array[pix_index] = (fake_array+(new_image->row_bytes * pix_index));
//    }
    for (png_uint_32 pix_index = 0; pix_index < new_image->height; pix_index++) {
        new_image->pixel_array[pix_index] = (png_bytep) malloc(new_image->row_bytes);
    }

//    for(int i = 0; i < new_image->height; i++){
//        for(int j = 0; j < png_get_rowbytes(new_image->png_ptr, new_image->info_ptr); j++){
//            new_image->pixel_array[i][j] = 0;
//        }
//    }

    if (new_image->color_type == PNG_COLOR_TYPE_PALETTE) {
        int palette_maximum_size = (int) pow(2.0, (double) new_image->bit_depth);
        new_image->palette = (png_colorp) malloc(sizeof(png_color) * palette_maximum_size);
        new_image->palette_num = 0;
    }
}


Image load_image(char *file_name) {
    Image new_image;
    if (!check_if_png(file_name, &new_image.file_pointer)) {
        printf("The file you are attempting to read is not a valid PNG file!\n");
        exit(1);
    }
    new_image.image_name = (char *) malloc(sizeof(char) * (strlen(file_name) + 1));
    strcpy(new_image.image_name, file_name);
    new_image.png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!(new_image.png_ptr)) {
        printf("Error creating read struct\n");
        fclose(new_image.file_pointer);
        exit(1);
    }
    png_set_sig_bytes(new_image.png_ptr, PNG_BYTES_TO_CHECK);
    new_image.info_ptr = png_create_info_struct(new_image.png_ptr);
    if (!(new_image.info_ptr)) {
        printf("Error creating read info struct\n");
        png_destroy_read_struct(&new_image.png_ptr, NULL, NULL);
        fclose(new_image.file_pointer);
        exit(1);
    }

    if (setjmp(png_jmpbuf(new_image.png_ptr))) {
        printf("Error during read\n");
        png_destroy_read_struct(&new_image.png_ptr, &new_image.info_ptr, NULL);
        fclose(new_image.file_pointer);
        exit(1);
    }

    /* Initialize connection between the file and the png_ptr. Also read in the information to fill in the metadata. */
    png_init_io(new_image.png_ptr, new_image.file_pointer);
    png_read_info(new_image.png_ptr, new_image.info_ptr);
    png_get_IHDR(new_image.png_ptr, new_image.info_ptr, &new_image.width, &new_image.height, &new_image.bit_depth,
                 &new_image.color_type, &new_image.interlace_method, NULL, NULL);

    /* We optionally set additional information if it's a palette. */
    if (new_image.color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(new_image.png_ptr, new_image.info_ptr, &new_image.palette, &new_image.palette_num);
        png_get_tRNS(new_image.png_ptr, new_image.info_ptr, &new_image.trans_alpha,
                     &new_image.trans_num, &new_image.trans_color);
    } else if (new_image.color_type == PNG_COLOR_TYPE_RGB | new_image.color_type == PNG_COLOR_TYPE_GRAY) {
        png_get_tRNS(new_image.png_ptr, new_image.info_ptr, &new_image.trans_alpha,
                     &new_image.trans_num, &new_image.trans_color);
    }

    new_image.pixel_array = (png_bytepp) malloc(sizeof(png_bytep) * new_image.height);

    if (new_image.pixel_array == NULL) {
        fprintf(stderr, "Memory allocation failed for pixel_array.\n");
        exit(EXIT_FAILURE);
    }


    new_image.row_bytes = png_get_rowbytes(new_image.png_ptr, new_image.info_ptr);
    png_bytep contiguous_rows = (png_bytep) malloc(sizeof(png_byte) * new_image.height * new_image.row_bytes);
    for (png_uint_32 pix_index = 0; pix_index < new_image.height; pix_index++) {
        // new_image.pixel_array[pix_index] = (png_bytep)malloc(new_image.row_bytes );
        new_image.pixel_array[pix_index] =
                contiguous_rows + (pix_index * new_image.row_bytes);///(png_bytep)malloc(new_image.row_bytes );
        if (new_image.pixel_array[pix_index] == NULL) {
            fprintf(stderr, "Memory allocation failed for row %u.\n", pix_index);
            // Cleanup previously allocated memory
            while (pix_index > 0) {
                free(new_image.pixel_array[pix_index--]);
            }
            free(new_image.pixel_array);
            png_destroy_read_struct(&new_image.png_ptr, &new_image.info_ptr, NULL);
            fclose(new_image.file_pointer);
            exit(EXIT_FAILURE); // Consider a more graceful exit or handling strategy
        }
    }
    png_read_image(new_image.png_ptr, new_image.pixel_array);
    return new_image;
}


void load_image2(Image *image, char *file_name) {
    if (!check_if_png(file_name, &image->file_pointer)) {
        printf("The file you are attempting to read is not a valid PNG file!\n");
        exit(1);
    }
    image->image_name = (char *) malloc(sizeof(char) * (strlen(file_name) + 1));
    strcpy(image->image_name, file_name);
    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!(image->png_ptr)) {
        printf("Error creating read struct\n");
        fclose(image->file_pointer);
        exit(1);
    }
    png_set_sig_bytes(image->png_ptr, PNG_BYTES_TO_CHECK);
    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!(image->info_ptr)) {
        printf("Error creating read info struct\n");
        png_destroy_read_struct(&image->png_ptr, NULL, NULL);
        fclose(image->file_pointer);
        exit(1);
    }

    if (setjmp(png_jmpbuf(image->png_ptr))) {
        printf("Error during read\n");
        png_destroy_read_struct(&image->png_ptr, &image->info_ptr, NULL);
        fclose(image->file_pointer);
        exit(1);
    }

    /* Initialize connection between the file and the png_ptr. Also read in the information to fill in the metadata. */
    png_init_io(image->png_ptr, image->file_pointer);
    png_read_info(image->png_ptr, image->info_ptr);
    png_get_IHDR(image->png_ptr, image->info_ptr, &image->width, &image->height, &image->bit_depth,
                 &image->color_type, &image->interlace_method, NULL, NULL);

    /* We optionally set additional information if it's a palette. */
    if (image->color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(image->png_ptr, image->info_ptr, &image->palette, &image->palette_num);
        png_get_tRNS(image->png_ptr, image->info_ptr, &image->trans_alpha,
                     &image->trans_num, &image->trans_color);
    } else if (image->color_type == PNG_COLOR_TYPE_RGB | image->color_type == PNG_COLOR_TYPE_GRAY) {
        png_get_tRNS(image->png_ptr, image->info_ptr, &image->trans_alpha,
                     &image->trans_num, &image->trans_color);
    }

    image->pixel_array = (png_bytepp) malloc(sizeof(png_bytep) * image->height);

    if (image->pixel_array == NULL) {
        fprintf(stderr, "Memory allocation failed for pixel_array.\n");
        exit(EXIT_FAILURE);
    }


    image->row_bytes = png_get_rowbytes(image->png_ptr, image->info_ptr);
    png_bytep contiguous_rows = (png_bytep) malloc(sizeof(png_byte) * image->height * image->row_bytes);
    for (png_uint_32 pix_index = 0; pix_index < image->height; pix_index++) {
        // new_image.pixel_array[pix_index] = (png_bytep)malloc(new_image.row_bytes );
        image->pixel_array[pix_index] =
                contiguous_rows + (pix_index * image->row_bytes);///(png_bytep)malloc(new_image.row_bytes );
        if (image->pixel_array[pix_index] == NULL) {
            fprintf(stderr, "Memory allocation failed for row %u.\n", pix_index);
            // Cleanup previously allocated memory
            while (pix_index > 0) {
                free(image->pixel_array[pix_index--]);
            }
            free(image->pixel_array);
            png_destroy_read_struct(&image->png_ptr, &image->info_ptr, NULL);
            fclose(image->file_pointer);
            exit(EXIT_FAILURE); // Consider a more graceful exit or handling strategy
        }
    }
    png_read_image(image->png_ptr, image->pixel_array);
}

int check_if_png(char *file_name, FILE **fp) {
    char buf[PNG_BYTES_TO_CHECK];
    /* Open the prospective PNG file. */
    if ((*fp = fopen(file_name, "rb")) == NULL) {
        printf("Error opening input file\n");
        return 0;
    }

    /* Read in some of the signature bytes. */
    if (fread(buf, 1, PNG_BYTES_TO_CHECK, *fp) != PNG_BYTES_TO_CHECK) {
        printf("Not enough bytes read!\n");
        return 0;
    }

    /* Compare the first PNG_BYTES_TO_CHECK bytes of the signature.
     * Return nonzero (true) if they match. */
    return (!png_sig_cmp(buf, 0, PNG_BYTES_TO_CHECK));
}

void Initialize_PNG_Reader(png_structp *read_ptr, png_infop *read_info_ptr, FILE *input_file) {
    *read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!*read_ptr) {
        fprintf(stderr, "Failed to create PNG read struct.\n");
        exit(EXIT_FAILURE); // Consider a more graceful exit or handling strategy
    }

    *read_info_ptr = png_create_info_struct(*read_ptr);
    if (!*read_info_ptr) {
        png_destroy_read_struct(read_ptr, NULL, NULL);
        fprintf(stderr, "Failed to create PNG info struct.\n");
        exit(EXIT_FAILURE); // Consider a more graceful exit or handling strategy
    }

    // Setup the error handling if you are using setjmp/longjmp approach
    if (setjmp(png_jmpbuf(*read_ptr))) {
        png_destroy_read_struct(read_ptr, read_info_ptr, NULL);
        fclose(input_file);
        fprintf(stderr, "Error during init_io.\n");
        exit(EXIT_FAILURE);
    }

    png_init_io(*read_ptr, input_file);
}


void Initialize_PNG_Writer(png_structp *write_ptr, png_infop *write_info_ptr, FILE *output_file) {
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







int remove_hash(Pixel_Data pixel){

    return 0;
}

unsigned int hash_pixel(Pixel_Data pixel) {
    const unsigned int prime = 31;
    unsigned int hash = 1;
    hash = prime * hash + Get_Red_Channel(pixel);
    hash = prime * hash + Get_Green_Channel(pixel);
    hash = prime * hash + Get_Blue_Channel(pixel);
    hash = prime * hash + Get_Alpha_Channel(pixel);
    return hash;
}

int find_key(Pixel_Data pixel, palette_hash *paletteHash) {

    // Assuming hash_pixel function correctly calculates a hash based on Pixel_Data
    int hash_index = (int)hash_pixel(pixel) % 256;

    png_node *key_to_find = paletteHash->hash_bucket[hash_index];

    while (key_to_find != NULL) {
        Pixel_Data other_pixel = key_to_find->pixel_info;

        if (match_node(pixel, other_pixel)) {

            return key_to_find->index; // Return the index of the matching node
        }

        key_to_find = key_to_find->next; // Move to next node in the list
    }

    return -1; // Return -1 if no match is found after checking all nodes in the bucket
}


int match_node(Pixel_Data pixel, Pixel_Data other_pixel){
    if (Get_Red_Channel(pixel) != Get_Red_Channel(other_pixel)) {
        return 0;
    }
    if (Get_Green_Channel(pixel) != Get_Green_Channel(other_pixel)) {
        return 0;
    }
    if (Get_Blue_Channel(pixel) != Get_Blue_Channel(other_pixel)) {
        return 0;
    }
    if(pixel.alpha_channel != other_pixel.alpha_channel){
        return 0;
    }
    return 1;
}



int insert_key(Pixel_Data pixel, palette_hash *paletteHash, png_byte new_index) {

    int hash_index = hash_pixel(pixel) % 256;  // Ensure hash_pixel is defined correctly

    if (paletteHash->hash_bucket[hash_index] == NULL) {

        png_node *new_node = (png_node*)malloc(sizeof(png_node));
        if (new_node == NULL) {
            fprintf(stderr, "Failed to allocate memory for new node\n");
            return -1;
        }

        // Initialize the new node
        new_node->pixel_info = pixel;
        new_node->index = new_index;
        new_node->next = NULL;
        new_node->prev = NULL;

        // Link the new node to the hash bucket
        paletteHash->hash_bucket[hash_index] = new_node;

    } else {
        // The bucket is not empty. Insert the new node at the beginning of the list
        png_node *new_node = (png_node*) malloc(sizeof(png_node));
        if (new_node == NULL) {
            fprintf(stderr, "Failed to allocate memory for new node\n");
            return -1;
        }

        // Initialize the new node
        new_node->pixel_info = pixel;
        new_node->index = new_index;
        new_node->next = paletteHash->hash_bucket[hash_index];
        new_node->prev = NULL;

        // Update the previous first node
        if (paletteHash->hash_bucket[hash_index] != NULL) {
            paletteHash->hash_bucket[hash_index]->prev = new_node;
        }

        // Set the new node as the first node in the bucket
        paletteHash->hash_bucket[hash_index] = new_node;
    }

    return 0;
}

png_bytepp get_image_data2(Image *image, int frame_number) {
    png_bytepp transfer_image = (png_bytepp) malloc(sizeof(png_bytep) * 32);
    png_uint_32 frames_per_row = image->width / 32;
    png_uint_32 start_row = (frame_number / frames_per_row) * 32;
    png_uint_32 start_col = (frame_number % frames_per_row) * 32;
;


    int bytes_per_frame = 128;


    for (int i = 0; i < 32; i++) {
        transfer_image[i] = (png_bytep)malloc(sizeof(png_byte) * 128);

        memcpy(transfer_image[i], &image->pixel_array[start_row + i][start_col], sizeof(png_byte) * 128);

    }

    return transfer_image;


}


png_bytepp get_image_data23(Image *image, int frame_number) {
    png_bytepp transfer_image = (png_bytepp)malloc(sizeof(png_bytep) * 32);
    if (!transfer_image) {
        // Handle malloc failure
        return NULL;
    }

    png_uint_32 frames_per_row = image->width / 32;
    png_uint_32 start_row = (frame_number / frames_per_row) * 32;
    png_uint_32 start_col = (frame_number % frames_per_row) * 32;

    for (int i = 0; i < 32; i++) {
        transfer_image[i] = (png_bytep)malloc(sizeof(png_byte) * 128);
        if (!transfer_image[i]) {
            // Handle malloc failure, free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(transfer_image[j]);
            }
            free(transfer_image);
            return NULL;
        }

        // Corrected to copy 128 bytes per row as intended
        memcpy(transfer_image[i], &image->pixel_array[start_row + i][start_col], sizeof(png_byte) * 128);
    }

    return transfer_image;
}




png_bytepp get_image_data(Image *image, int frame_number) {
    png_bytepp transfer_image = (png_bytepp) malloc(sizeof(png_bytep) * 32);
    png_uint_32 frames_per_row = image->width / 32;
    png_uint_32 start_row = ((frame_number / frames_per_row) * 32);
    png_uint_32 start_col = ((frame_number % frames_per_row) * 32);


//    printf("Get Image Start Row - %d\n", start_row);
//    printf("Get Image Start Col - %d\n\n\n", start_col);
//    sleep(5);
    int bytes_per_frame = 128;


    for (int i = 0; i < 32; i++) {
        transfer_image[i] = (png_bytep)malloc(sizeof(png_byte) * 128);
        memcpy(transfer_image[i], &image->pixel_array[start_row + i][start_col*4], sizeof(png_byte) * 128);

    }

    return transfer_image;

}

#ifdef __cplusplus
}
#endif