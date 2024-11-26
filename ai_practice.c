#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

/**
 * Enum for supported color types.
 */
typedef enum {
    CT_GRAY = PNG_COLOR_TYPE_GRAY,
    CT_PALETTE = PNG_COLOR_TYPE_PALETTE,
    CT_RGB = PNG_COLOR_TYPE_RGB,
    CT_GRAY_ALPHA = PNG_COLOR_TYPE_GRAY_ALPHA,
    CT_RGB_ALPHA = PNG_COLOR_TYPE_RGB_ALPHA
} color_type_enum;

/**
 * Function to map string to color_type_enum.
 */
color_type_enum get_color_type(const char *str) {
    if (strcasecmp(str, "gray") == 0)
        return CT_GRAY;
    if (strcasecmp(str, "palette") == 0)
        return CT_PALETTE;
    if (strcasecmp(str, "rgb") == 0)
        return CT_RGB;
    if (strcasecmp(str, "graya") == 0)
        return CT_GRAY_ALPHA;
    if (strcasecmp(str, "rgba") == 0)
        return CT_RGB_ALPHA;
    // Default to RGB if unknown
    return CT_RGB;
}

/**
 * Function to print usage instructions.
 */
void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s <input_png> <output_png> <target_color_type> <target_bit_depth>\n", prog_name);
    fprintf(stderr, "Color Types: gray, graya, palette, rgb, rgba\n");
    fprintf(stderr, "Bit Depths: 1, 2, 4, 8, 16\n");
}

/**
 * Function to read a PNG file.
 */
int read_png(const char *filename, png_structp *png_ptr, png_infop *info_ptr,
             png_bytep **row_pointers, int *width, int *height, int *bit_depth,
             int *color_type) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open input file %s.\n", filename);
        return -1;
    }

    // Read and verify PNG signature
    unsigned char header[8];
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        fprintf(stderr, "Error: File %s is not a valid PNG.\n", filename);
        fclose(fp);
        return -1;
    }

    // Initialize read struct
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!*png_ptr) {
        fprintf(stderr, "Error: png_create_read_struct failed.\n");
        fclose(fp);
        return -1;
    }

    // Initialize info struct
    *info_ptr = png_create_info_struct(*png_ptr);
    if (!*info_ptr) {
        fprintf(stderr, "Error: png_create_info_struct failed.\n");
        png_destroy_read_struct(png_ptr, NULL, NULL);
        fclose(fp);
        return -1;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(*png_ptr))) {
        fprintf(stderr, "Error: An error occurred while reading the PNG file.\n");
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        fclose(fp);
        return -1;
    }

    png_init_io(*png_ptr, fp);
    png_set_sig_bytes(*png_ptr, 8);

    // Read PNG info
    png_read_info(*png_ptr, *info_ptr);

    // Get image information
    png_get_IHDR(*png_ptr, *info_ptr, (png_uint_32*)width, (png_uint_32*)height,
                 bit_depth, color_type, NULL, NULL, NULL);

    // Ensure the image is grayscale or has alpha
    if (*color_type != PNG_COLOR_TYPE_GRAY && *color_type != PNG_COLOR_TYPE_GRAY_ALPHA) {
        fprintf(stderr, "Error: Input image is not grayscale or grayscale with alpha.\n");
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        fclose(fp);
        return -1;
    }

    // If grayscale with alpha, strip alpha for indexed palette
    if (*color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_strip_alpha(*png_ptr);
    }

    // Expand grayscale images of bit depth < 8 to 8 bits
    if (*bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(*png_ptr);
        *bit_depth = 8;
    }

    // Update info structure to apply transformations
    png_read_update_info(*png_ptr, *info_ptr);

    // Allocate memory for rows
    *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * (*height));
    if (!*row_pointers) {
        fprintf(stderr, "Error: Could not allocate memory for row pointers.\n");
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        fclose(fp);
        return -1;
    }

    for (int y = 0; y < *height; y++) {
        (*row_pointers)[y] = (png_byte*)malloc(png_get_rowbytes(*png_ptr, *info_ptr));
        if (!(*row_pointers)[y]) {
            fprintf(stderr, "Error: Could not allocate memory for row %d.\n", y);
            // Free previously allocated rows
            for (int k = 0; k < y; k++) {
                free((*row_pointers)[k]);
            }
            free(*row_pointers);
            png_destroy_read_struct(png_ptr, info_ptr, NULL);
            fclose(fp);
            return -1;
        }
    }

    // Read image data
    png_read_image(*png_ptr, *row_pointers);

    fclose(fp);
    return 0;
}

