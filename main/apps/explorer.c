#include "../all.h"
#include "explorer.h"

static struct ExplorerData {
    char* copy_path[FILESYSTEM_PATH_LEN];
    bool isMove;
    bool isCopy;
};

static bool _recursive_explorer(const char* folder, char* open, struct ExplorerData* data) {
    struct menuState menu = {
        .title = folder,
        .rightLeftControl = true
    };
    while (true) {
        uint16_t objcount = filesystem_objCount(folder);
        char* objlist[objcount+1];
        char* imglist[objcount+1];
        C_CLEAR(objlist);
        C_CLEAR(imglist);
        objlist[objcount] = "< back";
        filesystem_list(folder, objlist, objcount);
        for (uint16_t i = 0; i < objcount; i++) {
            char newPath[FILESYSTEM_PATH_LEN] = {0};
            filesystem_concat(newPath, folder, objlist[i]);
            imglist[i] = gui_getFileImage(newPath);
        }
        
        menu.pointsCount = objcount+1;
        menu.points = objlist;
        menu.imgs = imglist;

        gui_menu(&menu);
        if (menu.rightLeftState == 0) {
            if (menu.current == objcount) {
                C_FREE_LST(objlist, objcount);
                return false;
            }

            char newPath[FILESYSTEM_PATH_LEN] = {0};
            filesystem_concat(newPath, folder, objlist[menu.current]);
            C_FREE_LST(objlist, objcount);
            
            if (filesystem_isDirectory(newPath)) {
                if (_recursive_explorer(newPath, open, data)) return true;
            } else {
                
            }
        } else {
            const char* strs[] = {"nothing to paste", "mkdir", "< back"};
            if (data->isCopy) {
                if (data->isMove) {
                    strs[0] = "move";
                } else {
                    strs[0] = "paste";
                }
            }

            struct menuState menu2 = {
                .title = folder,
                .pointsCount = 2,
                .points = strs
            };

            bool running = true;
            while (running) {
                switch (gui_menu(&menu2)) {
                    case 0:
                        if (data->isCopy) {
                            
                            if (data->isMove) {
                            }
                        }
                        break;
                    case 1:
                        running = false;
                        break;
                    case 2:
                        running = false;
                        break;
                }
            }
        }
    }
}

static void _explorer(const char* folder, char* open) {
    struct ExplorerData data = {
        .isCopy = false
    };
    _recursive_explorer(folder, open, &data);
}

void explorer_run() {
    _explorer("/storage", NULL);
}