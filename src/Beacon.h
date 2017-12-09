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

/**
 * This class implements an AMX-compatible beacon on an Arduino with Ethernet
 * card (W5100 compatible).
 *
 * This is a singleton class, with no public constructor. Instead, the static "factory"
 * function setup() be called.
 * The user then has to call the send() function on a regular basis,
 * e.g. every 10 to 60 seconds. Alternatively, checkSend() can be called arbitrarily
 * often, without flooding the LAN. See example/Beacon.ino for an example.
 *
 * The class requires that the Ethernet has been setup before calling setup().
 * It then instantiates its own private EthernetUDP instance.
 * Shutting down is not (yet?) implemented.
 *
 * The beacon should transmit an UDP broadcast message to IP-address 239.255.250.250,
 * port 9131. However, the Arduino library does not consider this IP address to be a
 * broadcast address. Therefore, this implementation instead sends to broadcast address
 * 255.255.255.255.
 * 
 * Define ETHERNET_REVISION to 2 to use the new Ethernet2 library, supporting W5500 cards.
 * Otherwise, the old Ethernet library will be used.
 */

#pragma once

#ifndef ETHERNET_REVISION
#define ETHERNET_REVISION 1
#endif

//#define DEBUG

#include <Arduino.h>
#if defined(ETHERNET_REVISION) & (ETHERNET_REVISION == 2)
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#else
#include <Ethernet.h>
#include <EthernetUdp.h>
#endif

class Beacon {
private:
    static const IPAddress broadcastIp; // = "239.255.250.250";
    static const uint16_t broadcastPort = 9131;
    static const uint16_t dummyPort = 65000U;

    /** Minimal interval to send messages if using checkSend(). */
    static const unsigned long checkSendInterval = 20000UL;
    long lastSendTime;

    static Beacon *instance;

    char *payload;
    EthernetUDP udp;

    void doOne(const char *str);
    void doPair(const char *key, const char *value);
    void doPair(const char *key, const char *value, const char *value2);
    void createPayload(const char *hostname, const char *macaddress, const char *utility,
            const char *make, const char *model,
            const char *revision, const char *configName, const char *configUrl);

    Beacon(const char *hostname, const char *mac, const char *sdkClass,
            const char *make, const char *model, const char *revision,
            const char *configName, const char *configUrl);

    /**
     * Just dummy.
     */
    virtual ~Beacon() {
    };

    /**
     * Have the instance broadcast its message. The user has to insure that this is called
     * on a "regular" basis, normally every 10 to 60 seconds, not necessarily with high precision.
     */
    void sendInstance();

    /**
     * Calls send() if (an only if) sufficient time has passed since last time sent was called.
     */
    void checkSendInstance();

public:
    /**
     * Method for setting up the Beacon.
     *
     * @param hostname Hostname to be announced
     * @param mac Hardware MAC address of the type xx-xx-xx-xx-xx-xx
     * @param sdkClass Class to be announced
     * @param make
     * @param model
     * @param revision
     * @param configName
     * @param configUrl
     */
    static void setup(const char *hostname, const char *mac, const char *sdkClass,
            const char *make, const char *model, const char *revision,
            const char *configName, const char *configUrl);

    /**
     * Have the instance broadcast its message. The user has to insure that this is called
     * on a "regular" basis, normally every 10 to 60 seconds, not necessarily with high precision.
     */
    static void send() {
        instance->sendInstance();
    }

    /**
     * Calls send() if (an only if) sufficient time has passed since last time it was called.
     */
    static void checkSend() {
        instance->checkSendInstance();
    }
};
