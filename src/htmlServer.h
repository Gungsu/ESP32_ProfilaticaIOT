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
        uint8_t numBssid;
        String resp;
        bool existDataFile();
        void readSSID();
        void updateListSSID();
        ConnectWifiByDataHtml()
        {
            SPIFFS.begin(true);
        }

    private:
        bool consultExist(fs::FS &fs, const char *path);
    
    
};

void notFound(AsyncWebServerRequest *request);
void htmlSetup();
String processor(const String &var);
void updateConfWif(String ssid, String pass);
bool readNFileValue();

String readFile(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);

#endif