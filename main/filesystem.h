esp_err_t filesystem_init();
bool filesystem_exists(const char *path);
bool filesystem_isDirectory(const char *path);
int32_t filesystem_readFile(const char* path, void* buffer, int bufferLen);
uint32_t filesystem_size(const char* path);

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