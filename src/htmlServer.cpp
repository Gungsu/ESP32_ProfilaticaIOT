#include "htmlServer.h"

AsyncWebServer server(80);

const char *PARAM_INPUT_1 = "input1";
const char *PARAM_INPUT_2 = "input2";
const char *PARAM_INPUT_3 = "input3";
const char *PARAM_INPUT_4 = "ssid";
const char *PARAM_INPUT_5 = "pass";
const char *PARAM_INPUT_6 = "input4"; //Lote

String html;
String respH;
String doc1, doc2, doc3, doc4, doc5, doc6, doc7;
File file;

void notFound(AsyncWebServerRequest * request)
{
    request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char *path)
{
    String fileContent;
    //Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        //Serial.println("- failed to open file for reading");
        return "";
    }

    //Serial.println("- read from file:");
    while (file.available())
    {
        fileContent += file.readString();
    }
    file.close();
    return fileContent;
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
    //Serial.printf("Writing file: %s\r\n", path);
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
    file = fs.open(path, "r");
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
    html = readFile(SPIFFS, "/index.html");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send(200, "text/html", html); });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  String inputMessage;
                  String inputParam;
                  char payload[256];
                  deserializeJson(SPIFFS, "/ssid_conf.json");
                  // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
                  if (request->hasParam(PARAM_INPUT_1))
                  {
                      inputMessage = request->getParam(PARAM_INPUT_1)->value();
                      inputParam = PARAM_INPUT_1; // ID_SCOPE
                      Serial.print(inputMessage);
                      Serial.print(" ");
                      Serial.println(inputParam);
                      doc4 = inputMessage;
                      inputMessage = request->getParam(PARAM_INPUT_2)->value();
                      inputParam = PARAM_INPUT_2; // IOT_DEVICE_ID
                      Serial.print(inputMessage);
                      Serial.print(" ");
                      Serial.println(inputParam);
                      doc5 = inputMessage;
                      inputMessage = request->getParam(PARAM_INPUT_3)->value();
                      inputParam = PARAM_INPUT_3; // IOT_DEVICE_KEY
                      Serial.print(inputMessage);
                      Serial.print(" ");
                      Serial.println(inputParam);
                      doc6 = inputMessage;
                      inputMessage = request->getParam(PARAM_INPUT_6)->value();
                      inputParam = PARAM_INPUT_6; // LOTE
                      Serial.print(inputMessage);
                      Serial.print(" ");
                      Serial.println(inputParam);
                      doc7 = inputMessage;
                      serializeJson(SPIFFS, "/ssid_conf.json");
                      WiFi.disconnect();
                  }
                  else if (request->hasParam(PARAM_INPUT_4))
                  {
                      inputMessage = request->getParam(PARAM_INPUT_4)->value();
                      inputParam = PARAM_INPUT_4;
                      Serial.print(inputMessage);
                      Serial.print(" ");
                      Serial.println(inputParam);
                      doc1 = inputMessage; //SSID
                      inputMessage = request->getParam(PARAM_INPUT_5)->value();
                      inputParam = PARAM_INPUT_5;
                      Serial.print(inputMessage);
                      Serial.print(" ");
                      Serial.println(inputParam);
                      doc2 = inputMessage; //PASS
                      doc3 = "true";
                      serializeJson(SPIFFS, "/ssid_conf.json");
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
                  html = editFile(SPIFFS, "/confWifi.html");
                  request->send(200, "text/html", html); });
    server.on("/azure", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                html = readFile(SPIFFS, "/index.html");
                request->send(200, "text/html", html); });

    server.onNotFound(notFound);
    server.begin();
}

bool ConnectWifiByDataHtml::consultExist(fs::FS &fs, const char *path) {
    file = fs.open(path, "r");
    if (!file || file.isDirectory())
    {
        return false;
    }
    return true;
}

void deserializeJson(fs::FS &fs, const char *path)
{
    file = fs.open(path);
    if (!file || file.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return;
    }
    uint8_t lineCont = 0;
    while (file.available())
    {
        String line = file.readStringUntil('\n');
        lineCont++;
        int8_t pos = line.indexOf(':');

        if (pos != -1)
        {
            String parte1 = line.substring(1, pos - 1);
            int8_t x = line.indexOf(',');
            String parte2 = line.substring(pos + 3, line.length()-2);
            if (x == -1)
            {
                parte2 = line.substring(pos + 3, line.length() - 1);
            }
            if (parte1 == "ssid")
            {
                doc1 = parte2;
            }
            else if (parte1 == "pass")
            {
                doc2 = parte2;
            }
            else if (parte1 == "nFile")
            {
                doc3 = parte2;
            }
            else if (parte1 == "scope")
            {
                doc4 = parte2;
            }
            else if (parte1 == "devID")
            {
                doc5 = parte2;
            }
            else if (parte1 == "devKey")
            {
                doc6 = parte2;
            }
            else if (parte1 == "lote")
            {
                doc7 = parte2;
            }
        }
    }
    file.close(); // Fecha o arquivo
}

void serializeJson(fs::FS &fs, const char *path)
{
    char text[258] = "";
    String allthetext = "";
    memset(text, '\0', 258);
    allthetext = "{\r\n\"ssid\": \""+doc1+"\",\n\"pass\": \""+doc2+"\",\n";
    allthetext += "\"nFile\": \""+doc3+"\",\n\"scope\": \""+doc4+"\",\n\"devID\": \""+doc5+"\",\n\"devKey\": \"";
    allthetext += doc6 + "\",\n\"lote\": \""+doc7+"\",\n}\r\n";
    strcpy(text,allthetext.c_str());
    allthetext = "";
    writeFile(SPIFFS, path, text);

    //Serial.print(text);
}

void ConnectWifiByDataHtml::readDeviceConf()
{
        deserializeJson(SPIFFS, "/ssid_conf.json");
        ssid_data_html = doc1;
        ssid_pass_data_html = doc2;
        nFile = doc3;
        scope = doc4;
        devID = doc5;
        devKey = doc6;
        loteSave = doc7;
}

bool ConnectWifiByDataHtml::existDataFile()
{
    if (consultExist(SPIFFS, "/ssid_conf.json"))
    {
        deserializeJson(SPIFFS, "/ssid_conf.json");
        ssid_data_html = doc1;
        ssid_pass_data_html = doc2;
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

void updateConfWif(String ssid, String pass) {
    doc1 = ssid;
    doc2 = pass;
    doc3 = "false";
    serializeJson(SPIFFS, "/ssid_conf.json");
    delay(50);
}

bool readNFileValue() { //ATUALIZAR QUANDO CHAMAR
    if (doc3 == "true"){
        return true;
    }
    return false;
}

String readValueFJSON(String val)
{
    deserializeJson(SPIFFS, "/ssid_conf.json");
    if (val == "ssid")
    {
        return doc1;
    }
    else if (val == "pass")
    {
        return doc2;
    }
    else if (val == "nFile")
    {
        return doc3;
    }
    else if (val == "scope")
    {
        return doc4;
    }
    else if (val == "devID")
    {
        return doc5;
    }
    else if (val == "devKey")
    {
        return doc6;
    }
    else if (val == "lote")
    {
        return doc7;
    }
    return "0";
}

void updateLote(char *lote, uint16_t lenght)
{
    char nLote[128];
    memset(nLote, '\0', sizeof(nLote));
    strncpy(nLote, lote, lenght);
    doc7 = String(nLote);
    char payload[256];
    serializeJson(SPIFFS, "/ssid_conf.json");
    delay(50);
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