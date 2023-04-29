#include <string.h>
#include "utils.h"

void ptr_swap(void* left, void* right, void* buffer, const size_t size) {
    memcpy(buffer, left, size);
    memcpy(left, right, size);
    memcpy(right, buffer, size);
}
