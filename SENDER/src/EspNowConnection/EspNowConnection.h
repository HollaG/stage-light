#pragma once
#include "string"
#include <esp_now.h>
#include <WiFi.h>
#include <structs/structs.h>

class EspNowConnection
{
  uint8_t broadcastAddresses[20][6];
  int peerCount = 0;

  esp_now_peer_info_t peerInfo;
  CHANGE_MESSAGE change;

public:
  // constructor
  EspNowConnection();

  void addMacAddress(uint8_t *broadcastAddress);
  void setup();
  static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
  esp_err_t send(Light *light);
};