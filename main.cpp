#include <iostream>
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

// Part 2
// Compare tie breaking with cells of smaller g values and larger g values.
// Runtime, number of expanded cells.
// Explain what is observed an a reason for the observation.

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
    for (int i = 0; i <  7; ++i){
        char filename[50];
        //sprintf(filename, "test_input/test_%02d.in", i );
        sprintf(filename, "input/grid_%02d.in", i+1 );

        printf("Computing file %s\n", filename);

        printf("Adaptive: ");
        if (run_search(filename, cmp_smaller_g, true)) printf("Goal reached!\n");
        else printf("There is no path\n");

        printf("Regular:  ");
        if (run_search(filename, cmp_smaller_g, false)) printf("Goal reached!\n");
        else printf("There is no path\n");

    }

    return 0;
}

