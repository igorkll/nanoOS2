//кастыль ERROR_CHECK я сделал потому что ESP_RETURN_ON_ERROR почему то у меня отсутствует
//#define ERROR_CHECK(func, ...) { esp_err_t ret = func(__VA_ARGS__); if (ret != ESP_OK) return ret; } // эта реализация сама вызывает функцию
#define ERROR_CHECK(err) if (err != ESP_OK) return err //а эта принимает значения
#define C_SIZE(arr) sizeof(arr)/sizeof(*arr)

void wait(int time);
unsigned long uptime();
void yield();
void mYield();
void loop();
esp_err_t pin_up(uint16_t pin, uint8_t mode);
esp_err_t pin_down(uint16_t pin, uint8_t mode);
esp_err_t pin(uint16_t pin, uint8_t mode);