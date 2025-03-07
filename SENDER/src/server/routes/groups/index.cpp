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

    // strip the /api part
    path = path.substring(4);

    if (request->hasParam("select"))
    {
        String groupId = request->getParam("select")->value();

        // try to convert to integer
        int groupIndex = groupId.toInt();

        // TODO: data validation
        this->serverController->controller->changeGroup(groupIndex);

        // return 200
        request->send(200, "application/json", "{\"message\": \"Group changed\"}");
    }

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

    int id = path.substring(8 + 4).toInt();
    Serial.printf("Id = %d GroupCount = %d\n", id, groupCount);
    if (id < 0 || id >= groupCount)
    {
        request->send(400, "application/json", "{\"error\": \"Invalid group id\"}");
        return;
    }

    String json = Controller::groupToJson(&groups[id], id);
    request->send(200, "application/json", json);
}

/**
 * Body:
 * {
 * "name": "Group name",
 * id: number // the index at which to insert the group.
 * TODO: do a system where we can insert inbetween. Currently, we just override the group at the index.
 * }
 */
void GroupsRoute::POST(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    //
    // IMPORTANT NOTE
    // `changeGroup` always has to be called before `changeSlot`
    // as `changeGroup` will reset the slotIndex to 0.
    //
    // DynamicJsonDocument doc(200);
    // DeserializationError error = deserializeJson(doc, (char *)data);
    // if (!error)
    // {
    //     if (doc.containsKey("group"))
    //     {
    //         int groupIndex = doc["group"];
    //         this->serverController->controller->changeGroup(groupIndex);
    //     }

    //     if (doc.containsKey("slot"))
    //     {
    //         int slotIndex = doc["slot"];
    //         this->serverController->controller->changeSlot(slotIndex);
    //     }

    // }
    // else
    // {
    //     Serial.println(error.c_str());
    // }

    // request->send(200, "application/json", "{\"message\": \"Changes applied.\"}");
}