#include <stdio.h>
#include <stddef.h>
#include <esp_err.h>

#define FILESYSTEM_PATH_LEN 40
#define FILESYSTEM_EXP_LEN 4
#define FILESYSTEM_COPY_BUFFER 512
#define filesystem_toRealPath(var, path) char var[FILESYSTEM_PATH_LEN]; filesystem_realPath(var, path)

void pathcpy(char* dst, const char* path);
void filesystem_concat(char* dst, const char* path1, const char* path2);
int8_t filesystem_expansion(char* dst, const char* path);
int8_t filesystem_name(char* dst, const char* path);
void filesystem_realPath(char* dst, const char* path);

esp_err_t filesystem_init();
size_t filesystem_size(const char* path);

bool filesystem_exists(const char *path);
bool filesystem_isDirectory(const char *path);

bool filesystem_copy(const char *path1, const char *path2);
bool filesystem_move(const char *path1, const char *path2);
bool filesystem_remove(const char *path);

size_t filesystem_writeFile(const char* path, void* buffer, size_t bufferLen);
size_t filesystem_readFile(const char* path, void* buffer, size_t bufferLen);
void* filesystem_mallocReadFile(const char* path);
char* filesystem_stringReadFile(const char* path);
FILE* filesystem_open(const char* path, const char* mode);

void filesystem_currentDirectory(char* dst);
void filesystem_defaultDirectory();
void filesystem_changeDirectory(const char* path);
bool filesystem_mkdir(const char* path);

uint16_t filesystem_fileCount(const char* path);
uint16_t filesystem_dirCount(const char* path);
uint16_t filesystem_objCount(const char* path);
uint16_t filesystem_list(const char* path, char** list, uint16_t listSize);

#define filesystem_iterate(path, var, code) { \
    uint16_t _lstSize = filesystem_objCount(path); \
    char* _lst[_lstSize]; \
    C_CLEAR(_lst); \
    uint16_t _count = filesystem_list(path, _lst, _lstSize); \
    for (uint16_t _i = 0; _i < _count; _i++) { \
        char* var = _lst[_i]; \
        code \
        free(var); \
    } \
}