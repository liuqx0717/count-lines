#pragma once

#include <string>
#include <ostream>

#ifndef LOG_MAX_LINE_LEN
#   define LOG_MAX_LINE_LEN 2048
#endif

#ifdef _MSC_VER
#   define FORMAT_ATTR(...)
#else
#   define FORMAT_ATTR(...) __attribute__((format(__VA_ARGS__)))
#endif

#define TO_STR(s) #s
#define EXPAND_TO_STR(s) TO_STR(s)

#define LOG(os, ...)                                            \
    count_lines::log(__FILE__, __LINE__, os, __VA_ARGS__)


namespace count_lines {

void FORMAT_ATTR(printf, 4, 5)
log(const std::string &src_path,
    int                line_num,
    std::ostream      &out,
    const char        *fmt,
    ...);

}
