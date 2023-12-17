/** Bit Depth 4 Masks **/

// 240 in decimal
#define BIT4_L_ON 0xF0

// 15 in decimal
#define BIT4_R_ON 0x0F

// 240 in decimal
#define BIT4_R_OFF 0xF0

// 15 in decimal
#define BIT4_L_OFF 0x0F

/** Bit Depth 2 Masks **/

// 192 in decimal
#define BIT2_1_ON 0xC0

// 48 in decimal
#define BIT2_2_ON 0x30

// 12 in decimal
#define BIT2_3_ON 0x0C

// 3 in decimal
#define BIT2_4_ON 0x03

// 63 in decimal
#define BIT2_1_OFF 0x3F

// 207 in decimal
#define BIT2_2_OFF 0xCF

// 243 in decimal
#define BIT2_3_OFF 0xF3

// 252 in decimal
#define BIT2_4_OFF 0xFC

/** Bit Depth 1 Masks **/

// 128 in decimal
#define BIT1_1_ON 0x80

// 64 in decimal
#define BIT1_2_ON 0x40

// 32 in decimal
#define BIT1_3_ON 0x20

// 16 in decimal
#define BIT1_4_ON 0x10

// 8 in decimal
#define BIT1_5_ON 0x08

// 4 in decimal
#define BIT1_6_ON 0x04

// 2 in decimal
#define BIT1_7_ON 0x02

// 1 in decimal
#define BIT1_8_ON 0x01


// 127 in decimal
#define BIT1_1_OFF 0x7F

// 191 in decimal
#define BIT1_2_OFF 0xBF

// 223 in decimal
#define BIT1_3_OFF 0xDF

// 239 in decimal
#define BIT1_4_OFF 0xEF

// 247 in decimal
#define BIT1_5_OFF 0xF7

// 251 in decimal
#define BIT1_6_OFF 0xFB

// 253 in decimal
#define BIT1_7_OFF 0xFD

// 254 in decimal
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

/** This function initializes a pixel value. Based on the color type, a different amount of space may be need to be allocated.*/
void Initialize_Pixel(png_bytepp pixel, int color_type, int bit_depth);

/** Transform a png_byte into an actual color. */
png_color Byte_To_Color(const png_byte* pixel, int color_type, png_bytep trans_byte);

void Copy_Pixel(png_bytep copy_destination, png_bytepp copy_source, int x_coord,
                int y_coord, int color_type, int bit_depth);

png_bytep Get_Pixel(png_bytepp image, int x_coord, int y_coord, int color_type, int bit_depth, png_colorp palette,
                    png_bytep trans_alpha, int *num_trans);

/** Transform_RGBA_PNG is a function that carries a pixel transformation. Basically, it takes a pixel that is currently
 * in RGBA form, and transform it to the target_color_type specified.*/
png_bytep Transform_RBGA_PNG(const png_byte* pixel, int target_color_type, int src_bit_depth, int target_bit_depth);

/** Transforms a GA_Alpha typed pixel into any other color type. */
png_bytep Transform_GRAYALPHA_PNG(const png_byte* pixel, int target_color_type, int bit_depth);

/** Transforms GrayScaled type pixel into any other type. */
png_bytep Transform_GRAY_PNG(const png_byte* pixel, int target_color_type, int bit_depth);

/** Transforms a traditional RGB pixel into any other type. */
png_bytep Transform_RBG_PNG(const png_byte* pixel, int target_color_type, int bit_depth);

/** Transforms a color from a palette to a png_byte.*/
png_bytep Transform_Indexed_PNG(png_color pixel, int target_color_type, int bit_depth);