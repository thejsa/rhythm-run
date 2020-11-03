#pragma once
#include <cstdio>
#include <cstdarg>

// Debug print macro
#ifndef DEBUG
#define DEBUG 1
#endif

// https://stackoverflow.com/a/38237385
constexpr const char* _file_name(const char* path) {
    const char* file = path;
    while (*path) {
        if (*path++ == '/') {
            file = path;
        }
    }
    return file;
}

// https://stackoverflow.com/a/1644898
#define eprintf(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, _file_name(__FILE__), \
                                __LINE__, __FUNCTION__, ##__VA_ARGS__); } while (0)