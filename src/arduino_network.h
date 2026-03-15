#ifndef PONG_NETWORK_H
#define PONG_NETWORK_H
#include "Arduino.h"

void setup_network();
void printMacAddress(byte mac[]);
void printWifiData();
void printCurrentNet();
void web_server();

#endif
