#ifdef NDEBUG
#   undef NDEBUG
#endif

#include "common.h"
#include "count-lines.h"

#include <sstream>
#include <cassert>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;
using namespace count_lines;


static void
replace_pipes_with_spaces(string &str)
{
    for (size_t pos = 0;
         (pos = str.find('|')) != string::npos;
         pos += NUM_SPACES) {
        str.replace(pos, 1, NUM_SPACES, ' ');
    }
}


static void
test_log()
{
    ostringstream result;
    char expected[1024];
    int line1, line2;
    const char *expected_fmt =
        "unit-test.cpp:%d: Test log\n"
        "unit-test.cpp:%d: Test log 123\n";

    line1 = __LINE__; LOG(result, "Test log");
    line2 = __LINE__; LOG(result, "Test log %d", 123);

    snprintf(expected, sizeof(expected), expected_fmt, line1, line2);

    cout << __func__ << " result: " << result.str() << "\n";
    assert(result.str().compare(expected) == 0);
}


static void
verify_count_lines(const char                                   *test_name,
                   const config                                 &cfg,
                   const vector<pair<const char*, const char*>> &cases)
{

    for (const auto &ca : cases) {
        string in_str(ca.first);
        string expected_str(ca.second);
        istringstream in;
        ostringstream out;

        replace_pipes_with_spaces(in_str);
        replace_pipes_with_spaces(expected_str);
        in.str(in_str);

        count_lines::count_lines(cfg, in, out, cerr);

        cout << test_name << " result: " << out.str() << "\n";
        assert(out.str().compare(expected_str) == 0);
    }
}


static void
test_count_lines_default()
{
    config cfg = {};
    vector<pair<const char*, const char*>> cases {
        {
            ""
            ,
            ""
        },
        {
            "abc"
            ,
            "1|abc\n"
        },
        {
            "\n"
            ,
            ""
        },
        {
            "b\n"
            "a"
            ,
            "1|a\n"
            "1|b\n"
        },
        {
            "\n"
            "bcd\n"
            " a \n"
            " \n"
            " \n"
            "\n"
            "\n"
            "a\n"
            "c\n"
            "c\n"
            "c\n"
            "c\n"
            "c\n"
            "c\n"
            "c\n"
            "c\n"
            "c\n"
            "c\n"
            "bcd\n"
            "\n"
            ,
            " 2| \n"
            " 1| a \n"
            " 1|a\n"
            " 2|bcd\n"
            "10|c\n"
        },
    };

    verify_count_lines(__func__, cfg, cases);
}


static void
test_count_lines_sort_count()
{
    config cfg = {};
    const char *input =
        "c\n"
        "a\n"
        "b\n"
        "cc\n"
        "aa\n"
        "cc\n"
        "aa\n"
        "bbb\n"
        "bbb\n"
        "bbb\n"
        "bbb\n"
        "bbb\n"
        "bbb\n"
        "bbb\n"
        "bbb\n"
        "bbb\n"
        "bbb\n";
    vector<pair<const char*, const char*>> cases {
        {
            input
            ,
            " 1|a\n"
            " 1|b\n"
            " 1|c\n"
            " 2|aa\n"
            " 2|cc\n"
            "10|bbb\n"
        },
    };
    vector<pair<const char*, const char*>> cases_reverse {
        {
            input
            ,
            "10|bbb\n"
            " 2|aa\n"
            " 2|cc\n"
            " 1|a\n"
            " 1|b\n"
            " 1|c\n"
        },
    };

    cfg.sort_count = true;
    verify_count_lines(__func__, cfg, cases);
    cfg.reverse = true;
    verify_count_lines(__func__, cfg, cases_reverse);
}


static void
test_count_lines_sort_revert()
{
    config cfg = {};
    vector<pair<const char*, const char*>> cases {
        {
            "c\n"
            "c\n"
            "c\n"
            "cc\n"
            "ccc\n"
            "ccc\n"
            ,
            "2|ccc\n"
            "1|cc\n"
            "3|c\n"
        },
    };

    cfg.reverse = true;
    verify_count_lines(__func__, cfg, cases);
}


static void
test_count_lines_sort_sum()
{
    config cfg = {};
    vector<pair<const char*, const char*>> cases {
        {
            "2|ccc\n"
            "1|cc\n"
            "3|c\n"
            "2|ccc\n"
            "1|cc\n"
            "3|c\n"
            ,
            "6|c\n"
            "2|cc\n"
            "4|ccc\n"
        },
        {
            "100|a\n"
            "  1|a\n"
            "  1| a\n"
            ,
            "  1| a\n"
            "101|a\n"
        },
    };

    cfg.sum_results = true;
    verify_count_lines(__func__, cfg, cases);
}


int
main(int   argc,
     char *argv[])
{
    test_log();
    test_count_lines_default();
    test_count_lines_sort_count();
    test_count_lines_sort_revert();
    test_count_lines_sort_sum();

    return 0;
}