/**
 * Function to write a PNG file.
 */
int write_png(const char *filename, png_structp png_ptr_write, png_infop info_ptr_write,
              png_bytep **row_pointers, int width, int height, int bit_depth,
              int color_type) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open output file %s.\n", filename);
        return -1;
    }

    // Initialize write struct
    png_structp png_ptr_w = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr_w) {
        fprintf(stderr, "Error: png_create_write_struct failed.\n");
        fclose(fp);
        return -1;
    }

    // Initialize info struct for writing
    png_infop info_ptr_w = png_create_info_struct(png_ptr_w);
    if (!info_ptr_w) {
        fprintf(stderr, "Error: png_create_info_struct failed.\n");
        png_destroy_write_struct(&png_ptr_w, NULL);
        fclose(fp);
        return -1;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr_w))) {
        fprintf(stderr, "Error: An error occurred while writing the PNG file.\n");
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        fclose(fp);
        return -1;
    }

    png_init_io(png_ptr_w, fp);

    // Write the header
    png_set_IHDR(
            png_ptr_w,
            info_ptr_w,
            width,
            height,
            bit_depth,
            color_type,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE,
            PNG_FILTER_TYPE_BASE
    );

    // Handle palette if the target color type is palette-based
    if (color_type == CT_PALETTE) {
        // Create a palette (example: grayscale palette)
        png_color palette[256];
        int num_palette = 256;
        for (int i = 0; i < num_palette; i++) {
            palette[i].red = i;
            palette[i].green = i;
            palette[i].blue = i;
        }
        png_set_PLTE(png_ptr_w, info_ptr_w, palette, num_palette);
    }

    // Write the info struct
    png_write_info(png_ptr_w, info_ptr_w);

    // Write image data
    png_write_image(png_ptr_w, row_pointers);

    // Write the end of the PNG file
    png_write_end(png_ptr_w, info_ptr_write);

    // Cleanup
    png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
    fclose(fp);

    return 0;
}

/**
 * Function to free allocated row pointers.
 */
void free_rows(png_bytep *row_pointers, int height) {
    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
}

/**
 * Function to apply desired transformations based on target color_type and bit_depth.
 */
int apply_transformations(png_structp png_ptr, png_infop info_ptr,
                          int target_color_type, int target_bit_depth) {
    // Example transformation logic

    // 1. Convert palette-based to RGB if necessary
    if (target_color_type == CT_RGB && png_get_color_type(png_ptr, info_ptr) == CT_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }

    // 2. Convert grayscale to RGB/RGBA
    if ((target_color_type == CT_RGB || target_color_type == CT_RGB_ALPHA) &&
        (png_get_color_type(png_ptr, info_ptr) == CT_GRAY ||
         png_get_color_type(png_ptr, info_ptr) == CT_GRAY_ALPHA)) {
        png_set_gray_to_rgb(png_ptr);
    }

    // 3. Add alpha channel if required
    if (target_color_type == CT_RGB_ALPHA || target_color_type == CT_GRAY_ALPHA) {
        if (!(png_get_color_type(png_ptr, info_ptr) & PNG_COLOR_MASK_ALPHA)) {
            png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
        }
    }

    // 4. Remove alpha channel if not required
    if (target_color_type == CT_RGB || target_color_type == CT_GRAY) {
        if (png_get_color_type(png_ptr, info_ptr) & PNG_COLOR_MASK_ALPHA) {
            png_set_strip_alpha(png_ptr);
        }
    }

    // 5. Convert to grayscale if required
    if (target_color_type == CT_GRAY || target_color_type == CT_GRAY_ALPHA) {
        png_set_rgb_to_gray(png_ptr, 1, -1, -1);
    }

    // 6. Convert to palette-based if required
    if (target_color_type == CT_PALETTE) {
        // Ensure the image is in RGB before quantization
        if (png_get_color_type(png_ptr, info_ptr) != CT_RGB &&
            png_get_color_type(png_ptr, info_ptr) != CT_RGB_ALPHA) {
            png_set_gray_to_rgb(png_ptr);
        }
        // Set quantization parameters if needed
        // For simplicity, we'll use the default quantization
        // Note: png_set_quantize may require additional arguments based on libpng version
        // Hence, it's safer to handle palette creation manually
        // Remove or comment out png_set_quantize
        // png_set_quantize(png_ptr, NULL, 0, NULL, 0, 256);
    }

    // 7. Adjust bit depth
    if (png_get_bit_depth(png_ptr, info_ptr) != target_bit_depth) {
        if (target_bit_depth < png_get_bit_depth(png_ptr, info_ptr)) {
            png_set_strip_16(png_ptr);
        } else if (target_bit_depth > png_get_bit_depth(png_ptr, info_ptr)) {
            png_set_packing(png_ptr);
        }
        // Note: More sophisticated scaling can be implemented if needed
    }

    // Update the info structure to apply transformations
    png_read_update_info(png_ptr, info_ptr);

    return 0;
}

