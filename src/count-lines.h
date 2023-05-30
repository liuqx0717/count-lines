#pragma once

#include "common.h"
#include <istream>
#include <ostream>

#ifndef NUM_SPACES
#   define NUM_SPACES 2
#endif


namespace count_lines {

struct config {
    bool sum_results;
    bool sort_count;
    bool reverse;
};


void
count_lines(const config &cfg,
            std::istream &in,
            std::ostream &out,
            std::ostream &err);

}
