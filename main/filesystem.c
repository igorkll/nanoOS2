#include "main.h"
#include "filesystem.h"

#include <esp_vfs.h>
#include <esp_vfs_fat.h>

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
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

bool filesystem_isDirectory(const char *path) {
    struct stat s;
    if(stat(path, &s) == 0) {
        if (s.st_mode & S_IFDIR) {
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
    struct stat st; 
    if (stat(path, &st) == 0) return st.st_size;
    return -1; 
}