void customshell_run() {  
    while (true) {
        graphic_clear(color_black);
        graphic_drawText(1, 1, "custom shell", color_orange);
        graphic_drawText(1, 2 + graphic_getFontHeight(), "example", color_red);
        graphic_update();
        wait(1000);
    }
}