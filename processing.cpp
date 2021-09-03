#include "mystrlib.h"
#include "textsort.h"

size_t FileSize(FILE *stream){
    if (stream == nullptr){
        return FILESIZE_STREAM_IS_NULLPTR;
    }

    struct stat buff = {};
    
    if(stream != NULL){
        fstat(fileno(stream), &buff);
        return buff.st_size;
    }

    return 0;
}

int ReadBuffer(Text *text, FILE *stream){
    if (text == nullptr){
        return READBUFFER_TEXT_IS_NULLPTR;
    }
    if (stream == nullptr){
        return READBUFFER_STREAM_IS_NULLPTR;
    }

    text->buffer = (char *) calloc(text->filesize + 1, sizeof(char));
    if (text->buffer == nullptr){
        return CANT_ALLOCATE_MEMORY_BUFFER;
    }

    int count = fread(text->buffer, sizeof(char), text->filesize, stream);
    if (count != text->filesize){
        fclose(stream);
        return READBUFFER_FREAD_CANT_READ;
    }

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
    if (filename == nullptr){
        return CONSTRUCT_FILENAME_IS_NULLPTR;
    }
    if (text == nullptr){
        return CONSTRUCT_TEXT_IS_NULLPTR;
    }

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

int CmpReverseOrd(const void *left_line, const void *right_line){
    assert(left_line != nullptr);
    assert(right_line != nullptr);

    const Line left = *(Line *)left_line;
    const Line right = *(Line *)right_line;
    
    int iter_l = left.length - 2, iter_r = right.length - 2;
    for ( ; left.ptr[iter_l] != '\0' && right.ptr[iter_r] != '\0'; ){
        if (isalpha(left.ptr[iter_l]) == 0){
            --iter_l;
            continue;
        }
        if (isalpha(right.ptr[iter_r]) == 0){
            --iter_r;
            continue;
        }

        if (left.ptr[iter_l] == right.ptr[iter_r]){
            --iter_l; --iter_r;
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

int SortText(Text *text, int Cmp(const void*, const void*)){
    if (text == nullptr){
        return SORT_TEXT_IS_NULLPTR;
    }
    if (Cmp == nullptr){
        return SORT_CMP_IS_NULLPTR;
    }

    qsort(text->lines, text->nlines, sizeof(Line), Cmp);
    
    return 0;
}

// can be changed to int GenerateName(char *new_name, char *name, char *prefix)
char *GenerateName(char *filename, const char *prefix){ 
    if (filename == nullptr){
        return nullptr;
    }
    if (prefix == nullptr){
        return nullptr;
    }

    char *name_new = (char *) calloc(strlen(filename) + 1 + strlen(prefix), sizeof(char));
    if (name_new == nullptr){
        return nullptr;
    }

    strcpy(name_new, prefix);
    strcat(name_new, "_");
    strcat(name_new, filename);

    return name_new;
}

// can be changed to char *Function(int error_code){ return error_phrase(error_code); };
int PrintErrorCode(int error_code){
    printf("code: %x\n", error_code);
    switch (error_code){
        case WRONG_INPUT_FILE_NAME:
            puts("I cant find this input file (((");
            break;
        case WRONG_OUTPUT_FILE_NAME:
            puts("There is NO WAY to write into this file");
            break;
        case CANT_ALLOCATE_MEMORY_BUFFER:
             CANT_ALLOCATE_MEMORY_LINES:
            puts("I cant allocate memory for you girl... (((((((");
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
        case FILESIZE_STREAM_IS_NULLPTR:
            READBUFFER_STREAM_IS_NULLPTR:
            puts("Stream is null, I cant do anything(");
            break;
        case READBUFFER_TEXT_IS_NULLPTR:
            CONSTRUCT_TEXT_IS_NULLPTR:
            SORT_TEXT_IS_NULLPTR:
            puts("The text is null, what can I do?");
            break;
        case READBUFFER_FREAD_CANT_READ:
            puts("FREAD DOESNT WORK, WHAT THE...");
            break;
        case CONSTRUCT_FILENAME_IS_NULLPTR:
            puts("FILENAME IS NULL");
            break;
        case SORT_CMP_IS_NULLPTR:
            puts("Where is my comparator... give it back...");
            break;
        default:
            puts("I dont even know this error code ((((");
            break;
    }
    return 0;
}

void CheckError(int *error_code){
    if (*error_code){
        PrintErrorCode(*error_code);
        *error_code = 0;
    }
}
