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

#include "fs.h"
#include "nvs.h"
#include "wifi.h"
#include "ftp.h"
#include "graphic.h"
#include "lua.h"

void app_ftp() {
    printf("-------- run ftp\n");
    ftp_run(4000);
    loop();
}

void app_lua() {
    printf("-------- run lua\n");
    lua_run();
    graphic_image(0, 0, "/storage/img/fb.bmp");
    screen_update();
    loop();
}

void app_main() {
    printf("-------- init screen\n");
    ESP_ERROR_CHECK_WITHOUT_ABORT(screen_init());

    printf("-------- init fs\n");
    ESP_ERROR_CHECK_WITHOUT_ABORT(fs_init());

    graphic_image(0, 0, "/storage/img/logo.bmp");
    screen_update();
    wait(1000);

    printf("-------- init keyboard\n");
    ESP_ERROR_CHECK_WITHOUT_ABORT(keyboard_init());

    printf("-------- init nvs\n");
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_init());

    printf("-------- init wifi\n");
    ESP_ERROR_CHECK_WITHOUT_ABORT(wifi_init());

    xTaskCreate(app_ftp, "ftp", 1024 * 8, NULL, 1, NULL);
    xTaskCreate(app_lua, "lua", 1024 * 8, NULL, 1, NULL);
    loop();
}