esp_err_t filesystem_init();
bool filesystem_exists(const char *path);
bool filesystem_isDirectory(const char *path);
int filesystem_readFile(const char* path, void* buffer, int bufferLen);
int filesystem_size(const char* path);

int filesystem_fileCount(const char* path);
int filesystem_dirCount(const char* path);
int filesystem_objCount(const char* path);