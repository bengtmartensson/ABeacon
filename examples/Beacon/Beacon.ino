/*
 This files demonstrates how to use the Beacon class.
 *
 * 1. Initialize Ethernet.
 * 2. Initialize with  Beacon::newInstance(...) (This is a singleton class with no public constructor.)
 * 3. the send() function must be called "every now and then", e.g. once every 10 to 60 seconds,
 * not necessarily with great precision.
 * Alternatively, call checkSend() as often as you like.
 */


#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Beacon.h>

// Use DHCP in this example. Some say that it takes much memory...
#define DHCP

// In all examples I have seen, the MAC address is written TO the card, not read from it.
#define MACADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED

#ifndef DHCP
// If not DHCP, initialize the usual parameters manually
#define IPADDRESS  192,168,1,29
#define GATEWAY    192,168,1,254
#define DNSSERVER  192,168,1,4
#define SUBNETMASK 255,255,255,0
#endif // ! DHCP

void setup() {
    byte mac[] = { MACADDRESS };

// Initialize Ethernet, needed for the Beacon
#ifdef DHCP
    Ethernet.begin(mac);
#elif defined(GATEWAY) & defined(DNSSERVER) & defined(SUBNETMASK)
    Ethernet.begin(mac, IPAddress(IPADDRESS), IPAddress(DNSSERVER), IPAddress(GATEWAY), IPAddress(SUBNETMASK));
#else
    Ethernet.begin(mac, IPAddress(IPADDRESS));
#endif // !DHCP
    // Initialize the singleton class Beacon.
    // It will initialize a private UDP socket.
    Beacon::setup("arduino", "DE-AD-BE-EF-FE-ED", "Utility", "www.harctoolbox.org",
            "", "", "", "http://arduino/nosuchfile.html");
}

void loop() {
    Beacon::checkSend();
}
