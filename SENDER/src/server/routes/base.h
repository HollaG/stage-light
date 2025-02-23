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
    virtual void POST(AsyncWebServerRequest *request)
    {
        request->send(404, "text/plain", "Not Found !!");
    };
    virtual void PUT(AsyncWebServerRequest *request)
    {
        request->send(404, "text/plain", "Not Found !!");
    };
    virtual void DELETE(AsyncWebServerRequest *request)
    {
        request->send(404, "text/plain", "Not Found !!");
    };
};