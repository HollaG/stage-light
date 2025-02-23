#pragma once
#include "server/routes/base.h"
#include "server/ServerController.h"

class HomeRoute : public BaseRoute
{
public:
    explicit HomeRoute(ServerController *serverController) : BaseRoute(serverController) {}

    std::string getRoute()
    {
        return "/";
    }

    void GET(AsyncWebServerRequest *request);
};