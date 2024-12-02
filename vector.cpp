#include "vector.h"
#ifdef __cplusplus
extern "C"{
#endif
void vector_resize(Vector* vec) {

    /**< Attempt to reallocate memory to hold more elements */
    void* temp = realloc(vec->data, vec->byte_size * vec->max_capacity * 2);

    if (temp == nullptr) {
        fprintf(stderr, "Failed to reallocate memory\n");
        return;
    }

    /**< If it hasn't died by now, then we set our new vector, and adjust capacity. */
    vec->data = temp;
    vec->max_capacity *= 2;
}

void vector_push_back_int(Vector* vec, void* object) {
    if (vec->current_capacity == vec->max_capacity) {
        vector_resize(vec);
    }
    int* target = (int*)((char*)vec->data + vec->current_capacity * vec->byte_size);
    *target = *(int*)object;  // Assuming 'object' is a pointer to int
    vec->current_capacity++;
}

// Function to push back a new element
void vector_push_back(Vector* vec, void* object) {
    // Check if resizing is needed
    if (vec->current_capacity == vec->max_capacity) {
        vector_resize(vec);
    }

    // Copy the object into the array
    void* target = (char*)vec->data + vec->current_capacity * vec->byte_size;
    memcpy(target, object, vec->byte_size);  // Copy the object data into the vector

    // Increment the size
    vec->current_capacity++;
}

void vector_pop(Vector* vec) {
    if (vec->current_capacity > 0) {
        vec->current_capacity--;  // Decrease the size

        // If the vector stores dynamically allocated objects, clean up the last one
        if (vec->destructor != NULL) {
            void* last_element = (char*)vec->data + (vec->current_capacity * vec->byte_size);
            vec->destructor(last_element);  // Call the custom destructor for cleanup
        }
    } else {
        printf("Vector is already empty.\n");
    }
}

// Function to initialize the vector, passing function pointers as arguments
void vector_init(Vector* vec, size_t byte_size, size_t max_capacity,
                 void (*push_back_func)(struct Vector*, void*),
                 void (*pop_func)(struct Vector*)) {
    vec->data = malloc(byte_size * max_capacity);
    vec->current_capacity = 0;
    vec->max_capacity = max_capacity;
    vec->byte_size = byte_size;

    if(push_back_func == NULL){
        vec->push_back = vector_push_back;
    }
    else {
        vec->push_back = push_back_func;
    }

    if(pop_func == NULL){
        vec->pop = vector_pop;
    }
    else {
        vec->pop = pop_func;
    }
}

// Function to clean up the vector memory
void vector_free(Vector* vec) {
    free(vec->data);
    vec->data = NULL;
    vec->current_capacity = 0;
    vec->max_capacity = 0;
}

#ifdef __cplusplus
}
#endif