#include "darrays.h"

#include "fracture/core/systems/fracture_memory.h"
#include "fracture/core/systems/logging.h"

void* _darray_create(u64 capacity, u64 element_size) {
    u64 memory_requirement = element_size * capacity;
    memory_requirement += sizeof(u64) * DARRAY_FIELDS_LENGTH;
    u64* darray = fr_memory_allocate(memory_requirement, MEMORY_TYPE_DARRAY);
    fr_memory_zero(darray, memory_requirement);
    darray[DARRAY_CAPACITY] = capacity;
    darray[DARRAY_LENGTH] = 0;
    darray[DARRAY_ELEMENT_SIZE] = element_size;
    return (void*)(darray + DARRAY_FIELDS_LENGTH);
}

void darray_destroy(void* darray) {
    if (darray == NULL_PTR) {
        return;
    }
    u64* darray_header = (u64*)darray - DARRAY_FIELDS_LENGTH;
    fr_memory_free(
        darray_header,
        darray_header[DARRAY_CAPACITY] * darray_header[DARRAY_ELEMENT_SIZE] + sizeof(u64) * DARRAY_FIELDS_LENGTH,
        MEMORY_TYPE_DARRAY);
}

void* darray_resize(void* darray, u64 new_capacity) {
    u64* darray_header = (u64*)darray - DARRAY_FIELDS_LENGTH;
    u64 length = darray_header[DARRAY_LENGTH];
    u64 stride = darray_header[DARRAY_ELEMENT_SIZE];
    void* new_darray = _darray_create(new_capacity, stride);
    if (length > new_capacity) {
        fr_memory_copy(new_darray, darray, new_capacity * stride);
        darray_length_set(new_darray, new_capacity);
    } else {
        fr_memory_copy(new_darray, darray, length * stride);
        darray_length_set(new_darray, length);
    }
    darray_destroy(darray);
    return new_darray;
}

void* darray_pop(void* darray, void* dest) {
    u64 length = darray_length(darray);
    return darray_pop_at(darray, dest, length - 1);
}

void* darray_pop_at(void* darray, void* dest, u64 index) {
    u64 length = darray_length(darray);
    u64 stride = darray_element_size(darray);
    if (index >= length) {
        FR_CORE_ERROR("Attempting to access index: %d of darray with length: %d", index, length);
        return NULL_PTR;
    }
    u64 address = (u64)darray;
    address += index * stride;
    fr_memory_copy(dest, (void*)address, stride);
    if (index != length - 1) {
        fr_memory_copy((void*)address, (void*)(address + stride), (length - index) * stride);
    } else {
        fr_memory_zero((void*)address, stride);
    }
    darray_length_set(darray, length - 1);
    return darray;
}

void* _darray_push(void* darray, void* element) {
    u64 length = darray_length(darray);
    return _darray_insert_at(darray, element, length);
}

void* _darray_insert_at(void* darray, void* element, u64 index) {
    u64* darray_header = (u64*)darray - DARRAY_FIELDS_LENGTH;
    u64 length = darray_header[DARRAY_LENGTH];
    u64 stride = darray_header[DARRAY_ELEMENT_SIZE];
    u64 capacity = darray_header[DARRAY_CAPACITY];

    if (index > length) {
        FR_CORE_ERROR(
            "Attempting to insert array into darray at index: %d "
            "into array of length: %d",
            index,
            length);
        FR_CORE_ERROR(
            "Can only insert into indices within the array or append "
            "at the end with %d",
            length);
        return darray;
    }
    if (length >= capacity) {
        darray = darray_resize(darray, DARRAY_GROWTH_FACTOR * capacity);
    }
    u64 address = (u64)darray;
    if (index != length) {
        fr_memory_copy(
            (void*)(address + (index + 1) * stride), (void*)(address + index * stride), (length - index) * stride);
    }
    fr_memory_copy((void*)(address + index * stride), element, stride);
    darray_length_set(darray, length + 1);
    return darray;
}

void* darray_clear(void* darray) {
    u64 length = darray_length(darray);
    u64 stride = darray_element_size(darray);
    fr_memory_zero(darray, length * stride);
    darray_length_set(darray, 0);
    return darray;
}

u64 darray_length(void* darray) { return ((u64*)darray - DARRAY_FIELDS_LENGTH)[DARRAY_LENGTH]; }

void darray_length_set(void* darray, u64 length) {
    u64* darray_header = (u64*)darray - DARRAY_FIELDS_LENGTH;
    darray_header[DARRAY_LENGTH] = length;
}

u64 darray_capacity(void* darray) { return ((u64*)darray - DARRAY_FIELDS_LENGTH)[DARRAY_CAPACITY]; }

u64 darray_element_size(void* darray) { return ((u64*)darray - DARRAY_FIELDS_LENGTH)[DARRAY_ELEMENT_SIZE]; }

void* darray_copy(void* darray) {
    u64* darray_header = (u64*)darray - DARRAY_FIELDS_LENGTH;
    u64 length = darray_header[DARRAY_LENGTH];
    u64 stride = darray_header[DARRAY_ELEMENT_SIZE];
    u64 capacity = darray_header[DARRAY_CAPACITY];
    void* new_darray = _darray_create(capacity, stride);
    fr_memory_copy(new_darray, darray, length * stride);
    darray_length_set(new_darray, length);
    return new_darray;
}
