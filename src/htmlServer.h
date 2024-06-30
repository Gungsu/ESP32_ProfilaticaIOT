#ifndef HTMLSERVER
#define HTMLSERVER

#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#define SERVER_HOST_NAME "esp_server"

class ConnectWifiByDataHtml {
    public:
        String ssid_data_html, ssid_pass_data_html;
        String nFile;
        String loteSave;
        String scope;
        String devID;
        String devKey;
        uint8_t numBssid;
        String resp;
        bool existDataFile();
        void readSSID();
        void updateListSSID();
        void readDeviceConf();
        ConnectWifiByDataHtml()
        {
            SPIFFS.begin(true);
            readDeviceConf();
        }

    private:
        bool consultExist(fs::FS &fs, const char *path);
};

void notFound(AsyncWebServerRequest *request);
void htmlSetup();
String processor(const String &var);
void updateConfWif(String ssid, String pass);
void updateLote(char *lote, uint16_t leng);
String readValueFJSON(String val);
bool readNFileValue();

String readFile(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);

#endif