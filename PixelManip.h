

/* ************************
 *
 * Bit Depth 4 Masks ******
 **************************/
/** @Description Used when you want to isolate the first 4 bits of a byte.
 *
 * This flag represents 240 in decimal, and 11110000 in binary. */
#define BIT4_L_ON 0xF0

/** @Description Used when you want to isolate the first 4 bits of a byte.
 *
 * This flag represents 15 in decimal, and 00001111 in binary. */
#define BIT4_R_ON 0x0F

/* ************************
 * Bit Depth 2 Masks ******
 **************************/
/** @Description This is used to isolate the last 2 bits by using an AND operator.
 *
 * This flag represents 192 in decimal, and 11000000 in binary. */
#define BIT2_1_ON 0xC0

/** @Description Used to isolate the 5th and 6th bits in a byte using the AND operator.
 *
 * This flag represents 48 in decimal, and 00110000 in binary. */
#define BIT2_2_ON 0x30

/** @Description Used to isolate the 3rd and 4th bits in a byte using the AND operator.
 *
 * This flag represents 12 in decimal, and 00001100 in binary. */
#define BIT2_3_ON 0x0C

/** @Description Used to isolate the 1st and 2nd bits in a byte using the AND operator.
 *
 * This flag represents 3 in decimal, and 00000011 in binary. */
#define BIT2_4_ON 0x03

/** @Description Used when I want to clear out the 7th and 8th bits. Using the AND operator, it will allow me
 * to easily input pixel values in images with bit_depths of less than 8.
 *
 *
 * This flag represents 63 in decimal, and 00111111 in binary. */
#define BIT2_1_OFF 0x3F

/** @Description Used when I want to clear out the 5th and 6th bits. Using the AND operator, it will allow me
 * to easily input pixel values in images with bit_depths of less than 8.
 *
 * This flag represents 207 in decimal, and 11001111 in binary. */
#define BIT2_2_OFF 0xCF

/** @Description Used when I want to clear out the 3rd and 4th bits. Using the AND operator, it will allow me
 * to easily input pixel values in images with bit_depths of less than 8.
 *
 * This flag represents 243 in decimal, and 11110011 in binary. */
#define BIT2_3_OFF 0xF3

/** @Description Used when I want to clear out the 1st and 2nd bits. Using the AND operator, it will allow me
 * to easily input pixel values in images with bit_depths of less than 8.
 *
 * This flag represents 252 in decimal, and 11111100 in binary. */
#define BIT2_4_OFF 0xFC
/* ************************
 * Bit Depth 1 Masks ******
 **************************/

/** @Description Used to isolate the 1st bit in a byte using the AND operator.
 *
 * This flag represents 128 in decimal, and 10000000 in binary. */
#define BIT1_1_ON 0x80

/** @Description Used to isolate the 2nd bit in a byte using the AND operator.
 *
 * This flag represents 64 in decimal, and 01000000 in binary. */
#define BIT1_2_ON 0x40

/** @Description Used to isolate the 3rd bit in a byte using the AND operator.
 *
 * This flag represents 32 in decimal, and 00100000 in binary. */
#define BIT1_3_ON 0x20

/** @Description Used to isolate the 4th bit in a byte using the AND operator.
 *
 * This flag represents 16 in decimal, and 00010000 in binary. */
#define BIT1_4_ON 0x10

/** @Description Used to isolate the 5th bit in a byte using the AND operator.
 *
 * This flag represents 8 in decimal, and 00001000 in binary. */
#define BIT1_5_ON 0x08

/** @Description Used to isolate the 6th bit in a byte using the AND operator.
 *
 * This flag represents 4 in decimal, and 00000100 in binary. */
#define BIT1_6_ON 0x04

/** @Description Used to isolate the 7th bit in a byte using the AND operator.
 *
 * This flag represents 2 in decimal, and 00000010 in binary. */
#define BIT1_7_ON 0x02

/** @Description Used to isolate the 8th bit in a byte using the AND operator.
 *
 * This flag represents 1 in decimal, and 00000001 in binary. */
#define BIT1_8_ON 0x01

/** @Description Used when you want to clear out the 1st bit.
 *
 * This flag represents 127 in decimal, and 01111111 in binary. */
#define BIT1_1_OFF 0x7F

/** @Description Used when you want to clear out the 2nd bit.
 *
 * This flag represents 191 in decimal, and 10111111 in binary. */
#define BIT1_2_OFF 0xBF

