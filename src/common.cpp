#include "common.h"

#include <cstdio>
#include <cwchar>
#include <cstdarg>
#include <string>
#include <mutex>
#include <cassert>

using namespace std;


namespace count_lines {

void FORMAT_ATTR(printf, 4, 5)
log(const std::string &src_path,
    int                line_num,
    std::ostream      &out,
    const char        *fmt,
    ...)
{
    static mutex log_mutex;
    char buff[LOG_MAX_LINE_LEN];
    int n = 0;
    size_t last_slash;
    const char *base_name;
    va_list args;

    // Only print base file name.
    last_slash = src_path.find_last_of("/\\");
    if (last_slash == string::npos) {
        base_name = src_path.c_str();
    } else {
        assert(last_slash < src_path.size());
        base_name = src_path.c_str() + last_slash + 1;
    }

    va_start(args, fmt);
    try {
        n += snprintf(buff + n, LOG_MAX_LINE_LEN - n, "%s:%d: ",
                      base_name, line_num);
        n += vsnprintf(buff + n, LOG_MAX_LINE_LEN - n, fmt, args);
    } catch(...) {
        va_end(args);
        throw(runtime_error("Exception caught during sprintf()"));
    }
    va_end(args);

    {
        lock_guard<mutex> guard(log_mutex);

        out << buff << "\n";
    }
}

}
