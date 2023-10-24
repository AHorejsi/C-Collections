#ifndef UTILS_H
#define UTILS_H


#include <stddef.h>
#include <stdbool.h>

typedef char* string_t;
typedef void(*copier_t)(void*, const void*);
typedef void(*mover_t)(void*, void*);
typedef void(*destroyer_t)(void*);
typedef void*(*allocator_t)(size_t);
typedef void(*deallocator_t)(void*);
typedef bool(*predicate_t)(const void*);
typedef bool(*binary_predicate_t)(const void*, const void*);
typedef ptrdiff_t(*comparator_t)(const void*, const void*);
typedef void(*action_t)(void*);
typedef size_t(*randomizer_t)(void);

typedef struct {
    size_t itemSize;
    string_t typeName;
    copier_t copy;
    mover_t move;
    binary_predicate_t equals;
    destroyer_t destroy;
    allocator_t allocate;
    deallocator_t deallocate;
} meta_t;

typedef enum {
    UNDEFINED,
    LESS,
    EQUAL,
    GREATER
} comparison_t;

typedef struct {
    char* low;
    char* high;
} range_t;

void ptr_swap(void*, void*, void*, const size_t);


#endif
