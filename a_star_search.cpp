#include <iostream>
#include <queue>
#include <vector>
#include <set>
using namespace std;

struct cell{
    int i, j;
    int g, h;

    // Constructor for the cell class.
    cell(int i, int j, int g, int h): i(i), j(j), g(g), h(h) {}

    int f() const { return g + h; }

    void print(){
        printf("i = %d, j = %d, g = %d, h = %d,  f = %d\n", i, j, g, h, f());
    }
};

const int MAXN = 105;
const int INF = 10000000;

enum { UNBLOCKED = 0, BLOCKED = 1, UNEXPLORED = -1};
int explored_grid[MAXN][MAXN]; // The status of the grid explored so far.
int actual_grid[MAXN][MAXN]; // The actual status of the grid.

int g[MAXN][MAXN]; // The lowest known cost for the given cell.
int search[MAXN][MAXN]; // The last iteration where the cell's cost was updated.

set<pair<int, int> >; // The set of the cells that have been visited.
pair <int, int> tree[MAXN][MAXN]; // The cell where we came from in the search.

int n; // The size of the grid.

// Test comparator function for running. TODO Delete.
bool cmp(const cell &a, const cell &b){
    return a.f() > b.f();
}

// Returns the manhattan distance from cell (x0, y0) to cell (x1, y1).
int get_h(int x0, int y0, int x1, int y1){
    return abs(x1 - x0) + abs(y1 - y0);
}

// Check if cell i, j is (presumably) unblocked. Indices can be out of bounds.
bool unblocked(int i, int j){
    // If the indices are out of bounds.
    if (i < 0 or j < 0 or i >= n or j >= n) return false;
    // If the cell is blocked.
    if (explored_grid[i][j] == BLOCKED) return false;
    // The indices are in the bounds and the cell is either free or unexpolred.
    return true;
}

// TODO Implement.
void reset_variables(){
    // Reset variables. Reset tree. Reset array g to INF. Search matrix. closed.
}

// Compute the shortest path from:
// cell (start_i, start_j) to cell (goal_i, goal_j)
// Open is the pointer priority queue used for the search. It must be empty when
// calling the function but is not neccesarily empty when returning.
// Search count indicates the iteration number. Used to avoid refreshing the g
// matrix for every cell.
void compute_path(priority_queue<cell> &open,
    const int start_i, const int start_j, const int goal_i, const int goal_j,
    const int search_count){

    assert open.empty();

    int start_h = get_h(start_i, start_j, goal_i, goal_j);
    start_cell = cell(start_i, start_j, 0, start_h);

    open.push(start_cell);

    while (!open.empty()){
        // Extract min from open.
        cell cur = open.top();
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
                if (search[next_i][next_j] < search_count){
                    // Mark it as visited and reset the value of g.
                    g[next_i][next_j] = INF;
                    search[next_i][next_j] = search_count;
                }
                next_g = cur.g + 1;
                next_h = get_h(next_i, next_j, goal_i, goal_j);
                // It the solution improves the best know solution
                if (g[next_i][next_j] > next_g){
                    // Update valuw of best know solution.
                    g[next_i][next_j] = next_g;
                    // Udate tree.
                    tree[next_i][next_j] = make_pair(cur.i, cur.j);
                    // Add cell to open queue.
                    cell next = cell(next_i, next_j, next_g, next_h);
                    open.push(next);
                }
            }
        }
    }
}

// The comparator is the greater than comparator between cells.
// Start position, Finish position, Tie breaking function, is it Adaptive,
// Grid size
void run(function<bool (cell, cell)> comparator, int n){
    // Heap for open cells.
    priority_queue<cell, vector<cell>, decltype(&comparator)> open(&comparator);
    // Vector
    set<cell> closed;

    open.push(cell(0, 1));
    cell c = open.top(); c.print();
    open.push(cell(-1, 4));
    c = open.top(); c.print();

    open.push(cell(81, 4));
    c = open.top(); c.print();
    open.push(cell(-81, 4));
    c = open.top(); c.print();
    open.pop();
    c = open.top(); c.print();

}




int main(){

    return 0;
}
