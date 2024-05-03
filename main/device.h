void device_setAutoBacklight(bool);
bool device_isAutoBacklight();
void device_update();
void device_setBacklightValue(uint8_t value);
uint8_t device_getBacklightValue();
esp_err_t device_init();