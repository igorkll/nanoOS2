#include "../all.h"
#include "explorer.h"

static void _explorer(const char* folder, char* open) {
    uint16_t objcount = filesystem_objCount(folder);
    char* objlist[objcount+1];
    C_CLEAR(objlist);
    objlist[objcount] = "< back";
    filesystem_list(folder, objlist, objcount);
    
    struct menuState menu = {
        .pointsCount = objcount+1,
        .points = objlist,
        .title = folder
    };

    while (true) {
        gui_menu(&menu);
        if (menu.current == objcount) return;

        char newPath[FILESYSTEM_PATH_LEN] = {0};
        filesystem_concat(newPath, folder, objlist[menu.current]);
        
        if (filesystem_isDirectory(newPath)) {
            _explorer(newPath, NULL);
        } else {
            
        }
    }
}

void explorer_run() {
    _explorer("/storage", NULL);
}