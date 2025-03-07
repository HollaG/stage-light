#pragma once

#include "string"
#include <ESPAsyncWebServer.h>

// Forward declare ServerController to break the circular dependency
class ServerController;
class BaseRoute
{

public:
    ServerController *serverController;
    BaseRoute(ServerController *serverController)
    {
        this->serverController = serverController;
    }

    virtual std::string getRoute();

    virtual void GET(AsyncWebServerRequest *request)
    {
        request->send(404, "text/plain", "Not Found !!");
    };
    virtual void POST(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
    {
        request->send(404, "text/plain", "Not Found !!");
    };
    virtual void PUT(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
    {
        request->send(404, "text/plain", "Not Found !!");
    };
    virtual void DELETE(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
    {
        request->send(404, "text/plain", "Not Found !!");
    };
};