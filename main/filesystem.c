#include "main.h"
#include "filesystem.h"

#include <esp_vfs.h>
#include <esp_vfs_fat.h>
#include <dirent.h>
#include <unistd.h>

// ------------------------------------------------------------

static char currentPath[FILESYSTEM_PATHLEN] = {0};

char* filesystem_concat(const char* path1, const char* path2) {

}

// ------------------------------------------------------------

void filesystem_currentDirectory(char* ptr) {
    strcpy(ptr, currentPath);
}

void filesystem_changeDirectory(const char* path) {
    C_CLEAR(currentPath);
    currentPath[C_SIZE(currentPath) - 1] = '\0';
    strcpy(currentPath, path);
}

void filesystem_defaultDirectory() {
    filesystem_changeDirectory("/storage");
}

esp_err_t filesystem_init() {
    filesystem_defaultDirectory();

    static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
    esp_vfs_fat_mount_config_t storage_mount_config = {
        .max_files = 16,
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


bool filesystem_exists(const char *path) {
    struct stat state;
    return (stat(path, &state) == 0);
}

bool filesystem_isDirectory(const char *path) {
    struct stat state;
    if(stat(path, &state) == 0) {
        if (state.st_mode & S_IFDIR) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

int32_t filesystem_readFile(const char *path, void* buffer, int bufferLen) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) return 0;
    int size = fread(buffer, 1, bufferLen, file);
    fclose(file);
    return size;
}

void* filesystem_mallocReadFile(const char *path) {
    uint32_t len = filesystem_size(path);
    void* buffer = malloc(len);
    filesystem_readFile(path, buffer, len);
    return buffer;
}

char* filesystem_stringReadFile(const char *path) {
    uint32_t len = filesystem_size(path);
    char* buffer = malloc(len+1);
    buffer[len] = '\0';
    filesystem_readFile(path, buffer, len);
    return buffer;
}

uint32_t filesystem_size(const char* path) {
    struct stat state; 
    if (stat(path, &state) == 0) return state.st_size;
    return -1; 
}

bool filesystem_mkdir(const char* path) {
    if (filesystem_isDirectory(path)) return false;
    mkdir(path, S_IRWXU);
    return filesystem_isDirectory(path);
}

static uint16_t _count(const char* path, bool files, bool dirs) {
    DIR *dir = opendir(path);
    if (dir == NULL) return -1;
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
    return _count(path, true, false);
}

uint16_t filesystem_dirCount(const char* path) {
    return _count(path, false, true);
}

uint16_t filesystem_objCount(const char* path) {
    return _count(path, true, true);
}

uint16_t filesystem_list(const char* path, char** list, uint16_t listSize) {
    DIR *dir = opendir(path);
    if (dir == NULL) return -1;
    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") == 0) continue;
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") == 0) continue;
        uint16_t namelen = strlen(entry->d_name);
        char* str = malloc(namelen + 1);
        str[namelen] = '\0';
        strcpy(str, entry->d_name);
        list[count] = str;
        count++;
        if (count >= listSize) break;
    }
    closedir(dir);
    return count;
}