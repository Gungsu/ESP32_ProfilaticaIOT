#include "htmlServer.h"
#include <SPIFFS.h>

AsyncWebServer server(80);

const char *PARAM_INPUT_1 = "input1";
const char *PARAM_INPUT_2 = "input2";
const char *PARAM_INPUT_3 = "input3";

String html;

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

String processor(const String &var)
{
    if (var == "inputString")
    {
        return readFile(SPIFFS, "/inputString.txt");
    }
    else if (var == "inputInt")
    {
        return readFile(SPIFFS, "/inputInt.txt");
    }
    else if (var == "inputFloat")
    {
        return readFile(SPIFFS, "/inputFloat.txt");
    }
    else if (var == "index_html") {
        return readFile(SPIFFS, "/index.html");
    }
    return String();
}

void htmlSetup() {
    //const char vIndex[] PROGMEM = R"rawliteral(%index_html%)rawliteral";
    // Send web page with input fields to client
    SPIFFS.begin(true);

    html = readFile(SPIFFS, "/index.html");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send(200, "text/html", html); });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
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
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    request->send(200, "text/html", html);
    //request->send_P(200, "text/html", index_html, processor);
    });
    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                html = readFile(SPIFFS, "/confWifi.html");
                request->send(200, "text/html", html); 
                });
    server.on("/azure", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                html = readFile(SPIFFS, "/index.html");
                request->send(200, "text/html", html); });

    server.onNotFound(notFound);
    server.begin();
}