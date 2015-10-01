#include <iostream>
#include <queue>
#include <vector>
#include <set>
#include <string>
#include <cassert>
using namespace std;


const int MAXN = 105;
const int INF = 10000000;

// Careful, do not change values.
// Using memset to fill ints with UNEXPLORED and BLOCKED.
enum { UNBLOCKED = 0, BLOCKED = 1, UNEXPLORED = -1};

int explored_grid[MAXN][MAXN]; // The status of the grid explored so far.
int actual_grid[MAXN][MAXN]; // The actual status of the grid.

int g[MAXN][MAXN]; // The lowest known cost for the given cell.
int last_iter_searched[MAXN][MAXN]; // Last iter where the cost was updated.

set<pair<int, int> > closed; // The set of the cells that have been visited.
pair <int, int> tree[MAXN][MAXN]; // The cell where we came from in the search.

int n; // The size of the grid.


// The structure of each of the cells used in the priority_queue for the search.
struct cell{
    // The position of the cell in the grid.
    int i, j;
    // The assigned cost g and heuristic value h of the grid.
    int g, h;

    // Constructor for the cell class.
    cell(int i, int j, int g, int h): i(i), j(j), g(g), h(h) {}

    // Returns the value f = g + h of the grid.
    int f() const { return g + h; }

    // Prints the value of the grid.
    void print(){
        printf("cell(%d, %d), g = %d, h = %d,  f = %d\n", i, j, g, h, f());
    }
};

// Loads the grid stored in the grid_path and stores it in the variable grid.
// Returns n where n x n is the size of the grid.
int load_grid(string grid_path){
    // TODO Implement. Current is for testing only.
    actual_grid[0][0] = actual_grid[0][1] = actual_grid[0][2] = actual_grid[0][3] = actual_grid[0][4] = 0;     // = {0, 0, 0, 0, 0};
    actual_grid[1][0] = actual_grid[1][1] = actual_grid[1][3] = actual_grid[1][4] = 0;  actual_grid[1][2] = 1; // = {0, 0, 1, 0, 0};
    actual_grid[2][0] = actual_grid[2][1] = actual_grid[2][4] = 0;  actual_grid[2][2] = actual_grid[2][3] = 1; // = {0, 0, 1, 1, 0};
    actual_grid[3][0] = actual_grid[3][1] = actual_grid[3][4] = 0;  actual_grid[3][2] = actual_grid[3][3] = 1; // = {0, 0, 1, 1, 0};
    actual_grid[4][0] = actual_grid[4][1] = actual_grid[4][2] = actual_grid[4][4] = 0;  actual_grid[4][3] = 1; // = {0, 0, 0, 1, 0};
    return 5;
}

// Set the values of all the global variables before starting the search.
void init_variables(string grid_path){
    // Load grid and get grid size.
    n = load_grid(grid_path);
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


// Test comparator function for running. TODO Delete.
bool cmp(const cell &a, const cell &b){
    if (a.f() == b.f()) return a.h < b.h;
    return a.f() > b.f();
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
void compute_path(const int start_i, const int start_j,
                  const int goal_i, const int goal_j, const int search_count){

    priority_queue<cell, vector<cell>, decltype(&cmp)> open(&cmp);

    assert(open.empty());

    int start_h = get_h(start_i, start_j, goal_i, goal_j);
    cell start_cell = cell(start_i, start_j, 0, start_h);

    open.push(start_cell);

    while (!open.empty()){
        // Extract min from open.
        cell cur = open.top();

        printf("Popping: "); cur.print();

        open.pop();

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
            // printf("  Reseting (%d, %d)\n", next_i, next_j);
    //      printf("  last_iter_searched = %d\n", last_iter_searched[next_i][next_j]);
                    last_iter_searched[next_i][next_j] = search_count;
            // printf("  last_iter_searched = %d\n", last_iter_searched[next_i][next_j]);
                }
                int next_g = cur.g + 1;
                int next_h = get_h(next_i, next_j, goal_i, goal_j);
                // It the solution improves the best know solution
                if (g[next_i][next_j] > next_g){

            //printf("  g[%d][%d] = %d\n", next_i, next_j, g[next_i][next_j]);
                    // Update valuw of best know solution.
                    g[next_i][next_j] = next_g;

            //printf("  g[%d][%d] = %d\n", next_i, next_j, g[next_i][next_j]);
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


// The comparator is the greater than comparator between cells.
// Start position, Finish position, Tie breaking function, is it Adaptive,
// Grid size
void runr( function<bool (cell, cell)> comparator, int n){
    //function<bool (cell, cell)> comparator
    // bool (*comparator)(*cell, *cell)

    // // Heap for open cells.
    //priority_queue<cell, vector<cell>, decltype(&comparator)> open(&comparator);

    // // Vector
    // set<cell> closed;

    // priority_queue<cell, vector<cell>, decltype(&cmp)> open(&cmp);

    // open.push(cell(0, 1, 1, 1));
    // cell c = open.top(); c.print();
    // open.push(cell(7, 7, -1, 4));
    // c = open.top(); c.print();

    // open.push(cell(7, 7, 81, 4));
    // c = open.top(); c.print();
    // open.push(cell(-100, 100, -81, 4));
    // c = open.top(); c.print();
    // open.pop();
    // c = open.top(); c.print();

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


int main(){
    init_variables("asdf");
    int x0 = 4, y0 = 2;
    int x1 = 4, y1 = 4;
    int search_count = 1;

    g[x0][y0] = 0;

    explored_grid[x0][y0] = actual_grid[x0][y0];
    last_iter_searched[x0][y0] = last_iter_searched[x1][y1] = search_count;

    explore_neighbors(x0, y0);

    compute_path(x0, y0, x1, y1, search_count);

    int cur_i = x1, cur_j = y1;
    while(cur_i != -1){
        int next_i = tree[cur_i][cur_j].first;
        int next_j = tree[cur_i][cur_j].second;
        printf("cur = (%d, %d)\n", cur_i, cur_j);
        cur_i = next_i; cur_j = next_j;
    }


    return 0;
}
