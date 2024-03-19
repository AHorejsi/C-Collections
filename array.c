#include <string.h>
#include "array.h"

static void copy_elements(array_t* this, const array_t* other) {
    size_t itemSize = other->meta->itemSize;
    char* thisPtr = (char*)(this->data);
    char* otherPtr = (char*)(other->data);
    char* otherEnd = otherPtr + (other->length * itemSize);
    copier_t copier = other->meta->copy;

    while (otherPtr < otherEnd) {
        copier(thisPtr, otherPtr);

        thisPtr += itemSize;
        otherPtr += itemSize;
    }
}

void array_copy(array_t* this, const array_t* other) {
    array_destroy(this);

    this->data = other->meta->allocate(other->length * other->meta->itemSize);
    this->length = other->length;
    this->meta = other->meta;

    copy_elements(this, other);
}

static void invalidate(array_t* this) {
    this->data = NULL;
    this->meta = NULL;
    this->length = -1;
}

void array_move(array_t* this, array_t* other) {
    this->data = other->data;
    this->length = other->length;
    this->meta = other->meta;

    invalidate(other);
}

void array_destroy(array_t* this) {
    destroyer_t destroyer = this->meta->destroy;

    if (NULL != destroyer) {
        size_t itemSize = this->meta->itemSize;
        char* ptr = (char*)(this->data);
        char* end = ptr + (this->length * itemSize);

        while (ptr < end) {
            destroyer(ptr);

            ptr += itemSize;
        }
    }

    this->meta->deallocate(this->data);
    
    invalidate(this);
}

bool array_same_type(const array_t* this, const array_t* other) {
    return this->meta == other->meta || 0 == strcmp(this->meta->typeName, other->meta->typeName);
}

void* array_get(array_t* this, const size_t index) {
    return (void*)array_get_const(this, index);
}

const void* array_get_const(const array_t* this, const size_t index) {
    return (char*)(this->data) + (index * this->meta->itemSize);
}

void array_set_copy(array_t* this, const size_t index, const void* new) {
    void* ptr = array_get(this, index);

    this->meta->copy(ptr, new);
}

void array_set_move(array_t* this, const size_t index, void* new) {
    void* ptr = array_get(this, index);

    this->meta->move(ptr, new);
}

ptrdiff_t array_find(const array_t* this, const size_t fromIndex, const void* item) {
    return array_look(this, fromIndex, item, this->meta->equals);
}

ptrdiff_t array_find_if(const array_t* this, const size_t fromIndex, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data) + (fromIndex * itemSize);
    char* end = (char*)(this->data) + (this->length * itemSize);
    ptrdiff_t index = fromIndex;

    while (ptr < end) {
        if (pred(ptr)) {
            return index;
        }

        ++index;
        ptr += itemSize;
    }

    return -1;
}

ptrdiff_t array_find_last(const array_t* this, const size_t fromIndex, const void* item) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data) + (fromIndex * itemSize);
    char* end = (char*)(this->data);
    ptrdiff_t index = fromIndex;
    binary_predicate_t equality = this->meta->equals;

    while (ptr >= end) {
        if (equality(ptr, item)) {
            return index;
        }

        --index;
        ptr -= itemSize;
    }

    return -1;
}

ptrdiff_t array_find_last_if(const array_t* this, const size_t fromIndex, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data) + (fromIndex * itemSize);
    char* end = (char*)(this->data);
    ptrdiff_t index = fromIndex;
    binary_predicate_t equality = this->meta->equals;

    while (ptr >= end) {
        if (pred(ptr)) {
            return index;
        }

        --index;
        ptr -= itemSize;
    }

    return -1;
}

ptrdiff_t array_look(const array_t* this, const size_t fromIndex, const void* item, const binary_predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data) + (fromIndex * itemSize);
    char* end = (char*)(this->data) + (this->length * itemSize);
    ptrdiff_t index = fromIndex;

    while (ptr < end) {
        if (pred(ptr, item)) {
            return index;
        }

        ++index;
        ptr += itemSize;
    }

    return -1;
}

size_t array_count(const array_t* this, const void* item) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);
    size_t amount = 0;
    binary_predicate_t equality = this->meta->equals;

    while (ptr < end) {
        if (equality(ptr, item)) {
            ++amount;
        }

        ptr += itemSize;
    }

    return amount;
}