/** * @Description Used when you want to clear out the 3rd bit.
 *
 * This flag represents 239 in decimal, and 11011111 in binary. */
#define BIT1_3_OFF 0xDF

/** @Description Used when you want to clear out the 4th bit.
 *
 * This flag represents 239 in decimal, and 11101111 in binary. */
#define BIT1_4_OFF 0xEF

/** @Description Used when you want to clear out the 5th bit.
 *
 * This flag represents 247 in decimal, and 11110111 in binary. */
#define BIT1_5_OFF 0xF7

/** @Description Used when you want to clear out the 6th bit.
 *
 * This flag represents 251 in decimal, and 11111011 in binary. */
#define BIT1_6_OFF 0xFB

/** @Description Used when you want to clear out the 7th bit.
 *
 * This flag represents 253 in decimal, and 11111101 in binary.*/
#define BIT1_7_OFF 0xFD

/** @Description Used when you want to clear out the 8th bit.
 *
 * This flag represents 254 in decimal, and 11111110 in binary. */
#define BIT1_8_OFF 0xFE


#include "png.h"

/** @Description Pixel_Data is a struct that provides a unified way to dynamically reference different types of pixel
 * data in Libpng. It supports operations on both png_byte and png_color data types, allowing for a single function
 * to handle and return.
 *
 * @members byte_data: This is the data_type that we use if the Pixel_Data is NOT for indexed images.
 * @members color_data:  This is the data_type we use if the Pixel_Data is for index images.
 * @members color_type:  Specifies the color type. If this is equal to PNG_COLOR_TYPE_PALETTE, then this struct contains
 * png_color.
 **/
typedef struct {
    union {
        struct{
            png_colorp color_data;
        };
        struct{
            png_bytep byte_data_16;
            png_uint_16 red_16_bit;
            png_uint_16 green_16_bit;
            png_uint_16 blue_16_bit;
        };

        struct {
            png_bytep byte_data;
            png_byte red;
            png_byte green;
            png_byte blue;
        };
    };
    png_uint_16 alpha_channel;
    int color_type;
    int bit_depth;
} Pixel_Data;


typedef struct PNG_Node {
    int index;
    Pixel_Data pixel_info;
    struct PNG_Node *next;
    struct PNG_Node *prev;
} node;


typedef struct palette_hash_table {
    const int MAX_VALUE;
    node *hash_bucket[256];
} palette_hash;


typedef struct png_channels {
    int red;
    int green;
    int blue;
    int alpha;
} png_channels;

/** @Description This is a function that we use to initialize a png_byte pointer. Based on the color type, and the
 * bit_depth, the amount of space that we'll need to allocate varies. As a result, the function considers these factors
 * and allocates the pointer according.
 *
 * @Description By default, if the color_type does not result to any assignment in the switch(), the pixel value is allocated
 * to have 4 channels.
 *
 * @param pixel - This parameter allows to modify the memory allocation for a png_byte array.
 * @param color_type - The type of color for the pixel.
 * @param bit_depth - The bit_depth for the pixel. It represents the size of each individual channel.
 * */
void Initialize_Pixel(png_bytepp pixel, int color_type, int bit_depth);
/** @ByteToColor Writing. Probably deleting
*
*/
png_color Byte_To_Color(const png_byte* pixel, int color_type, png_bytep trans_byte);

void Copy_Pixel(png_bytep copy_destination, png_bytepp copy_source, int x_coord,
                int y_coord, int color_type, int bit_depth);


/** @Description This function is designed any specific pixel that is in PNG file.
 *  Get_Pixel contains the necessarily functionality to unpack pixels that are less than a single byte. At the end
 *  of the function, it sends over a png_bytep that represents the values for each channel in a pixel.
 *
 * @param image - This is the array of pixels that we are working with. It comes the PNG file.
 * @param x_coord - This is the x-coordinate for the image. It tells us what column the pixel is in.
 * @param y_coord - This is the y-coordinate for the image. It tells us what row the pixel is in.
 * @param color_type - This signifies the color_type for the image. This is important since it helps informs us
 * on what size to select for the png_byte.
 * @param bit_depth - The size of the color channel.
 * @param palette - An optional pointer to a palette. Only used when using PNG_COLOR_TYPE_PALETTE.
 * @param trans_alpha - Optional value containing alpha channel for indexed images.
 * @param num_trans - Optional value to denote # of entries in trans_alpha. */
