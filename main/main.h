#pragma message("main.h")

#include <sdkconfig.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#include <lwip/err.h>
#include <lwip/sys.h>

#include <esp_system.h>
#include <esp_log.h>

#include <sys/unistd.h>
#include <sys/stat.h>

#include <esp_err.h>
#include <esp_random.h>
#include <esp_event.h>

#include <driver/gpio.h>

// ----------------

#include <functions.h>