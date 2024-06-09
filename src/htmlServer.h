#ifndef HTMLSERVER
#define HTMLSERVER

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#define SERVER_HOST_NAME "esp_server"

void notFound(AsyncWebServerRequest *request);
void htmlSetup();
String processor(const String &var);
String readFile(fs::FS &fs, const char *path);

void writeFile(fs::FS &fs, const char *path, const char *message);

#endif