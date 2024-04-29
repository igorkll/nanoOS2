#include "stdint.h"

bool storage_save(const char* path, uint8_t version, void* ptr, size_t size);
bool storage_load(const char* path, uint8_t version, void* ptr, size_t size);
bool storage_loadWithDefault(const char* path, uint8_t version, void* ptr, size_t size, void* default);