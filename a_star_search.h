#include <iostream>

#ifndef __A_STAR_SEARCH_H_INCLUDED__ // If this file hasn't been included yet.
#define __A_STAR_SEARCH_H_INCLUDED__ // Let the compiler know it has been included.

#include "cell_priority_queue.h"

// Return true iff the goal cell can be reached from the start cell.
bool run_search(string grid_path, function<bool (cell, cell)> cmp);

#endif
