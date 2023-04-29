#ifndef UTILS_H
#define UTILS_H


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef void(*copier_t)(void*, const void*);
typedef void(*mover_t)(void*, void*);
typedef void(*destroyer_t)(void*);
typedef void*(*allocator_t)(const size_t);
typedef void(*deallocator_t)(void*);
typedef bool(*predicate_t)(const void*);
typedef bool(*binary_predicate_t)(const void*, const void*);
typedef bool(*equality_t)(const void*, const void*);
typedef int8_t(*comparator_t)(const void*, const void*);
typedef void(*action_t)(void*);
typedef size_t(*randomizer_t)(void);

typedef struct {
    size_t itemSize;
    char* containedType;
    copier_t copy;
    mover_t move;
    equality_t equals;
    destroyer_t destroy;
    allocator_t allocate;
    deallocator_t deallocate;
} meta_t;

void ptr_swap(void*, void*, void*, const size_t);


#endif
