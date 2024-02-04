//кастыль RET_ERROR я сделал потому что ESP_RETURN_ON_ERROR почему то у меня отсутствует
//#define RET_ERROR(func, ...) { esp_err_t ret = func(__VA_ARGS__); if (ret != ESP_OK) return ret; } // эта реализация сама вызывает функцию
#define RET_ERROR(err) if (err != ESP_OK) return err //а эта принимает значения
#define C_SIZE(arr) sizeof(arr)/sizeof(*arr)

void wait(int time);
void yeild();
void loop();
bool file_exists(const char *filename);
esp_err_t pin_up(uint16_t pin, uint8_t mode);
esp_err_t pin_down(uint16_t pin, uint8_t mode);
esp_err_t pin(uint16_t pin, uint8_t mode);