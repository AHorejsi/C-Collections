#include <stdlib.h>
#include <string.h>
#include "utils.h"
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

void copy(array_t* this, const array_t* other) {
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

void move(array_t* this, array_t* other) {
    this->data = other->data;
    this->length = other->length;
    this->meta = other->meta;

    invalidate(other);
}

void destroy(array_t* this) {
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

bool is_same_type(const array_t* this, const array_t* other) {
    return this->meta == other->meta || return 0 == strcmp(this->meta->containedType, other->meta->containedType);
}

bool empty(const array_t* this) {
    return 0 == this->length;
}

bool null_or_empty(const array_t* this) {
    return NULL == this || empty(this);
}

void* get(array_t* this, const size_t index) {
    return (void*)get_const(this, index);
}

const void* get_const(const array_t* this, const size_t index) {
    return (char*)(this->data) + (index * this->meta->itemSize);
}

void* front(array_t* this) {
    return (void*)front_const(this);
}

const void* front_const(const array_t* this) {
    return get_const(this, 0);
}

void* back(array_t* this) {
    return (void*)back_const(this);
}

const void* back_const(const array_t* this) {
    return get_const(this, this->length - 1);
}

void set_copy(array_t* this, const size_t index, const void* new) {
    void* ptr = get(this, index);

    this->meta->copy(ptr, new);
}

void set_first_copy(array_t* this, const void* new) {
    set_copy(this, 0, new);
}

void set_last_copy(array_t* this, const void* new) {
    set_copy(this, this->length - 1, new);
}

void set_move(array_t* this, const size_t index, void* new) {
    void* ptr = get(this, index);

    this->meta->move(ptr, new);
}

void set_first_move(array_t* this, void* new) {
    set_move(this, 0, new);
}

void set_last_move(array_t* this, void* new) {
    set_move(this, this->length - 1, new);
}

bool contains(const array_t* this, const void* item) {
    return -1 != find(this, 0, item);
}

ptrdiff_t find(const array_t* this, const size_t fromIndex, const void* item) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data) + (fromIndex * itemSize);
    char* end = (char*)(this->data) + (this->length * itemSize);
    ptrdiff_t index = fromIndex;
    equality_t equality = this->meta->equals;

    while (ptr < end) {
        if (equality(ptr, item)) {
            return index;
        }

        ++index;
        ptr += itemSize;
    }

    return -1;
}

ptrdiff_t find_if(const array_t* this, const size_t fromIndex, const predicate_t pred) {
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

ptrdiff_t find_last(const array_t* this, const size_t fromIndex, const void* item) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data) + (fromIndex * itemSize);
    char* end = (char*)(this->data);
    ptrdiff_t index = fromIndex;
    equality_t equality = this->meta->equals;

    while (ptr >= end) {
        if (equality(ptr, item)) {
            return index;
        }

        --index;
        ptr -= itemSize;
    }

    return -1;
}

ptrdiff_t find_last_if(const array_t* this, const size_t fromIndex, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data) + (fromIndex * itemSize);
    char* end = (char*)(this->data);
    ptrdiff_t index = fromIndex;
    equality_t equality = this->meta->equals;

    while (ptr >= end) {
        if (pred(ptr)) {
            return index;
        }

        --index;
        ptr -= itemSize;
    }

    return -1;
}

ptrdiff_t look(const array_t* this, const size_t fromIndex, const void* item, const binary_predicate_t pred) {
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

size_t count(const array_t* this, const void* item) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);
    size_t amount = 0;
    equality_t equality = this->meta->equals;

    while (ptr < end) {
        if (equality(ptr, item)) {
            ++amount;
        }

        ptr += itemSize;
    }

    return amount;
}

size_t count_if(const array_t* this, const predicate_t pred) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);
    size_t amount = 0;
    equality_t equality = this->meta->equals;

    while (ptr < end) {
        if (equality(ptr)) {
            ++amount;
        }

        ptr += itemSize;
    }

    return amount;
}

void swap(array_t* this, array_t* other) {
    void* tempData = this->data;
    this->data = other->data;
    other->data = tempData;

    ptrdiff_t tempLength = this->length;
    this->length = other->length;
    other->length = tempLength;

    meta_t tempMeta = this->meta;
    this->meta = other->meta;
    other->meta = tempMeta;
}

void fill(array_t* this, const void* value) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);
    copier_t copier = this->meta->copy;

    while (ptr < end) {
        copier(ptr, value);

        ptr += itemSize;
    }
}

bool all(const array_t* this, const predicate_t pred) {
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

bool any(const array_t* this, const predicate_t pred) {
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

bool none(const array_t* this, const predicate_t pred) {
    return !any(this, pred);
}

ptrdiff_t binary_search(const array_t* this, const void* item, const comparator_t comp) {
    size_t lowIndex = 0;
    size_t highIndex = this->length - 1;

    while (lowIndex < highIndex) {
        size_t midIndex = (lowIndex + highIndex) / 2;
        const void* elem = get_const(this, midIndex);
        int8_t comp = comp(item, elem);

        if (comp < 0) {
            highIndex = midIndex - 1;
        }
        else if (comp > 0) {
            lowIndex = midIndex + 1;
        }
        else {
            return midIndex;
        }
    }

    return -1;
}

bool sorted(const array_t* this, const comparator_t comp) {
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

void* minimum(array_t* this, const comparator_t comp) {
    return (void*)minimum_const(this, comp);
}

const void* minimum_const(const array_t* this, const comparator_t comp) {
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

void* maximum(array_t* this, const comparator_t comp) {
    return (void*)maximum_const(this, comp);
}

const void* maximum_const(const array_t* this, const comparator_t comp) {
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

void for_each(array_t* this, const action_t act) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);

    while (ptr < end) {
        act(ptr);

        ptr += itemSize;
    }
}

void* random(array_t* this, const randomizer_t random) {
    return (void*)random_const(this, random);
}

const void* random_const(const array_t* this, const randomizer_t random) {
    return get_const(this, random() % this->length);
}

void reverse(array_t* this) {
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

void shuffle(array_t* this, const randomizer_t random) {
    size_t itemSize = this->meta->itemSize;
    void* buffer = this->meta->allocate(itemSize);

    for (size_t index = 1; index < this->length; ++index) {
        void* ptr = get(this, index);
        void* randPtr = get(this, random() % index);

        ptr_swap(ptr, randPtr, buffer, itemSize);
    }

    this->meta->deallocate(buffer);
}

void replace(array_t* this, const void* old, const void* new) {
    size_t itemSize = this->meta->itemSize;
    char* ptr = (char*)(this->data);
    char* end = ptr + (this->length * itemSize);
    equality_t equality = this->meta->equals;
    copier_t copier = this->meta->copy;

    while (ptr < end) {
        if (equality(old, ptr)) {
            copier(ptr, new);
        }

        ptr += itemSize;
    }
}

void replace_if(array_t* this, const void* new, const predicate_t pred) {
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

bool equals(const array_t* this, const array_t* other) {
    if (this->length != other->length) {
        return false;
    }

    size_t itemSize = this->meta->itemSize;
    char* thisPtr = (char*)(this->data);
    char* thisEnd = thisPtr + (this->length * itemSize);
    char* otherPtr = (char*)(other->data);
    equality_t equality = this->meta->equals;

    while (thisPtr < thisEnd) {
        if (!equality(thisPtr, otherPtr)) {
            return false;
        }

        thisPtr += itemSize;
        otherPtr += itemSize
    }

    return true;
}
