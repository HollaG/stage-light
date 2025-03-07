#include "EspNowConnection.h"

// constructor
EspNowConnection::EspNowConnection()
{
  for (int i = 0; i < 20; i++)
  {
    for (int j = 0; j < 6; j++)
    {
      broadcastAddresses[i][j] = 0;
    }
  }
}

void EspNowConnection::addMacAddress(uint8_t *broadcastAddress)
{
  for (int i = 0; i < 20; i++)
  {
    if (broadcastAddresses[i][0] == 0)
    {
      for (int j = 0; j < 6; j++)
      {
        broadcastAddresses[i][j] = broadcastAddress[j];
      }
      break;
    }
  }
  peerCount++;
}

void EspNowConnection::setup()
{
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // register peer
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // register first peer
  for (int i = 0; i < peerCount; i++)
  {
    Serial.printf("[EspNowConnection] Adding peer %d\n", i);
    memcpy(peerInfo.peer_addr, broadcastAddresses[i], 6);
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
      Serial.println("Failed to add peer");
      return;
    }
  }
}

// callback when data is sent
void EspNowConnection::OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

esp_err_t EspNowConnection::send(Light *change)
{
  Serial.printf("[EspNowConnection] Sending message: R %d, G %d, B %d\n", change->r, change->g, change->b);
  esp_err_t result = esp_now_send(0, (uint8_t *)&change, sizeof(change));

  if (result == ESP_OK)
  {
    Serial.println("Sent with success");
  }
  else
  {
    Serial.println("Error sending the data");
  }

  return result;
}