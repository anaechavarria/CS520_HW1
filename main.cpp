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


// Part 2
// Compare tie breaking with cells of smaller g values and larger g values.
// Runtime, number of expanded cells.
// Explain what is observed an a reason for the observation.
void run_part_2(){
    printf("Breaking ties in favor of smaller g.\n");
    for (int i = 0; i < 7; ++i){
        char filename[50];

        sprintf(filename, "test_input/test_%02d.in", i );
        //sprintf(filename, "input/grid_%02d.in", i + 1 );

        clock_t start  = clock();
        run_search(filename, cmp_smaller_g, true, false);
        clock_t end = clock();
        printf("Running time:%.4f\n", get_excecution_time(start, end));
    }

    printf("\n\n");

    printf("Breaking ties in favor of larger g.\n");
    for (int i = 0; i < 7; ++i){
        char filename[50];

        sprintf(filename, "test_input/test_%02d.in", i );
        //sprintf(filename, "input/grid_%02d.in", i + 1 );

        clock_t start  = clock();
        run_search(filename, cmp_larger_g, true, false);
        clock_t end = clock();
        printf("Running time:%.4f\n", get_excecution_time(start, end));
    }
}


// Part 3
// Compare repeated forward with repeated backward.
// Break ties in favor of cells with larger g values.
// Runtime, number of expanded cells.
// Explain what is observed an a reason for the observation.

// Part 5
// Compare repeated forward and adaptive A*
// Break ties in favor of cells with larger g values.
// Runtime, number of expanded cells.

int main(){

    run_part_2();
    // printf("\n\n");

    // for (int i = 0; i <  7; ++i){
    //     char filename[50];
    //     // sprintf(filename, "test_input/test_%02d.in", i );
    //     sprintf(filename, "input/grid_%02d.in", i+1 );

    //     printf("Computing file %s\n", filename);

    //     printf("Forward: ");
    //     if (run_search(filename, cmp_smaller_g, true, false)) printf("Goal reached!\n");
    //     else printf("There is no path\n");

    //     printf("Backward:  ");
    //     if (run_search(filename, cmp_smaller_g, false, false)) printf("Goal reached!\n");
    //     else printf("There is no path\n");

    // }

    return 0;
}

