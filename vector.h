//
// Created by jonat on 9/30/2024.
//

#ifndef DMI_STRUCT_C_VECTOR_H
#define DMI_STRUCT_C_VECTOR_H

#endif //DMI_STRUCT_C_VECTOR_H
#ifdef __cplusplus

extern "C" {
#endif
#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * A vector is basically a structure that manages dynamically allocated arrays.
 * @param data  This represents an array of any generic data type. It should grow, shrink as needed.
 * @param current_capacity  The current number of elements that has been pushed into a vector's array.
 * @param max_capacity  This is the maximum size before an array is needed to resize.
 * @param byte_size  The size of single index for data.
 *
 * @param push_back  A function pointer that dictates pushing the data inward.
 * @param pop  A function that pops a particular index and returns the value.
 * */
typedef struct Vector {
    void* data;
    size_t current_capacity;
    size_t max_capacity;
    size_t byte_size;

    void (*push_back)(struct Vector*, void*);
    void (*pop)(struct Vector*);
    void (*destructor)(void*);
} Vector;

/** A function to resize a vector. Basically, whenever size == capacity, then it resizes the vector to be twice as long.
 * */
void vector_resize(Vector* vec);

/** Pushback operation for integers. */
void vector_push_back_int(Vector* vec, void* object);

// Function to push back a new element
void vector_push_back(Vector* vec, void* object);

// Function to pop an element (remove the last element)
void vector_pop(Vector* vec);


/** Initialize a vector with the proper functions parameters.
 * @param vec The vector we are initializing.
 * @param byte_size Size of a single object in bytes.
 * @param max_capacity The starting capacity.
 * @param push_back_func The pushback function point.
 * @param pop_func The pop function.
 * */
void vector_init(Vector* vec, size_t byte_size, size_t max_capacity,
                 void (*push_back_func)(struct Vector*, void*),
                 void (*pop_func)(struct Vector*));

void vector_free(Vector* vec);
#ifdef __cplusplus
}
#endif