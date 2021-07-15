#define INITIAL_SIZE 1

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void exit_if(bool condition, char* file_name, const char* function_name, int line_number) {
    if (condition) {
        printf("Exception at %s(%s:%d)\n", function_name, file_name, line_number);
        exit(EXIT_FAILURE);
    }
}

int min(int a, int b) {
    return a < b ? a : b;
}

struct Vector {
    // Internal array to hold all elements
    /**
     * Double pointer of generic type - A pointer to an array of void pointers
     * This has to be a double pointer because this needs to be an array holding pointers to some generic objects
     * _array holds the memory address to the array of void pointers
     * _array[i] holds the memory address to the i-th element of the array
     * *_array[i] will access the object being referenced by the pointer in the array
     */
    void** _array;
    // Number of elements currently held in the container 
    int _size;
    // Number of elements that can be held in the container
    int _max_size;
};

struct Vector* create(const int max_size) {
    // Max size check
    exit_if(max_size <= 0, __FILE__, __func__, __LINE__);
    // Create a vector object by allocating size of one vector struct in memory
    struct Vector* vector = malloc(sizeof(struct Vector));
    exit_if(vector == NULL, __FILE__, __func__, __LINE__);
    // Allocate max size for internal array
    vector->_array = malloc(sizeof(void**) * max_size);
    exit_if(vector->_array == NULL, __FILE__, __func__, __LINE__);
    // Initialize all to NULL
    bzero(vector->_array, sizeof(void**) * max_size);
    // Set max size and size
    vector->_max_size = max_size;
    vector->_size = 0;
    return vector;
}

struct Vector* create_with_default_size() {
    return create(INITIAL_SIZE);
}

void resize(struct Vector* vector, int max_size) {
    // Save states for old vector
    int old_size = vector->_size;
    int old_max_size = vector->_max_size;
    void* old_array = vector->_array;
    // Create new vector
    vector->_array = malloc(sizeof(void**) * max_size);
    // Determine how many elements should be copied from the old array to the new one
    vector->_size = min(max_size, old_size);
    // Copy the old vector over
    vector->_array = memcpy(vector->_array, old_array, sizeof(void**) * vector->_size);
    // Set the right max size
    vector->_max_size = max_size;
    // Free old vector
    free(old_array);
}

void resize_2x(struct Vector* vector) {
    resize(vector, vector->_max_size * 2);
}

const bool is_half_full(const struct Vector vector) {
    return vector._size > (vector._max_size - 1) / 2;
}

void push_back(struct Vector* vector, void* element) {
    // Check for null
    exit_if(vector == NULL, __FILE__, __func__, __LINE__);
    exit_if(element == NULL, __FILE__, __func__, __LINE__);
    if (is_half_full(*vector)) {
        // Resize if half of the array is full
        resize_2x(vector);
    }

    vector->_array[vector->_size++] = element;
}

void* get(const struct Vector vector, const int index) {
    // Bound check
    exit_if(index < 0, __FILE__, __func__, __LINE__);
    exit_if(index > vector._max_size - 1, __FILE__, __func__, __LINE__);
    return vector._array[index];
}

const int size(const struct Vector vector) {
    return vector._size;
}

const int max_size(const struct Vector vector) {
    return vector._max_size;
}

void print(const struct Vector vector) {
    printf("\n====================\n");
    printf("Printing vector...\n");
    printf("Size: %d\n", vector._size);
    printf("Max size: %d\n", vector._max_size);
    for (int i = 0; i < vector._max_size; i++) {
        printf("Index %d holds item at address %p\n", i, vector._array[i]);
    }
    printf("====================\n\n");
}

// void* front(const struct Vector* vector) {

// }

// void* back(const struct Vector* vector) {

// }

// bool empty(const struct Vector* vector) {

// }

void set(struct Vector* vector, int index, void* element) {
    // TODO: Bound check
    vector->_array[index] = element;
}

// insert
// erase
// clear
// pop_back


int main() {
    struct Vector* vector = create_with_default_size();
    print(*vector);

    int a = 1;
    printf("Pushing a at address %p\n", &a);
    push_back(vector, (void*) &a);
    print(*vector);

    printf("Pushing a at address: %p\n", &a);
    a = 2;
    push_back(vector, (void*) &a);
    print(*vector);
    void* b = get(*vector, 0);
    printf("B is: %d\n", *((int*) b));
    b = get(*vector, 1);
    printf("B is: %d\n", *((int*) b));

    int c = 3;
    set(vector, 0, &c);
    print(*vector);

    return 0;
}