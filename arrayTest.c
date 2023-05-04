#include "arrayTest.h"

static void copy_test();

static void move_test();

static void destroy_test();

static void is_same_type_test();

static void empty_test();

static void null_or_empty_test();

static void get_test();

static void get_const_test();

static void front_test();

static void front_const_test();

static void back_test();

static void back_const_test();

static void set_copy_test();

static void set_first_copy_test();

static void set_last_copy_test();

static void set_move_test();

static void set_first_move_test();

static void set_last_move_test();

static void contains_test();

static void find_test();

static void find_if_test();

static void find_last_test();

static void find_last_if_test();

static void look_test();

static void count_test();

static void count_if_test();

static void swap_test();

static void fill_test();

static void all_test();

static void any_test();

static void none_test();

static void binary_search_test();

static void lower_bound_test();

static void upper_bound_test();

static void sort_test();

static void stable_sort_test();

static void sorted_test();

static void minimum_test();

static void minimum_const_test();

static void maximum_test();

static void maximum_const_test();

static void for_each_test();

static void random_test();

static void random_const_test();

static void partition_test();

//static void stable_partition_test();

static void reverse_test();

static void shuffle_test();

static void replace_test();

static void replace_if_test();

static void equals_test();

//static void compare_test();

//static void perm_test();

//static void next_perm_test();

//static void prev_perm_test();

void run_array_tests() {
    copy_test();
    move_test();
    destroy_test();
    is_same_type_test();
    empty_test();
    null_or_empty_test();
    get_test();
    get_const_test();
    front_test();
    front_const_test();
    back_test();
    back_const_test();
    set_copy_test();
    set_first_copy_test();
    set_last_copy_test();
    set_move_test();
    set_first_move_test();
    set_last_move_test();
    contains_test();
    find_test();
    find_if_test();
    find_last_test();
    find_last_if_test();
    look_test();
    count_test();
    count_if_test();
    swap_test();
    fill_test();
    all_test();
    any_test();
    none_test();
    binary_search_test();
    lower_bound_test();
    upper_bound_test();
    sort_test();
    stable_sort_test();
    sorted_test();
    minimum_test();
    minimum_const_test();
    maximum_test();
    maximum_const_test();
    for_each_test();
    random_test();
    random_const_test();
    partition_test();
    //stable_partition_test();
    reverse_test();
    shuffle_test();
    replace_test();
    replace_if_test();
    equals_test();
    //compare_test();
    //perm_test();
    //next_perm_test();
    //prev_perm_test();
}
