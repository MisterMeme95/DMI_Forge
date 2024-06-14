#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON/cJSON.h"
#include <png.h>
#include "../PixelManip.h"
#include <math.h>

#include <unistd.h>
#define MAX_NODES 400

typedef struct {
    int width;
    int height;
} ImageDimensions;

// Function to calculate dimensions of a PNG based on number of frames and frame dimensions
ImageDimensions calculate_dmi_dimensions(int frames, int frame_width, int frame_height) {
    int W = ceil(sqrt(frames));       // Calculate number of columns (width)
    int H = ceil((double)frames / W); // Calculate number of rows (height)

    // Calculate dimensions of the final image
    ImageDimensions img_dim;
    img_dim.width = W * frame_width;  // Total width of the PNG
    img_dim.height = H * frame_height; // Total height of the PNG

    return img_dim;
}

void get_tile_position(int local_id, int *start_row, int *start_col, int IMAGE_WIDTH) {
    int tiles_per_row = IMAGE_WIDTH / 32;

    *start_row = (local_id / tiles_per_row) * 32;
    *start_col = (local_id % tiles_per_row) * 32;
}


void alpha_blend(png_bytepp source_image, int start_row, int start_col, png_bytepp dest_image, int image_width, int image_height) {
    for (int i = 0; i < 32; i++) {
        int src_row = start_row + i;
        if (src_row >= image_height) {
            printf("Skipping src_row %d as it exceeds image height %d\n", src_row, image_height);
          //  sleep(10);
            continue;
        }

        for (int j = 0; j < 32; j++) {
            int src_col = start_col + j;
            if (src_col >= image_width) {
                printf("Skipping src_col %d as it exceeds image width %d\n", src_col, image_width);
                //sleep(10);

                continue;
            }

//            printf("Accessing src_row %d, src_col %d\n", src_row, src_col);
            png_bytep src_pixel = source_image[src_row] + src_col * 4;
            png_bytep dst_pixel = dest_image[i] + j * 4;

            double alpha = src_pixel[3] / 255.0;

            dst_pixel[0] = (png_byte)(alpha * src_pixel[0] + (1 - alpha) * dst_pixel[0]);
            dst_pixel[1] = (png_byte)(alpha * src_pixel[1] + (1 - alpha) * dst_pixel[1]);
            dst_pixel[2] = (png_byte)(alpha * src_pixel[2] + (1 - alpha) * dst_pixel[2]);
            dst_pixel[3] = (png_byte)(255 * (alpha + (1 - alpha) * (dst_pixel[3] / 255.0)));
        }
    }

}





int get_local_id(int gid, int *list_of_gid, int gid_size) {
    if (gid_size == 0) {
        fprintf(stderr, "Error: list_of_gid is empty.\n");
        return -1;  // Indicates an error or non-computable situation
    }

    // Assume list_of_gid is sorted; if not, you must sort it first
    // This is critical: sorting the list here is not shown for simplicity

    if (gid == 0) {
        return 0;  // Assuming gid 0 means local_id should be 0
    }

    for (int i = 0; i < gid_size; i++) {
        if (list_of_gid[i] > gid) {
            return (i == 0) ? 0 : gid - list_of_gid[i - 1];
        }
    }

    // If all gids in the list are less than the given gid
    return gid - list_of_gid[gid_size - 1];
}
typedef struct {
    int color_type;         // PNG color type, e.g., PNG_COLOR_TYPE_RGBA
    png_structp png_ptr;    // Pointer to png struct
    png_infop info_ptr;     // Pointer to png info
    png_bytepp row_pointers; // Array of pointers to pixel rows
    int width;              // Width of the image
    int height;             // Height of the image
    int gid;                // Global identifier for the tile
    char *mapId;            // ID used for BYOND's mapping system
} DMNode;

void write_dm_file(const char *filename, DMNode* dm_nodes, size_t node_count) {
    FILE *fp = fopen(filename, "w");  // Open file for writing
    if (!fp) {
        fprintf(stderr, "Failed to open file %s for writing.\n", filename);
        return;
    }

    // Write the initial line before the loop
    fprintf(fp, "turf\n");

    // Loop through dm_nodes and write each node's data formatted as specified
    for (size_t i = 0; i < node_count; i++) {
        fprintf(fp, "\n\tt%d\n", i);
        fprintf(fp, "\t\ticon = 'fakemap.dmi'\n");
        fprintf(fp, "\t\ticon_state = \"%d\"\n", i);  // Incrementing gid for example
    }

    fclose(fp);  // Close the file after writing
}
void create_png_with_text_chunks(const char *filename, DMNode* dm_nodes, size_t node_count) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Failed to open file %s for writing.\n", filename);
        return;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, 640, 640, 8, PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    char dimensions_info[1024];
    sprintf(dimensions_info, "width = 32, height = 32");
    char state_info[1024];
