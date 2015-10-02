#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include "cell_priority_queue.h"

using namespace std;

// // Constructor using the comparator function as a parameter.
PriorityQueue::PriorityQueue(function<bool (cell, cell)> comparator){
    size = 0;
    comp = comparator;
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

// Push the given cell to the queue.
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

// Pop the smallest element from the queue and return it.
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

        // If the parent is greater than its smallest child, swap and recurr.
        // Otherwise stop because the tree is balanced.
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

// True iff cell a is less than cell b using the compare function of the class.
bool PriorityQueue::compare(cell a, cell b){
    return comp(a, b);
}

bool PriorityQueue::empty(){
    assert(size >= 0);
    return size == 0;
}
