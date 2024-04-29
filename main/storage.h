#include "stdint.h"

#define STORAGE_SYSCONF_VERSION 0
#define STORAGE_SYSCONF_PATH "/data/sysconf.cfg"
struct {
    uint8_t cropX;
    uint8_t cropY;
} sysconf;

bool storage_save(const char* path, uint8_t version, void* ptr, size_t size);
bool storage_load(const char* path, uint8_t version, void* ptr, size_t size);
bool storage_loadWithDefault(const char* path, uint8_t version, void* ptr, size_t size, void* default);

void storage_sysconf_push();
void storage_sysconf_pull();
bool storage_sysconf_save();
bool storage_sysconf_load();