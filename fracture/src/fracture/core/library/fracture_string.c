#include "fracture_string.h"

#include <string.h>

#include "fracture/core/systems/fracture_memory.h"
#include "fracture/core/systems/logging.h"

char* fr_string_duplicate(const char* string) {
#if FR_STRING_SAFETY_CHECKS == 1
    if (string == NULL_PTR) {
        FR_CORE_ERROR("Cannot duplicate a NULL string");
        return NULL_PTR;
    }
#endif

    u64 length = strlen(string);
    char* result = (char*)fr_memory_allocate(length + 1, MEMORY_TYPE_STRING);

    memcpy(result, string, length);
    result[length] = '\0';

    return result;
}

u64 fr_string_length(const char* string) {
#if FR_STRING_SAFETY_CHECKS == 1
    if (string == NULL_PTR) {
        FR_CORE_ERROR("Cannot get the length of a NULL string");
        return 0;
    }
#endif

    return strlen(string);
}

b8 fr_string_compare(const char* string1, const char* string2) {
#if FR_STRING_SAFETY_CHECKS == 1
    if (string1 == NULL_PTR || string2 == NULL_PTR) {
        FR_CORE_ERROR("Cannot compare NULL strings");
        return FALSE;
    }
#endif

    return strcmp(string1, string2);
}
