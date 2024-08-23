#include <tmxlite/Map.hpp>
#include <tmxlite/LayerGroup.hpp>
#include <tmxlite/TileLayer.hpp>
#include <iostream>
#include <array>
#include <string>
#include <cstring>

#include "data_structure.h"
#include <cstdio>
#include <vector>
//#define PAUSE_AT_END 1
#include <unistd.h>
#include <cmath>
#include <chrono>

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

struct pop_node{
    char *id;
    char pop_components[10000] = {0};
};

void incrementMapId(char *id) {
    size_t length = strlen(id);
    for (size_t i = length - 1; i >= 0; i--) {
        if (id[i] < 'z') {
            id[i]++;
            break;
        } else {
            id[i] = 'a';
        }
    }
}

int calculateMaxCharacters(int size) {
    int base = 26; // Base for 'a' to 'z'
    int length = 1;

    // Calculate the number of characters required
    while (std::pow(base, length) < size) {
        length++;
    }

    return length;
}
char* change_extension(const char *filename) {
    // Find the last occurrence of ".png" in the string
    const char *extension = strrchr(filename, '.');

    // Check if the extension is ".png"
    if (extension && strcmp(extension, ".png") == 0) {
        // Allocate memory for the new string
        size_t new_length = strlen(filename) - 4 + 4 + 1; // length without ".png" + ".dmi" + null terminator
        char *new_filename = (char *)malloc(new_length);

        // Copy the original filename up to the ".png"
        strncpy(new_filename, filename, extension - filename);

        // Add the new extension
        strcpy(new_filename + (extension - filename), ".dmi");

        return new_filename;
    } else {
        // Return a copy of the original filename if it doesn't end with ".png"
        return strdup(filename);
    }
}

char* remove_path(const char *filename) {
    // Find the last occurrence of '/' or '\'
    const char *last_slash = strrchr(filename, '/');
    if (!last_slash) {
        last_slash = strrchr(filename, '\\');
    }

    // Start after the last slash, or at the beginning if no slash was found
    const char *basename = (last_slash) ? last_slash + 1 : filename;

    // Return a copy of the basename
    return strdup(basename);
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

        int map_width = stuff.width / sizer.x;
        int map_height = stuff.height / sizer.y;

        printf("Map Width (In Tiles) - %d\n", map_width);
        printf("Map Height (In Tiles) - %d\n", map_height);

        const auto& mapProperties = map.getProperties();
        std::cout << "Map class: " << map.getClass() << std::endl;
        unsigned long long tile_set_size =  map.getTilesets().size();
        std::vector<std::uint32_t> gid_vectors(tile_set_size);
        std::vector<Image> images(tile_set_size);
        int counter = 0;
        dmi_list dmi_lookup;
        initialize_dmi_vector(&dmi_lookup);
        for (const auto& tileset : map.getTilesets())
        {

            gid_vectors[counter] = tileset.getFirstGID();

            char* c_string = new char[tileset.getImagePath().size() + 1];

            std::strcpy(c_string, tileset.getImagePath().c_str());
            if(strcmp(c_string, "") != 0){
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
        std::vector<pop_node*> vector_of_nodes;
        std::unordered_map<std::string, pop_node*> check_nodes;
        int count = 0;
        size_t num_of_tiles = tileLayers[0].getTiles().size();
        std::vector<char**> map_array;//(60000);
        int map_index = 0;
        size_t num_of_tile_layers = tileLayers.size();
        auto start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < num_of_tiles; i++) {
            auto *map_node = static_cast<pop_node *>(calloc(1, sizeof(pop_node)));
            for(int j = 0; j < num_of_tile_layers; j++){
                const auto &tile_data = tileLayers[j].getTiles();

                auto tile = tile_data[i];
                if(tile.ID != 0){
                    auto tile_index = find_nearest_gid(gid_vectors, tile.ID);
                    auto local_id = tile.ID - gid_vectors[tile_index];
                    auto &tile_sheet = images[tile_index];
                    char* new_string = change_extension(tile_sheet.image_name);


                    if(find_dmi(new_string, &dmi_lookup.hash_table, nullptr) == nullptr){
                        insert_dmi(new_string,  &dmi_lookup.hash_table);
                        dmi_lookup.array[dmi_lookup.currently_filled] = find_dmi(new_string, &dmi_lookup.hash_table, nullptr);
                        dmi_lookup.currently_filled++;
                    }
                    auto* iconState = (icon_state*) malloc(sizeof(icon_state));
                    iconState->number_of_frames = 1;
                    iconState->dirs = 1;
                    int length = snprintf(NULL, 0, "%d", local_id) + 1;
                    iconState->state = (char*)malloc(sizeof(char) *length);
                    snprintf(iconState->state, length, "%d", local_id);



                    png_bytepp new_image_data = get_image_data(&tile_sheet,local_id);
                    auto dmi_file = find_dmi(new_string, &dmi_lookup.hash_table, nullptr);

                    dmi_file->bit_depth = tile_sheet.bit_depth;
                    dmi_file->color_type = tile_sheet.color_type;
                    dmi_file->bytes_per_pixel = 4;

                    add_iconstate(dmi_file, iconState,new_image_data);
                    char temp[1000] = {0};   // Temporary buffer for the formatted string
                    sprintf(temp, "/turf{icon = '%s'; icon_state = \"%s\"}, ", dmi_file->name,
                            iconState->state);
                    strcat(map_node->pop_components, temp);
               }
            }
            strcat(map_node->pop_components, "/area");

            std::string key(map_node->pop_components); // Convert char array to std::string
            if (check_nodes.find(key) == check_nodes.end()) {
                check_nodes[key] = map_node;

                vector_of_nodes.push_back(map_node);
                map_array.push_back(&map_node->id);
//                map_array[map_index] = &map_node->id;
//                map_index++;

            }
            else {
                auto item = check_nodes[key];
                map_array.push_back(&item->id);
//                map_array[map_index] = &item->id;
//                map_index++;
            }

        }

        for(int i = 0; i < 32; i++){
            create_dmi(dmi_lookup.array[i]);
        }

        auto char_num = calculateMaxCharacters(vector_of_nodes.size()) + 1;
        char *start_val = (char*)malloc(sizeof(char) * char_num);
        for(int i = 0; i < char_num - 1; i++){
            start_val[i] = 'a';
        }
        start_val[char_num] = '\0';
//        printf("Number of characters = %d\n", );
        for(int i = 0; i < vector_of_nodes.size(); i++){
            vector_of_nodes[i]->id = (char*)malloc(sizeof(char) * char_num);

            for(int k = 0; k < char_num - 1; k++){
                vector_of_nodes[i]->id[k] = start_val[k];
            }
            incrementMapId(start_val);
        }


        FILE *file = fopen("output.dmm", "w+");
        for(auto & vector_of_node : vector_of_nodes){
            fprintf(file, "\"%s\" = (%s)\n", vector_of_node->id, vector_of_node->pop_components);
        }
        fprintf(file, "(1,1,1) = {\"\n");


        /* This will be where we output the map array.*/
        int array_counter = 0;
        for(int i = 0; i < map_height; i++){
            for(int j = 0; j < map_width; j++){
                fprintf(file, "%s", *(map_array[array_counter]));
                array_counter++;
            }
            fprintf(file, "\n");
        }

        fprintf(file, "\"}");
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        // Print the duration in seconds
        std::cout << "Function took " << duration.count() << " seconds to execute." << std::endl;

    exit(EXIT_SUCCESS);
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