#include "index.h"
#include <sstream>
#include <ArduinoJson.h>

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

void HomeRoute::POST(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    //
    // IMPORTANT NOTE
    // `changeGroup` always has to be called before `changeSlot`
    // as `changeGroup` will reset the slotIndex to 0.
    //
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, (char *)data);
    if (!error)
    {
        if (doc.containsKey("groupIndex"))
        {
            int groupIndex = doc["groupIndex"];
            this->serverController->controller->changeGroup(groupIndex);
        }

        if (doc.containsKey("slotIndex"))
        {
            int slotIndex = doc["slotIndex"];
            this->serverController->controller->changeSlot(slotIndex);
        }

        // this->serverController->controller->sendLight();
        Light light = this->serverController->controller->getLight();
        esp_err_t result = esp_now_send(0, (uint8_t *)&light, sizeof(light));

        if (result == ESP_OK)
        {
            Serial.println("Sent with success");
        }
        else
        {
            Serial.println("Error sending the data");
        }
    }
    else
    {
        Serial.println(error.c_str());
    }

    request->send(200, "application/json", "{\"message\": \"Changes applied.\"}");
}