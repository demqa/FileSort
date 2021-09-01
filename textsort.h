enum ErrorCode{
    WRONG_INPUT_FILE_NAME = 0x1111,
    WRONG_OUTPUT_FILE_NAME,
    CANT_ALLOCATE_MEMORY_BUFFER,
    CANT_ALLOCATE_MEMORY_LINES,
    CANT_GENERATE_FILENAME_ORIG_OUT,
    CANT_GENERATE_FILENAME_REVERSE_OUT,
    DESTRUCT_TEXT_TEXT__IS_NULLPTR,
    NLINES_TEXT_IS_NULLPTR,
    WRITE_TEXT_IS_NULLPTR,
    WRITE_FILENAME_IS_NULLPTR,
    WHAT_IS_WITH_MY_PROGRAM,
};

int CmpAlphaOrd(const void *left, const void *right);
int CmpOriginalText(const void *left, const void *right);
void SortText(Text *text, int Cmp(const void*, const void*), int ReverseOrder);
char *GenOutFileName(char *filename, const char *prefix);
int PrintErrorCode(int error_code);
int ConstructText(const char *filename, Text *text);
int DestructText(Text *text);
int WriteText(Text *text, const char *filename);

/* 
#define case_of_switch(enum_const) \
    case enum_const:               \
        return #enum_const;
*/
