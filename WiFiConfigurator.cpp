#include <SPIFFS.h>
#include "WiFiConfigurator.h"
#include "ConfiguratorHTML.h"

void WiFiConfigurator::start ( ) {

    checkResetState();

    if ( isConfigFileExists() ) {
        connectWiFi();
    } else {
        startAP();
        startWebServer();
    }
}

void WiFiConfigurator::checkResetState ( ) {
    bool resetState = DetermineResetState ( );
    bool success;

    do {
        success = SPIFFS.begin ( false );

        if ( SpiffsBeginHandler != nullptr ) {
            SpiffsBeginHandler ( success );
        }
    } while (! success );

    if ( resetState ) {
        SPIFFS.remove(configFileName);
    }

    if ( ResetStateHandler != nullptr ) {
        ResetStateHandler ( resetState );
    }
}

bool WiFiConfigurator::isConfigFileExists(){
    bool configFileExists = SPIFFS.exists ( configFileName );

    if ( ConfigFileExistsHandler != nullptr ) {
        ConfigFileExistsHandler( configFileName.c_str(), configFileExists );
    }

    return configFileExists;
}

void WiFiConfigurator::connectWiFi(){
    File file = SPIFFS.open(configFileName, FILE_READ);
    String ssid = file.readStringUntil('\n');
    String wifiPassword = file.readStringUntil('\n');
    file.close();
    ssid.trim();
    wifiPassword.trim();
    WiFi.begin(ssid, wifiPassword);

    if ( ConnectWifiHandler != nullptr ) {
        ConnectWifiHandler(ssid.c_str());
    }

}

void WiFiConfigurator::startAP(){
    WiFi.mode(WIFI_AP);

    if ( StartAPHandler != nullptr ) {
        StartAPHandler();
    }
}

void WiFiConfigurator::startWebServer(){
    if ( webServer == nullptr ) {
        webServer = new WebServer();
    }

    webServer -> onNotFound ( onRoot );
    webServer -> on ("/", onRoot );
    webServer -> on ("/scan", onScan );
    webServer -> on ("/favicon.ico", HTTP_GET, [](){
        webServer -> send ( 204 );
    });

    webServer -> begin();

    if ( StartWebServerHandler != nullptr ) {
        StartWebServerHandler(*webServer);
    }

    while ( true ) {
        webServer -> handleClient();
        delay(10);
    }
}

void WiFiConfigurator::onRoot(){

    String html = ConfiguratorHTML::AP_HEAD;

    if (webServer -> method() == HTTP_POST
        && webServer -> arg("setup") =="setup") {

        String ssid = webServer -> arg("ssid");

        String wifiPassword = webServer -> arg("wifiPassword");

        if ( ssid.isEmpty () ) {
            html += ConfiguratorHTML::AP_SSID_ERROR;
            html += ConfiguratorHTML::AP_FORM;
        } else {
            File file = SPIFFS.open(configFileName, FILE_WRITE);
            file.println(ssid);
            file.println(wifiPassword);
            file.close();

            String savedConfig = ConfiguratorHTML::AP_SAVED_CONFIG;
            savedConfig.replace ("$ssid",ssid);
            html += savedConfig;

            webServer -> send (200,"text/html", html);
        }

        if ( HttpSubmitHandler != nullptr ) {
            HttpSubmitHandler ( ssid, wifiPassword );
        }

        if ( ! ssid.isEmpty () ) {
            delay (5000);
            ESP.restart();
        }
    } else {
        html += ConfiguratorHTML::AP_FORM;

        if ( HttpRootHandler != nullptr ) {
            HttpRootHandler();
        }
    }

    html += ConfiguratorHTML::AP_FOOT;
    webServer -> send (200,"text/html", html);
}

void WiFiConfigurator::onScan(){
    int n = WiFi.scanNetworks();
    String ssidList ="";

    String json ="[";
    for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        if (i > 0) json +=",";
        json +="\"" + ssid +"\"";
        ssidList += ssid +"\n";
    }
    json +="]";

    webServer -> send (200,"application/json", json);

    if ( HttpScanHandler != nullptr ) {
        HttpScanHandler(ssidList.c_str());
    }
}

void WiFiConfigurator::clearAllHandlers(){
    ResetStateHandler = nullptr;
    SpiffsBeginHandler = nullptr;
    ConfigFileExistsHandler = nullptr;
    StartAPHandler = nullptr;
    StartWebServerHandler = nullptr;
    HttpRootHandler = nullptr;
    HttpScanHandler = nullptr;
    HttpSubmitHandler = nullptr;
    ConnectWifiHandler = nullptr;
}

void WiFiConfigurator::restoreDefaultHandlers(){
    DetermineResetState = DefaultHandler::DetermineResetState;
    ResetStateHandler = DefaultHandler::ResetStateHandler;
    SpiffsBeginHandler = DefaultHandler::SpiffsBeginHandler;
    ConfigFileExistsHandler = DefaultHandler::ConfigFileExistsHandler;
    StartAPHandler = DefaultHandler::StartAPHandler;
    StartWebServerHandler = DefaultHandler::StartWebServerHandler;
    HttpRootHandler = DefaultHandler::HttpRootHandler;
    HttpScanHandler = DefaultHandler::HttpScanHandler;
    HttpSubmitHandler = DefaultHandler::HttpSubmitHandler;
    ConnectWifiHandler = DefaultHandler::ConnectWifiHandler;
}

/***** Initial Settings *****/
WebServer* WiFiConfigurator::webServer = nullptr;
Print& WiFiConfigurator::printStream = Serial;

String WiFiConfigurator::configFileName
    ="/wifi_configurator.txt";

bool (*WiFiConfigurator::DetermineResetState)()
    = DefaultHandler::DetermineResetState;

void (*WiFiConfigurator::ResetStateHandler)(bool)
    = DefaultHandler::ResetStateHandler;

void (*WiFiConfigurator::SpiffsBeginHandler)(bool)
    = DefaultHandler::SpiffsBeginHandler;

void (*WiFiConfigurator::ConfigFileExistsHandler)(const char*, bool)
    = DefaultHandler::ConfigFileExistsHandler;

void (*WiFiConfigurator::StartAPHandler)()
    = DefaultHandler::StartAPHandler;

void (*WiFiConfigurator::StartWebServerHandler)(WebServer& webServer)
    = DefaultHandler::StartWebServerHandler;

void (*WiFiConfigurator::HttpRootHandler)()
    = DefaultHandler::HttpRootHandler;

void (*WiFiConfigurator::HttpScanHandler)(const char* ssidList)
    = DefaultHandler::HttpScanHandler;

void (*WiFiConfigurator::HttpSubmitHandler)(String ssid, String wifiPassword)
    = DefaultHandler::HttpSubmitHandler;

void (*WiFiConfigurator::ConnectWifiHandler)(const char* ssid)
    = DefaultHandler::ConnectWifiHandler;