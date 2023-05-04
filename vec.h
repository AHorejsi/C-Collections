#ifndef VEC_H
#define VEC_H


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "utils.h"

typedef struct {
    void* data;
    ptrdiff_t capacity;
    ptrdiff_t count;
    meta_t* meta;
} vec_t;

void copy(vec_t*, const vec_t*);

void move(vec_t*, vec_t*);

void destroy(vec_t*);

bool empty(const vec_t*);

bool null_or_empty(const vec_t*);

void* get(vec_t*, const size_t);

const void* get_const(const vec_t*, const size_t);

void* front(vec_t*);

const void* front_const(const vec_t*);

void* back(vec_t*);

const void* back_const(const vec_t*);

void set_copy(vec_t*, const size_t, const void*);

void set_first_copy(vec_t*, const void*);

void set_last_copy(vec_t*, const void*);

void set_move(vec_t*, const size_t, void*);

void set_first_move(vec_t*, void*);

void set_last_move(vec_t*, void*);

void reserve(vec_t*, const size_t);

void shrink_to_fit(vec_t*);

//void push_back_copy(vec_t*, const void*);

//void push_back_move(vec_t*, void*);

//void insert_copy(vec_t*, const size_t, const void*);

//void insert_move(vec_t*, const size_t, void*);

//void resize(vec_t*, const size_t*, const void*);

//void pop_back(vec_t*);

//void erase(vec_t*, const size_t);

//void erase(vec_t*, const size_t, const size_t);

//bool contains(const vec_t*, const void*);

//ptrdiff_t find(const vec_t*, const size_t, const void*);

//ptrdiff_t find_if(const vec_t*, const size_t, const predicate_t);

//ptrdiff_t find_last(const vec_t*, const size_t, const void*);

//ptrdiff_t find_last_if(const vec_t*, const size_t, const predicate_t);

//ptrdiff_t look(const vec_t*, const size_t, const void*, const binary_predicate_t);

//size_t count(const vec_t*, const void*);

//size_t count_if(const vec_t*, const predicate_t);

//void swap(vec_t*, vec_t*);

//void fill(vec_t*, const void*);

//bool all(const vec_t*, const predicate_t);

//bool any(const vec_t*, const predicate_t);

//bool none(const vec_t*, const predicate_t);

//ptrdiff_t binary_search(const vec_t*, const void*, const comparator_t);

//ptrdiff_t lower_bound(const vec_t*, const void*, const comparator_t);

//ptrdiff_t upper_bound(const vec_t*, const void*, const comparator_t);

//void sort(vec_t*, const comparator_t);

//void stable_sort(vec_t*, const comparator_t);

//void sort(vec_t*, const comparator_t);

//void stable_sort(vec_t*, const comparator_t);

//bool sorted(const vec_t*, const comparator_t);

//void* minimum(vec_t*, const comparator_t);

//const void* minimum_const(const vec_t*, const comparator_t);

//void* maximum(vec_t*, const comparator_t);

//const void* maximum_const(const vec_t*, const comparator_t);

//void for_each(vec_t*, const action_t);

//void* random(vec_t*, const randomizer_t);

//const void* random_const(const vec_t*, const randomizer_t);

//void partition(vec_t*, const predicate_t);

//void stable_partition(vec_t*, const predicate_t);

//void reverse(vec_t*);

//void shuffle(vec_t*, const randomizer_t);

//void rotate(vec_t*, const size_t);

//void replace(vec_t*, const void*, const void*);

//void replace_if(vec_t*, const void*, const predicate_t);

//bool equals(const vec_t*, const vec_t*);

//int8_t compare(const vec_t*, const vec_t*, const comparator_t);

//bool perm(const vec_t*, const vec_t*);

//void next_perm(vec_t*);

//void prev_perm(vec_t*);
