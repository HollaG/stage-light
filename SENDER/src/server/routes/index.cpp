#include "index.h"
#include <sstream>

void HomeRoute::GET(AsyncWebServerRequest *request)
{
   // send the groupIndex and slotIndex as json
    // {
    //     groupIndex, slotIndex
    // }

    std::stringstream ss;
    ss << "{ \"groupIndex\": " << this->serverController->controller->groupIndex << ", \"slotIndex\": " << this->serverController->controller->slotIndex << " }";

    String response = ss.str().c_str();
    request->send(200, "application/json", response);


}