#include "DefaultHandler.h"
#include "WifiConfigurator.h"

bool DefaultHandler::DetermineResetState() {
    pinMode(RESET_PIN, RESET_PIN_MODE);
    return (digitalRead(RESET_PIN) == HIGH);
}

void DefaultHandler::ResetStateHandler(bool isReset) {
    Print& printStream = WiFiConfigurator::printStream;

    printStream.print("Reset state: ");
    if (isReset) {
        printStream.println("HIGH");
    } else {
        printStream.println("LOW");
    }
}

void DefaultHandler::SpiffsBeginHandler(bool success) {
    Print& printStream = WiFiConfigurator::printStream;

    if (success) {
        printStream.println("SPIFFS initialized successfully.");
    } else {
        printStream.println("Failed to initialize SPIFFS.");
    }
}

void DefaultHandler::ConfigFileExistsHandler(const char* configFileName, bool exists) {
    Print& printStream = WiFiConfigurator::printStream;

    printStream.print("Configuration file '");
    printStream.print(configFileName);
    printStream.print("' "); 
    if (exists) {
        printStream.println("exists.");
    } else {
        printStream.println("does not exist.");
    }
}

void DefaultHandler::StartAPHandler() {
    Print& printStream = WiFiConfigurator::printStream;

    String ssid = WiFi.softAPSSID();

    printStream.println("Access Point is started...");
    printStream.print("AP SSID: ");
    printStream.println(ssid);
}

void DefaultHandler::StartWebServerHandler(WebServer& webServer) {
    Print& printStream = WiFiConfigurator::printStream;

    IPAddress ipAddress = WiFi.softAPIP();
    printStream.print("Web server is started, the URL is http://");
    printStream.println( ipAddress.toString() + "/");
}

void DefaultHandler::HttpRootHandler() {
    Print& printStream = WiFiConfigurator::printStream;
    printStream.println("HTTP Root Handler called.");
}

void DefaultHandler::HttpSubmitHandler(String ssid, String wifiPassword){
    Print& printStream = WiFiConfigurator::printStream;

    if ( ssid.isEmpty ( ) ) {
        printStream.println ( "SSID is required." ) ;
    } else {
        printStream.println ( "Submitted SSID: " + ssid ) ;
        printStream.println ( "Restarting after 5 seconds..." ) ;
    }
}

void DefaultHandler::HttpScanHandler(const char* ssidList) {
    Print& printStream = WiFiConfigurator::printStream;
    printStream.println("Scaned SSID:");
    printStream.println(ssidList);
}

void DefaultHandler::ConnectWifiHandler(const char* ssid) {
    Print& printStream = WiFiConfigurator::printStream;

    printStream.print ( "SSID " );
    printStream.println ( ssid );

    while (WiFi.status() != WL_CONNECTED) {
        printStream.print ("WIFI status:" );
        switch (WiFi.status()) {
            case WL_IDLE_STATUS:        printStream.println("IDLE"); break;
            case WL_NO_SSID_AVAIL:      printStream.println("NO_SSID_AVAIL"); break;
            case WL_SCAN_COMPLETED:     printStream.println("SCAN_COMPLETED"); break;
            case WL_CONNECT_FAILED:     printStream.println("CONNECT_FAILED"); break;
            case WL_CONNECTION_LOST:    printStream.println("CONNECTION_LOST"); break;
            case WL_DISCONNECTED:       printStream.println("DISCONNECTED"); break;
            default: printStream.println("UNKNOWN"); break;
        }

        delay(1000);
    }

    printStream.println("WIFI status: CONNECTED");

    while (WiFi.localIP() == IPAddress(0, 0, 0, 0)) {
        delay(1000);
    }

    printStream.print ("IP:");
    printStream.println ( WiFi.localIP().toString());

    printStream.print ("Subnet Mask:");
    printStream.println ( WiFi.subnetMask().toString());

    printStream.print ("Gateway:");
    printStream.println ( WiFi.gatewayIP().toString());

    printStream.print ("DNS:");
    printStream.println ( WiFi.dnsIP().toString());

    printStream.print ("Host name:");
    printStream.println ( WiFi.getHostname());
}