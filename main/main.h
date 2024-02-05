#include <sdkconfig.h>

#include <stdint.h>
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

#include <esp_flash.h>
#include <esp_flash_spi_init.h>
#include <esp_partition.h>
#include <esp_vfs.h>
#include <esp_vfs_fat.h>
#include <soc/spi_pins.h>
#include <esp_task_wdt.h>

#include "functions.h"