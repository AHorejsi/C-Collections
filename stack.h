#ifndef STACK_H
#define STACK_H


#include <stddef.h>
#include <stdbool.h>
#include "utils.h"

#define STACK_DEFAULT_CAPACITY 16

typedef struct {
    void* data;
    size_t capacity;
    size_t count;
    meta_t* meta;
} stack_t;

bool stack_empty(const stack_t*);

void stack_copy(stack_t*, const stack_t*);

void stack_move(stack_t*, stack_t*);

void stack_destroy(stack_t*);

bool stack_same_type(const stack_t*, const stack_t*);

void stack_push_copy(stack_t*, const void*);

void stack_push_move(stack_t*, void*);

void stack_pop(stack_t*);

void* stack_peek(stack_t*);

const void* stack_peek_const(const stack_t*);

void stack_clear(stack_t*);

void stack_trim(stack_t*);

void stack_swap(stack_t*, stack_t*);


#endif
