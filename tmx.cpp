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

int main()
{
    tmx::Map map;

    if (map.load("Ancient Ruins example map.tmx"))
    {

        /**
         *
         *
         */
        tmx::Rectangle<float> dimensions;
        auto stuff = map.getBounds();
        if (map.isInfinite())
        {
            std::cout << "Map is infinite.\n";
        }
        else
        {
            dimensions = map.getBounds();
            //std::cout << "Map Dimensions: " << map.getBounds() << std::endl;
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
        for (const auto& layer : layers)
        {
            auto proper = layer->getProperties();
            std::cout << "Found Layer: " << layer->getName() << std::endl;
            std::cout << "Layer Type: " << LayerStrings[static_cast<std::int32_t>(layer->getType())] << std::endl
            << std::endl;
            std::cout << "Layer Dimensions: " << layer->getSize() << std::endl;

          //  layer.
            if (layer->getType() == tmx::Layer::Type::Group)
            {
                std::cout << "Checking sublayers" << std::endl;
                const auto& sublayers = layer->getLayerAs<tmx::LayerGroup>().getLayers();
                std::cout << "LayerGroup has " << sublayers.size() << " layers" << std::endl;
                for (const auto& sublayer : sublayers)
                {
                    std::cout << "Found Layer: " << sublayer->getName() << std::endl;
                    std::cout << "Sub-layer Type: " << LayerStrings[static_cast<std::int32_t>(sublayer->getType())] << std::endl;
                    std::cout << "Sub-layer Dimensions: " << sublayer->getSize() << std::endl;

                    if (sublayer->getType() == tmx::Layer::Type::Object)
                    {
                        std::cout << sublayer->getName() << " has " << sublayer->getLayerAs<tmx::ObjectGroup>().getObjects().size() << " objects" << std::endl;
                    }
                    else if (sublayer->getType() == tmx::Layer::Type::Tile)
                    {
                        std::cout << sublayer->getName() << " has " << sublayer->getLayerAs<tmx::TileLayer>().getTiles().size() << " tiles" << std::endl;
                    }
                }
            }

            if(layer->getType() == tmx::Layer::Type::Object)
            {
                const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
                std::cout << "Found " << objects.size() << " objects in layer" << std::endl;
                for(const auto& object : objects)
                {
                    std::cout << "Object " << object.getUID() << ", " << object.getName() <<  std::endl;
                    const auto& properties = object.getProperties();
                    std::cout << "Object has " << properties.size() << " properties" << std::endl;
                    for(const auto& prop : properties)
                    {
                        std::cout << "Found property: " << prop.getName() << std::endl;
                        std::cout << "Type: " << int(prop.getType()) << std::endl;
                    }

                    if (!object.getTilesetName().empty())
                    {
                        std::cout << "Object uses template tile set " << object.getTilesetName() << "\n";
                    }
                }
            }

            if (layer->getType() == tmx::Layer::Type::Tile)
            {
                //layer->getLayerAs<tmx::TileLayer>().getTiles();
                const auto& tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
                if (tiles.empty())
                {
                    const auto& chunks = layer->getLayerAs<tmx::TileLayer>().getChunks();
                    if (chunks.empty())
                    {
                        std::cout << "Layer has missing tile data\n";
                    }
                    else
                    {
                        std::cout << "Layer has " << chunks.size() << " tile chunks.\n";
                    }
                }
                else
                {
                    std::cout << "Layer has " << tiles.size() << " tiles.\n";
                    auto dater = tiles.data();
                                    }
            }

            const auto& properties = layer->getProperties();
            std::cout << properties.size() << " Layer Properties:" << std::endl;
            for (const auto& prop : properties)
            {
                std::cout << "Found property: " << prop.getName() << std::endl;
                std::cout << "Type: " << int(prop.getType()) << std::endl;
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
