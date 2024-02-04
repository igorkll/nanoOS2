#include "main.h"
#include "fs.h"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"

/*
esp_err_t fs_init_rom() {
    esp_vfs_fat_mount_config_t storage_mount_config = {
        .max_files = 10,
        .format_if_mount_failed = false,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    };

    return esp_vfs_fat_spiflash_mount_ro("/rom", "rom", &storage_mount_config);
}
*/

static esp_err_t fs_init_storage() {
    static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
    esp_vfs_fat_mount_config_t storage_mount_config = {
        .max_files = 16,
        .format_if_mount_failed = false,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    };

    return esp_vfs_fat_spiflash_mount_rw_wl("/storage", "storage", &storage_mount_config, &s_wl_handle);
}

esp_err_t fs_init() {
    return fs_init_storage();
}