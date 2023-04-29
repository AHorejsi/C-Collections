#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "vec.h"

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

void copy(vec_t* this, const vec_t* other) {
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

void move(vec_t* this, vec_t* other) {
    this->data = other->data;
    this->count = other->count;
    this->capacity = other->capacity;
    this->meta = other->meta;

    invalidate(other);
}

static void destroy_array(vec_t* this) {
    destroyer_t destroyer = this->meta->destroy;

    if (NULL != destroyer) {
        size_t itemSize = this->meta->itemSize;
        char* ptr = (char*)(this->data);
        char* end = ptr + (this->count * itemSize);

        while (ptr < end) {
            destroyer(ptr);

            ptr += itemSize;
        }
    }

    this->meta->deallocate(this->data);
}

void destroy(vec_t* this) {
    destroy_array(this);
    invalidate(this);
}

bool empty(const vec_t* this) {
    return 0 == this->count;
}

bool null_or_empty(const vec_t* this) {
    return NULL == this || empty(this);
}

void* get(vec_t* this, const size_t index) {
    return (void*)get_const(this, index);
}

const void* get_const(const vec_t* this, const size_t index) {
    return (char*)(this->data) + (index * this->meta->itemSize);
}

void* front(vec_t* this) {
    return (void*)front_const(this);
}

const void* front_const(const vec_t* this) {
    return get_const(this, 0);
}

void* back(vec_t* this) {
    return (void*)back_const(vec);
}

const void* back_const(const vec_t* this) {
    return get_const(this, this->count - 1);
}

void set_copy(vec_t* this, const size_t index, const void* item) {
    void* ptr = get(this, index);

    this->meta->copy(ptr, item);
}

void set_first_copy(vec_t* this, const void* item) {
    set_copy(this, 0, item);
}

void set_last_copy(vec_t* this, const void* item) {
    set_copy(this, this->count - 1, item);
}

void set_move(vec_t* this, const size_t index, void* item) {
    void* ptr = get(this, index);

    this->meta->move(ptr, item);
}

void set_first_move(vec_t* this, void* item) {
    set_move(this, 0, item);
}

void set_last_move(vec_t* this, void* item) {
    set_move(this, this->count - 1, item);
}

static void reallocate_array(vec_t* this, const size_t newCapacity) {
    size_t itemSize = this->meta->itemSize;
    char* newData = (char*)malloc(itemSize * newCapacity);
    char* data = (char*)(this->data);
    char* end = data + (this->count * itemSize);
    mover_t mover = this->meta->move;

    while (data < end) {
        mover(newData, data);

        newData += itemSize;
        data += itemSize;
    }

    destroy_array(this);

    this->data = newData;
    this->capacity = newCapacity;
}

void reserve(vec_t* this, const size_t newCapacity) {
    if (newCapacity > this->capacity) {
        reallocate_array(this, newCapacity);
    }
}

void shrink_to_fit(vec_t* this) {
    if (this->count < this->capacity) {
        reallocate_array(this, this->count);
    }
}

void push_back_copy(vec_t* this, const void* item) {
    insert_copy(this, this->length, item);
}

void push_back_move(vec_t* this, void* item) {
    insert_move(this, this->length, item);
}

void insert_copy(vec_t* this, const size_t index, const void* item) {
    
}

void insert_move(vec_t*, const size_t, void*);
