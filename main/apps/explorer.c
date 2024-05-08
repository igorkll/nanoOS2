#include "explorer.h"
#include "../all.h"

static struct ExplorerData {
    char* copy_path[FILESYSTEM_PATH_LEN];
    bool isMove;
    bool isCopy;
};

static bool _recursive_explorer(const char* folder, char* open, struct ExplorerData* data) {
    struct menuState menu = {
        .title = folder,
        .rightLeftControl = true,
        .checker = control_needExitChecker
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
            imglist[i] = malloc(FILESYSTEM_PATH_LEN);
            gui_getFileImage(imglist[i], newPath);
        }
        
        menu.pointsCount = objcount+1;
        menu.points = objlist;
        menu.imgs = imglist;

        if (gui_menu(&menu) == -1) {
            C_FREE_LST(objlist, objcount);
            C_FREE_LST(imglist, objcount);
            return true;
        } 
        if (menu.rightLeftState == CONTROL_LEFT) {
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
                .pointsCount = 3,
                .points = strs,
                .checker = control_needExitChecker
            };

            bool running = true;
            while (running) {
                switch (gui_menu(&menu2)) {
                    case -1:
                        C_FREE_LST(objlist, objcount);
                        C_FREE_LST(imglist, objcount);
                        return true;
                    case 0:
                        if (data->isCopy) {
                            char newPath[FILESYSTEM_PATH_LEN] = {0};
                            char name[FILESYSTEM_PATH_LEN] = {0};
                            filesystem_name(name, data->copy_path);
                            filesystem_concat(newPath, folder, name);

                            if (strcmp(newPath, data->copy_path) == 0) {
                                gui_splash("destination path is equal to the original path");
                            } else if (memcmp(newPath, data->copy_path, strlen(data->copy_path)) == 0) {
                                gui_splash("it is not possible to copy the folder to itself");
                            } else {
                                bool result;
                                if (data->isMove) {
                                    gui_status("moving...");
                                    result = filesystem_move(data->copy_path, newPath);
                                } else {
                                    gui_status("copying...");
                                    result = filesystem_copy(data->copy_path, newPath);
                                }
                                data->isCopy = false;
                                data->isMove = false;
                                if (!result) gui_splash("failed");
                            }

                            running = false;
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
        } else {
            bool isRight = menu.rightLeftState == CONTROL_RIGHT;
            if (menu.current == objcount) {
                if (!isRight) {
                    C_FREE_LST(objlist, objcount);
                    C_FREE_LST(imglist, objcount);
                    return false;
                }
            } else {
                char newPath[FILESYSTEM_PATH_LEN];
                char name[FILESYSTEM_PATH_LEN];
                pathcpy(name, objlist[menu.current]);
                filesystem_concat(newPath, folder, objlist[menu.current]);
                C_FREE_LST(objlist, objcount);
                C_FREE_LST(imglist, objcount);
                
                bool openFlag = false;
                if (isRight) {
                    const char* strs[] = {"open", "delete", "rename", "copy", "cut", "< back"};
                    if (open != NULL) {
                        strs[0] = "select";
                    }

                    struct menuState menu2 = {
                        .title = newPath,
                        .pointsCount = 6,
                        .points = strs,
                        .checker = control_needExitChecker
                    };

                    switch (gui_menu(&menu2)) {
                        case -1:
                            return true;
                        case 0:
                            openFlag = true;
                            break;
                        case 1:
                            xstr str = xstr_new();
                            xstr_minsize(&str, FILESYSTEM_PATH_LEN);
                            xstr_fill(&str, "delete %s?", name);
                            if (gui_yesno(str.ptr)) {
                                gui_status("deleting...");
                                if (!filesystem_remove(newPath)) gui_splash("failed");
                            }
                            xstr_del(&str);
                            break;
                        case 2:
                            break;
                        case 3:
                            data->isCopy = true;
                            data->isMove = false;
                            pathcpy(data->copy_path, newPath);
                            break;
                        case 4:
                            data->isCopy = true;
                            data->isMove = true;
                            pathcpy(data->copy_path, newPath);
                            break;
                    }
                } else {
                    openFlag = true;
                }

                if (openFlag) {
                    if (filesystem_isDirectory(newPath)) {
                        if (_recursive_explorer(newPath, open, data)) return true;
                    } else {
                        if (open == NULL) {
                            explorer_open(newPath);
                        } else {
                            pathcpy(open, newPath);
                            return true;
                        }
                    }
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

static void _errSplash() {
    gui_splash("file is not supported");
}


void explorer_open(const char* path) {
    char exp[FILESYSTEM_EXP_LEN] = {0};
    int8_t expLen = filesystem_expansion(exp, path);
    if (expLen > 0) {
        strtool_lower(exp);
        void* restore = system_beforeApp();
        if (strcmp(exp, "bmp") == 0) {
            viewer_open(path);
        } else if (strcmp(exp, "pkg") == 0) {
            pkg_open(path);
        } else if (strcmp(exp, "lua") == 0) {
            lua_open(path);
        } else {
            _errSplash();
        }
        system_afterApp(restore);
    } else {
        _errSplash();
    }
}

void explorer_run() {
    _explorer("/storage", NULL);
}