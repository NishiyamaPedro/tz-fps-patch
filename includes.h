#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#define LOG(...)                               \
    do                                         \
    {                                          \
        printf("%s:%d: ", __func__, __LINE__); \
        printf(__VA_ARGS__);                   \
        printf("\n");                          \
        fflush(stdout);                        \
    } while (0)
