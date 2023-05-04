#ifndef ARRAY_H
#define ARRAY_H


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "utils.h"

typedef struct {
    void* data;
    ptrdiff_t length;
    meta_t* meta;
} array_t;

void copy(array_t*, const array_t*);

void move(array_t*, array_t*);

void destroy(array_t*);

bool is_same_type(const array_t*, const array_t*);

bool empty(const array_t*);

bool null_or_empty(const array_t*);

void* get(array_t*, const size_t);

const void* get_const(const array_t*, const size_t);

void* front(array_t*);

const void* front_const(const array_t*);

void* back(array_t*);

const void* back_const(const array_t*);

void set_copy(array_t*, const size_t, const void*);

void set_first_copy(array_t*, const void*);

void set_last_copy(array_t*, const void*);

void set_move(array_t*, const size_t, void*);

void set_first_move(array_t*, void*);

void set_last_move(array_t*, void*);

bool contains(const array_t*, const void*);

ptrdiff_t find(const array_t*, const size_t, const void*);

ptrdiff_t find_if(const array_t*, const size_t, const predicate_t);

ptrdiff_t find_last(const array_t*, const size_t, const void*);

ptrdiff_t find_last_if(const array_t*, const size_t, const predicate_t);

ptrdiff_t look(const array_t*, const size_t, const void*, const binary_predicate_t);

size_t count(const array_t*, const void*);

size_t count_if(const array_t*, const predicate_t);

void swap(array_t*, array_t*);

void fill(array_t*, const void*);

bool all(const array_t*, const predicate_t);

bool any(const array_t*, const predicate_t);

bool none(const array_t*, const predicate_t);

ptrdiff_t binary_search(const array_t*, const void*, const comparator_t);

ptrdiff_t lower_bound(const array_t*, const void*, const comparator_t);

ptrdiff_t upper_bound(const array_t*, const void*, const comparator_t);

void sort(array_t*, const comparator_t);

void stable_sort(array_t*, const comparator_t);

bool sorted(const array_t*, const comparator_t);

void* minimum(array_t*, const comparator_t);

const void* minimum_const(const array_t*, const comparator_t);

void* maximum(array_t*, const comparator_t);

const void* maximum_const(const array_t*, const comparator_t);

void for_each(array_t*, const action_t);

void* random(array_t*, const randomizer_t);

const void* random_const(const array_t*, const randomizer_t);

size_t partition(array_t*, const predicate_t);

//size_t stable_partition(array_t*, const predicate_t);

void reverse(array_t*);

void shuffle(array_t*, const randomizer_t);

void replace(array_t*, const void*, const void*);

void replace_if(array_t*, const void*, const predicate_t);

bool equals(const array_t*, const array_t*);

//int8_t compare(const array_t*, const array_t*, const comparator_t);

//bool perm(const array_t*, const array_t*);

//void next_perm(array_t*);

//void prev_perm(array_t*);*/


#endif
