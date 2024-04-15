#include "../all.h"
#include "explorer.h"

static void _explorer(const char* folder, char* open) {
    uint16_t current = 0;
    while (true) {
        uint16_t objcount = filesystem_objCount(folder);
        char* objlist[objcount+1];
        char* imglist[objcount+1];
        C_CLEAR(objlist);
        C_CLEAR(imglist);
        objlist[objcount] = "< back";
        filesystem_list(folder, objlist, objcount);
        for (uint16_t i = 0; i < objcount; i++) {
            imglist[i] = gui_getFileImage(objlist[i]);
        }
        
        struct menuState menu = {
            .pointsCount = objcount+1,
            .points = objlist,
            .title = folder,
            .imgs = imglist,
            .current = current
        };

        current = gui_menu(&menu);
        if (menu.current == objcount) {
            C_FREE_LST(objlist, objcount);
            return;
        }

        char newPath[FILESYSTEM_PATH_LEN] = {0};
        filesystem_concat(newPath, folder, objlist[menu.current]);
        C_FREE_LST(objlist, objcount);
        
        if (filesystem_isDirectory(newPath)) {
            _explorer(newPath, NULL);
        } else {
            
        }
    }
}

void explorer_run() {
    _explorer("/storage", NULL);
}