#include "index.h"



GroupsRoute::GroupsRoute(ServerController *serverController)
    : BaseRoute(serverController)
{
}

std::string GroupsRoute::getRoute()
{
    return "/groups";
}

void GroupsRoute::GET(AsyncWebServerRequest *request)
{
    String path = request->url(); // Get full URL
    Serial.println("Full URL: " + path);

    if (path == "/groups")
    {
        this->GET_all(request);
        return;
    }

    if (path.startsWith("/groups/"))
    {
        this->GET_by_id(request);
        return;
    }
    else
    {
        request->send(400, "application/json", "{\"error\": \"Invalid request\"}");
    }
}


void GroupsRoute::GET_all(AsyncWebServerRequest *request)
{
    Controller *controller = this->serverController->controller;
    int groupCount = 0;
    Group *groups = controller->getGroups(&groupCount);

    String json = Controller::groupOptionsToJson(groups, groupCount);
    request->send(200, "application/json", json);
}

void GroupsRoute::GET_by_id(AsyncWebServerRequest *request)
{
    Controller *controller = this->serverController->controller;
    int groupCount = 0;
    Group *groups = controller->getGroups(&groupCount);

    String path = request->url(); // Get full URL
    Serial.println("Full URL: " + path);

    int id = path.substring(8).toInt();
    if (id < 0 || id >= groupCount)
    {
        request->send(400, "application/json", "{\"error\": \"Invalid group id\"}");
        return;
    }

    String json = Controller::groupToJson(&groups[id]);
    request->send(200, "application/json", json);
}