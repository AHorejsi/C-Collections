#ifndef ARRAY_H
#define ARRAY_H


#include <stddef.h>
#include <stdbool.h>
#include "utils.h"

typedef struct {
    void* data;
    ptrdiff_t length;
    meta_t* meta;
} array_t;

void array_copy(array_t*, const array_t*);

void array_move(array_t*, array_t*);

void array_destroy(array_t*);

bool array_same_type(const array_t*, const array_t*);

void* array_get(array_t*, const size_t);

const void* array_get_const(const array_t*, const size_t);

void array_set_copy(array_t*, const size_t, const void*);

void array_set_move(array_t*, const size_t, void*);

ptrdiff_t array_find(const array_t*, const size_t, const void*);

ptrdiff_t array_find_if(const array_t*, const size_t, const predicate_t);

ptrdiff_t array_find_last(const array_t*, const size_t, const void*);

ptrdiff_t array_find_last_if(const array_t*, const size_t, const predicate_t);

ptrdiff_t array_look(const array_t*, const size_t, const void*, const binary_predicate_t);

size_t array_count(const array_t*, const void*);

size_t array_count_if(const array_t*, const predicate_t);

void array_swap(array_t*, array_t*);

void array_fill(array_t*, const void*);

bool array_all(const array_t*, const predicate_t);

bool array_any(const array_t*, const predicate_t);

bool array_none(const array_t*, const predicate_t);

ptrdiff_t array_binary_search(const array_t*, const void*, const comparator_t);

ptrdiff_t array_lower_bound(const array_t*, const void*, const comparator_t);

ptrdiff_t array_upper_bound(const array_t*, const void*, const comparator_t);

void array_sort(array_t*, const comparator_t);

void array_stable_sort(array_t*, const comparator_t);

bool array_sorted(const array_t*, const comparator_t);

void* array_minimum(array_t*, const comparator_t);

const void* array_minimum_const(const array_t*, const comparator_t);

void* array_maximum(array_t*, const comparator_t);

const void* array_maximum_const(const array_t*, const comparator_t);

void array_for_each(array_t*, const action_t);

size_t array_partition(array_t*, const predicate_t);

//size_t array_stable_partition(array_t*, const predicate_t);

void array_reverse(array_t*);

void array_shuffle(array_t*, const randomizer_t);

void array_replace(array_t*, const void*, const void*);

void array_replace_if(array_t*, const void*, const predicate_t);

bool array_equals(const array_t*, const array_t*);

//int array_compare(const array_t*, const array_t*, const comparator_t);


#endif
