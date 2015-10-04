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


int main(){
    for (int i = 0; i <  7; ++i){
        char filename[50];
        sprintf(filename, "test_input/test_%02d.in", i );

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

