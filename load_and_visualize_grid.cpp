#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

// Loads the grid stored in the grid_path and pretty prints it to stdout.
int main(){
    string grid_path = "input/grid_04.in";
    // string grid_path = "test_input/test_00.in";

    freopen(grid_path.c_str(), "r", stdin);

    int n;
    cin >> n;
    int i0, j0, i1, j1;
    cin >> i0 >> j0 >> i1 >> j1;

    printf("Search must be run from cell (%d, %d) to cell(%d, %d)\n\n", i0, j0, i1, j1);

    // Print the indices of the grid.
    printf("    |");
    for (int i = 0; i < n; ++i) printf(" %3d", i);
    printf(" |\n");
    // Print a horizontal dividing line.
    printf("----+");
    for (int i = 0; i < n; ++i) printf("----");
    printf("-+\n");

    for (int i = 0; i < n; ++i){
        string line; cin >> line;
        printf("%3d |", i);
        for (int j = 0; j < n; ++j){
            printf("   %c", line[j]);
        }
        printf(" |\n");
    }

    printf("----+");
    for (int i = 0; i < n; ++i) printf("----");
    printf("-+\n");

    return 0;
}
