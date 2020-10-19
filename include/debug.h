#pragma once
#include <stdarg.h>

// Debug print macro
#ifndef DEBUG
#define DEBUG 1
#endif

// https://stackoverflow.com/a/1644898
#define eprintf(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__); } while (0)
