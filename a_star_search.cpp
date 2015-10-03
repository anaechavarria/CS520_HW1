#include <iostream>
#include <cstdio>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "cell_priority_queue.h"
using namespace std;


const int MAXN = 105;
const int INF = 10000000;

// The possible values for the cells in the grid.
enum { UNBLOCKED = 0, BLOCKED = 1, UNEXPLORED = -1};

int explored_grid[MAXN][MAXN]; // The status of the grid explored so far.
int actual_grid[MAXN][MAXN]; // The actual status of the grid.

int g[MAXN][MAXN]; // The lowest known cost for the given cell.
int last_iter_searched[MAXN][MAXN]; // Last iter where the cost was updated.

set<pair<int, int> > closed; // The set of the cells that have been visited.
pair <int, int> tree[MAXN][MAXN]; // The cell where we came from in the search.

int n; // The size of the grid.


// Loads the grid stored in the grid_path and stores it in the variable grid.
// Returns n where n x n is the size of the grid.
int load_grid(string grid_path, int &x0, int &y0, int &x1, int &y1){
    freopen(grid_path.c_str(), "r", stdin);

    int grid_size;

    cin >> grid_size;
    cin >> x0 >> y0 >> x1 >> y1;

    for (int i = 0; i < grid_size; ++i){
        string line; cin >> line; cout << line << endl;
        assert (line.size() == grid_size);
        for (int j = 0; j < grid_size; ++j){
            if (line[j] == '.') actual_grid[i][j] = UNBLOCKED;
            else if (line[j] == '#') actual_grid[i][j] = BLOCKED;
            else assert(false);
        }
    }
    return grid_size;
}

// Set the values of all the global variables before starting the search.
void init_variables(string grid_path, int &x0, int &y0, int &x1, int &y1){
    // Load grid and get grid size.
    n = load_grid(grid_path, x0, y0, x1, y1);

    for (int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            explored_grid[i][j] = UNEXPLORED;
            g[i][j] = INF;
            last_iter_searched[i][j] = 0;
            tree[i][j] = make_pair(-1, -1);
        }
    }
    closed.clear();
}

// TODO Implement.
void reset_variables(){
    // Reset variables. Reset tree. Reset array g to INF. Search matrix. closed.
}


// Returns the manhattan distance from cell (x0, y0) to cell (x1, y1).
int get_h(int x0, int y0, int x1, int y1){
    return abs(x1 - x0) + abs(y1 - y0);
}

// Check if the inidices (i, j) are inside the grid.
bool inside(int i, int j){
    if (i < 0 or j < 0 or i >= n or j >= n) return false;
    return true;
}

// Check if cell i, j is (presumably) unblocked. Indices can be out of bounds.
bool unblocked(int i, int j){
    // If the indices are out of bounds.
    if (!inside(i, j)) return false;
    // If the cell is blocked.
    if (explored_grid[i][j] == BLOCKED) return false;
    // The indices are in the bounds and the cell is either free or unexpolred.
    return true;
}


// Compute the shortest path from:
// cell (start_i, start_j) to cell (goal_i, goal_j)
// Open is the pointer priority queue used for the search. It must be empty when
// calling the function but is not neccesarily empty when returning.
// Search count indicates the iteration number. Used to avoid refreshing the g
// matrix for every cell.
void compute_path(PriorityQueue &open, const int start_i, const int start_j,
                  const int goal_i, const int goal_j, const int search_count){

    assert(open.empty());

    int start_h = get_h(start_i, start_j, goal_i, goal_j);
    cell start_cell = cell(start_i, start_j, 0, start_h);

    open.push(start_cell);

    while (!open.empty()){
        // Extract min from open.
        cell cur = open.pop();

        printf("Popping: "); cur.print();

        // If this solution is worse than the best solution know to the goal.
        // Compare using f because f is the minimum possible cost to the goal.
        // Break because the answer is not going to improve.
        if (g[goal_i][goal_j] <= cur.f()) break;

        // If this solution doesn't improve the known soution of the given cell.
        // This is not in the pseudocode but is needed because we may have
        // several copies of the same cell in the queue.
        if (cur.g > g[cur.i][cur.j]) continue;

        closed.insert(make_pair(cur.i, cur.j));

        // Move to the neighbors of cur
        int di[] = {+1, -1,  0,  0};
        int dj[] = { 0,  0, +1, -1};
        for (int k = 0; k < 4; ++k){
            int next_i = cur.i + di[k];
            int next_j = cur.j + dj[k];
            if (unblocked(next_i, next_j)){
                // If this cell has not been visited in this iteration.
                if (last_iter_searched[next_i][next_j] < search_count){
                    // Mark it as visited and reset the value of g.
                    g[next_i][next_j] = INF;
                    last_iter_searched[next_i][next_j] = search_count;
                }
                int next_g = cur.g + 1;
                int next_h = get_h(next_i, next_j, goal_i, goal_j);
                // It the solution improves the best know solution
                if (g[next_i][next_j] > next_g){

                    // Update valuw of best know solution.
                    g[next_i][next_j] = next_g;

                    // Udate tree.
                    tree[next_i][next_j] = make_pair(cur.i, cur.j);
                    // Add cell to open queue.
                    cell next = cell(next_i, next_j, next_g, next_h);
                    open.push(next);
                    printf("  Pushing: "); next.print();
                }
            }
        }
    }
}

void explore_neighbors(int i, int j){
    assert(explored_grid[i][j] != UNEXPLORED);

    int di[] = {+1, -1,  0,  0};
    int dj[] = { 0,  0, +1, -1};
    for (int k = 0; k < 4; ++k){
        int next_i = i + di[k];
        int next_j = j + dj[k];
        if (inside(next_i, next_j)){
            explored_grid[next_i][next_j] = actual_grid[next_i][next_j];
            cout << (explored_grid[next_i][next_j] == BLOCKED) << endl;
        }
    }
}

// Get the path from the start cell to cell (goal_i, goal_j)
vector<pair<int, int> > get_path(int goal_i, int goal_j){
    vector<pair<int, int> > path;

    int cur_i = goal_i, cur_j = goal_j;
    while(cur_i != -1){ // Iterate until the current cell has no parent.
        path.push_back(make_pair(cur_i, cur_j));

        int next_i = tree[cur_i][cur_j].first;
        int next_j = tree[cur_i][cur_j].second;
        cur_i = next_i; cur_j = next_j;
    }
    reverse(path.begin(), path.end());
    return path;
}

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
    int x0, y0, x1, y1;
    init_variables("test_input/test_00.in", x0, y0, x1, y1);

    int search_count = 1;

    g[x0][y0] = 0;

    explored_grid[x0][y0] = actual_grid[x0][y0];
    last_iter_searched[x0][y0] = last_iter_searched[x1][y1] = search_count;

    explore_neighbors(x0, y0);

    PriorityQueue open = PriorityQueue(cmp_smaller_g);
    compute_path(open, x0, y0, x1, y1, search_count);

    vector<pair<int, int> > path = get_path(x1, y1);
    for (int i = 0; i < path.size(); ++i) printf("(%d, %d) ", path[i].first, path[i].second);
    printf("\n");


    return 0;
}
