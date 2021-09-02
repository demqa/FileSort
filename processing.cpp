#include "mystrlib.h"
#include "textsort.h"

size_t FileSize(FILE *stream){
    assert(stream != nullptr);

    struct stat buff = {};
    
    if(stream != NULL){
        fstat(fileno(stream), &buff);
        return buff.st_size;
    }

    return 0;
}

int ReadBuffer(Text *text, FILE *stream){
    text->buffer = (char *) calloc(text->filesize + 1, sizeof(char));
    if (text->buffer == nullptr){
        return CANT_ALLOCATE_MEMORY_BUFFER;
    }

    fread(text->buffer, sizeof(char), text->filesize, stream);
    text->buffer[text->filesize] = '\0';

    fclose(stream);

    return 0;
}

int NumberOfLines(Text *text){
    if (text == nullptr){
        return NLINES_TEXT_IS_NULLPTR;
    }

    for (int i = 0; i < text->filesize; i++){
        if (text->buffer[i] == '\n'){
            text->nlines++;
        }
    }

    text->lines = (Line *) calloc(text->nlines, sizeof(Line));
    if (text->lines == nullptr){
        return CANT_ALLOCATE_MEMORY_LINES;
    }

    return 0;
}

int ConstructText(const char *filename, Text *text){
    assert(filename != nullptr);
    assert(text != nullptr);

    FILE *stream = fopen(filename, "rb");
    if (stream == nullptr){
        return WRONG_INPUT_FILE_NAME;
    }

    text->filesize = FileSize(stream);

    int error_code = ReadBuffer(text, stream);
    if (error_code){
        return error_code;
    }

    error_code = NumberOfLines(text);
    if (error_code){
        return error_code;
    }

    int index = 0;
    text->lines[index].order = index;
    text->lines[index++].ptr = text->buffer;
    
    bool flag = false;
    char *current = text->buffer;
    char *end = current + text->filesize;
 
    while (current < end){
        if (flag && *current != '\n'){
            text->lines[index - 1].length = current - text->lines[index - 1].ptr;
            text->lines[index].order = index;
            text->lines[index++].ptr = current;
            flag = false;
        }
        else if (*current == '\n'){
            *current = '\0';
            flag = true;
        }
        current++;
    }
    text->lines[index - 1].length = current - text->lines[index - 1].ptr;
    text->nlines = index;
    
    return 0;
}

int WriteText(Text *text, const char *filename){
    if (text == nullptr){
        return WRITE_TEXT_IS_NULLPTR;
    }
    if (filename == nullptr){
        return WRITE_FILENAME_IS_NULLPTR;
    }

    FILE *stream = fopen(filename, "w");
    if (stream == nullptr){
        return WRONG_OUTPUT_FILE_NAME;
    }
    
    int num = 0;
    while (num < text->nlines){
        fputs(text->lines[num++].ptr, stream);
        fputc('\n', stream);
    }

    fclose(stream);
    return 0;
}

int DestructText(Text *text){ 
    if (text == nullptr){
        return DESTRUCT_TEXT_TEXT__IS_NULLPTR;
    }

    text->nlines = 0xDADA;
    text->filesize = 0xBEBE;

    free(text->buffer);
    text->buffer = nullptr;

    free(text->lines);
    text->lines = nullptr;

    return 0;
}

int CmpAlphaOrd(const void *left_line, const void *right_line){
    assert(left_line != nullptr);
    assert(right_line != nullptr);

    const Line left = *(Line *)left_line;
    const Line right = *(Line *)right_line;
    
    int iter_l = 0, iter_r = 0;
    for ( ; left.ptr[iter_l] != '\0' && right.ptr[iter_r] != '\0'; ){
        if (isalpha(left.ptr[iter_l]) == 0){
            ++iter_l;
            continue;
        }
        if (isalpha(right.ptr[iter_r]) == 0){
            ++iter_r;
            continue;
        }

        if (left.ptr[iter_l] == right.ptr[iter_r]){
            ++iter_l; ++iter_r;
        }else{
            if(left.ptr[iter_l] < right.ptr[iter_r]){
                return -1;
            }else{
                return 1;
            }
        }
    }

    return left.length - right.length;
}

int CmpOriginalText(const void *left_line, const void *right_line){
    assert(left_line != nullptr);
    assert(right_line != nullptr);
    
    const Line left = *(Line *)left_line;
    const Line right = *(Line *)right_line;

    return (left.order - right.order);
}

void SortText(Text *text, int Cmp(const void*, const void*)){
    assert(text != nullptr);

    qsort(text->lines, text->nlines, sizeof(Line), Cmp); // You cant reverse every single comparator, only CmpAlphaOrd
}

char *GenOutFileName(char *filename, const char *prefix){
    assert(filename != nullptr);
    assert(prefix != nullptr);

    char *nameNew = (char *) calloc(strlen(filename) + 1 + strlen(prefix), sizeof(char));
    if (nameNew == nullptr){
        return nullptr;
    }

    strcpy(nameNew, prefix);
    strcat(nameNew, "_");
    strcat(nameNew, filename);

    return nameNew;
}

// change to char *Function(int error_code){ return error_phrase(error_code); };
int PrintErrorCode(int error_code){
    switch (error_code){
        case WRONG_INPUT_FILE_NAME:
            puts("I cant find this input file (((");
            break;
        case WRONG_OUTPUT_FILE_NAME:
            puts("There is NO WAY to write into this file");
            break;
        case CANT_ALLOCATE_MEMORY_BUFFER:
             CANT_ALLOCATE_MEMORY_LINES:
            puts("I cant allocate memory for you (");
            break;
        case CANT_GENERATE_FILENAME_ORIG_OUT:
            puts("I cant generate filename orig out");
            break;
        case CANT_GENERATE_FILENAME_REVERSE_OUT:
            puts("I cant generate filename reverse");
            break;
        case DESTRUCT_TEXT_TEXT__IS_NULLPTR:
            puts("I cant destoy text, because it is nullptr");
            break;
        case NLINES_TEXT_IS_NULLPTR:
            puts("I cant count number of lines, because text is nullptr");
            break;
        case WRITE_TEXT_IS_NULLPTR:
            puts("I cant write text when it is nullptr");
            break;
        case WHAT_IS_WITH_MY_PROGRAM:
            puts("If you got there, I can only admire you");
            break;
        default:
            puts("I dont even know this error code ((((");
            printf("%x\n", error_code);
            break;
    }
    return 0;
}
