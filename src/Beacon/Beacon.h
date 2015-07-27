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

/*
 * This class implements an AMX-compatible beacon on an Arduino with Ethernet
 * card (W5100 compatible).
 *
 * This is a singleton class, with no public constructor. Instead, the static "factory"
 * function newInstance should be called, which returns a pointer to the newly
 * created instance, provided it has not been instantiated before.
 * The user then has to call the send() function on a regular basis,
 * e.g. every 10 to 60 seconds. See BeaconMain.cpp for an example.
 *
 * The class requires that the Ethernet has been setup before calling newInstance.
 * It then instantiates its own private EthernetUDP instance.
 *
 * The beacon should transmit an UDP broadcast message to IP-address 239, 255, 250, 250,
 * port 9131. However, the Arduino library does not consider this IP address to be a
 * broadcast address. Therefore, this implementation instead sends to broadcast address
 * 255.255.255.255.
 */

#ifndef BEACON_H
#define	BEACON_H

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

class Beacon {
private:
    static const IPAddress broadcastIp; // = "239.255.250.250";
    static const uint16_t broadcastPort; // = 9131;

    static Beacon *instance;

    String payload;
    EthernetUDP udp;

    String pack(const char *key, String& value);
    String createPayload(String uuid, String utility, String make, String model,
            String revision, String configName, String configUrl);

    Beacon(const char *hostname, const char *mac, const char *sdkClass,
            const char *make, const char *model, const char *revision,
            const char *configName, const char *configUrl);

    virtual ~Beacon() {
    };
public:
    /**
     * Factory method for constructing a (the) instance of the Beacon.
     *
     * @param hostname
     * @param mac
     * @param sdkClass
     * @param make
     * @param model
     * @param revision
     * @param configName
     * @param configUrl
     * @return pointer to the instance.
     */
    static Beacon *newInstance(const char *hostname, const char *mac, const char *sdkClass,
            const char *make, const char *model, const char *revision,
            const char *configName, const char *configUrl);

    /**
     * Getting the single instance (possibly NULL).
     * @return pointer to the instance.
     */
    static Beacon *getInstance() {
        return instance;
    }

    /**
     * Delete the instance.
     */
    static void deleteInstance() {
        delete instance;
        instance == NULL;
    }

    /**
     * Have the instance send its message. The user has to insure that this is called
     * on a "regular" basis.
     */
    void send();
};

#endif	/* BEACON_H */
