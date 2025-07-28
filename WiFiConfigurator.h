#ifndef WIFI_CONFIGURATOR_H
#define WIFI_CONFIGURATOR_H

#include "DefaultHandler.h"

class WiFiConfigurator {
    private:
    static WebServer* webServer;

    static void checkResetState();
    static bool isConfigFileExists();
    static void connectWiFi();
    static void startAP();
    static void startWebServer();
    static void onRoot();
    static void onScan();

    public:
    static String configFileName;
    static Print& printStream;

    static void start ( );
    static bool (*DetermineResetState)();
    static void (*ResetStateHandler)(bool);
    static void (*SpiffsBeginHandler)(bool);
    static void (*ConfigFileExistsHandler)(const char*, bool);
    static void (*StartAPHandler)();
    static void (*StartWebServerHandler)(WebServer& webServer);
    static void (*HttpRootHandler)();
    static void (*HttpScanHandler)(const char* ssidList);
    static void (*HttpSubmitHandler)(String ssid, String wifiPassword);
    static void (*ConnectWifiHandler)(const char* ssid);

    static void clearAllHandlers ();
    static void restoreDefaultHandlers ();
};

#endif