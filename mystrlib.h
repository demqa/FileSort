#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <locale>
#include <stddef.h>

struct Line{
    char *ptr = nullptr;
    int length = 0;
    int order = 0;
};

struct Text{
    char *buffer = nullptr;
    size_t filesize = 0;
    int nlines = 0;
    Line *lines = nullptr;
};
