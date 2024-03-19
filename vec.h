#ifndef VEC_H
#define VEC_H


#include <stddef.h>
#include <stdbool.h>
#include "utils.h"

#define VEC_DEFAULT_CAPACITY 16

typedef struct {
    void* data;
    ptrdiff_t capacity;
    ptrdiff_t count;
    meta_t* meta;
} vec_t;

bool vec_empty(const vec_t*);

void vec_copy(vec_t*, const vec_t*);

void vec_move(vec_t*, vec_t*);

void vec_destroy(vec_t*);

bool vec_same_type(const vec_t*, const vec_t*);

void* vec_get(vec_t*, const size_t);

const void* vec_get_const(const vec_t*, const size_t);

void vec_set_copy(vec_t*, const size_t, const void*);

void vec_set_move(vec_t*, const size_t, void*);

void vec_push_back_copy(vec_t*, const void*);

void vec_push_back_move(vec_t*, void*);

void vec_insert_copy(vec_t*, const size_t, const void*);

void vec_insert_move(vec_t*, const size_t, void*);

void vec_pop_back(vec_t*);

void vec_erase(vec_t*, const size_t);

void vec_erase_range(vec_t*, const size_t, const size_t);

size_t vec_erase_if(vec_t*, const predicate_t);

size_t vec_erase_all_of(vec_t*, const void*);

bool vec_erase_first_of(vec_t*, const void*);

size_t vec_erase_amount_of(vec_t*, const void*, const size_t);

void vec_clear(vec_t*);

void vec_trim(vec_t*);

ptrdiff_t vec_find(const vec_t*, const size_t, const void*);

ptrdiff_t vec_find_if(const vec_t*, const size_t, const predicate_t);

ptrdiff_t vec_find_last(const vec_t*, const size_t, const void*);

ptrdiff_t vec_find_last_if(const vec_t*, const size_t, const predicate_t);

ptrdiff_t vec_look(const vec_t*, const size_t, const void*, const binary_predicate_t);

size_t vec_count(const vec_t*, const void*);

size_t vec_count_if(const vec_t*, const predicate_t);

void vec_swap(vec_t*, vec_t*);

void vec_fill(vec_t*, const void*);

bool vec_all(const vec_t*, const predicate_t);

bool vec_any(const vec_t*, const predicate_t);

bool vec_none(const vec_t*, const predicate_t);

//ptrdiff_t vec_binary_search(const vec_t*, const void*, const comparator_t);

//ptrdiff_t vec_lower_bound(const vec_t*, const void*, const comparator_t);

//ptrdiff_t vec_upper_bound(const vec_t*, const void*, const comparator_t);

//void vec_sort(vec_t*, const comparator_t);

//void vec_stable_sort(vec_t*, const comparator_t);

bool vec_sorted(const vec_t*, const comparator_t);

void* vec_minimum(vec_t*, const comparator_t);

const void* vec_minimum_const(const vec_t*, const comparator_t);

void* vec_maximum(vec_t*, const comparator_t);

const void* vec_maximum_const(const vec_t*, const comparator_t);

void vec_for_each(vec_t*, const action_t);

//size_t vec_partition(vec_t*, const predicate_t);

void vec_reverse(vec_t*);

void vec_shuffle(vec_t*, const randomizer_t);

void vec_replace(vec_t*, const void*, const void*);

void vec_replace_if(vec_t*, const void*, const predicate_t);

bool vec_equals(const vec_t*, const vec_t*);

//comparison_t vec_compare(const vec_t*, const vec_t*, const comparator_t);


#endif
