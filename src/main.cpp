#include "count-lines.h"

#include <iostream>
#include <string>
#include <exception>
#include <tclap/CmdLine.h>

#ifndef VERSION
#   define VERSION "0.0.0"
#endif


using namespace count_lines;
using namespace TCLAP;
using namespace std;


static void
print_exception(const exception& e,
                int              level=0)
{
    cerr << string(level, ' ') << "exception: " << e.what() << '\n';
    try {
        rethrow_if_nested(e);
    } catch(const exception& nestedException) {
        print_exception(nestedException, level + 1);
    }
}


int
main(int   argc,
     char *argv[])
{
    config cfg = {};

    try {
        CmdLine cmd("Count the occurrence of the lines using a hash table. "
                    "Read from stdin.",
                    ' ' /* delimiter */, VERSION);
        SwitchArg sort_count("c", "sort-count",
                             "Sort by count (by default sort by line content).",
                             cmd);
        SwitchArg reverse("r", "reverse", "Reverse the sorting order.", cmd);
        SwitchArg sum_results("s", "sum-results",
                              "Sum the results of previous runs.",
                              cmd);
        
        cmd.parse(argc, argv);

        cfg.sort_count = sort_count.getValue();
        cfg.reverse = reverse.getValue();
        cfg.sum_results = sum_results.getValue();

    } catch(ArgException &e) {
        cerr << "Error: " << e.error() << "for arg " << e.argId() << "\n";
    }

    try {
        count_lines::count_lines(cfg, cin, cout, cerr);
    } catch (exception &e) {
        print_exception(e);
        return 1;
    }

    return 0;
}