size_t array_count_if(const array_t* this, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);
    size_t amount = 0;

    while (ptr < end) {
        if (pred(ptr)) {
            ++amount;
        }

        ptr += itemSize;
    }

    return amount;
}

void array_swap(array_t* this, array_t* other) {
    void* tempData = this->data;
    this->data = other->data;
    other->data = tempData;

    ptrdiff_t tempLength = this->length;
    this->length = other->length;
    other->length = tempLength;

    meta_t* tempMeta = this->meta;
    this->meta = other->meta;
    other->meta = tempMeta;
}

void array_fill(array_t* this, const void* value) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);
    copier_t copier = this->meta->copy;

    while (ptr < end) {
        copier(ptr, value);

        ptr += itemSize;
    }
}

bool array_all(const array_t* this, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);

    while (ptr < end) {
        if (!pred(ptr)) {
            return false;
        }

        ptr += itemSize;
    }

    return true;
}

bool array_any(const array_t* this, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);

    while (ptr < end) {
        if (pred(ptr)) {
            return true;
        }

        ptr += itemSize;
    }

    return false;
}

bool array_none(const array_t* this, const predicate_t pred) {
    return !array_any(this, pred);
}

ptrdiff_t array_binary_search(const array_t* this, const void* item, const comparator_t comp) {
    ptrdiff_t lowIndex = 0;
    ptrdiff_t highIndex = this->length - 1;

    while (lowIndex <= highIndex) {
        ptrdiff_t midIndex = (lowIndex + highIndex) / 2;
        const void* elem = array_get_const(this, midIndex);
        ptrdiff_t comparison = comp(item, elem);

        if (comparison < 0) {
            lowIndex = midIndex + 1;
        }
        else if (comparison > 0) {
            highIndex = midIndex - 1;
        }
        else {
            return midIndex;
        }
    }

    return -1;
}

static ptrdiff_t array_binary_search_bound(const array_t* this, const void* item, const comparator_t comp, bool isLower) {
    ptrdiff_t lowIndex = 0;
    ptrdiff_t highIndex = this->length - 1;

    while (lowIndex <= highIndex) {
        ptrdiff_t midIndex = (lowIndex + highIndex) / 2;
        const void* elem = array_get_const(this, midIndex);

        ptrdiff_t comparison = comp(elem, item);

        if (comp < 0) {
            lowIndex = midIndex + 1;
        }
        else if (comp > 0) {
            highIndex = midIndex - 1;
        }
        else {
            if (isLower) {
                const void* prev = array_get_const(this, midIndex - 1);

                if (midIndex == lowIndex || 0 != comp(prev, item)) {
                    return midIndex;
                }
                else {
                    highIndex = midIndex - 1;
                }
            }
            else {
                const void* next = array_get_const(this, midIndex + 1);

                if (midIndex == highIndex || 0 != comp(next, item)) {
                    return midIndex;
                }
                else {
                    lowIndex = midIndex + 1;
                }
            }
        }
    }
}

ptrdiff_t array_lower_bound(const array_t* this, const void* item, const comparator_t comp) {
    return array_binary_search_bound(this, item, comp, true);
}

ptrdiff_t array_upper_bound(const array_t* this, const void* item, const comparator_t comp) {
    return array_binary_search_bound(this, item, comp, false);
}

static char* median_of_three(char* lowPtr, char* highPtr, const size_t itemSize, const comparator_t comp, void* swapBuffer) {
    size_t count = (highPtr - lowPtr) / itemSize + 1;
    size_t midIndex = count / 2;
    char* midPtr = lowPtr + (midIndex * itemSize);

    if (comp(highPtr, lowPtr) < 0) {
        ptr_swap(highPtr, lowPtr, swapBuffer, itemSize);
    }
    if (comp(midPtr, lowPtr) < 0) {
        ptr_swap(highPtr, lowPtr, swapBuffer, itemSize);
    }
    if (comp(highPtr, midPtr) < 0) {
        ptr_swap(highPtr, midPtr, swapBuffer, itemSize);
    }

    return midPtr;
}

