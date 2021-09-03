#include "mystrlib.h"
#include "textsort.h"

/////////////// TODO LIST //////////////////
/// TODO: PUT FUNCTIONS WITH TEXT TO LIB ///
/// TODO: CHANGE SOME INTS TO SIZE_T  //////
/// TODO: TEST PROGRAM & WRITE TESTS  //////
/// TODO: SPREAD PROGRAM FILES IN FOLDERS //
/// TODO: ADD COMMENTS FOR DOXYGEN  ////////
/// TODO: ADD RUSSIAN  LANG SUPPORT  ///////
////////////////////////////////////////////

int main(int argc, char *argv[]){
    
    Text text = {};
    int error_code = 0;

    char *input_file  = nullptr;
    char *output_file = nullptr;

    if (argc == 3){
        input_file  = argv[1];
        output_file = argv[2];
    }
    else{
        printf("Put input.txt output.txt after ./program\n");
        return 0;
    }

    error_code = ConstructText(input_file, &text);
    CheckError(&error_code);

    error_code = SortText(&text, CmpAlphaOrd);
    CheckError(&error_code);

    error_code = WriteText(&text, output_file);
    CheckError(&error_code);

    error_code = SortText(&text, CmpOriginalText);
    CheckError(&error_code);

    char *output_original_order = GenerateName(input_file, "input_like");
    if (output_original_order != nullptr){
        error_code = WriteText(&text, output_original_order);
        CheckError(&error_code);
        free(output_original_order);
    }
    else{
        error_code = CANT_GENERATE_FILENAME_ORIG_OUT;
        PrintErrorCode(error_code);
    }

    error_code = SortText(&text, CmpReverseOrd);
    CheckError(&error_code);

    char *output_reverse = GenerateName(input_file, "reverse");
    if (output_original_order != nullptr){
        error_code = WriteText(&text, output_reverse);
        CheckError(&error_code);
        free(output_reverse);
    }
    else{
        error_code = CANT_GENERATE_FILENAME_REVERSE_OUT;
        PrintErrorCode(error_code);
    }

    error_code = DestructText(&text);
    CheckError(&error_code);

    return 0;
}
