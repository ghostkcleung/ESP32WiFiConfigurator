#ifndef CONFIGURATOR_HANDLER_H
#define CONFIGURATOR_HANDLER_H

#include <WebServer.h>

namespace DefaultHandler {

    const int RESET_PIN = 32;
    const int RESET_PIN_MODE = INPUT_PULLDOWN;

    void ResetStateHandler(bool isReset);
    void SpiffsBeginHandler(bool success);
    void ConfigFileExistsHandler(const char* configFileName,bool exists);
    void StartAPHandler();
    void StartWebServerHandler(WebServer& webServer);
    void HttpRootHandler();
    void HttpScanHandler(const char* ssidList);
    void HttpSubmitHandler(String ssid, String wifiPassword);
    void ConnectWifiHandler(const char* ssid);
    bool DetermineResetState();
};
#endif