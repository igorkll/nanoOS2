#include "main.h"
#include "filesystem.h"

#include <esp_vfs.h>
#include <esp_vfs_fat.h>
#include <dirent.h>
#include <unistd.h>

// ------------------------------------------------------------

static char currentPath[FILESYSTEM_PATH_LEN] = {0};

void pathcpy(char* dst, const char* path) {
    strncpy(dst, path, FILESYSTEM_PATH_LEN);
    dst[FILESYSTEM_PATH_LEN - 1] = '\0';
}

void filesystem_concat(char* dst, const char* path1, const char* path2) {
    uint8_t len = strlen(path1);
    uint8_t len2 = strlen(path2);

    if (path1[len-1] == '/') len--;
    if (path2[len2-1] == '/') len2--;

    if (path2[0] == '/') {
        memcpy(dst, path2, len2);
        dst[len2] = '\0';
        return;
    } else if (strcmp(path2, ".") == 0) {
        memcpy(dst, path1, len);
        dst[len] = '\0';
        return;
    }

    memcpy(dst, path1, len);
    dst[len] = '/';
    memcpy(dst + len + 1, path2, len2);
    dst[(len + len2) + 1] = '\0';
}

int8_t filesystem_expansion(char* dst, const char* path) {
    uint8_t len = strlen(path);
    uint8_t expLen = 0;
    for (uint8_t i = len - 1; i > len - (FILESYSTEM_EXP_LEN + 2); i--) {
        if (path[i] == '.') {
            uint8_t index = 0;
            uint8_t pathIndex = len - 1;
            while (index < 3) {
                if (path[pathIndex] == '.') break;
                dst[index++] = path[pathIndex--];
            }
            return expLen;
        }
        expLen++;
    }
    return -1;
}

void filesystem_realPath(char* dst, const char* path) {
    filesystem_concat(dst, currentPath, path);
}

// ------------------------------------------------------------

void filesystem_currentDirectory(char* dst) {
    pathcpy(dst, currentPath);
}

void filesystem_defaultDirectory() {
    filesystem_changeDirectory("/storage");
}

esp_err_t filesystem_init() {
    pathcpy(currentPath, "/storage");

    static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
    esp_vfs_fat_mount_config_t storage_mount_config = {
        .max_files = 4,
        .format_if_mount_failed = false,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    };

    esp_err_t initState = esp_vfs_fat_spiflash_mount_rw_wl("/storage", "storage", &storage_mount_config, &s_wl_handle);
    if (initState == ESP_OK) {
        filesystem_mkdir("data");
        filesystem_mkdir("data/pkg");
        filesystem_mkdir("data/files");
    }
    return initState;
}

// ------------------------------------------------------------

void filesystem_changeDirectory(const char* path) {
    filesystem_toRealPath(realPath, path);
    C_CLEAR(currentPath);
    currentPath[C_SIZE(currentPath) - 1] = '\0';
    pathcpy(currentPath, realPath);
    uint8_t last = strlen(currentPath) - 1;
    if (currentPath[last] == '/') currentPath[last] = '\0';
}

bool filesystem_exists(const char *path) {
    filesystem_toRealPath(realPath, path);
    struct stat state;
    return (stat(realPath, &state) == 0);
}

bool filesystem_isDirectory(const char *path) {
    filesystem_toRealPath(realPath, path);
    struct stat state;
    if(stat(realPath, &state) == 0) {
        if (state.st_mode & S_IFDIR) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

int32_t filesystem_readFile(const char *path, void* buffer, int bufferLen) {
    filesystem_toRealPath(realPath, path);
    FILE *file = fopen(realPath, "rb");
    if (file == NULL) return 0;
    int size = fread(buffer, 1, bufferLen, file);
    fclose(file);
    return size;
}

void* filesystem_mallocReadFile(const char *path) {
    filesystem_toRealPath(realPath, path);
    uint32_t len = filesystem_size(realPath);
    void* buffer = malloc(len);
    filesystem_readFile(realPath, buffer, len);
    return buffer;
}

char* filesystem_stringReadFile(const char *path) {
    filesystem_toRealPath(realPath, path);
    uint32_t len = filesystem_size(realPath);
    char* buffer = malloc(len+1);
    buffer[len] = '\0';
    filesystem_readFile(realPath, buffer, len);
    return buffer;
}

FILE* filesystem_open(const char* path, const char* mode) {
    filesystem_toRealPath(realPath, path);
    return fopen(realPath, mode);
}

uint32_t filesystem_size(const char* path) {
    filesystem_toRealPath(realPath, path);
    struct stat state; 
    if (stat(realPath, &state) == 0) return state.st_size;
    return 0; 
}

bool filesystem_mkdir(const char* path) {
    filesystem_toRealPath(realPath, path);
    if (filesystem_isDirectory(realPath)) return false;
    mkdir(realPath, S_IRWXU);
    return filesystem_isDirectory(realPath);
}

static uint16_t _count(const char* path, bool files, bool dirs) {
    DIR *dir = opendir(path);
    if (dir == NULL) return 0;
    uint16_t count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") == 0) continue;
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") == 0) continue;
        if ((files && entry->d_type == DT_REG) || (dirs && entry->d_type == DT_DIR)) count++;
    }
    closedir(dir);
    return count;
}

uint16_t filesystem_fileCount(const char* path) {
    filesystem_toRealPath(realPath, path);
    return _count(realPath, true, false);
}

uint16_t filesystem_dirCount(const char* path) {
    filesystem_toRealPath(realPath, path);
    return _count(realPath, false, true);
}

uint16_t filesystem_objCount(const char* path) {
    filesystem_toRealPath(realPath, path);
    return _count(realPath, true, true);
}

uint16_t filesystem_list(const char* path, char** list, uint16_t listSize) {
    if (listSize == 0) return 0;
    filesystem_toRealPath(realPath, path);
    DIR *dir = opendir(realPath);
    if (dir == NULL) return 0;
    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") == 0) continue;
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") == 0) continue;
        char* str = malloc(strlen(entry->d_name) + 1);
        strcpy(str, entry->d_name);
        list[count] = str;
        count++;
        if (count >= listSize) break;
    }
    closedir(dir);
    return count;
}