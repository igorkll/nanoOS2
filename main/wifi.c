#include "main.h"
#include <esp_wifi.h>
#include <esp_mac.h>

#define ESP_WIFI_CHANNEL                    0
#define MAX_STA_CONN                        16 //скоко максимальна пользователей при раздаче смогут подключиться

const char *WIFI_TAG = "wifi";

// -------------------------------- AP

static void wifi_ap_eventhandler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(WIFI_TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(WIFI_TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

esp_err_t wifi_accesspoint(const char* ssid, const char* password) {
    // -------- make config
    
    int channel = ESP_WIFI_CHANNEL;
    wifi_config_t wifi_config = {
        .ap = {
            .ssid_len = strlen(ssid),
            .channel = channel,
            .max_connection = MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .pmf_cfg = {
                .required = false,
            },
        },
    };

    strcpy((char *)wifi_config.ap.ssid, ssid);
    strcpy((char *)wifi_config.ap.password, password);

    if (strlen(password) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    // -------- wifi init

    esp_netif_create_default_wifi_ap();
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_ap_eventhandler, NULL, NULL));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_start());
    xprintf("accesspoint created. name:\"%s\"\n", ssid);
    return ESP_OK;
}

// -------------------------------- STA

#define ESP_MAXIMUM_RETRY                   4

#define WIFI_CONNECTED_BIT                  BIT0
#define WIFI_FAIL_BIT                       BIT1

#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD   WIFI_AUTH_OPEN
#define ESP_WIFI_SAE_MODE                   WPA3_SAE_PWE_HUNT_AND_PECK
#define H2E_IDENTIFIER                      ""

static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;
static void wifi_sta_eventhandler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(WIFI_TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(WIFI_TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(WIFI_TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

esp_err_t wifi_connect(const char* ssid, const char* password) {
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    s_wifi_event_group = xEventGroupCreate();
    ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_sta_eventhandler,
                                                        NULL,
                                                        &instance_any_id));
    ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_sta_eventhandler,
                                                        NULL,
                                                        &instance_got_ip));

    // -------- make config

    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
            .sae_pwe_h2e = ESP_WIFI_SAE_MODE,
            .sae_h2e_identifier = H2E_IDENTIFIER,
        },
    };

    strcpy((char *)wifi_config.sta.ssid, ssid);
    strcpy((char *)wifi_config.sta.password, password);

    // -------- wifi init
    
    esp_netif_create_default_wifi_sta();
    ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        xprintf("connected to wifi. name:\"%s\"\n", ssid);
    } else if (bits & WIFI_FAIL_BIT) {
        xprintf("failed connect to wifi. name:\"%s\"\n", ssid);
        return ESP_ERR_WIFI_BASE;
    } else {
        return ESP_ERR_WIFI_BASE;
    }

    return ESP_OK;
}

// -------------------------------- BASE

esp_err_t wifi_init() {
    ERROR_CHECK(esp_netif_init());
    ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ERROR_CHECK(esp_wifi_init(&cfg));
    
    return ESP_OK;
}

esp_err_t wifi_stop() {
    ERROR_CHECK(esp_wifi_stop());
    xprintf("wifi disabled");
    return ESP_OK;
}