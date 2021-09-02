#include "mystrlib.h"
#include "textsort.h"

/////////////// TODO LIST //////////////////
/// TODO: TEST PROGRAM & WRITE TESTS  //////
/// TODO: SPREAD PROGRAM FILES IN FOLDERS //
/// TODO: ADD COMMENTS FOR DOXYGEN  ////////
/// TODO: ADD RUSSIAN  LANG SUPPORT  ///////
////////////////////////////////////////////

int main(int argc, char *argv[]){
    
    Text text = {};
    int error_code = 0;
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
    if (error_code){
        PrintErrorCode(error_code);
        return 0;
    }

    SortText(&text, CmpAlphaOrd);

    error_code = WriteText(&text, output_file);
    if (error_code){
        PrintErrorCode(error_code);
        return 0;
    }

    SortText(&text, CmpOriginalText);

    char *output_original_order = GenOutFileName(input_file, "input_like");
    if (output_original_order != nullptr){
        WriteText(&text, output_original_order);
        free(output_original_order);
    }
    else{
        error_code = CANT_GENERATE_FILENAME_ORIG_OUT;
        PrintErrorCode(error_code);
    }

    /*

    SortText(&text, CmpOriginalText);

    char *output_reverse = GenOutFileName(input_file, "reverse");
    if (output_original_order != nullptr){
        WriteText(&text, output_reverse);
        free(output_reverse);
    }
    else{
        error_code = CANT_GENERATE_FILENAME_REVERSE_OUT;
        PrintErrorCode(error_code);
    }
    
    */

    error_code = DestructText(&text);
    if (error_code){
        PrintErrorCode(error_code);
        return 0;
    }

    return 0;
}