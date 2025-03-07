#include "ServerController.h"
#include "static_files.h"
#include <pgmspace.h>

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
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

    HomeRoute *home = new HomeRoute(this);
    GroupsRoute *groups = new GroupsRoute(this);
    SlotsRoute *slots = new SlotsRoute(this);

    // array of routes
    BaseRoute *routes[] = {slots, groups, home};

    // for each route, register all verbs
    for (int i = 0; i < sizeof(routes) / sizeof(routes[0]); i++)
    {
        std::string route = routes[i]->getRoute(); // add /api in front
        route = "/api" + route;

        Serial.println(route.c_str());
        server.on(route.c_str(), HTTP_GET, [routes, i](AsyncWebServerRequest *request)
                  { routes[i]->GET(request); });

        // server.on(route.c_str(), HTTP_POST, [routes, i](AsyncWebServerRequest *request)
        //           { routes[i]->POST(request); });
        server.on(route.c_str(), HTTP_POST, [routes, i](AsyncWebServerRequest *request) {}, [routes, i](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {}, [routes, i](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                  { routes[i]->POST(request, data, len, index, total); });

        server.on(route.c_str(), HTTP_PUT, [routes, i](AsyncWebServerRequest *request) {}, [routes, i](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {}, [routes, i](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                  { routes[i]->PUT(request, data, len, index, total); });
        server.on(route.c_str(), HTTP_DELETE, [routes, i](AsyncWebServerRequest *request) {}, [routes, i](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {}, [routes, i](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                  { routes[i]->DELETE(request, data, len, index, total); });
    }

    // Finally, create a route for each of the build artifacts.
    // If you look in the static_files.h, at the bottom you will see the files[].
    // In there, each file has a .path.
    // This creates a route for each path, so if you have test.html, you would be able to access it at example.com/test
    // Or if you have test.png, you would be able to access it at example.com/test.png
    for (int i = 0; i < static_files::num_of_files; i++)
    {
        server.on(static_files::files[i].path, [i](AsyncWebServerRequest *request)
                  {
                      AsyncWebServerResponse *response = request->beginResponse(static_files::files[i].type, static_files::files[i].size, [i](uint8_t *buffer, size_t maxLen, size_t index) -> size_t
                                                                                {
                        size_t toRead = maxLen;
                        if (index + toRead > static_files::files[i].size) {
                            toRead = static_files::files[i].size - index;
                        }
                        memcpy_P(buffer, static_files::files[i].contents + index, toRead);
                        return toRead; });
                      response->addHeader("Content-Encoding", "gzip");
                      request->send(response); });
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", static_files::f_index_html_contents, static_files::f_index_html_size);
        response->addHeader("Content-Encoding", "gzip");  // Set gzip encoding header
        request->send(response); });
};
