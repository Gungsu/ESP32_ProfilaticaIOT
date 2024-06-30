#include "htmlServer.h"

AsyncWebServer server(80);

const char *PARAM_INPUT_1 = "input1";
const char *PARAM_INPUT_2 = "input2";
const char *PARAM_INPUT_3 = "input3";
const char *PARAM_INPUT_4 = "ssid";
const char *PARAM_INPUT_5 = "pass";

String html;
String respH;

void notFound(AsyncWebServerRequest * request)
{
    request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char *path)
{
    //Serial.printf("Reading file: %s\r\n", path);
    File file = fs.open(path, "r");
    if (!file || file.isDirectory())
    {
        //Serial.println("- empty file or failed to open file");
        return String();
    }
    //Serial.println("- read from file:");
    String fileContent;
    while (file.available())
    {
        fileContent += String((char)file.read());
    }
    file.close();
    //Serial.println(fileContent);
    return fileContent;
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\r\n", path);
    File file = fs.open(path, "w");
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("- file written");
    }
    else
    {
        Serial.println("- write failed");
    }
    file.close();
}

String editFile(fs::FS &fs, const char *path)
{
    // Serial.printf("Reading file: %s\r\n", path);
    File file = fs.open(path, "r");
    // Serial.println("- read from file:");
    String fileContent;
    String varTest;
    uint8_t maxCarc;
    char act;
    while (file.available())
    {
        act = file.read();
        fileContent += String((char)act);
    }
    file.close();
    fileContent.replace("%LISTA_WIFI%", respH);
    return fileContent;
}

void htmlSetup() {
    //const char vIndex[] PROGMEM = R"rawliteral(%index_html%)rawliteral";
    // Send web page with input fields to client

    html = readFile(SPIFFS, "/index.html");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send(200, "text/html", html); });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                String inputMessage;
                String inputParam;
                char payload[256];
                JsonDocument doc;
                String fileReJSON = readFile(SPIFFS,"/ssid_conf.json");
                deserializeJson(doc, fileReJSON);
                // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
                if (request->hasParam(PARAM_INPUT_1))
                {
                    inputMessage = request->getParam(PARAM_INPUT_1)->value();
                    inputParam = PARAM_INPUT_1; //ID_SCOPE
                    Serial.print(inputMessage);
                    Serial.print(" ");
                    Serial.println(inputParam);
                    doc["scope"] = inputMessage;
                    inputMessage = request->getParam(PARAM_INPUT_2)->value();
                    inputParam = PARAM_INPUT_2; //IOT_DEVICE_ID
                    Serial.print(inputMessage);
                    Serial.print(" ");
                    Serial.println(inputParam);
                    doc["devID"] = inputMessage;
                    inputMessage = request->getParam(PARAM_INPUT_3)->value();
                    inputParam = PARAM_INPUT_3; //IOT_DEVICE_KEY
                    Serial.print(inputMessage);
                    Serial.print(" ");
                    Serial.println(inputParam);
                    doc["devKey"] = inputMessage;
                }
                else if (request->hasParam(PARAM_INPUT_4))
                {
                    inputMessage = request->getParam(PARAM_INPUT_4)->value();
                    inputParam = PARAM_INPUT_4;
                    Serial.print(inputMessage);
                    Serial.print(" ");
                    Serial.println(inputParam);
                    doc["ssid"] = inputMessage;
                    inputMessage = request->getParam(PARAM_INPUT_5)->value();
                    inputParam = PARAM_INPUT_5;
                    Serial.print(inputMessage);
                    Serial.print(" ");
                    Serial.println(inputParam);                    
                    doc["pass"] = inputMessage;
                    doc["nFile"] = "true";
                    WiFi.disconnect();
                }
                else
                {
                    inputMessage = "No message sent";
                    inputParam = "none";
                }
                serializeJson(doc, payload);
                writeFile(SPIFFS, "/ssid_conf.json", payload);
                request->send(200, "text/html", html);
                  // request->send_P(200, "text/html", index_html, processor);
              });
    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request)
              {

                html = editFile(SPIFFS,"/confWifi.html");
                request->send(200, "text/html", html);

                });
    server.on("/azure", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                html = readFile(SPIFFS, "/index.html");
                request->send(200, "text/html", html); });

    server.onNotFound(notFound);
    server.begin();
}

void ConnectWifiByDataHtml::readDeviceConf() {
    JsonDocument doc;
    String fileReJSON = readFile(SPIFFS,"/ssid_conf.json");
    deserializeJson(doc, fileReJSON);
    const char *i1 = doc["scope"];
    scope = String(i1);
    const char *i2 = doc["devID"];
    devID = String(i2);
    const char *i3 = doc["devKey"];
    devKey = String(i3);
}

bool ConnectWifiByDataHtml::consultExist(fs::FS &fs, const char *path) {
    File file = fs.open(path, "r");
    if (!file || file.isDirectory())
    {
        return false;
    }
    return true;
}

bool ConnectWifiByDataHtml::existDataFile()
{
    if (consultExist(SPIFFS, "/ssid_conf.json")) {
        String values = readFile(SPIFFS, "/ssid_conf.json");
        JsonDocument doc;
        deserializeJson(doc, values);
        const char *ssid = doc["ssid"];
        const char *pass = doc["pass"];
        const char *nFileH = doc["nFile"];
        const char *loteH = doc["lote"];
        this->ssid_data_html = String(ssid);
        this->ssid_pass_data_html = String(pass);
        this->nFile = String(nFileH);
        this->loteSave = String(loteH);
        return true;
    } else {
        return false;
    }
    return false;
}

void updateConfWif(String ssid, String pass) {
    String values = readFile(SPIFFS, "/ssid_conf.json");
    JsonDocument doc;
    deserializeJson(doc, values);
    doc["ssid"] = ssid;
    doc["pass"] = pass;
    doc["nFile"] = "false";
    char payload[256];
    serializeJson(doc, payload);
    writeFile(SPIFFS, "/ssid_conf.json", payload);
    delay(50);
}

void updateLote(char *lote, uint16_t lenght) {
    String values = readFile(SPIFFS, "/ssid_conf.json");
    JsonDocument doc;
    deserializeJson(doc, values);
    char nLote[128];
    memset(nLote,'\0',sizeof(nLote));
    strncpy(nLote,lote,lenght-1);
    doc["lote"] = String(nLote);
    char payload[256];
    serializeJson(doc, payload);
    writeFile(SPIFFS, "/ssid_conf.json", payload);
    //Serial.println(payload);
    delay(50);
}

String readValueFJSON(String val) {
    String values = readFile(SPIFFS, "/ssid_conf.json");
    JsonDocument doc;
    deserializeJson(doc, values);
    Serial.println(values);
    return doc[val];
}

bool readNFileValue() {
    String values = readFile(SPIFFS, "/ssid_conf.json");
    JsonDocument doc;
    const char *value;
    deserializeJson(doc, values);
    value = doc["nFile"];
    if (value[0] == 'f') {
        return false;
    } else {
        return true;
    }
    return false;
}

void ConnectWifiByDataHtml::updateListSSID() {
    this->numBssid = WiFi.scanNetworks();
    this->resp = "<option value = ''>Select SSID</ option>";
    for (uint8_t i = 0; i < this->numBssid; i++)
    {
        this->resp += "<option value = '";
        this->resp += WiFi.SSID(i);
        this->resp += "'>";
        this->resp += WiFi.SSID(i);
        this->resp += "</ option>";
    }
    respH = this->resp;
}