#include "count-lines.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cassert>
#include <exception>
#include <functional>

using namespace std;


namespace count_lines {

/*
 * Parse the result of a previous run (for sum_results mode).
 * The input string will be replaced with the original string without
 * the count number. The count number will be returned.
 */

static size_t
parse_result(std::string  &line,
             std::ostream &err)
{
    static regex re(
        R"_(^ *(\d+) {)_" EXPAND_TO_STR(NUM_SPACES) R"_(}(.*)$)_",
        regex_constants::ECMAScript | regex_constants::optimize);
    match_results<string::const_iterator> matches;
    size_t delta;

    if (!regex_match(line, matches, re)) {
        LOG(err, "Failed to parse: %s", line.c_str());
        throw(runtime_error("Invalid input line"));
    }

    delta = stoul(matches[1].str());
    if (delta == 0) {
        LOG(err, "Failed to parse: %s", line.c_str());
        throw(runtime_error("Invalid input line"));
    }

    line = matches[2].str();
    return delta;
}


static bool
comp(bool                        by_count,
     bool                        reverse,
     const pair<string, size_t> &l,
     const pair<string, size_t> &r)
{
    if (by_count) {
        /*
         * For the lines with the same count, always sort them in
         * ascending order.
         */
        return (l.second < r.second && !reverse) ||
               (l.second > r.second && reverse) ||
               (l.second == r.second && l.first < r.first);
    } else {
        return reverse != (l < r);
    }
}


void
count_lines(const config &cfg,
            std::istream &in,
            std::ostream &out,
            std::ostream &err)
{
    unordered_map<string, size_t> line_count;
    vector<pair<string, size_t>> result_vec;
    string line;
    size_t max_count = 0;
    int max_count_width = 0;
    char out_prefix[64];

    while (getline(in, line)) {
        size_t delta = 1;
        size_t count;

        if (line.empty()) {
            continue;
        }

        if (cfg.sum_results) {
            delta = parse_result(line, err);
        }
        assert(delta > 0);

        count = line_count[line] += delta;
        max_count = max(max_count, count);
    }

    if (line_count.empty()) {
        return;
    }

    result_vec.reserve(line_count.size());
    result_vec.assign(line_count.begin(), line_count.end());
    sort(result_vec.begin(), result_vec.end(),
         [&cfg] (const auto &l, const auto &r) {
            return comp(cfg.sort_count, cfg.reverse, l, r);
         });

    for (size_t n = max_count; n > 0; n /= 10) {
        max_count_width++;
    }
    assert(max_count_width > 0);

    for (const auto &res : result_vec) {
        snprintf(out_prefix, sizeof(out_prefix), "%*zu%*s",
                 max_count_width, res.second, NUM_SPACES, "");
        out << out_prefix << res.first << "\n";
    }
}

}
