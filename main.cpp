#include <iostream>
#include <ctime>
#include "a_star_search.h"

using namespace std;


// True iff cell a is less than cell b.
// Break ties in favor of the cell with the smaller g value.
bool cmp_smaller_g(const cell &a, const cell &b){
    if (a.f() == b.f()) return a.g < b.g;
    return a.f() < b.f();
}

// True iff cell a is less than cell b.
// Break ties in favor of the cell with the larger g value.
bool cmp_larger_g(const cell &a, const cell &b){
    if (a.f() == b.f()) return a.g > b.g;
    return a.f() < b.f();
}

double get_excecution_time(clock_t start, clock_t end){
    return 1.0 * (end - start) / CLOCKS_PER_SEC;
}

void run_search_on_all_inputs(function<bool (cell, cell)> cmp, bool forward, bool adaptive){
    for (int i = 0; i < 50; ++i){
        char filename[50];

        sprintf(filename, "input/grid_%02d.in", i + 1 );

        printf("file = %s, ", filename);
        clock_t start  = clock();
        run_search(filename, cmp, forward, adaptive);
        clock_t end = clock();
        printf(", time = %.4f\n", get_excecution_time(start, end));
    }
}

void run_search_on_all_tests(function<bool (cell, cell)> cmp, bool forward, bool adaptive){
    for (int i = 0; i < 7; ++i){
        char filename[50];

        sprintf(filename, "test_input/test_%02d.in", i );

        printf("file = %s, ", filename);
        clock_t start  = clock();
        run_search(filename, cmp, forward, adaptive);
        clock_t end = clock();
        printf(", time: %.4f\n", get_excecution_time(start, end));
    }
}


// Part 2
// Compare tie breaking with cells of smaller g values and larger g values.
// Runtime, number of expanded cells.
// Explain what is observed an a reason for the observation.
void run_part_2(){
    printf("Breaking ties in favor of smaller g.\n");
    run_search_on_all_inputs(cmp_smaller_g, true, false);

    printf("\n\n");

    printf("Breaking ties in favor of larger g.\n");
    run_search_on_all_inputs(cmp_larger_g, true, false);
}

// Part 3
// Compare repeated forward with repeated backward.
// Break ties in favor of cells with larger g values.
// Runtime, number of expanded cells.
// Explain what is observed an a reason for the observation.
void run_part_3(){
    printf("Forward A*.\n");
    run_search_on_all_inputs(cmp_larger_g, true, false);

    printf("\n\n");

    printf("Backward A*.\n");
    run_search_on_all_inputs(cmp_larger_g, false, false);
}

// Part 5
// Compare repeated forward and adaptive A*
// Break ties in favor of cells with larger g values.
// Runtime, number of expanded cells.
void run_part_5(){
    printf("Regular A*.\n");
    run_search_on_all_inputs(cmp_larger_g, true, false);

    printf("\n\n");

    printf("Adaptive A*.\n");
    run_search_on_all_inputs(cmp_larger_g, true, true);
}

int main(){
    run_part_2();
    printf("\n\n");
    run_part_3();
    printf("\n\n");
    run_part_5();

    return 0;
}

