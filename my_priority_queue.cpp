#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

// The structure of each of the cells used in the priority queue.
struct cell{
    // The position of the cell in the grid.
    int i, j;
    // The assigned cost g and heuristic value h of the grid.
    int g, h;

    // Constructor for the cell class.
    cell(int i, int j, int g, int h): i(i), j(j), g(g), h(h) {}

    // Empty contructor to be able to create arrays of cells.
    cell(){}

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

        int parent(int);
        int left_child(int);
        int right_child(int);

        bool compare(cell, cell);

        void push(cell);
        cell pop();
        cell top();

    private:
        // The maximum size of the queue.
        static const int MAX_SIZE = 315 * 315;
        // Returns true iff cell a is less than cell b.
        function<bool (cell, cell)> comp;
        // The binary tree that represents the queue.
        cell tree[MAX_SIZE];
        // The number of elements in the queue.
        int size;
};

// Constructor using the comparator function as a parameter.
PriorityQueue::PriorityQueue(function<bool (cell, cell)> comparator){
    comp = comparator;
    size = 0;
}

// Get the index of the parent of the node at the given index.
// Note: parent(0) = 0 for all others, parent(i) < i.
int PriorityQueue::parent(int index){
    return (index - 1) / 2;
}

// Get the index of the left child of the node at the given index.
int PriorityQueue::left_child(int index){
    return 2 * index + 1;
}

// Get the index of the right child of the node at the given index.
int PriorityQueue::right_child(int index){
    return 2* index + 2;
}

void PriorityQueue::push(cell c){
    assert(size < MAX_SIZE);
    // Insert in the last position and increment the size.
    int i = size++;
    tree[i] = c;

    // While child less than parent.
    while (i > 0 and comp(tree[i], tree[parent(i)]) ){
        swap(tree[i], tree[parent(i)]);
        i = parent(i);
    }
}

cell PriorityQueue::pop(){
    assert(size > 0);

    cell popped_cell = tree[0];

    // Put the last cell in the root.
    tree[0] = tree[--size];

    int i = 0;
    bool swapped;
    do {
        swapped = false;

        int left = left_child(i);
        int right = right_child(i);

        int min_index = i;

        // If left child is less than best seen cell.
        if (left < size and comp(tree[left], tree[min_index])) min_index = left;

        // If right child is less than best seen cell.
        if (right < size and comp(tree[right], tree[min_index])) min_index = right;

        if (min_index != i){
            swapped = true;
            swap(tree[i], tree[min_index]);
            i = min_index;
        }
    } while(swapped);

    for (int j = 0; j < size; ++j){
        printf("%d ", tree[j].f());
    }
    printf("\n");

    return popped_cell;
}

cell PriorityQueue::top(){
    assert(size > 0);
    return tree[0];
}

// True iff cell a is less than cell b using the compare function of the class.
bool PriorityQueue::compare(cell a, cell b){
    return comp(a, b);
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
    int a = 0;
    int b = a++;
    cout << a << " " << b << endl;
    int c = ++a;
    cout << a << " " << c << endl;

    PriorityQueue q1 = PriorityQueue(cmp_smaller_g);
    PriorityQueue q2 = PriorityQueue(cmp_larger_g);

    cell c1 = cell(0, 1, -3, 0);
    cell c1_1 = cell(0, 1, 5, 0);
    cell c2 = cell(0, 1, 8, 0);
    cell c3 = cell(0, 1, -5, 0);
    cell c4 = cell(0, 1, -4, 0);
    cell c5 = cell(0, 1, -10, 0);

    q1.push(c1);
    q1.push(c1_1);
    q1.push(c2);
    q1.push(c3);
    q1.push(c4);
    q1.push(c5);

    q1.pop().print();
    q1.pop().print();
    q1.pop().print();
    q1.pop().print();
    q1.pop().print();
    q1.pop().print();


    // cout << q1.compare(c1, c2) << " " << q2.compare(c1, c2) << endl;
    // cout << q1.compare(c2, c1) << " " << q2.compare(c2, c1) << endl;
    // cout << q1.compare(c1, c3) << " " << q2.compare(c1, c3) << endl;
    // cout << q1.compare(c3, c4) << " " << q2.compare(c3, c4) << endl;

    // printf("Parent 0 = %d and must be %d\n", q1.parent(0), -1);
    // printf("Parent 1 = %d and must be %d\n", q1.parent(1), 0);
    // printf("Parent 2 = %d and must be %d\n", q1.parent(2), 0);
    // printf("Parent 3 = %d and must be %d\n", q1.parent(3), 1);
    // printf("Parent 4 = %d and must be %d\n", q1.parent(4), 1);
    // printf("Parent 4 = %d and must be %d\n", q1.parent(5), 2);
    // printf("Parent 4 = %d and must be %d\n", q1.parent(6), 2);

    // printf("Left child 0 = %d and must be %d\n", q1.left_child(0), 1);
    // printf("Left child 1 = %d and must be %d\n", q1.left_child(1), 3);
    // printf("Left child 2 = %d and must be %d\n", q1.left_child(2), 5);
    // printf("Left child 3 = %d and must be %d\n", q1.left_child(3), 7);
    // printf("Left child 4 = %d and must be %d\n", q1.left_child(4), 9);
    // printf("Left child 4 = %d and must be %d\n", q1.left_child(5), 11);
    // printf("Left child 4 = %d and must be %d\n", q1.left_child(6), 13);

    // printf("Right child 0 = %d and must be %d\n", q1.right_child(0), 2);
    // printf("Right child 1 = %d and must be %d\n", q1.right_child(1), 4);
    // printf("Right child 2 = %d and must be %d\n", q1.right_child(2), 6);
    // printf("Right child 3 = %d and must be %d\n", q1.right_child(3), 8);
    // printf("Right child 4 = %d and must be %d\n", q1.right_child(4), 10);
    // printf("Right child 4 = %d and must be %d\n", q1.right_child(5), 12);
    // printf("Right child 4 = %d and must be %d\n", q1.right_child(6), 14);


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
