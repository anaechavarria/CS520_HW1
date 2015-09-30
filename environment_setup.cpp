#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cassert>
#include <iterator>
using namespace std;

const int MAXN = 105; // Maximum grid length.
int grid[MAXN][MAXN]; // -1 not visited, 0 blank, 1 blocked.
vector< pair<int, int> > open_cells; // Ordered set of unvisited cells.

// Reset the values of the global variables to start a new grid.
void reset_variables(int n){
	// Reset the grid. Set all cells to -1.
	memset(grid, -1, sizeof(grid));
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

// Check if cell i, j is unvisited. Also works with indices out of bounds.
bool cell_open(int i, int j, int n){
	if (i < 0 or j < 0 or i >= n or j >= n) return false;
	return grid[i][j] == -1;
}

// Decide the value of cell (i, j) and use DFS to set the value of the 
// neighboring cells.
// The grid has size n x n and p is the probabilty that cell (i, j) is blocked.
void build_cell(int i, int j, int n, double p){
	// Cell i, j must be unvisited.
	assert(cell_open(i, j, n));
	// Look for the position of the element (i, j) in the unvisited list.
	pair<int, int> cur_cell = make_pair(i, j);
	vector<pair<int, int> >::iterator it = lower_bound(open_cells.begin(), 
		open_cells.end(), cur_cell);
	// Erase (i, j) from unvisited list.
	assert(*(it) == cur_cell);
	open_cells.erase(it);

	// Block cell with probabilty p.	
	grid[i][j] = random_double() < p ? 1 : 0;

	// If unblocked, add it to stack.
		// Search for open neighbors.
		int di[] = {+1, -1,  0,  0};
		int dj[] = { 0,  0, +1, -1};
		for (int k = 0; k < 4; ++k){
			int ni = i + di[k]; 
			int nj = j + dj[k];
			if (cell_open(ni, nj, n)){
				build_cell(ni, nj, n, p);
			}
		}
	}

}

void print_grid(int n){
	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; ++j){
			if (grid[i][j] == 0) printf(".");
			else if (grid[i][j] == 1) printf("#");
			else assert(false);
		}
		printf("\n");
	}
}

int main(){
	int num_gridworlds = 50;
	int grid_size = 101;
	double p = 0.3;

	for (int i = 0; i <  num_gridworlds; ++i){
		reset_variables(grid_size);
		while(!open_cells.empty()){
			// Chose a random cell in open_cells.
			int start_index = rand() % (open_cells.size());
			pair<int, int> start_cell = open_cells[start_index];
			int start_i = start_cell.first;
			int start_j = start_cell.second;
			// Start dfs on selected cell.
			build_cell(start_i, start_j, grid_size, p);
		}
		print_grid(grid_size);
	}
}