Pixel_Data Get_Pixel(png_bytepp image, int x_coord, int y_coord, int color_type, int bit_depth, png_colorp palette,
                     png_bytep trans_alpha, int *num_trans);

Pixel_Data Transform_RGBA_PNG2(Pixel_Data pixel, int target_color_type, int src_bit_depth, int target_bit_depth);

/** @Description Transform_RGBA_PNG is a function that carries a pixel transformation. Basically, it takes a pixel that is currently
 * in RGBA form, and transform it to the target_color_type specified.*/
png_bytep Transform_RBGA_PNG(const png_byte* pixel, int target_color_type, int src_bit_depth, int target_bit_depth);

/** @Description Transforms a GA_Alpha typed pixel into any other color type. */
png_bytep Transform_GRAYALPHA_PNG(const png_byte* pixel, int target_color_type, int bit_depth);

/** @Description Transforms GrayScaled type pixel into any other type. */
png_bytep Transform_GRAY_PNG(const png_byte* pixel, int target_color_type, int bit_depth);

/** @Description Transforms a traditional RGB pixel into any other type. */
Pixel_Data Transform_RGB_PNG(Pixel_Data* pixel, int target_color_type, int bit_depth);

/**
 * @TO_DO FIX SCENARIO FOR PIXELS THAT ARE LESS THAN 1 BYTE. IT IS NOT PROPERLY CLEARING THE BITS AND USES A AND instead
 * OF AN OR BITMASK.
 * */

void Set_Pixel(png_bytepp image, Pixel_Data* new_pixel, int x_coord,
               int y_coord, int color_type, int bit_depth, png_colorp palette,
               png_bytep trans_alpha, int *num_trans, palette_hash* pal_hash);

Pixel_Data Transform_RBGA_PNG2(Pixel_Data pixel, int target_color_type, int src_bit_depth, int target_bit_depth);
int Get_Gray_Value(Pixel_Data pixel);
/** @Description This function takes pixel data of the color_type PNG_COLOR_TYPE_PALETTE, and transforms it into
 * the equivalent values of another color_type and bit_depth.
 * */
void Transform_Indexed_PNG(Pixel_Data pixel, Pixel_Data* new_pixel, int target_color_type, int target_bit_depth);

/**
 * @Description Transforms the pixel into a specified color type and bit depth.
 *
 *
 * This function converts the color representation and bit depth of a given pixel to the target specifications.
 * It supports various color types and bit depths, providing flexibility in how pixel data is processed and stored.
 * It works by using various subroutines like: [INCLUDE THEM HERE]
 *
 * @param pixel The pixel data to be transformed.
 * @param target_color_type - The desired color type for the transformed pixel.
 * @param target_bit_depth - The desired bit depth for the transformed pixel.
 * @return A new Pixel_Data structure containing the transformed pixel data.
 */
Pixel_Data Pixel_Transformation(Pixel_Data pixel, int target_color_type, int target_bit_depth);

void Set_Channel_Value(Pixel_Data *pixel, png_bytep channel, int channel_offset);
void Set_Red_Channel(Pixel_Data *pixel);
void Set_Green_Channel(Pixel_Data *pixel);
void Set_Blue_Channel(Pixel_Data *pixel);

int Get_Red_Channel(Pixel_Data pixel);
int Get_Green_Channel(Pixel_Data pixel);
int Get_Blue_Channel(Pixel_Data pixel);
int Get_Alpha_Channel(Pixel_Data pixel);

Pixel_Data Combine_Pixels(Pixel_Data foreground, Pixel_Data background, int color_type);
void Read_PNG(png_structp* read_ptr, png_infop* read_info_ptr, FILE* input_file, png_bytepp* row_pointers,
              png_uint_32 *height, png_uint_32* width);

void Write_PNG(png_structp* write_ptr, png_infop* write_info_ptr, FILE* output_file, png_bytepp row_pointers,
               png_uint_32 height, png_uint_32 width, int bit_depth, int color_type);
void Initialize_Pixels(png_bytepp *pixel_array, int height, size_t bytes_per_row);
void Initialize_Pixel2(Pixel_Data* pixel, int color_type, int bit_depth);
void Initialize_PNG(png_structp* read_ptr, png_infop* read_info_ptr, FILE* input_file);
void Initialize_PNG_Writer(png_structp* write_ptr, png_infop* write_info_ptr, FILE* output_file);
void Initialize_PNG_Reader(png_structp* read_ptr, png_infop* read_info_ptr, FILE* input_file);