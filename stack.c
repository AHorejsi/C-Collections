#include <string.h>
#include "stack.h"

bool stack_empty(const stack_t* this) {
    return 0 == this->count;
}

static void copy_elements(stack_t* this, const stack_t* other) {
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

void stack_copy(stack_t* this, const stack_t* other) {
    stack_destroy(this);

    this->data = other->meta->allocate(other->capacity * other->meta->itemSize);
    this->count = other->count;
    this->capacity = other->capacity;
    this->meta = other->meta;

    copy_elements(this, other);
}

static void invalidate(stack_t* this) {
    this->data = NULL;
    this->count = -1;
    this->capacity = -1;
    this->meta = NULL;
}

void stack_move(stack_t* this, stack_t* other) {
    this->data = other->data;
    this->count = other->count;
    this->capacity = other->capacity;
    this->meta = other->meta;

    invalidate(other);
}

static void destroy_elements(stack_t* this) {
    destroyer_t destroyer = this->meta->destroy;

    if (NULL != destroyer) {
        size_t itemSize = this->meta->itemSize;
        char* ptr = (char*)(this->data);
        char* end = ptr + (itemSize * this->count);

        while (ptr != end) {
            destroyer(ptr);

            ptr += itemSize;
        }
    }
}

void stack_destroy(stack_t* this) {
    destroy_elements(this);

    this->meta->deallocate(this);

    invalidate(this);
}

bool stack_same_type(const stack_t* this, const stack_t* other) {
    return this->meta == other->meta || 0 == strcmp(this->meta->typeName, other->meta->typeName);
}

static void resize_if_needed(stack_t* this) {
    if (this->count == this->capacity) {
        ptrdiff_t newCapacity = this->capacity * 3 / 2;

        this->capacity = newCapacity;
        this->data = this->meta->allocate(newCapacity * this->meta->itemSize);
    }
}

void stack_push_copy(stack_t* this, const void* new) {
    resize_if_needed(this);

    char* ptr = (char*)(this->data) + (this->count * this->meta->itemSize);

    this->meta->copy(ptr, new);
    ++(this->count);
}

void stack_push_move(stack_t* this, void* new) {
    resize_if_needed(this);

    char* ptr = (char*)(this->data) + (this->count * this->meta->itemSize);

    this->meta->move(ptr, new);
    ++(this->count);
}

void stack_pop(stack_t* this) {
    void* last = stack_peek(this);

    this->meta->destroy(last);

    --(this->count);
}

void* stack_peek(stack_t* this) {
    return (void*)stack_peek_const(this);
}

const void* stack_peek_const(const stack_t* this) {
    return (char*)(this->data) + ((this->count - 1) * this->meta->itemSize);
}

void stack_clear(stack_t* this) {
    destroy_elements(this);
    this->meta->deallocate(this->data);

    this->capacity = STACK_DEFAULT_CAPACITY;
    this->count = 0;
    this->data = this->meta->allocate(STACK_DEFAULT_CAPACITY * this->meta->itemSize);
}

void stack_trim(stack_t* this) {
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

    this->meta->deallocate(this->data);

    this->data = newData;
    this->count = this->capacity;
}

void stack_swap(stack_t* this, stack_t* other) {
    void* tempData = this->data;
    this->data = other->data;
    other->data = tempData;

    meta_t* tempMeta = this->meta;
    this->meta = this->meta;
    other->meta = tempMeta;

    ptrdiff_t tempCount = this->count;
    this->count = other->count;
    other->count = tempCount;

    ptrdiff_t tempCapacity = this->capacity;
    this->capacity = other->capacity;
    other->capacity = tempCapacity;
}