//
    png_text* text_ptr = (png_text*) calloc(node_count + 1, sizeof(png_text));

    char zTxt[100000] = "#BEGIN DMI\nversion = 4.0\n\twidth = 32\n\theight = 32\n";
    for (size_t i = 0; i < node_count; i++) {
        char additional_info[256];
        sprintf(additional_info, "state = \"%d\"\n\tdirs = 1\n\tframes = 1\n\tdelay = 1\n", i);
        strcat(zTxt, additional_info);  // Append new state info to zTxt
    }


    png_text text_chunk;
    text_chunk.compression = PNG_TEXT_COMPRESSION_zTXt;
    text_chunk.key = "Description";
    text_chunk.text = zTxt;
    text_chunk.text_length = strlen(zTxt);

    png_set_text(png_ptr, info_ptr, &text_chunk, 1);  // Correctly apply the single text_chunk

    png_write_info(png_ptr, info_ptr);

    // Image data handling
    png_bytepp row_pointers = (png_bytepp)malloc(640 * sizeof(png_bytep));
    for (int y = 0; y < 640; y++) {
        row_pointers[y] = (png_bytep)malloc(640 * 4); // Assuming RGBA
        memset(row_pointers[y], 0, 640 * 4); // Initialize the row to be transparent or black
    }

    // Copying each node's image data
    int tiles_per_row = 640 / 32;
    for (size_t i = 0; i < node_count; i++) {
        int dest_x = (int)(i % tiles_per_row) * 32;
        int dest_y = (int)(i / tiles_per_row) * 32;

        for (int y = 0; y < 32; y++) {
            memcpy(&row_pointers[dest_y + y][dest_x * 4], dm_nodes[i].row_pointers[y], 32 * 4);
        }
    }

    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);

    // Cleanup
    for (int y = 0; y < 640; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    free(text_ptr);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
}
void Initialize_DM(DMNode* node) {
    // Set constants
    node->color_type = PNG_COLOR_TYPE_RGBA;
    node->width = 32;   // Set width
    node->height = 32;  // Set height

    // Allocate and initialize the png_struct with the desired error handler
    node->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (node->png_ptr == NULL) {
        fprintf(stderr, "Failed to create PNG struct\n");
        return;
    }

    // Allocate and initialize the png_info
    node->info_ptr = png_create_info_struct(node->png_ptr);
    if (node->info_ptr == NULL) {
        png_destroy_write_struct(&node->png_ptr, NULL);
        fprintf(stderr, "Failed to create PNG info\n");
        return;
    }

    // Set up error handling (if you are using setjmp/longjmp)
    if (setjmp(png_jmpbuf(node->png_ptr))) {
        png_destroy_write_struct(&node->png_ptr, &node->info_ptr);
        fprintf(stderr, "Error during PNG creation\n");
        return;
    }

    // Allocate memory for row pointers
    // Set basic PNG info (assuming 8-bit depth per channel)
    png_set_IHDR(node->png_ptr, node->info_ptr, node->width, node->height, 8, node->color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    node->row_pointers = (png_bytepp) malloc(sizeof(png_bytep) * node->height);
    for (int i = 0; i < node->height; i++) {
        node->row_pointers[i] = (png_bytep) malloc(png_get_rowbytes(node->png_ptr, node->info_ptr));
    }

    for(int i = 0; i < 32; i++){
        for(int o = 0; o < png_get_rowbytes(node->png_ptr, node->info_ptr); o++){
            node->row_pointers[i][o] = 0;
        }
    }

}

DMNode* allocate_and_initialize_nodes(size_t num_nodes) {
    DMNode* nodes = (DMNode*)malloc(num_nodes * sizeof(DMNode));
    if (nodes == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    for (size_t i = 0; i < num_nodes; i++) {
        Initialize_DM(&nodes[i]);
    }

    return nodes;
}

void Free_DM(DMNode* node) {
    if (node->row_pointers != NULL) {
        for (int i = 0; i < node->height; i++) {
            free(node->row_pointers[i]);
        }
        free(node->row_pointers);
    }

    if (node->png_ptr && node->info_ptr) {
        png_destroy_write_struct(&node->png_ptr, &node->info_ptr);
    }

    if (node->mapId != NULL) {
        free(node->mapId);
    }
}

void incrementMapId(char *id) {
    int length = strlen(id);
    for (int i = length - 1; i >= 0; i--) {
        if (id[i] < 'z') {
            id[i]++;
            break;
        } else {
            id[i] = 'a';
        }
    }
}

int Get_ID(int gid, int *list_of_gid, int gid_size);
// Function to read the entire content of a file into a buffer
char *readFile(char *filename) {
    char *buffer = NULL;
    long length;
    FILE *f = fopen(filename, "rb"); // open the file in binary mode

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length + 1);
        if (buffer) {
            fread(buffer, 1, length, f);
            buffer[length] = '\0'; // null-terminate the buffer
        }
        fclose(f);
    }

    return buffer;
}

