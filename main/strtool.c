#include <strtool.h>
#include <ctype.h>

void strtool_lower(char* str) {
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

void strtool_upper(char* str) {
    for(int i = 0; str[i]; i++){
        str[i] = toupper(str[i]);
    }
}