static char* quick_sort_partition(char* lowPtr, char* highPtr, const size_t itemSize, const comparator_t comp, void* swapBuffer) {
    char* pivot = median_of_three(lowPtr, highPtr, itemSize, comp, swapBuffer);

    ptr_swap(pivot, lowPtr, swapBuffer, itemSize);

    lowPtr -= itemSize;
    highPtr += itemSize;

    while (true) {
        do {
            lowPtr += itemSize;
        } while (comp(lowPtr, pivot) < 0);

        do {
            highPtr -= itemSize;
        } while (comp(highPtr, pivot) > 0);

        if (lowPtr >= highPtr) {
            return highPtr;
        }

        ptr_swap(lowPtr, highPtr, swapBuffer, itemSize);
    }
}

static void quick_sort(char* lowPtr, char* highPtr, const size_t itemSize, const comparator_t comp, void* swapBuffer) {
    if (lowPtr < highPtr) {
        char* partitionPoint = quick_sort_partition(lowPtr, highPtr, itemSize, comp, swapBuffer);

        quick_sort(lowPtr, partitionPoint - itemSize, itemSize, comp, swapBuffer);
        quick_sort(partitionPoint + itemSize, highPtr, itemSize, comp, swapBuffer);
    }
}

void array_sort(array_t* this, const comparator_t comp) {
    size_t itemSize = this->meta->itemSize;
    char* lowPtr = (char*)(this->data);
    char* highPtr = lowPtr + (this->length * itemSize);
    void* swapBuffer = this->meta->allocate(itemSize);

    quick_sort(lowPtr, highPtr - itemSize, itemSize, comp, swapBuffer);

    this->meta->deallocate(swapBuffer);
}

static range_t make_subarray(char* lowPtr, char* highPtr, const allocator_t alloc) {
    size_t subSize = highPtr - lowPtr;
    char* subArray = (char*)alloc(subSize);
    
    memcpy(subArray, lowPtr, subSize);

    return (range_t){ subArray, subArray + subSize };
}

static void merge(char* lowPtr, char* midPtr, char* highPtr, const size_t itemSize, const comparator_t comp, const allocator_t alloc, const deallocator_t dealloc) {
    range_t left = make_subarray(lowPtr, midPtr, alloc);
    range_t right = make_subarray(midPtr, highPtr, alloc);

    char* leftPtr = left.low;
    char* rightPtr = right.low;
    char* ptr = lowPtr;

    while (leftPtr != left.high && rightPtr != right.high) {
        if (comp(leftPtr, rightPtr) <= 0) {
            memcpy(ptr, leftPtr, itemSize);

            leftPtr += itemSize;
        }
        else {
            memcpy(ptr, rightPtr, itemSize);

            rightPtr += itemSize;
        }

        ptr += itemSize;
    }

    dealloc(left.low);
    dealloc(right.low);
}

static void merge_sort(char* lowPtr, char* highPtr, const size_t itemSize, const comparator_t comp, const allocator_t alloc, const deallocator_t dealloc) {
    size_t size = highPtr - lowPtr;
    size_t count = size / itemSize;

    if (count > 1) {
        size_t midIndex = size / 2 / itemSize;
        char* midPtr = lowPtr + (midIndex * itemSize);

        merge_sort(lowPtr, midPtr, itemSize, comp, alloc, dealloc);
        merge_sort(midPtr, highPtr, itemSize, comp, alloc, dealloc);

        merge(lowPtr, midPtr, highPtr, itemSize, comp, alloc, dealloc);
    }
}

void array_stable_sort(array_t* this, const comparator_t comp) {
    size_t itemSize = this->meta->itemSize;
    char* lowPtr = (char*)(this->data);
    char* highPtr = lowPtr + (this->length * itemSize);

    merge_sort(lowPtr, highPtr, itemSize, comp, this->meta->allocate, this->meta->deallocate);
}

bool array_sorted(const array_t* this, const comparator_t comp) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + ((this->length - 1) * itemSize);

    while (ptr < end) {
        if (comp(ptr, ptr + itemSize) > 0) {
            return false;
        }

        ptr += itemSize;
    }

    return true;
}

void* array_minimum(array_t* this, const comparator_t comp) {
    return (void*)array_minimum_const(this, comp);
}

const void* array_minimum_const(const array_t* this, const comparator_t comp) {
    size_t itemSize = this->meta->itemSize;
    char* min = (char*)(this->data);
    char* ptr = min + itemSize;
    char* end = min + (this->length * itemSize);
    
    while (ptr < end) {
        if (comp(min, ptr) < 0) {
            min = ptr;
        }

        ptr += itemSize;
    }

    return min;
}