/**
 * Main function.
 *
 * Usage:
 *   ./png_transformer <input_png> <output_png> <target_color_type> <target_bit_depth>
 *
 * Example:
 *   ./png_transformer input.png output.png rgba 8
 */
int main(int argc, char *argv[]) {
    if (argc != 5) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_png = argv[1];
    const char *output_png = argv[2];
    const char *target_color_str = argv[3];
    int target_bit_depth = atoi(argv[4]);

    // Validate bit_depth
    if (target_bit_depth != 1 && target_bit_depth != 2 &&
        target_bit_depth != 4 && target_bit_depth != 8 &&
        target_bit_depth != 16) {
        fprintf(stderr, "Error: Unsupported bit depth %d.\n", target_bit_depth);
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Map target_color_str to color_type_enum
    color_type_enum target_color_type = get_color_type(target_color_str);
    if (target_color_type != CT_GRAY && target_color_type != CT_PALETTE &&
        target_color_type != CT_RGB && target_color_type != CT_GRAY_ALPHA &&
        target_color_type != CT_RGB_ALPHA) {
        fprintf(stderr, "Error: Unsupported color type '%s'.\n", target_color_str);
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Read the input PNG
    png_structp png_ptr_read;
    png_infop info_ptr_read;
    png_bytep *row_pointers_read;
    int width, height, bit_depth;
    int color_type;

    if (read_png(input_png, &png_ptr_read, &info_ptr_read, &row_pointers_read,
                 &width, &height, &bit_depth, &color_type) != 0) {
        fprintf(stderr, "Error: Failed to read input PNG.\n");
        return EXIT_FAILURE;
    }

    // Apply transformations
    if (apply_transformations(png_ptr_read, info_ptr_read, target_color_type, target_bit_depth) != 0) {
        fprintf(stderr, "Error: Failed to apply transformations.\n");
        free_rows(row_pointers_read, height);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        return EXIT_FAILURE;
    }

    // Allocate transformed row pointers after transformations
    png_bytep *row_pointers_write = (png_bytep*)malloc(sizeof(png_bytep) * height);
    if (!row_pointers_write) {
        fprintf(stderr, "Error: Could not allocate memory for transformed row pointers.\n");
        free_rows(row_pointers_read, height);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        return EXIT_FAILURE;
    }

    for (int y = 0; y < height; y++) {
        row_pointers_write[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr_read, info_ptr_read));
        if (!row_pointers_write[y]) {
            fprintf(stderr, "Error: Could not allocate memory for transformed row %d.\n", y);
            for (int k = 0; k < y; k++) {
                free(row_pointers_write[k]);
            }
            free(row_pointers_write);
            free_rows(row_pointers_read, height);
            png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
            return EXIT_FAILURE;
        }
        memcpy(row_pointers_write[y], row_pointers_read[y], png_get_rowbytes(png_ptr_read, info_ptr_read));
    }

    // Write the output PNG
    if (write_png(output_png, png_ptr_read, info_ptr_read, row_pointers_write,
                  width, height, target_bit_depth, target_color_type) != 0) {
        fprintf(stderr, "Error: Failed to write output PNG.\n");
        free_rows(row_pointers_write, height);
        free_rows(row_pointers_read, height);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        return EXIT_FAILURE;
    }

    // Cleanup
    free_rows(row_pointers_write, height);
    free_rows(row_pointers_read, height);
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);

    printf("Successfully transformed %s to %s with color type %s and bit depth %d.\n",
           input_png, output_png, target_color_str, target_bit_depth);

    return EXIT_SUCCESS;
}
