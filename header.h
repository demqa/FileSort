// rename header.h to something else... p.e. filename.h

#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <locale>

enum ErrorCode{
    WRONG_INPUT_FILE_NAME = 0x1111,
    WRONG_OUTPUT_FILE_NAME,
    CANT_ALLOCATE_MEMORY_BUFFER,
    CANT_ALLOCATE_MEMORY_LINES,
    CANT_GENERATE_FILENAME_ORIG_OUT,
};
