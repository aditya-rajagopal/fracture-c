/**
 * @file darrays.h
 * @author Aditya Rajagopal
 * @brief Contains an implementation of a dynamic array data structure.
 * @details The dynamic array is a data structure that can grow and shrink in
 * size as needed. It is a contiguous block of memory that can be accessed using
 * an index. The dynamic array is useful for storing a collection of elements of
 * the same type. The dynamic array is returned as a void pointer to the block
 * of memory that contains the array. Before this block of memory the length of
 * the array, the capacity of the array, and the size of each element in the
 * array are stored continuously. The dynamic array is a generic data structure
 * and can store elements of any type.
 * @version 0.0.1
 * @date 2024-02-17
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "fracture/core/defines.h"
#include "fracture/fracture_core.h"

/**
 * @brief A dynamic array data structure.
 * NOTE: This is currently not used in the engine and is a work in progress.
 * 
 */
typedef struct darray {
    u64 length;       /**< The length of the dynamic array i.e where is the current valid element */
    u64 capacity;   /**< The capacity of the dynamic array. */
    u64 element_size; /**< The size of each element in the dynamic array. */
    void* data;     /**< A pointer to the data in the dynamic array. */
} darray;

typedef enum darray_fields {
    DARRAY_LENGTH,
    DARRAY_CAPACITY,
    DARRAY_ELEMENT_SIZE,
    DARRAY_FIELDS_LENGTH
} darray_fields;

#define DARRAY_DEFAULT_CAPACITY 8
#define DARRAY_GROWTH_FACTOR 2

/**
 * @brief Creates a new dynamic array with the given size and element size.
 * This is a private function and should not be called directly.
 *
 * @param size The size of the dynamic array.
 * @param element_size The size of each element in the dynamic array.
 * @return void* A pointer to the newly created dynamic array.
 */
FR_API void* _darray_create(u64 size, u64 element_size);

/**
 * @brief Creates a new dynamic array with the default capacity and the given element size.
 *
 * @param type The type of the elements in the dynamic array.
 * @return void* A pointer to the newly created dynamic array.
 */
#define darray_create(type)                                                    \
    _darray_create(DARRAY_DEFAULT_CAPACITY, sizeof(type))

/**
 * @brief Created a new dynamic array with the given size and the given type.
 *
 * @param size The size of the dynamic array.
 * @param type The type of the elements in the dynamic array.
 * @return void* A pointer to the newly created dynamic array.
 */
#define darray_reserve(size, type) _darray_create(size, sizeof(type))

/**
 * @brief Destroys the given dynamic array and frees the memory.
 *
 * @param darray A pointer to the dynamic array to destroy.
 */
FR_API void darray_destroy(void* darray);

/**
 * @brief Pops the last element from the dynamic array and stores it in the
 * given destination.
 *
 * @param darray A pointer to the dynamic array to pop from.
 * @param dest A pointer to the destination to store the popped element.
 * @return void* returns the dynamic array
 */
FR_API void* darray_pop(void* darray, void* dest);

/**
 * @brief Pops the element at the given index from the dynamic array and stores it in the
 * given destination.
 *
 * @param darray A pointer to the dynamic array to pop from.
 * @param dest A pointer to the destination to store the popped element.
 * @param index The index of the element to pop.
 * @return void* returns the dynamic array
 */
FR_API void* darray_pop_at(void* darray, void* dest, u64 index);

/**
 * @brief Pushes the given element to the end of the dynamic array.
 * This is a private function and should not be called directly.
 *
 * @param darray A pointer to the dynamic array to push to.
 * @param element A pointer to the element to push.
 * @return void* A pointer to the dynamic array with the pushed element.
 */
FR_API void* _darray_push(void* darray, void* element);

/**
 * @brief Inserts the given element at the given index in the dynamic array.
 * This is a private function and should not be called directly.
 *
 * @param darray A pointer to the dynamic array to insert into.
 * @param element A pointer to the element to insert.
 * @param index The index to insert the element at.
 * @return void* A pointer to the dynamic array with the inserted element.
 */
FR_API void* _darray_insert_at(void* darray, void* element, u64 index);

/**
 * @brief Pushes the given element to the end of the dynamic array and returns
 * the dynamic array.
 *
 * @param darray A pointer to the dynamic array to push to.
 * @param element The element to push.
 * @return void* A pointer to the dynamic array with the pushed element.
 */
#define darray_push(darray, element)                                           \
    {                                                                          \
      typeof(element) _element = element;                                      \
      darray = _darray_push(darray, &_element);                                         \
    }

/**
 * @brief Inserts the given element at the given index in the dynamic array and
 * returns the dynamic array.
 *
 * @param darray A pointer to the dynamic array to insert into.
 * @param element The element to insert.
 * @param index The index to insert the element at.
 * @return void* A pointer to the dynamic array with the inserted element.
 */
#define darray_insert_at(darray, element, index)                               \
    {                                                                          \
      typeof(element) _element = element;                                      \
      darray = _darray_insert_at(darray, &_element, index);                             \
    }

/**
 * @brief Clears the given dynamic array.
 *
 * @param darray A pointer to the dynamic array to clear.
 * @return void* A pointer to the cleared dynamic array.
 */
FR_API void* darray_clear(void* darray);

/**
 * @brief Gets the length of the given dynamic array.
 *
 * @param darray A pointer to the dynamic array to get the length of.
 * @return u64 The length of the dynamic array.
 */
FR_API u64 darray_length(void* darray);

/**
 * @brief Gets the capacity of the given dynamic array.
 *
 * @param darray A pointer to the dynamic array to get the capacity of.
 * @return u64 The capacity of the dynamic array.
 */
FR_API u64 darray_capacity(void* darray);

/**
 * @brief Gets the element size of the given dynamic array.
 *
 * @param darray A pointer to the dynamic array to get the element size of.
 * @return u64 The element size of the dynamic array.
 */
FR_API u64 darray_element_size(void* darray);

/**
 * @brief Sets the length of the given dynamic array. If the new length is
 * greater than the current length, the new elements are initialized to zero and
 * the array is resized if necessary.
 *
 * @param darray A pointer to the dynamic array to set the length of.
 * @param length The new length of the dynamic array.
 */
FR_API void darray_length_set(void* darray, u64 length);

/**
 * @brief Copies the contents of the given dynamic array to a new dynamic array
 * with the same capacity and element size.
 *
 * @param darray A pointer to the dynamic array to copy.
 * @return void* A pointer to the copied dynamic array.
 */
FR_API void* darray_copy(void* darray);

/**
 * @brief Resizes the given dynamic array to the new capacity.
 * This is a private function and should not be called directly.
 *
 * @param darray A pointer to the dynamic array to resize.
 * @param new_capacity The new capacity of the dynamic array.
 * @return void* A pointer to the resized dynamic array.
 */
FR_API void* darray_resize(void* darray, u64 new_capacity);

/**
 * @brief Grows the given dynamic array by the growth factor.
 * This is a private function and should not be called directly.
 *
 * @param darray A pointer to the dynamic array to grow.
 * @return void* A pointer to the grown dynamic array.
 */
#define darray_grow(darray)                                                    \
    darray_resize(darray, DARRAY_GROWTH_FACTOR * darray_capacity(darray));
