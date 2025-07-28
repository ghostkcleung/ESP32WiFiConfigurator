#include "time.h"
#include "WiFiConfigurator.h"

const char* ntpServer = "pool.ntp.org";

void setup() {
    Serial.begin ( 9600 );
    WiFiConfigurator::start();

    configTime(3600*8, 0, ntpServer);

    struct tm timeinfo;
    while ( !getLocalTime( &timeinfo ) ) {
        Serial.println("Failed to obtain time");
    }

    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void loop() {
    delay ( 10000 );
}
