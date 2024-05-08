#include "../../main/main.h"
#include "../../main/system.h"

#ifdef SCREEN_BASESPI
    #define _INIT_SCREEN_SPI(bufferSize) spi_bus_config_t buscfg = system_baseSPI
#else
    #define _INIT_SCREEN_SPI(bufferSize) \
        spi_bus_config_t buscfg={ \
            .mosi_io_num=SCREEN_DIN, \
            .sclk_io_num=SCREEN_CLK, \
            .miso_io_num=-1, \
            .quadwp_io_num=-1, \
            .quadhd_io_num=-1, \
            .max_transfer_sz=bufferSize \
        }; \
        ret = spi_bus_initialize(SCREEN_SPI, &buscfg, SPI_DMA_CH_AUTO); \
        if (ret != ESP_OK) return ret
#endif