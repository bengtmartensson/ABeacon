/*
Copyright (C) 2015 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
*/

#include "Beacon.h"
#include <string.h>

// The correct broadcast address is:
//IPAddress Beacon::broadcastIp(239, 255, 250, 250);

// However, the Arduino does not recognize it as broadcast, so use this instead.
const IPAddress Beacon::broadcastIp(255, 255, 255, 255);

Beacon *Beacon::instance = NULL;

void Beacon::doPair(const char *key, const char *value) {
    if (value != NULL && value[0] != '\0') {
        doOne("<");
        doOne(key);
        doOne("=");
        doOne(value);
        doOne(">");
    }
}

void Beacon::doPair(const char *key, const char *value, const char *value2) {
        doOne("<");
        doOne(key);
        doOne("=");
        doOne(value);
        doOne("@");
        doOne(value2);
        doOne(">");
}

void Beacon::doOne(const char* str) {
    strcat(payload, str);
}

// TODO: put the string literals in progmem.

void Beacon::createPayload(/*const char *uuid,*/ const char *hostname, const char *macaddress,
        const char *utility, const char *make, const char *model,
        const char *revision, const char *configName, const char *configUrl) {
    payload = new char[150]; // FIXME
    payload[0] = '\0';
    doOne("AMXB");
    doPair("-UUID", hostname, macaddress);
    doPair("-SDKClass", utility);
    doPair("-Make", make);
    doPair("-Model", model);
    doPair("-Revision", revision);
    doPair("Config-Name", configName);
    doPair("Config-URL", configUrl);
    doOne("\r");
}

Beacon::Beacon(const char *hostname, const char *macAddress, const char *sdkClass, const char *make, const char *model,
        const char *revision, const char *configName, const char *configUrl)
: lastSendTime(-checkSendInterval) /* Insures that checkSendInstance sends first time */ {
    udp.begin(dummyPort); // ??? Arduino stupidity (?) -- I do not want a port to listen to.
    createPayload(hostname /*+ "@" +*/, macAddress, sdkClass, make, model, revision, configName, configUrl);
}

void Beacon::setup(const char *hostname, const char *mac, const char *sdkClass,
            const char *make, const char *model, const char *revision,
            const char *configName, const char *configUrl) {
    if (instance != NULL) // already instantiated
        return; // should throw exception...

    instance = new Beacon(hostname, mac, sdkClass, make, model, revision, configName, configUrl);
}

void Beacon::sendInstance() {
    udp.beginPacket(broadcastIp, broadcastPort);
    udp.write(payload);
    udp.endPacket();
#ifdef DEBUG
    static long n = 1;
    Serial.println(n++);
#endif
}

void Beacon::checkSendInstance() {
    if (millis() - lastSendTime >= checkSendInterval) {
        lastSendTime = millis();
        send();
    }
}
