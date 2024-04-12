#include "../all.h"
#include "explorer.h"

static char* _explorer(const char* folder) {
    uint8_t len = strlen(folder);
    char path[len + 1];
    path[len] = '\0';
    strcpy(path, folder);

    while (!control_needExitWithoutGui()) {
    }
}

void explorer_run() {
}