#include <string.h>
#include <stdlib.h>
#include "vec.h"

bool vec_empty(const vec_t* this) {
    return 0 == this->count;
}

static void copy_elements(vec_t* this, const vec_t* other) {
    size_t itemSize = other->meta->itemSize;
    char* thisPtr = (char*)(this->data);
    char* otherPtr = (char*)(other->data);
    char* otherEnd = otherPtr + (other->count * itemSize);
    copier_t copier = other->meta->copy;

    while (otherPtr < otherEnd) {
        copier(thisPtr, otherPtr);

        thisPtr += itemSize;
        otherPtr += itemSize;
    }
}

void vec_copy(vec_t* this, const vec_t* other) {
    this->data = other->meta->allocate(other->capacity * other->meta->itemSize);
    this->count = other->count;
    this->capacity = other->capacity;
    this->meta = other->meta;

    copy_elements(this, other);
}

static void invalidate(vec_t* this) {
    this->data = NULL;
    this->count = -1;
    this->capacity = -1;
    this->meta = NULL;
}

void vec_move(vec_t* this, vec_t* other) {
    this->data = other->data;
    this->count = other->count;
    this->capacity = other->capacity;
    this->meta = other->meta;

    invalidate(other);
}

static void destroy_elements(vec_t* this, const size_t fromIndex, const size_t toIndex) {
    destroyer_t destroyer = this->meta->destroy;

    if (NULL != destroyer) {
        size_t itemSize = this->meta->itemSize;
        char* ptr = (char*)(this->data) + (fromIndex * itemSize);
        char* end = (char*)(this->data) + (toIndex * itemSize);

        while (ptr != end) {
            destroyer(ptr);

            ptr += itemSize;
        }
    }
}

void vec_destroy(vec_t* this) {
    destroy_elements(this, 0, this->count - 1);

    this->meta->deallocate(this->data);

    invalidate(this);
}

bool vec_same_type(const vec_t* this, const vec_t* other) {
    return this->meta == other->meta || 0 == strcmp(this->meta->typeName, other->meta->typeName);
}

void* vec_get(vec_t* this, const size_t index) {
    return (void*)vec_get_const(this, index);
}

const void* vec_get_const(const vec_t* this, const size_t index) {
    return (char*)(this->data) + (index * this->meta->itemSize);
}

void vec_set_copy(vec_t* this, const size_t index, const void* new) {
    void* ptr = vec_get(this, index);

    this->meta->copy(ptr, new);
}

void vec_set_move(vec_t* this, const size_t index, void* new) {
    void* ptr = vec_get(this, index);

    this->meta->move(ptr, new);
}

void vec_push_back_copy(vec_t* this, const void* new) {
    vec_insert_copy(this, this->count, new);
}

void vec_push_back_move(vec_t* this, void* new) {
    vec_insert_move(this, this->count, new);
}

static void resize_if_needed(vec_t* this) {
    if (this->count == this->capacity) {
        size_t newCapacity = this->capacity * 3 / 2;

        this->capacity = newCapacity;
        this->data = realloc(this->data, newCapacity * this->meta->itemSize);
    }
}

static void shift_for_insertion(vec_t* this, const size_t index) {
    size_t itemSize = this->meta->itemSize;
    void* buffer = this->meta->allocate(itemSize);
    char* ptr = (char*)(this->data) + (this->count * itemSize);
    char* end = (char*)(this->data) + (index * itemSize);

    while (ptr != end) {
        ptr_swap(ptr, ptr + itemSize, buffer, itemSize);

        ptr -= itemSize;
    }

    this->meta->destroy(buffer);
}

void vec_insert_copy(vec_t* this, const size_t index, const void* new) {
    resize_if_needed(this);
    shift_for_insertion(this, index);
    
    char* ptr = (char*)(this->data) + (index * this->meta->itemSize);

    this->meta->copy(ptr, new);
    ++(this->count);
}

void vec_insert_move(vec_t* this, const size_t index, void* new) {
    resize_if_needed(this);
    shift_for_insertion(this, index);

    char* ptr = (char*)(this->data) + (index * this->meta->itemSize);

    this->meta->move(ptr, new);
    ++(this->count);
}

void vec_pop_back(vec_t* this) {
    vec_erase(this, this->count - 1);
}

void vec_erase(vec_t* this, const size_t index) {
    vec_erase_range(this, index, index + 1);
}

static void shift_elements_from_erasure(vec_t* this, const size_t fromIndex, const size_t toIndex) {
    size_t itemSize = this->meta->itemSize;
    char* frontPtr = (char*)(this->data) + (itemSize * fromIndex);
    char* backPtr = (char*)(this->data) + (itemSize * toIndex);
    char* end = (char*)(this->data) + (itemSize * this->count);
    mover_t mover = this->meta->move;

    while (backPtr != end) {
        mover(frontPtr, backPtr);

        frontPtr += itemSize;
        backPtr += itemSize;
    }
}

