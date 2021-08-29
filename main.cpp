#include "header.h"

//////////////// TODO LIST /////////////////
////////////////////////////////////////////
/// TODO: RENAME HEADER.H  /////////////////
/// TODO: SPREAD FUNCTIONS IN .CPP FILES  //
/// TODO: REVERSE ORDER SORTING  ///////////
/// TODO: TEST PROGRAM & WRITE TESTS  //////
/// TODO: SPREAD PROGRAM FILES IN FOLDERS //
/// TODO: ADD COMMENTS FOR DOXYGEN  ////////
/// TODO: ADD RUSSIAN  LANG SUPPORT  ///////
////////////////////////////////////////////


struct Line{
    char *ptr = nullptr;
    int length;
    int order;
};

struct Text{
    char *buffer = nullptr;
    size_t filesize;
    int nlines;
    Line *lines = nullptr;
};

size_t FileSize(FILE *stream){
    assert(stream != nullptr);

    struct stat buff = {};
    
    if(stream != NULL){
        fstat(fileno(stream), &buff);
        return buff.st_size;
    }

    return 0;
}


int NumberOfLines(const char *buffer, size_t filesize){
    assert(buffer != nullptr);

    int nlines = 0;
    for (int i = 0; i < filesize; i++){
        if (buffer[i] == '\n'){
            nlines++;
        }
    }
    return nlines;
}

int ConstructText(const char *filename, Text *text){
    assert(filename != nullptr);
    assert(text != nullptr);

    FILE *stream = fopen(filename, "rb");
    if (stream == nullptr){
        return WRONG_INPUT_FILE_NAME;
    }

    text->filesize = FileSize(stream);

    text->buffer = (char *) calloc(text->filesize, sizeof(char));
    if (text->buffer == nullptr){
        return CANT_ALLOCATE_MEMORY_BUFFER;
    }

    fread(text->buffer, sizeof(char), text->filesize, stream);
    fclose(stream);

    int nlines = NumberOfLines(text->buffer, text->filesize);
    assert(nlines != 0);

    text->lines = (Line *) calloc(nlines, sizeof(Line));
    if (text->lines == nullptr){
        return CANT_ALLOCATE_MEMORY_LINES;
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
    
    return 1;
}

int WriteText(Text *text, const char *filename){
    assert(filename != nullptr);
    assert(text != nullptr);

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
    return 1;
}

int DestructText(Text *text){ 
    text->nlines = 0xBEBE;
    text->filesize = 0xDADA;

    free(text->buffer);
    text->buffer = nullptr;

    free(text->lines);
    text->lines = nullptr;

    return 1;
}

int CmpAlphaOrd(const void *left, const void *right){
    const Line c = *(Line *)left;
    const Line d = *(Line *)right;

    int left_length = c.length;
    int right_length = d.length;
    
    int iter_l = 0, iter_r = 0;
    for ( ; c.ptr[iter_l] != '\0' && d.ptr[iter_r] != '\0'; ){
        if (isalpha(c.ptr[iter_l]) == 0){
            ++iter_l;
            continue;
        }
        if (isalpha(d.ptr[iter_r]) == 0){
            ++iter_r;
            continue;
        }

        if (c.ptr[iter_l] == d.ptr[iter_r]){
            ++iter_l; ++iter_r;
        }else{
            if(c.ptr[iter_l] < d.ptr[iter_r]){
                return -1;
            }else{
                return 1;
            }
        }
    }

    return left_length - right_length;
}

int CmpOriginalTest(const void *left, const void *right){
    const Line c = *(Line *)left;
    const Line d = *(Line *)right;

    return (c.order - d.order);
}

void SortText(Text *text, int Cmp(const void*, const void*)){
    qsort(text->lines, text->nlines, sizeof(Line), Cmp);
}

char *GenOutFileName(char *filename){
    char *nameNew = (char *) calloc(strlen(filename) + 10, sizeof(char));
    if (nameNew == nullptr){
        return nullptr;
    }

    strcpy(nameNew, "InputLike_");
    strcat(nameNew, filename);
    return nameNew;
}

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
        default:
            puts("I dont even know this error code ((((");
            break;
    }
    return 1;
}

int main(int argc, char *argv[]){
    
    Text text = {};
    int error_code = 1;
    char *input_file = nullptr;
    char *output_file = nullptr;

    if (argc == 3){
        input_file = argv[1];
        output_file = argv[2];
    }
    else{
        printf("Put input.txt output.txt after ./program\n");
        return 0;
    }

    error_code = ConstructText(input_file, &text);
    if (error_code != 1){
        PrintErrorCode(error_code);
        return 0;
    }


    SortText(&text, CmpAlphaOrd);


    error_code = WriteText(&text, output_file);
    if (error_code != 1){
        PrintErrorCode(error_code);
        return 0;
    }


    SortText(&text, CmpOriginalTest);


    char *output_original_order = GenOutFileName(input_file);
    if (output_original_order != nullptr){
        WriteText(&text, output_original_order);
        free(output_original_order);
    }
    else{
        error_code = CANT_GENERATE_FILENAME_ORIG_OUT;
        PrintErrorCode(error_code);
    }

    DestructText(&text);

    return 0;
}