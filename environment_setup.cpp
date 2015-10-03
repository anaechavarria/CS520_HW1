#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cassert>
#include <iterator>
using namespace std;

const int MAXN = 105; // Maximum grid length.

// Careful not to change the value of NOT_VISITED since we are using memeset to
// set the cells initially which does it by bytes (-1 = 1111 1111 ... 1111).
enum { UNBLOCKED = 0, BLOCKED = 1, NOT_VISITED = -1};

int grid[MAXN][MAXN]; // The resulting grid
vector< pair<int, int> > open_cells; // Ordered set of unvisited cells.

// Reset the values of the global variables to start a new grid.
void reset_variables(int n){
    // Reset the grid. Set all cells to not visited.
    // Not visited must be an integer that has all the 8 bytes the same for this
    // statement to work properly.
    memset(grid, NOT_VISITED, sizeof(grid));
    // Add all cells as open_cells.
    open_cells.clear();
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            // Note that they are ordered increasingly.
            // (a, b) < (c, d) if a < c or (a = c and b < d).
            open_cells.push_back(make_pair(i, j));
        }
    }
}

// Return a random number between 0 and 1.
double random_double(){
    return ((double)rand() / (double)(RAND_MAX));
}

// Checks if the cell i, j is inside the bounds of the grid.
bool inside(int i, int j, int n){
    if (i < 0 or j < 0 or i >= n or j >= n) return false;
    return true;
}

// Check if cell i, j is unvisited. Also works with indices out of bounds.
bool cell_open(int i, int j, int n){
    if (!inside(i, j, n)) return false;
    return grid[i][j] == NOT_VISITED;
}

void remove_cell_from_open_list(int i, int j, int n){
    // Cell i, j must be unvisited.
    assert(cell_open(i, j, n));
    // Look for the position of the element (i, j) in the unvisited list.
    pair<int, int> cur_cell = make_pair(i, j);
    vector<pair<int, int> >::iterator it = lower_bound(open_cells.begin(),
        open_cells.end(), cur_cell);
    // Erase (i, j) from unvisited list.
    assert(*(it) == cur_cell);
    open_cells.erase(it);
}

// TODO: Change comments.
// Decide the value of cell (i, j) and use DFS to set the value of the
// neighboring cells.
// The grid has size n x n and p is the probabilty that cell (i, j) is blocked.
void build_cell(const int i, const int j, const int n){
    assert(grid[i][j] == UNBLOCKED);

    // Search for neighbors.
    vector<pair<int, int> > neighbors;
    int di[] = {+2, -2,  0,  0};
    int dj[] = { 0,  0, +2, -2};
    for (int k = 0; k < 4; ++k){
        // The next cell tht is two cells away.
        int next_i = i + di[k];
        int next_j = j + dj[k];
        neighbors.push_back(make_pair(next_i, next_j));
    }
    // Make a random order of the neighbors.
    random_shuffle(neighbors.begin(), neighbors.end());

    // Recurr on the neighbors in a random order.
    for (int k = 0; k < neighbors.size(); ++k){

        int next_i = neighbors[k].first;
        int next_j = neighbors[k].second;
        int inter_i = (i + next_i) / 2;
        int inter_j = (j + next_j) / 2;

        if (cell_open(next_i, next_j, n)) remove_cell_from_open_list(next_i, next_j, n);
        if (cell_open(inter_i, inter_j, n)) remove_cell_from_open_list(inter_i, inter_j, n);

        if (cell_open(next_i, next_j, n) and cell_open(inter_i, inter_j, n)){
            // Mark the next two cell as unblocked.
            grid[next_i][next_j] = grid[inter_i][inter_j] = UNBLOCKED;
            // Mark the neigbors of the intermediate cell as blocked.
            for (int l = 0; l < 4; ++l){
                int inter_neigh_i = inter_i + di[l] / 2;
                int inter_neigh_j = inter_j + dj[l] / 2;
                if (cell_open(inter_neigh_i, inter_neigh_j, n)){
                    remove_cell_from_open_list(inter_neigh_i, inter_neigh_j, n);
                    grid[inter_neigh_i][inter_neigh_j] = BLOCKED;
                }
            }
            build_cell(next_i, next_j, n);
        }else{
            // Mark the next two cells as blocked.
            if (cell_open(next_i, next_j, n)){
                grid[next_i][next_j] = BLOCKED;
            }
            if (cell_open(inter_i, inter_j, n)){
                grid[inter_i][inter_j] = BLOCKED;
            }
        }
    }
}

void print_grid(int n){
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            if (grid[i][j] == UNBLOCKED) printf(". ");
            else if (grid[i][j] == BLOCKED) printf("# ");
            else assert(false);
        }
        printf("\n");
    }
    printf("\n");
}

int main(){
    int num_gridworlds = 3;
    int grid_size = 10;

    for (int i = 0; i <  num_gridworlds; ++i){
        reset_variables(grid_size);
        while(!open_cells.empty()){
            // Chose a random cell in open_cells.
            int start_index = rand() % (open_cells.size());
            pair<int, int> start_cell = open_cells[start_index];
            int start_i = start_cell.first;
            int start_j = start_cell.second;
            // Start dfs on selected cell.
            remove_cell_from_open_list(start_i, start_j, grid_size);
            grid[start_i][start_j] = UNBLOCKED;
            build_cell(start_i, start_j, grid_size);
        }
        // Just print the first cell. TODO Save to a plain text file.
        print_grid(grid_size);
    }
}
