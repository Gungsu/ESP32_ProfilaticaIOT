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
                  // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
                  if (request->hasParam(PARAM_INPUT_1))
                  {
                      inputMessage = request->getParam(PARAM_INPUT_1)->value();
                      inputParam = PARAM_INPUT_1;
                      Serial.print(inputMessage);
                      Serial.print(" ");
                      Serial.println(inputParam);
                      inputMessage = request->getParam(PARAM_INPUT_2)->value();
                      inputParam = PARAM_INPUT_2;
                      Serial.print(inputMessage);
                      Serial.print(" ");
                      Serial.println(inputParam);
                      inputMessage = request->getParam(PARAM_INPUT_3)->value();
                      inputParam = PARAM_INPUT_3;
                      Serial.print(inputMessage);
                      Serial.print(" ");
                      Serial.println(inputParam);
                  }
                  else if (request->hasParam(PARAM_INPUT_4))
                  {
                      inputMessage = request->getParam(PARAM_INPUT_4)->value();
                      JsonDocument doc;
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
                      char payload[128];
                      serializeJson(doc, payload);
                      writeFile(SPIFFS, "/ssid_conf.json", payload);
                      WiFi.disconnect();
                  }
                  else
                  {
                      inputMessage = "No message sent";
                      inputParam = "none";
                  }
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
        this->ssid_data_html = String(ssid);
        this->ssid_pass_data_html = String(pass);
        this->nFile = String(nFileH);
        return true;
    } else {
        return false;
    }
    return false;
}

void updateConfWif(String ssid, String pass) {
    JsonDocument doc;
    doc["ssid"] = ssid;
    doc["pass"] = pass;
    doc["nFile"] = "false";
    char payload[128];
    serializeJson(doc, payload);
    writeFile(SPIFFS, "/ssid_conf.json", payload);
    delay(50);
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