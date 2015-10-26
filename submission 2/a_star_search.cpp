#include <iostream>
#include <cstdio>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include <iterator>
#include "a_star_search.h"
using namespace std;

#define debug false

const int MAXN = 105;
const int INF = 1000000000;

// The possible values for the cells in the grid.
enum { UNBLOCKED = 0, BLOCKED = 1, UNEXPLORED = -1};

int explored_grid[MAXN][MAXN]; // The status of the grid explored so far.
int actual_grid[MAXN][MAXN]; // The actual status of the grid.

int g[MAXN][MAXN]; // The lowest known cost for the given cell.
int h[MAXN][MAXN]; // The value of the heuristic the given cell.
int last_iter_searched[MAXN][MAXN]; // Last iter where the cost was updated.

set<pair<int, int> > closed; // The set of the cells that have been expanded.
pair <int, int> tree[MAXN][MAXN]; // The cell where we came from in the search.

int n; // The size of the grid.


// Statistics.
// The total number of cells inserted in the closed list among all searches.
int num_of_cells_expanded;
// The number of searches performed before finishing.
int num_of_searches;
// The total number of cells that the agent had to move.
int num_of_moves;


// Returns the manhattan distance from cell (i0, j0) to cell (i1, j1).
int manhattan_distance(int i0, int j0, int i1, int j1){
    return abs(i1 - i0) + abs(j1 - j0);
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

// Loads the grid stored in the grid_path and stores it in the variable grid.
// Returns n where n x n is the size of the grid.
// The values (i0, j0) will the inicial cell to start the search.
// The values (i1, j1) will the goal cell for the search.
int load_grid(string grid_path, int &i0, int &j0, int &i1, int &j1){
    freopen(grid_path.c_str(), "r", stdin);

    int grid_size;
    cin >> grid_size;
    cin >> i0 >> j0 >> i1 >> j1;

    for (int i = 0; i < grid_size; ++i){
        string line; cin >> line;
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
void init_variables(string grid_path, int &i0, int &j0, int &i1, int &j1){
    // Load grid and get grid size.
    n = load_grid(grid_path, i0, j0, i1, j1);

    for (int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            explored_grid[i][j] = UNEXPLORED; // None of the cells are explored.
            last_iter_searched[i][j] = 0; // No cell has been searched yet.
            h[i][j] = manhattan_distance(i, j, i1, j1); // The value of h.
        }
    }
    closed.clear();

    // Reset the statistics.
    num_of_cells_expanded = num_of_searches = num_of_moves = 0;

}

// Reset the variables in between searches.
void reset_variables(int start_i, int start_j){
    closed.clear();
    for (int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            g[i][j] = INF;
            tree[i][j] = make_pair(-1, -1);
        }
    }
    g[start_i][start_j] = 0;
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

    int start_h = h[start_i][start_j];
    cell start_cell = cell(start_i, start_j, 0, start_h);

    open.push(start_cell);

    while (!open.empty()){
        // Extract min from open.
        cell cur = open.pop();

        if (debug){ printf("Popping: "); cur.print(); }

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
                int next_h = h[next_i][next_j];
                // It the solution improves the best know solution
                if (g[next_i][next_j] > next_g){
                    // Update value of best know solution.
                    g[next_i][next_j] = next_g;
                    // Update tree.
                    tree[next_i][next_j] = make_pair(cur.i, cur.j);
                    // Add cell to open queue.
                    cell next = cell(next_i, next_j, next_g, next_h);
                    open.push(next);
                    if (debug){ printf("  Pushing: "); next.print(); }
                }
            }
        }
    }
}

// Update the values of h to be h(s) = g(goal) - g(s) if s was a closed cell.
// (i1, j1) are the coordinates of the goal cell.
void update_h(int i1, int j1){
    set<pair<int, int> >::iterator it;
    for (it = closed.begin(); it != closed.end(); ++it){
        int i = it->first;
        int j = it->second;
        h[i][j] = g[i1][j1] - g[i][j];
    }
}


// Explore the neighbors of the cell (i, j). It updates the values of the
// neighbors in the explored grid to match the values in the actual grid.
void explore_neighbors(int i, int j){
    assert(explored_grid[i][j] != UNEXPLORED);

    int di[] = {+1, -1,  0,  0};
    int dj[] = { 0,  0, +1, -1};
    for (int k = 0; k < 4; ++k){
        int next_i = i + di[k];
        int next_j = j + dj[k];
        if (inside(next_i, next_j)){
            explored_grid[next_i][next_j] = actual_grid[next_i][next_j];
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

// Walk along the given path of cells and explore the grid as you go.
// If the path is blocked, stop the walk.
// Return the last cell in the path that could be visited.
pair<int, int> walk_path(vector< pair<int,int> > path){
    for (int k = 0; k < path.size(); ++k){
        int i = path[k].first;
        int j = path[k].second;

        if (actual_grid[i][j] == BLOCKED){
            // We could at least visit the first cell in the path.
            assert(k > 0);
            assert(explored_grid[i][j] = BLOCKED);
            return path[k - 1];
        }
        explored_grid[i][j] = actual_grid[i][j];
        explore_neighbors(i, j);

        num_of_moves++;
    }
    return path.back();
}

// Return true iff the goal cell can be reached from the start cell.
// grid_path: is the path of the input grid (and start and target cells).
// cmp: is the cell comparison function to use.
// forward: is true if the search is forward (from agent to goal) and false if
//          it is backward (from goal to agent).
// adaptive: is true iff the search is an adaptive A* search.
bool run_search(string grid_path, function<bool (cell, cell)> cmp,
                bool forward, bool adaptive){
    // Load the grid.
    int i0, j0, i1, j1;
    if (forward) init_variables(grid_path, i0, j0, i1, j1);
    else init_variables(grid_path, i1, j1, i0, j0);

    // The agent know the cell it is in and its neighboring cells.
    explored_grid[i0][j0] = actual_grid[i0][j0];
    explore_neighbors(i0, j0);

    // Begin the searches.
    for (int search_count = 1; (i0 != i1) or (j0 != j1); search_count++){
        // Reset variables before search.
        reset_variables(i0, j0);
        last_iter_searched[i0][j0] = last_iter_searched[i1][j1] = search_count;

        // Run the search
        PriorityQueue open = PriorityQueue(cmp);
        assert(open.empty() and closed.empty());
        compute_path(open, i0, j0, i1, j1, search_count);

        // Update statistics.
        num_of_cells_expanded += closed.size();
        num_of_searches++;

        // Could never get to the goal cell.
        if (tree[i1][j1] == make_pair(-1, -1)) break;

        // Update the heuristic values.
        if (adaptive) update_h(i1, j1);

        // Walk the path until blocked or reaching goal.
        vector<pair<int, int> > path = get_path(i1, j1);
        pair<int, int> last_walked_cell = walk_path(path);
        i0 = last_walked_cell.first; j0 = last_walked_cell.second;

        if (debug){
            printf("The path is ");
            for (int i = 0; i < path.size(); ++i){
                printf("(%d, %d) ", path[i].first, path[i].second);
            }
            printf("\n");
            printf("Walked until (%d, %d)\n", i0, j0);
        }
    }
    bool target_reached = (i0 == i1) and (j0 == j1);

    // Print the search statistics.
    printf("target_reached = %d, num_of_cells_expanded = %7d, num_of_searches = %4d, num_of_moves = %4d",
        target_reached, num_of_cells_expanded, num_of_searches, num_of_moves);

    // Return if the goal cell could be reached.
    return (target_reached);

}