int calculateMapIdLength(int n) {
    if (n == 0) return 1;  // To handle log(0) scenario
    return (int)ceil(log(n) / log(26));
}

int main() {
    printf("Died #11\n");
    // Replace the following path with your actual file path
    char *filename = "onmyotest.tmj";
    char *data = readFile(filename);
    int tileheight = 0, tilewidth = 0;
    cJSON *json = cJSON_Parse(data);
    char *mapIdArray[MAX_NODES];
    DMNode* dm_nodes = allocate_and_initialize_nodes(400);

    cJSON *tilesets = cJSON_GetObjectItem(json, "tilesets");
    cJSON *layers = cJSON_GetObjectItem(json,"layers");

    int max_length = 0;
    // First determine the maximum length of the arrays
    cJSON *layer;
    cJSON_ArrayForEach(layer, layers) {
        cJSON *data = cJSON_GetObjectItem(layer, "data");
        if (cJSON_IsArray(data)) {
            int current_length = cJSON_GetArraySize(data);
            if (current_length > max_length) {
                max_length = current_length;
            }
        }
    }

    printf("max_length = %d\n", max_length);

    cJSON_ArrayForEach(layer, layers) {
        // Access properties of each layer like 'data', 'id', etc.
        cJSON *data2 = cJSON_GetObjectItem(layer, "data");
        // Do something with 'data'
        printf("%s\n", cJSON_Print(data2));
    }
    cJSON *tileset_obj;// = cJSON_GetObjectItem(json, "tilesets");
    int size_of_tile = cJSON_GetArraySize(tilesets);
    int *list_of_gid = (int*)malloc(sizeof(int) * size_of_tile);
    int counter = 0;
    cJSON_ArrayForEach(tileset_obj, tilesets) {
        cJSON *firstgid = cJSON_GetObjectItem(tileset_obj, "firstgid");
        cJSON *source = cJSON_GetObjectItem(tileset_obj, "source");


        if (cJSON_IsNumber(firstgid) && cJSON_IsString(source)) {
            list_of_gid[counter] = firstgid->valueint;
            counter += 1;
            printf("Tileset - First GID: %d, Source: %s\n", firstgid->valueint, source->valuestring);
        }

    }


    tilewidth = atoi(cJSON_Print(cJSON_GetObjectItem(json, "tilewidth")));
    tileheight = atoi(cJSON_Print(cJSON_GetObjectItem(json, "tileheight")));
    FILE* GrassTile = fopen("final_grass.png", "rb");
    if(!GrassTile) {
        printf("Can't open file %s for reading\n", "final_grass");
        return EXIT_FAILURE;
    }
    FILE* DirtTile = fopen("grass_and_dirt-02-24-05.png", "rb");
    png_bytepp row_pointers;
    char *input_file = NULL;
    int bit_depth = 0, color_type = 0, interlace_method = 0, compression = 0, filter = 0;
    png_structp read_ptr;
    png_infop read_info_ptr;
    png_bytep trans_alpha = NULL;
    png_uint_32 height = 0;
    png_uint_32 width = 0;


    png_bytepp row_pointers2;
    int bit_depth2 = 0, color_type2 = 0, interlace_method2 = 0, compression2 = 0, filter2 = 0;
    png_structp read_ptr2;
    png_infop read_info_ptr2;
    png_bytep trans_alpha2 = NULL;
    png_uint_32 height2 = 0;
    png_uint_32 width2 = 0;


    DMNode *list_of_nodes = allocate_and_initialize_nodes(400);

    Read_PNG(&read_ptr, &read_info_ptr, GrassTile, &row_pointers, &height, &width);
    Read_PNG(&read_ptr2, &read_info_ptr2, DirtTile, &row_pointers2, &height2, &width2);

    png_byte color_type1 = png_get_color_type(read_ptr, read_info_ptr);
    png_byte bit_depth1 = png_get_bit_depth(read_ptr, read_info_ptr);

    printf("Color Type: %d, Bit Depth: %d\n", color_type1, bit_depth1);
    int node_count = 0;  // Counter for added nodes
    char currentMapId[3] = "aa"; // Start from "aaa"
    for (int i = 0; i < max_length; i++) {
        int checker = 0;
        DMNode new_node;
        Initialize_DM(&new_node);
        new_node.mapId = strdup(currentMapId);
        //printf("%s\n", new_node.mapId);
        incrementMapId(currentMapId);
        mapIdArray[node_count] = new_node.mapId;
        cJSON_ArrayForEach(layer, layers) {
            cJSON *data1 = cJSON_GetObjectItem(layer, "data");
            if (cJSON_IsArray(data1)) {
//                printf("Died at top of loop\n");
                cJSON *item = cJSON_GetArrayItem(data1, i);
//                printf("Died at top of loop2\n");
                if (item != NULL && cJSON_IsNumber(item)) {
                    int local_id = get_local_id(item->valueint, list_of_gid, counter);
                    int start_row, start_col;

                    if(checker == 0){
                        get_tile_position(local_id, &start_row, &start_col, (int)width);
                        printf("GID = %d\nLocal ID is %d\n\n", item->valueint, get_local_id(item->valueint, list_of_gid, counter));
                        printf("Start_Row = %d\nStart_Col = %d\n", start_row, start_col);
                        printf("Final Grass\n");
                        alpha_blend(row_pointers, start_row, start_col, new_node.row_pointers,
                                    (int)width, (int)height);


                    }

                    else {
                        get_tile_position(local_id, &start_row, &start_col, (int)width2);
                        new_node.gid = item->valueint;
                        printf("GID = %d\nLocal ID is %d\n\n", item->valueint, get_local_id(item->valueint, list_of_gid, counter));
                        printf("Start_Row = %d\nStart_Col = %d\n", start_row, start_col);
                        printf("Dirt Grass\n");
                        alpha_blend(row_pointers2, start_row, start_col, new_node.row_pointers,
                                    (int)width2, (int)height2);

                    }
;
                }
            }
            checker += 1;
        }
        dm_nodes[node_count] = new_node;
        node_count += 1;
    }

    printf("Tile Width = %d\nTile Height = %d\n",tilewidth, tileheight);

    ImageDimensions dimensions = calculate_dmi_dimensions(400, 32, 32);
    printf("PNG Dimensions: %d x %d pixels\n", dimensions.width, dimensions.height);

    printf("Listing DMNode details:\n");

    FILE *file = fopen("FakeMap.dmm", "w");  // Open the file for writing


    for (size_t i = 0; i < node_count; i++) {
        if (dm_nodes[i].mapId != NULL) {
            fprintf(file, "\"%s\" = (/turf/t%d, /area)\n", dm_nodes[i].mapId, i);
        } else {
            fprintf(file, "[NULL = (/turf/t%d, /area)\n", i);  // Handle NULL mapId
        }
    }

    // Insert two newlines before starting the mapId array
    fprintf(file, "\n\n");

    // Print the mapIdArray in the required format
    int char_count = 0;  // Count of characters to manage lines
    fprintf(file, "(1,1,1) = {\"\n"); // Start the first line

    for (size_t i = 0; i < node_count; i++) {
        const char* mapId = dm_nodes[i].mapId ? dm_nodes[i].mapId : "NULL";
        size_t mapId_len = strlen(mapId);

        if (char_count >= 20) {
            fprintf(file, "\n");  // End current line and start a new one
            char_count = 0;  // Reset character count for the new line
        }

        fprintf(file, "%s", mapId);  // Print mapId directly
        char_count += 1;  // Update character count
    }

    fprintf(file, "\"}");

    create_png_with_text_chunks("fakemap.dmi", dm_nodes, node_count);
    write_dm_file("fakemap.dm", dm_nodes, node_count);
    return 0;
}


