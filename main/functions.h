//кастыль ERROR_CHECK я сделал потому что ESP_RETURN_ON_ERROR почему то у меня отсутствует
//#define ERROR_CHECK(func, ...) { esp_err_t ret = func(__VA_ARGS__); if (ret != ESP_OK) return ret; } // эта реализация сама вызывает функцию
#define ERROR_CHECK(err) if (err != ESP_OK) return err //а эта принимает значения
#define C_SIZE(arr) sizeof(arr)/sizeof(*arr)
#define C_CLEAR(arr) memset(arr, 0, sizeof(arr));
#define C_FREE_LST(arr, size) for (int _i = 0; _i < size; _i++) free(arr[_i])
#define C_FREE_LST_AUTO(arr) C_FREE_LST(arr, sizeof(arr))

#define nmax(a,b) (a > b ? a : b)
#define nmin(a,b) (a < b ? a : b)

void wait(int time);
bool waitUntil(int time, bool(*)());
unsigned long uptime();
void yield();
void mYield();
void loop();
int nRound(float num);
float clamp(float n, float lower, float upper);
int map(int value, int low, int high, int low_2, int high_2);
float fmap(float value, float low, float high, float low_2, float high_2);
int rmap(int value, int low, int high, int low_2, int high_2);
uint8_t CRTValue(uint8_t val);
esp_err_t pin_up(uint16_t pin, uint8_t mode);
esp_err_t pin_down(uint16_t pin, uint8_t mode);
esp_err_t pin(uint16_t pin, uint8_t mode);
esp_err_t function_init();