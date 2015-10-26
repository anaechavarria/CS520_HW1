#include <iostream>

#ifndef __A_STAR_SEARCH_H_INCLUDED__ // If this file hasn't been included yet.
#define __A_STAR_SEARCH_H_INCLUDED__ // Let the compiler know it has been included.

#include "cell_priority_queue.h"

// Return true iff the goal cell can be reached from the start cell.
// grid_path: is the path of the input grid (and start and target cells).
// cmp: is the cell comparison function to use.
// forward: is true if the search is forward (from agent to goal) and false if
//          it is backward (from goal to agent).
// adaptive: is true iff the search is an adaptive A* search.
bool run_search(string grid_path, function<bool (cell, cell)> cmp,
                bool forward, bool adaptive);

#endif
