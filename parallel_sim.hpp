#pragma once
#include "simulation.hpp"


class Parallel_Simulation {
    std::vector<jthread> workers;
    // TODO: break up the initial field on n_workers rectangles of size N / n_workers X (M+1) / n_workers
    // Critical section: adjacent fields on the edges of each rectangle.
    // Possible solution: Create halo of this fields to prevent from race conditions on edges of rectangles
    // Downsides: It may be wrong if 2 thread start traversing its rectangle with unupdated edges of rectangle, while 1 thread updates it's work area.
    // If we store the pointers to field values instead of values we could traverse in all rectangles, wait for all thread to finish work,
    // start counting in each thread in order(1 thread then second one) it would be simple parrallel propagate_flow function
};
