#include "main.h"
#include "filesystem.h"

#include <esp_vfs.h>
#include <esp_vfs_fat.h>
#include <dirent.h>

static esp_err_t fs_init_storage() {
    static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
    esp_vfs_fat_mount_config_t storage_mount_config = {
        .max_files = 16,
        .format_if_mount_failed = false,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    };

    return esp_vfs_fat_spiflash_mount_rw_wl("/storage", "storage", &storage_mount_config, &s_wl_handle);
}

esp_err_t filesystem_init() {
    return fs_init_storage();
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

int filesystem_readFile(const char *path, void* buffer, int bufferLen) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) return 0;
    int size = fread(buffer, 1, bufferLen, file);
    fclose(file);
    return size;
}

int filesystem_size(const char* path) {
    struct stat state; 
    if (stat(path, &state) == 0) return state.st_size;
    return -1; 
}

static int _count(const char* path, bool files, bool dirs) {
    DIR *dir = opendir(path);
    if (dir == NULL) return -1;
    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") == 0) continue;
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") == 0) continue;
        if ((files && entry->d_type == DT_REG) || (dirs && entry->d_type == DT_DIR)) count++;
    }
    closedir(dir);
    return count;
}

int filesystem_fileCount(const char* path) {
    return _count(path, true, false);
}

int filesystem_dirCount(const char* path) {
    return _count(path, false, true);
}

int filesystem_objCount(const char* path) {
    return _count(path, true, true);
}