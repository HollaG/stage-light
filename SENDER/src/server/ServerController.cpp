#include "ServerController.h"

ServerController::ServerController() : server(80)
{
}

void ServerController::begin(Controller *controller)
{
  this->controller = controller;
  Serial.println("Starting Web Server...");
  Serial.println(WiFi.softAPIP());

  registerRoutes();

  server.begin();
}

void ServerController::registerRoutes()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", "hello-world"); });

  server.on("/groups", HTTP_GET, [this](AsyncWebServerRequest *request)
            {
        String path = request->url(); // Get full URL
        Serial.println("Full URL: " + path);

        int groupCount = 0;
        Group *groups = (this->controller)->getGroups(&groupCount);

        if (path == "/groups")
        {
            String json = Controller::groupOptionsToJson(groups, groupCount);
            request->send(200, "application/json", json);
            return;
        }

        if (path.startsWith("/groups/"))
        {
            String idStr = path.substring(8); // Get substring after "/groups/"
            int groupId = idStr.toInt();      // Convert to integer

            Serial.printf("Requested Group ID: %d\n", groupId);
            if (groupId >= 0 && groupId < 20)
            {
                String json = Controller::groupToJson(&groups[groupId]);
                request->send(200, "application/json", json);
            }
            else
            {
                request->send(404, "application/json", "{\"error\": \"Invalid Group ID\"}");
            }
        }
        else
        {
            request->send(400, "application/json", "{\"error\": \"Invalid request\"}");
        } });
}
