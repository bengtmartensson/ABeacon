# ABeacon

This project consists of a class implementing an AMX-compatible beacon
on an Arduino with Ethernet card.
Both the new Ethernet2 library (supporting W5500 chip shields) and the
old Ethernet library are supported. To use the Ethernet2 library, define
`ETHERNET_REVISION` to 2 in `Beacon.h`.

The AMX beacon protocol is described e.g. [here](http://www.globalcache.com/files/docs/API-GC-100.pdf).

This is a singleton class, with no public constructor. Instead, the
static "factory" function setup() should be called.
The user then has to call the send() function on
a regular basis, e.g. every 10 to 60 seconds.
Alternatively, checkSend() can be called arbitrarily often,
without flooding the LAN. See example/Beacon.ino for an example.

The class requires that the Ethernet has been setup before calling newInstance.
It then instantiates its own private EthernetUDP instance.

The beacon should transmit an UDP broadcast message to IP-address
239.255.250.250, port 9131. However, the Arduino library does not
consider this IP address to be a broadcast address. Therefore, this
implementation instead sends to broadcast address 255.255.255.255.
