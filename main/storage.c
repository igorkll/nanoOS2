#include "filesystem.h"
#include <esp_vfs.h>

bool storage_save(const char* path, uint8_t version, void* ptr, size_t size) {
    FILE* file = filesystem_open(path, "wb");
    if (file == NULL) return false;
    fwrite(&version, 1, 1, file);
    fwrite(ptr, 1, size, file);
    fclose(file);
    return true;
}

bool storage_load(const char* path, uint8_t version, void* ptr, size_t size) {
    FILE* file = filesystem_open(path, "rb");
    if (file == NULL) return false;
    uint8_t realVersion;
    if (fread(&realVersion, 1, 1, file) != 1 || realVersion != version || fread(ptr, 1, size, file) != size) {
        fclose(file);
        return false;
    }
    fclose(file);
    return true;
}

bool storage_loadWithDefault(const char* path, uint8_t version, void* ptr, size_t size, void* default) {
    bool state = storage_load(path, version, ptr, size);
    if (!state && default != NULL) {
        memcpy(ptr, default, size);
        return true;
    }
    return state;
}