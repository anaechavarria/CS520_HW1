#include <iostream>

using namespace std;

// The structure of each of the cells used in the priority queue.
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


class PriorityQueue
{
   public:
        PriorityQueue(function<bool (cell, cell)> comparator);
        bool compare(cell, cell);

    private:
        // Returns true iff cell a > cell b.
        function<bool (cell, cell)> comp;
};

// Member functions definitions including constructor
PriorityQueue::PriorityQueue(function<bool (cell, cell)> comparator){
    comp = comparator;
    cout << "Created" << endl;
}


bool PriorityQueue::compare(cell a, cell b){
    return comp(a, b);
}

// Test comparator function for running. TODO Delete.
bool cmp(const cell &a, const cell &b){
    if (a.f() == b.f()) return a.h < b.h;
    return a.f() > b.f();
}


int main(){

    PriorityQueue q = PriorityQueue(cmp);

    cell c1 = cell(0, 1, 1, 1);
    cell c2 = cell(0, 1, 1, 1);
    cell c3 = cell(0, 1, -1, 1);
    cell c4 = cell(0, 1, -1, -1);

    cout << q.compare(c1, c2) << endl;
    cout << q.compare(c2, c1) << endl;
    cout << q.compare(c1, c3) << endl;
    cout << q.compare(c3, c4) << endl;

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
