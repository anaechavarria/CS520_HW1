#include <string>
using namespace std;

#ifndef __X_H_INCLUDED__   // if x.h hasn't been included yet...
#define __X_H_INCLUDED__   //   #define this so the compiler knows it has been included

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


// The class that represents the priority queue.
class PriorityQueue
{
   public:
        // Constructor using the less than compare function.
        PriorityQueue(function<bool (cell, cell)> comparator);
        // PriorityQueue(string);
        // Push the given cell to the queue.
        void push(cell);
        // Pop the smallest element from the queue and return it.
        cell pop();

    private:
        // The maximum size of the queue.
        static const int MAX_SIZE = 315 * 315;
        // Returns true iff cell a is less than cell b.
        function<bool (cell, cell)> comp;
        // The binary tree that represents the queue.
        cell tree[MAX_SIZE];
        // The number of elements in the queue.
        int size;

        // Get the index of the parent of the node at the given index.
        int parent(int);
        // Get the index of the left child of the node at the given index.
        int left_child(int);
        // Get the index of the right child of the node at the given index.
        int right_child(int);
        // Calls comp on the input and returns whatever it returns.
        bool compare(cell, cell);
};


// bool cmp_larger_g(const cell, const cell);
// bool cmp_smaller_g(const cell, const cell);

#endif


