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

// This is the correct broadcast address:
//IPAddress Beacon::broadcastIp(239, 255, 250, 250);

// however, the Arduino does not recognize it as broadcast, so use this instead.
const IPAddress Beacon::broadcastIp(255, 255, 255, 255);

const uint16_t Beacon::broadcastPort = 9131;

Beacon *Beacon::instance = NULL;

String Beacon::pack(const char *key, String& value) {
    return value == NULL ? "" : ("<" + String(key) + "=" + value + ">");
}

String Beacon::createPayload(String uuid, String utility, String make, String model,
        String revision, String configName, String configUrl) {
    return "AMXB"
            + pack("-UUID", uuid)
            + pack("-SDKClass", utility)
            + pack("-Make", make)
            + pack("-Model", model)
            + pack("-Revision", revision)
            + pack("Config-Name", configName)
            + pack("Config-URL", configUrl) + "\r";
}

Beacon::Beacon(const char *hostname, const char *macAddress, const char *sdkClass, const char *make, const char *model,
        const char *revision, const char *configName, const char *configUrl) {
    udp.begin(broadcastPort); // ??? Arduino stupidity (?) -- I do not want a port to listen to.
    payload = createPayload(String(hostname) + "@" + String(macAddress), sdkClass, make, model, revision, configName, configUrl);
}

Beacon *Beacon::newInstance(const char *hostname, const char *mac, const char *sdkClass,
            const char *make, const char *model, const char *revision,
            const char *configName, const char *configUrl) {
    if (instance != NULL) // already instantiated
        return NULL;

    instance = new Beacon(hostname, mac, sdkClass, make, model, revision, configName, configUrl);
    return instance;
}

void Beacon::send() {
    udp.beginPacket(broadcastIp, broadcastPort);
    udp.write(payload.c_str());
    udp.endPacket();
}
