/* ************************
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
typedef struct Node {
    void *data;
    void *next;
    void *prev;
} Node;


typedef struct Linked_list {
    Node* head;
    Node* tail;
} Linked_list;


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
/** @ByteToColor Writing
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
png_bytep Get_Pixel(png_bytepp image, int x_coord, int y_coord, int color_type, int bit_depth, png_colorp palette,
                    png_bytep trans_alpha, int *num_trans);

/** @Description Transform_RGBA_PNG is a function that carries a pixel transformation. Basically, it takes a pixel that is currently
 * in RGBA form, and transform it to the target_color_type specified.*/
png_bytep Transform_RBGA_PNG(const png_byte* pixel, int target_color_type, int src_bit_depth, int target_bit_depth);

/** @Description Transforms a GA_Alpha typed pixel into any other color type. */
png_bytep Transform_GRAYALPHA_PNG(const png_byte* pixel, int target_color_type, int bit_depth);

/** @Description Transforms GrayScaled type pixel into any other type. */
png_bytep Transform_GRAY_PNG(const png_byte* pixel, int target_color_type, int bit_depth);

/** @Description Transforms a traditional RGB pixel into any other type. */
png_bytep Transform_RBG_PNG(const png_byte* pixel, int target_color_type, int bit_depth);

/** @Description Transforms a color from a palette to a png_byte.*/
png_bytep Transform_Indexed_PNG(png_color pixel, int target_color_type, int bit_depth);