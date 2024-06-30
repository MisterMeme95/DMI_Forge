#include "data_structure.h"
#include <stdio.h>
#include <stdlib.h>

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
    int hash_index = hash_pixel(pixel) % 256;

    node *key_to_find = paletteHash->hash_bucket[hash_index];

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

int remove_hash(Pixel_Data pixel){

    return 0;
}


int insert_key(Pixel_Data pixel, palette_hash *paletteHash, png_byte new_index) {

    int hash_index = hash_pixel(pixel) % 256;  // Ensure hash_pixel is defined correctly

    if (paletteHash->hash_bucket[hash_index] == NULL) {

        node *new_node = (node*)malloc(sizeof(node));
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
        node *new_node = (node*) malloc(sizeof(node));
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
