/**
 * @file fracture_string.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-02-20
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/fracture_core.h"

#ifdef _STRING_SAFETY_CHECKS
    #define FR_STRING_SAFETY_CHECKS 1
#else
    #define FR_STRING_SAFETY_CHECKS 0
#endif

/**
 * @brief Duplicates a string
 * @details Duplicates a string and returns a pointer to the new string. The new
 * string is heap allocated and must be freed using fr_memory_free when no
 * longer needed with type MEMORY_TYPE_STRING and the length of the string to
 * free is the length of the string + 1.
 *
 * @param string String to duplicate
 * @return char* Pointer to the heap allocated new string
 */
FR_API char* fr_string_duplicate(const char* string);

/**
 * @brief Returns the length of the string
 * @details Returns the length of the string, not including the null terminator
 *
 * @param string String to get the length of
 * @return u64 Length of the string
 */
FR_API u64 fr_string_length(const char* string);

/**
 * @brief Compares two strings and returns true if they are equal
 * @details Case sensitive comparison of two strings. Returns true if the
 * strings are equal, false otherwise
 *
 * @param string1 First string to compare
 * @param string2 Second string to compare
 * @return b8 True if the strings are equal, false otherwise
 */
FR_API b8 fr_string_compare(const char* string1, const char* string2);