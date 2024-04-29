#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define STORAGE_SYSCONF_VERSION 0
#define STORAGE_SYSCONF_PATH "data/sysconf.cfg"

struct sysconf_type {
    uint8_t cropX;
    uint8_t cropY;
};

bool storage_save(const char* path, uint8_t version, void* ptr, size_t size);
bool storage_load(const char* path, uint8_t version, void* ptr, size_t size);
bool storage_loadWithDefault(const char* path, uint8_t version, void* ptr, size_t size, void* defaultPtr);

void storage_sysconf_push();
void storage_sysconf_pull();
bool storage_sysconf_save();
bool storage_sysconf_load();
struct sysconf_type* storage_sysconf_ptr();