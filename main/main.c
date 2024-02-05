//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/
//nanoOS2(в отличии от своей первой версии) должна быть отказа стойкой
//не и должна ломаться хотябы сама по себе!
//в случаи ошибки - начхать, идти дальше загружаться
//систему делаю по сути чисто для себя
//в системе будет поддержка lua приложений, но я не хочу сильно привязывать ОС к lua
//в идиале оська должна грузиться даже при полностью полетевшей файловой системе(fatfs)
//цитата от дена: красота не главное, главное чтоб работало - а у меня нихуя не красиво и нихуя не работает

#include "main.h"

#include "drivers/keyboard.h"
#include "drivers/screen.h"

#include "filesystem.h"
#include "nvs.h"
#include "wifi.h"
#include "graphic.h"
#include "color.h"
#include "gui.h"

void init() {
    wdt_hal_disable();
    
    printf("-------- init screen\n");
    ESP_ERROR_CHECK_WITHOUT_ABORT(screen_init());

    printf("-------- init filesystem\n");
    ESP_ERROR_CHECK_WITHOUT_ABORT(filesystem_init());

    printf("-------- init keyboard\n");
    ESP_ERROR_CHECK_WITHOUT_ABORT(keyboard_init());

    printf("-------- init nvs\n");
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_init());

    printf("-------- init wifi\n");
    ESP_ERROR_CHECK_WITHOUT_ABORT(wifi_init());
}

void menu() {

}

void app_main() {
    init();
    
    graphic_clear(color_black);
    graphic_drawRect(1, 1, 4, 4, color_red);
    graphic_drawText(1, 6, "WHAT????", color_red);
    graphic_update();
    wait(1000);

    char* strs[] = {"wifi", "bluetouch", "test", "explorer", "settings", "scrap mechanic", "garry`s mod", "GTA 5", "TOP BIOS"};
    while (true) {
        gui_menu("cirtis", C_SIZE(strs), strs);
        gui_splash("line1\nline2\n123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
        gui_splash("what????");
        gui_splash("12345");
        gui_splash("JOPA!!!!");
    }

    loop();
}