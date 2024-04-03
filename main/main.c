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
#include "drivers/leds.h"

#include "filesystem.h"
#include "nvs.h"
#include "wifi.h"
#include "graphic.h"
#include "color.h"

#include "shell.h"

void init(const char* title, esp_err_t(*func)()) {
    printf("-------- init %s\n", title);
    ESP_ERROR_CHECK_WITHOUT_ABORT(func());
}

void app_main() {
    // init
    init("base", function_init);
    init("leds", leds_init);
    init("screen", screen_init);
    init("filesystem", filesystem_init);
    init("keyboard", keyboard_init);
    init("nvs", nvs_init);
    init("wifi", wifi_init);

    // menu
    shell_run();
}