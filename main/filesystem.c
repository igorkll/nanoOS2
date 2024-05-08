#include "main.h"
#include "filesystem.h"
#include "system.h"

#include <esp_vfs.h>
#include <esp_vfs_fat.h>
#include <dirent.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>

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
            int8_t index = expLen - 1;
            uint8_t pathIndex = len - 1;
            while (index >= 0) {
                if (path[pathIndex] == '.') break;
                dst[index--] = path[pathIndex--];
            }
            return expLen;
        }
        expLen++;
    }
    dst[expLen] = '\0';
    return -1;
}

int8_t filesystem_name(char* dst, const char* path) {
    uint8_t len = strlen(path);
    uint8_t pathLen = 0;
    for (uint8_t i = len - 1; i > 0; i--) {
        if (path[i] == '/') {
            int8_t index = pathLen - 1;
            uint8_t pathIndex = len - 1;
            while (index >= 0) {
                if (path[pathIndex] == '/') break;
                dst[index--] = path[pathIndex--];
            }
            return pathLen;
        }
        pathLen++;
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

static esp_err_t _init_storage() {
    static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
    esp_vfs_fat_mount_config_t storage_mount_config = {
        .max_files = 4,
        .format_if_mount_failed = false,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    };

    return esp_vfs_fat_spiflash_mount_rw_wl("/storage", "storage", &storage_mount_config, &s_wl_handle);
}

#ifdef SDCARD_ENABLE
#include <sdmmc_cmd.h>
const char* SDCARD = "cdcard";

static bool sdcard_needFormat = false;
static sdmmc_host_t host = SDSPI_HOST_DEFAULT();
static sdspi_device_config_t slot_config = {
    .gpio_cs = -1,
    .gpio_cd = -1,
    .gpio_wp = -1
};

static esp_err_t _sdcard_mount(bool format) {
    ESP_LOGI(SDCARD, "Mounting sdcard");

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = format,
        .max_files = 4,
        .allocation_unit_size = 16 * 1024
    };
    sdmmc_card_t* card;
    esp_err_t ret = esp_vfs_fat_sdspi_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            sdcard_needFormat = true;
            ESP_LOGE(SDCARD, "Failed to mount filesystem.");
        } else {
            ESP_LOGE(SDCARD, "Failed to initialize the card (%s).", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    sdcard_needFormat = false;
    ESP_LOGI(SDCARD, "sdcard mounted");
    sdmmc_card_print_info(stdout, card);
    return ESP_OK;
}

static esp_err_t _init_sdcard() {
    esp_err_t ret;

    #ifdef SDCARD_BASESPI
        spi_bus_config_t bus_cfg = system_baseSPI;
        host.slot = BASESPI_SPI;
    #else
        spi_bus_config_t bus_cfg = {
            .mosi_io_num = SDCARD_MOSI,
            .miso_io_num = SDCARD_MISO,
            .sclk_io_num = SDCARD_CLK,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = 4000,
        };

        #ifdef SDCARD_SPI
            host.slot = SDCARD_SPI;
        #endif
        ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
        if (ret != ESP_OK) {
            ESP_LOGE(SDCARD, "Failed to initialize bus.");
            return ESP_FAIL;
        }
    #endif
    
    slot_config.host_id = host.slot;
    #ifdef SDCARD_CS
        slot_config.gpio_cs = SDCARD_CS;
    #endif
    #ifdef SDCARD_CD
        slot_config.gpio_cd = SDCARD_CD;
    #endif
    #ifdef SDCARD_WP
        slot_config.gpio_wp = SDCARD_WP;
    #endif

    return _sdcard_mount(false);
}

bool filesystem_sdcard_needFormat() {
    return sdcard_needFormat;
}

bool filesystem_sdcard_remount(bool format) {
    return _sdcard_mount(format) == ESP_OK;
}

#else

bool filesystem_sdcard_needFormat() {
    return false;
}

bool filesystem_sdcard_format() {
    return false;
}

#endif

esp_err_t filesystem_init() {
    pathcpy(currentPath, "/storage");

    // -------- sdcard storage
    #ifdef SDCARD_ENABLE
        printf("sdcard: %s\n", esp_err_to_name(_init_sdcard()));
    #endif

    // -------- internal storage
    esp_err_t initState = _init_storage();
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

size_t filesystem_writeFile(const char* path, void* buffer, size_t bufferLen) {
    filesystem_toRealPath(realPath, path);
    FILE *file = fopen(realPath, "wb");
    if (file == NULL) return 0;
    size_t size = fwrite(buffer, 1, bufferLen, file);
    fclose(file);
    return size;
}

size_t filesystem_readFile(const char *path, void* buffer, size_t bufferLen) {
    filesystem_toRealPath(realPath, path);
    FILE *file = fopen(realPath, "rb");
    if (file == NULL) return 0;
    size_t size = fread(buffer, 1, bufferLen, file);
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

size_t filesystem_size(const char* path) {
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

bool filesystem_copy(const char *path1, const char *path2) {
    if (filesystem_isDirectory(path1)) {
        filesystem_mkdir(path2);
        bool okay = true;
        filesystem_iterate(path1, name, {
            char newPath1[FILESYSTEM_PATH_LEN];
            char newPath2[FILESYSTEM_PATH_LEN];
            filesystem_concat(newPath1, path1, name);
            filesystem_concat(newPath2, path2, name);
            if (!filesystem_copy(newPath1, newPath2)) okay = false;
        });
        return okay;
    } else {
        FILE* file1 = filesystem_open(path1, "rb");
        if (!file1) return false;
        FILE* file2 = filesystem_open(path2, "wb");
        if (!file2) {
            fclose(file1);
            return false;
        }

        uint8_t buffer[FILESYSTEM_COPY_BUFFER];
        while (true) {
            uint16_t num = fread(buffer, 1, FILESYSTEM_COPY_BUFFER, file1);
            if (num == 0) break;
            fwrite(buffer, 1, num, file2);
        }

        fclose(file1);
        fclose(file2);
        return true;
    }
    return false;
}

bool filesystem_move(const char *path1, const char *path2) {
    if (filesystem_copy(path1, path2)) {
        return filesystem_remove(path1);
    }
    return false;
}

bool filesystem_remove(const char *path) {
    if (filesystem_isDirectory(path)) {
        bool okay = true;
        filesystem_iterate(path, name, {
            char newPath[FILESYSTEM_PATH_LEN];
            filesystem_concat(newPath, path, name);
            if (!filesystem_remove(newPath)) okay = false;
        });
        if (!okay) return false;
    }
    filesystem_toRealPath(realPath, path);
    return remove(realPath) >= 0;
}