void* array_maximum(array_t* this, const comparator_t comp) {
    return (void*)array_maximum_const(this, comp);
}

const void* array_maximum_const(const array_t* this, const comparator_t comp) {
    size_t itemSize = this->meta->itemSize;
    char* max = (char*)(this->data);
    char* ptr = max + itemSize;
    char* end = max + (this->length * itemSize);

    while (ptr < end) {
        if (comp(max, ptr) > 0) {
            max = ptr;
        }

        ptr += itemSize;
    }

    return max;
}

void array_for_each(array_t* this, const action_t act) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);

    while (ptr < end) {
        act(ptr);

        ptr += itemSize;
    }
}

static size_t find_not(array_t* this, const predicate_t pred) {
    for (size_t index = 0; index < this->length; ++index) {
        const void* elem = array_get_const(this, index);

        if (!pred(elem)) {
            return index;
        }
    }

    return this->length;
}

size_t array_partition(array_t* this, const predicate_t pred) {
    size_t startIndex = find_not(this, pred);

    if (this->length == startIndex) {
        return startIndex;
    }
    else {
        size_t itemSize = this->meta->itemSize;
        void* buffer = this->meta->allocate(itemSize);

        for (size_t index = startIndex + 1; index < this->length; ++index) {
            void* elem1 = array_get(this, index);

            if (pred(elem1)) {
                void* elem2 = array_get(this, startIndex);

                ptr_swap(elem1, elem2, buffer, itemSize);

                ++startIndex;
            }
        }

        this->meta->deallocate(buffer);
    }
}

void array_reverse(array_t* this) {
    size_t itemSize = this->meta->itemSize;
    char* low = (char*)(this->data);
    char* high = low + ((this->length - 1) * itemSize);
    void* buffer = this->meta->allocate(itemSize);

    while (low < high) {
        ptr_swap(low, high, buffer, itemSize);

        low += itemSize;
        high -= itemSize;
    }

    this->meta->deallocate(buffer);
}

void array_shuffle(array_t* this, const randomizer_t random) {
    size_t itemSize = this->meta->itemSize;
    void* buffer = this->meta->allocate(itemSize);

    for (size_t index = 2; index < this->length; ++index) {
        void* ptr = array_get(this, index);
        void* randPtr = array_get(this, random() % index);

        ptr_swap(ptr, randPtr, buffer, itemSize);
    }

    this->meta->deallocate(buffer);
}

void array_replace(array_t* this, const void* old, const void* new) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);
    binary_predicate_t equality = this->meta->equals;
    copier_t copier = this->meta->copy;

    while (ptr < end) {
        if (equality(old, ptr)) {
            copier(ptr, new);
        }

        ptr += itemSize;
    }
}

void array_replace_if(array_t* this, const void* new, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);
    copier_t copier = this->meta->copy;

    while (ptr < end) {
        if (pred(ptr)) {
            copier(ptr, new);
        }

        ptr += itemSize;
    }
}

bool array_equals(const array_t* this, const array_t* other) {
    if (this->length != other->length || this->meta != other->meta) {
        return false;
    }

    size_t itemSize = this->meta->itemSize;
    char* thisPtr = (char*)(this->data);
    char* thisEnd = thisPtr + (this->length * itemSize);
    char* otherPtr = (char*)(other->data);
    binary_predicate_t equality = this->meta->equals;

    while (thisPtr < thisEnd) {
        if (!equality(thisPtr, otherPtr)) {
            return false;
        }

        thisPtr += itemSize;
        otherPtr += itemSize;
    }

    return true;
}

comparison_t array_compare(const array_t* this, const array_t* other, const comparator_t comp) {
    if (this->meta != other->meta) {
        return UNDEFINED;
    }

    size_t itemSize = this->meta->itemSize;
    char* thisPtr = (char*)(this->data);
    char* thisEnd = thisPtr + (this->length * itemSize);
    char* otherPtr = (char*)(this->data);
    char* otherEnd = otherPtr + (this->length * itemSize);

    while (thisPtr != thisEnd && otherPtr != otherEnd) {
        ptrdiff_t compare = comp(thisPtr, otherPtr);

        if (compare < 0) {
            return LESS;
        }
        else if (compare > 0) {
            return GREATER;
        }

        thisPtr += itemSize;
        otherPtr += itemSize;
    }

    return EQUAL;
}
