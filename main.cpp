#include "header.h"


size_t FileSize(FILE *stream){
    struct stat buff = {};
    
    if(stream != NULL){
        fstat(fileno(stream), &buff);
        return buff.st_size;
    }

    return 0;
}

int ReadFile(const char *filename, char *lines[]){
    FILE *stream = fopen(filename, "rb");
    size_t filesize = FileSize(stream);
    
    char *p = (char *) calloc(filesize, sizeof(char)); // rename to buffer
    fread(p, sizeof(char), filesize, stream);
    fclose(stream);

    int index = 0;
    lines[index++] = p;
    
    bool flag = false;
    char *current = p;
    char *end = current + filesize;
    while (current < end){
        if (flag && *current != '\n'){
            lines[index++] = current;
            flag = false;
        }
        if (*current == '\n'){
            *current = '\0';
            flag = true;
        }
        current++;
    }

    return index;
}

int WriteFile(const char *filename, char *lines[], int nlines){
    if(filename == NULL || lines == NULL){
        return 0;
    }

    FILE *stream = fopen(filename, "w");
    char *p, line[MAX_LINE_LEN];
    
    int num = 0;
    while (num < nlines){
        fputs(lines[num++], stream);
        fputc('\n', stream);
    }

    fclose(stream);
    return 1;
}

int Min(int a, int b){
    return (a < b) ? a : b;
}

int Cmp(const void *left, const void *right){
    const char *c = *(const char **)left;
    const char *d = *(const char **)right;

    int left_length = strlen(c);
    int right_length = strlen(d);
    printf("%d %d\n", left_length, right_length);

    for (int i = 0; i < Min(left_length, right_length); ++i){
        if (c[i] == d[i]){
            continue;
        }else{
            if(c[i] < d[i]){
                return -1;
            }else{
                return 1;
            }
        }
    }

    return left_length - right_length;
}

void Solve(char **lines, int nlines){
    qsort(lines, nlines, sizeof(char *),  Cmp);
}

int main(){
    char *lines[MAX_LINES] = {};

    int nlines = ReadFile(FILE_INPUT, lines);
    
    assert(nlines >= 1);
    printf("%d\n", nlines);

    Solve(lines, nlines);

    int res = WriteFile(FILE_OUTPUT, lines, nlines);
    assert(res == 1);

    free(lines[0]);

    return 0;
}