void vec_erase_range(vec_t* this, const size_t fromIndex, const size_t toIndex) {
    if (toIndex <= fromIndex) {
        return;
    }

    destroy_elements(this, fromIndex, toIndex);
    shift_elements_for_erasure(this, fromIndex, toIndex);

    this->count -= toIndex - fromIndex;
}

size_t vec_erase_if(vec_t* this, const predicate_t pred) {
    size_t amountRemoved = 0;
    size_t count = this->count;

    for (size_t index = 0; index < count; ++index) {
        const void* ptr = vec_get_const(this, index);

        if (pred(ptr)) {
            vec_erase(this, index);

            ++amountRemoved;
        }
    }

    return amountRemoved;
}

size_t vec_erase_all_of(vec_t* this, const void* item) {
    return vec_erase_amount_of(this, item, this->count);
}

bool vec_erase_first_of(vec_t* this, const void* item) {
    return vec_erase_amount_of(this, item, 1);
}

size_t vec_erase_amount_of(vec_t* this, const void* item, const size_t targetAmount) {
    size_t amountRemoved = 0;
    size_t count = this->count;
    
    for (size_t index = 0; index < count; ++index) {
        const void* ptr = vec_get_const(this, index);

        if (equality(ptr, item)) {
            vec_erase(this, index);

            ++amountRemoved;

            if (amountRemoved == targetAmount) {
                break;
            }
        }
    }

    return amountRemoved;
}

void vec_clear(vec_t* this) {
    destroy_elements(this, 0, this->count - 1);

    this->capacity = DEFAULT_CAPACITY;
    this->count = 0;
    this->data = this->meta->allocate(DEFAULT_CAPACITY * this->meta->itemSize);
}

void trim(vec_t* this) {
    if (this->count == this->capacity) {
        return;
    }

    char* newData = (char*)(this->meta->allocate(this->count));

    size_t itemSize = this->meta->itemSize;
    char* newPtr = newData;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->count * itemSize);
    copier_t copier = this->meta->copy;
    
    while (ptr != end) {
        copier(newPtr, ptr);

        ptr += itemSize;
        newPtr += itemSize;
    }

    this->data = newData;
    this->count = this->capacity;
}

ptrdiff_t vec_find(const vec_t* this, const size_t fromIndex, const void* item) {
    return vec_look(this, fromIndex, item, this->meta->equals);
}

ptrdiff_t vec_find_if(const vec_t* this, const size_t fromIndex, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data) + (fromIndex * itemSize);
    char* end = (char*)(this->data) + (this->count * itemSize);
    ptrdiff_t index = fromIndex;

    while (ptr != end) {
        if (pred(ptr)) {
            return index;
        }

        ++index;
        ptr += itemSize;
    }

    return -1;
}

ptrdiff_t vec_find_last(const vec_t* this, const size_t fromIndex, const void* item) {
    size_t itemSize = this->meta->itemSize;
    binary_predicate_t equality = this->meta->equals;
    char* ptr = (char*)(this->data) + (fromIndex * itemSize);
    char* end = (char*)(this->data);
    ptrdiff_t index = fromIndex;

    while (ptr >= end) {
        if (equality(ptr, item)) {
            return index;
        }

        --index;
        ptr -= itemSize;
    }

    return -1;
}

ptrdiff_t vec_find_last_if(const vec_t* this, const size_t fromIndex, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data) + (fromIndex * itemSize);
    char* end = (char*)(this->data);
    ptrdiff_t index = fromIndex;

    while (ptr > end) {
        if (pred(ptr)) {
            return index;
        }

        --index;
        ptr -= itemSize;
    }

    return -1;
}

ptrdiff_t vec_look(const vec_t* this, const size_t fromIndex, const void* item, const binary_predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data) + (fromIndex * itemSize);
    char* end = (char*)(this->data) + (this->count * itemSize);
    ptrdiff_t index = fromIndex;

    while (ptr != end) {
        if (pred(ptr, item)) {
            return index;
        }

        ++index;
        ptr += itemSize;
    }

    return -1;
}

size_t vec_count(const vec_t* this, const void* item) {
    size_t itemSize = this->meta->itemSize;
    binary_predicate_t equality = this->meta->equals;
    char* ptr = (char*)(this->data);
    char* end = (char*)(this->data) + (this->count * itemSize);
    size_t amount = 0;

    while (ptr != end) {
        if (equality(ptr, item)) {
            ++amount;
        }

        ptr += itemSize;
    }

    return amount;
}

size_t vec_count_if(const vec_t* this, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = (char*)(this->data) + (this->count * itemSize);
    size_t amount = 0;

    while (ptr != end) {
        if (pred(ptr)) {
            ++amount;
        }

        ptr += itemSize;
    }

    return amount;
}

