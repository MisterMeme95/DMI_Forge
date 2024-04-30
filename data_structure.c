#include "data_structure.h"

unsigned int hash_pixel(Pixel_Data pixel) {
    //printf("Hashing. . .#1\n");
    const unsigned int prime = 31;
   // printf("Hashing. . .#2\n");
    unsigned int hash = 1;
   // printf("Hashing. . .#3\n");
    hash = prime * hash + Get_Red_Channel(pixel);
  //  printf("Hashing. . .#4\n");
    hash = prime * hash + Get_Green_Channel(pixel);
 //  printf("Hashing. . .#5\n");
    hash = prime * hash + Get_Blue_Channel(pixel);
   // printf("Hashing. . .#6\n");
    hash = prime * hash + Get_Alpha_Channel(pixel);
   // printf("Hashing. . .#7\n");
 //  printf("RGBA = (%d, %d, %d, %d)\n", Get_Red_Channel(pixel), Get_Green_Channel(pixel), Get_Blue_Channel(pixel),
      //    Get_Alpha_Channel(pixel));
   //printf("Hash = %d\n", hash);
  // printf("Hash %% 256 = %d\n", hash % 256);
    return hash;
}
#include <unistd.h>
#include <stdio.h> // for printf

int find_key(Pixel_Data pixel, palette_hash *paletteHash) {
    int hash_index = hash_pixel(pixel) % 256;  // Assuming hash_pixel function correctly calculates a hash based on Pixel_Data
    //printf("Find Hash_Index = %d\n", hash_index); // Debugging output to confirm hash index calculation

    node *key_to_find = paletteHash->hash_bucket[hash_index];
    int iteration_count = 0; // Counter to track how many nodes we inspect

    while (key_to_find != NULL) {
      //  printf("While loop #%d - Checking node at address %p\n", iteration_count, (void*)key_to_find);


      //  printf("Died here. .34 \n");
        Pixel_Data other_pixel = key_to_find->pixel_info;
        //printf("Died here. . \n");
        if (match_node(pixel, other_pixel)) { // Assuming match_node correctly compares two Pixel_Data objects
//            printf("Match found for pixel at index %d\n", key_to_find->index);
//            printf("Original Pixel = (%d, %d, %d)\n", Get_Red_Channel(pixel),  Get_Green_Channel(pixel),
//                   Get_Blue_Channel(pixel));
//            printf("Other Original Pixel = (%d, %d, %d)\n", Get_Red_Channel(other_pixel),  Get_Green_Channel(other_pixel),
//                   Get_Blue_Channel(other_pixel));
            return key_to_find->index; // Return the index of the matching node
        }
        //printf("Died here. .#2\n");
        key_to_find = key_to_find->next; // Move to next node in the list
        iteration_count++;
    }

   // printf("No matching key found. Exiting function.\n");
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

//    if(pixel.red != other_pixel.red){
//        return 0;
//    }
//    if(pixel.green != other_pixel.green){
//        return 0;
//    }
//
//    if(pixel.blue != other_pixel.blue){
//        return 0;
//    }
//
//    if(pixel.alpha_channel != other_pixel.alpha_channel){
//        return 0;
//    }

    return 1;
}

int remove_hash(Pixel_Data pixel){

    return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

int insert_key(Pixel_Data pixel, palette_hash *paletteHash, png_byte new_index) {

    int hash_index = hash_pixel(pixel) % 256;  // Ensure hash_pixel is defined correctly
//    printf("Hash Vale for Insert = %d\n", hash_index);
    // Check if the bucket at the hash index is empty (NULL)
    if (paletteHash->hash_bucket[hash_index] == NULL) {
//        printf("Empty!\n");
        // Allocate memory for a new node if the bucket is empty
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL) {
            fprintf(stderr, "Failed to allocate memory for new node\n");
            return -1;  // return error code if malloc fails
        }

        // Initialize the new node
        new_node->pixel_info = pixel;
        new_node->index = new_index;
        new_node->next = NULL;
        new_node->prev = NULL;

        // Link the new node to the hash bucket
        paletteHash->hash_bucket[hash_index] = new_node;
     //   printf("Inserted!\n\n\n\n");
    } else {
        // The bucket is not empty. Insert the new node at the beginning of the list
        node *new_node = malloc(sizeof(node));
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

    return 0;  // Successful insertion
}
