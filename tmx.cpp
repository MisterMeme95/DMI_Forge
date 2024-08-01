#include <tmxlite/Map.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/LayerGroup.hpp>
#include <tmxlite/TileLayer.hpp>
#include <iostream>
#include <array>
#include <string>
#include <unistd.h>
#include <cstring>
#include "PixelManip.h"
#include <stdio.h>
#include <cmath>
#include <vector>
#define PAUSE_AT_END 1
int check_if_png(char *file_name, FILE **fp) {
    char buf[PNG_BYTES_TO_CHECK];
    /* Open the prospective PNG file. */
    if ((*fp = fopen(file_name, "rb")) == NULL) {
        printf("Error opening input file\n");
        return 0;
    }

    /* Read in some of the signature bytes. */
    if (fread(buf, 1, PNG_BYTES_TO_CHECK, *fp) != PNG_BYTES_TO_CHECK){
        printf("Not enough bytes read!\n");
        return 0;
    }

    /* Compare the first PNG_BYTES_TO_CHECK bytes of the signature.
     * Return nonzero (true) if they match. */
    return(!png_sig_cmp((png_const_bytep)buf, 0, PNG_BYTES_TO_CHECK));
}

Image load_image(char* file_name){
    Image new_image;
    if(!check_if_png(file_name, &new_image.file_pointer)) {
        printf("The file you are attempting to read is not a valid PNG file!\n");
        exit(1);
    }
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
    if(new_image.color_type == PNG_COLOR_TYPE_PALETTE){
        png_get_PLTE(new_image.png_ptr, new_image.info_ptr, &new_image.palette, &new_image.palette_num);
        png_get_tRNS(new_image.png_ptr, new_image.info_ptr, &new_image.trans_alpha,
                     &new_image.trans_num, &new_image.trans_color);
    }

    else if(new_image.color_type == PNG_COLOR_TYPE_RGB | new_image.color_type == PNG_COLOR_TYPE_GRAY){
        png_get_tRNS(new_image.png_ptr, new_image.info_ptr, &new_image.trans_alpha,
                     &new_image.trans_num, &new_image.trans_color);
    }

    new_image.pixel_array = (png_bytepp)malloc(sizeof(png_bytep) * new_image.height);

    if (new_image.pixel_array == NULL) {
        fprintf(stderr, "Memory allocation failed for pixel_array.\n");
        exit(EXIT_FAILURE);
    }


    new_image.row_bytes = png_get_rowbytes(new_image.png_ptr,new_image.info_ptr);
    png_bytep contiguous_rows = (png_bytep) malloc(sizeof(png_byte) * new_image.height * new_image.row_bytes);
    for (png_uint_32 pix_index = 0; pix_index < new_image.height; pix_index++) {
        // new_image.pixel_array[pix_index] = (png_bytep)malloc(new_image.row_bytes );
        new_image.pixel_array[pix_index] = contiguous_rows + (pix_index * new_image.row_bytes);///(png_bytep)malloc(new_image.row_bytes );
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
namespace
{
    const std::array<std::string, 4u> LayerStrings =
            {
                    std::string("Tile"),
                    std::string("Object"),
                    std::string("Image"),
                    std::string("Group"),
            };
}

int find_nearest_gid(std::vector<uint32_t> &gid_vectors, uint32_t global_tid);
int main()
{
    tmx::Map map;

    if (map.load("Ancient Ruins example map.tmx"))
    {

        /**
         *
         *
         */

        auto stuff = map.getBounds();
        if (map.isInfinite())
        {
            std::cout << "Map is infinite.\n";
        }
        else
        {

            std::cout << "Map Dimensions: " << map.getBounds() << std::endl;
        }

        auto sizer = map.getTileSize();
        printf("Height - %f\nWidth -%f\nTile Size - %d, %d\n", stuff.height, stuff.width, sizer.x, sizer.y);


        //sleep(50);

        const auto& mapProperties = map.getProperties();
        std::cout << "Map class: " << map.getClass() << std::endl;
        unsigned long long tile_set_size =  map.getTilesets().size();
        std::vector<std::uint32_t> gid_vectors(tile_set_size);
        std::vector<Image> images(tile_set_size);
        int counter = 0;
        for (const auto& tileset : map.getTilesets())
        {

            gid_vectors[counter] = tileset.getFirstGID();

            char* c_string = new char[tileset.getImagePath().size() + 1];

            std::strcpy(c_string, tileset.getImagePath().c_str());
            if(strcmp(c_string, "") == 0){
                std::cout << tileset.getName() << std::endl;
                auto llol  = tileset.getTiles();
                for(const auto& t : llol){
                    std::cout << "ID - " << t.ID << std::endl;
                }
            }
            else {
                std::cout << tileset.getName() << std::endl;
                Image new_image = load_image(c_string);
                images[counter] = new_image;
            }


            counter++;
        }

        std::cout << "Map has " << mapProperties.size() << " properties" << std::endl;
        for (const auto& prop : mapProperties)
        {
            std::cout << "Found property: " << prop.getName() << std::endl;
            std::cout << "Type: " << int(prop.getType()) << std::endl;
        }

        std::cout << std::endl;

        const auto& layers = map.getLayers();

        std::cout << "Map has " << layers.size() << " layers" <<  std::endl;
        std::vector<tmx::TileLayer> tileLayers;


        for (const auto& layer : layers)
        {

            if (layer->getType() == tmx::Layer::Type::Tile) {
                tileLayers.push_back(layer->getLayerAs<tmx::TileLayer>());
            }

        }

        int num_of_tiles = tileLayers[0].getTiles().size();
        int num_of_tile_layers = tileLayers.size();
        for(int i = 0; i < num_of_tiles; i++) {
            for(int j = num_of_tile_layers-1; j >= 0; j--){
                auto tile_data = tileLayers[j].getTiles();
                tile_data[i].

            }

        }



    }
    else
    {
        std::cout << "Failed loading map" << std::endl;
    }

#if defined(PAUSE_AT_END)
    std::cout << std::endl << "Press return to quit..." <<std::endl;
    std::cin.get();
#endif

    return 0;
}


int find_nearest_gid(std::vector<uint32_t> &gid_vectors, uint32_t global_tid) {
    int low = 0;
    int high = (int)gid_vectors.size() - 1;
    int result = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if(gid_vectors[mid] == global_tid){
            return mid;
        }
        if (gid_vectors[mid] < global_tid) {
            result = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return result;
}