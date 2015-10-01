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

int n;
const int INF = 10000000;

bool cmp(const cell &a, const cell &b){
    return a.f() > b.f();
}

// g[i][j] the known distance to cell i, j.
// tree[i][j] = (k, l) of cell where I came from.
// search[i][j] As described in the problem.
// closed[i][j] As described in the problem.

int get_h(int x0, int y0, int x1, int y1){
    return abs(x1 - x0) + abs(y1 - y0);
}

// Check if cell i, j is in the grid.
bool inside(int i, int j){ // TODO Make n a global variable
    if (i < 0 or j < 0 or i >= n or j >= n) return false;
    return true;
}

void reset_variables(){
    // Reset variables. Reset tree. Reset array g to INF. Search matrix. closed.
}

void compute_path(priority_queue<cell> &open, const int start_i, const int start_j,
        const int start_g, const int goal_i, const int goal_j, const int search_count){
    assert open.empty();

    int start_h = get_h(start_i, start_j, goal_i, goal_j);
    start_cell = cell(start_i, start_j, start_g, start_h);

    open.push(start_cell);

    while (!open.empty()){
        // Extract min from open.
        cell cur = open.top();
        open.pop();
        // If this solution is worse than the best solution know to the goal.
        // Break because the answer is not going to improve.
        if (cur.g() > g[goal_i][goal_i]) break;

        // If this solution doesn't improve the know soution of the given cell.
        // This is not in the pseudocode but is needed because we may have
        // several copies of the same cell in the queue.
        if (cur.f() > g[cur.i][cur.j]) continue;

        closed[cur.i][cur.j] = true;

        // Move to the neighbors of i, j
        int di[] = {+1, -1,  0,  0};
        int dj[] = { 0,  0, +1, -1};
        for (int k = 0; k < 4; ++k){
            int next_i = cur.i + di[k];
            int next_j = cur.j + dj[k];
            if (inside(next_i, next_j)){
                // If this cell has not been visited in this iteration.
                if (search[next_i][next_j] < search_count){
                    // Mark it as visited and reset the value of g.
                    search[next_i][next_j] = search_count;
                    g[next_i][next_j] = INF;
                }
                next_g = cur.g + 1;
                next_h = get_h(next_i, next_j, goal_i, goal_j);
                // It the solution improves the best know solution
                if (g[next_i][next_j] > next_g + next_h){
                    // Update valuw of best know solution.
                    g[next_i][next_j] = next_g + next_h;
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