void vec_swap(vec_t* this, vec_t* other) {
    void* tempData = this->data;
    this->data = other->data;
    other->data = tempData;

    meta_t* tempMeta = this->meta;
    this->meta = other->meta;
    other->meta = tempMeta;

    ptrdiff_t tempCount = this->count;
    this->count = other->count;
    other->count = tempCount;

    ptrdiff_t tempCapacity = this->capacity;
    this->capacity = other->capacity;
    other->capacity = tempCapacity;
}

void vec_fill(vec_t* this, const void* item) {
    size_t itemSize = this->meta->itemSize;
    copier_t copier = this->meta->copy;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->count * itemSize);

    while (ptr != end) {
        copier(ptr, item);

        ptr += itemSize;
    }
}

bool vec_all(const vec_t* this, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->count * itemSize);

    while (ptr != end) {
        if (!pred(ptr)) {
            return false;
        }

        ptr += itemSize;
    }

    return true;
}

bool vec_any(const vec_t* this, const predicate_t pred) {
    return vec_find_if(this, 0, pred);
}

bool vec_none(const vec_t* this, const predicate_t pred) {
    return !vec_any(this, pred);
}

bool vec_sorted(const vec_t* this, const comparator_t comp) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = (char*)(this->data) + ((this->count - 1) * itemSize);

    while (ptr != end) {
        if (comp(ptr, ptr + itemSize) > 0) {
            return false;
        }

        ptr += itemSize;
    }

    return true;
}

void* vec_minimum(vec_t* this, const comparator_t comp) {
    return (void*)vec_minimum_const(this, comp);
}

const void* vec_minimum_const(const vec_t* this, const comparator_t comp) {
    size_t itemSize = this->meta->itemSize;
    void* min = this->data;
    char* ptr = (char*)(this->data) + itemSize;
    char* end = (char*)(this->data) + (this->count * itemSize);

    while (ptr != end) {
        if (comp(ptr, min) < 0) {
            min = ptr;
        }

        ptr += itemSize;
    }

    return min;
}

void* vec_maximum(vec_t* this, const comparator_t comp) {
    return (void*)vec_maximum_const(this, comp);
}

const void* vec_maximum_const(const vec_t* this, const comparator_t comp) {
    size_t itemSize = this->meta->itemSize;
    void* max = this->data;
    char* ptr = (char*)(this->data) + itemSize;
    char* end = (char*)(this->data) + (this->count * itemSize);

    while (ptr != end) {
        if (comp(ptr, max) > 0) {
            max = ptr;
        }

        ptr += itemSize;
    }

    return max;
}

void vec_for_each(vec_t* this, const action_t act) {
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->count * this->meta->itemSize);

    while (ptr != end) {
        act(ptr);
    }
}

void vec_reverse(vec_t* this) {
    size_t itemSize = this->meta->itemSize;
    char* start = (char*)(this->data);
    char* end = start + ((this->count - 1) * itemSize);
    void* buffer = this->meta->allocate(itemSize);

    while (start < end) {
        ptr_swap(start, end, buffer, itemSize);
    }

    this->meta->deallocate(buffer);
}

void vec_shuffle(vec_t* this, const randomizer_t random) {
    size_t itemSize = this->meta->itemSize;
    void* buffer = this->meta->allocate(itemSize);

    for (size_t index = 2; index < this->count; ++index) {
        void* ptr = vec_get(this, index);
        void* randPtr = vec_get(this, random() % index);

        ptr_swap(ptr, randPtr, buffer, itemSize);
    }

    this->meta->deallocate(buffer);
}

void vec_replace(vec_t* this, const void* old, const void* new) {
    size_t itemSize = this->meta->itemSize;
    binary_predicate_t equality = this->meta->equals;
    copier_t copier = this->meta->copy;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->count * itemSize);

    while (ptr != end) {
        if (equality(ptr, old)) {
            copier(ptr, new);
        }

        ptr += itemSize;
    }
}

void vec_replace_if(vec_t* this, const void* new, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    copier_t copier = this->meta->copy;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->count * itemSize);

    while (ptr != end) {
        if (pred(ptr)) {
            copier(ptr, new);
        }

        ptr += itemSize;
    }
}

bool vec_equals(const vec_t* this, const vec_t* other) {
    if (this->count != other->count || this->meta != other->meta) {
        return false;
    }

    size_t itemSize = this->meta->itemSize;
    binary_predicate_t equality = this->meta->equals;
    char* thisPtr = (char*)(this->data);
    char* thisEnd = thisPtr + (this->count * itemSize);
    char* otherPtr = (char*)(this->data);

    while (thisPtr != thisEnd) {
        if (!equality(thisPtr, otherPtr)) {
            return false;
        }

        thisPtr += itemSize;
        otherPtr += itemSize;
    }

    return true;
}
