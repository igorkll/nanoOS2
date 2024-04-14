#include "../all.h"
#include "explorer.h"

static char* _explorer(const char* folder) {
    uint8_t len = strlen(folder);
    char path[len + 1];
    path[len] = '\0';
    strcpy(path, folder);

    printf("\n");
    filesystem_iterate("/storage", filename, {
        printf("obj: %s\n", filename);
    });

    while (!control_needExitWithoutGui()) {
    }

    return NULL;
}

void explorer_run() {
    _explorer("data");
}