/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen

 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x4F, 0x9C };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192, 168, 1, 50);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)
int udpServerPort = 8088;
int tcpServerPort = 8088;
int udpListenPort = 8088;

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 1, 55);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
EthernetUDP udp;

void sendUdpPacket(char *buf)
{
    Serial.println("begin packet");
    udp.beginPacket(server, udpServerPort);
    
    Serial.println("udp write...");
    udp.write(buf, strlen(buf));
    
    Serial.println("end packet");
    udp.endPacket();
    
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
//  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Serial.println("Configuring ethernet");
    Ethernet.begin(mac, ip);
    
 // }
  // give the Ethernet shield a second to initialize:
  Serial.println("delay...");
  delay(1500);
  Serial.println("connecting...");

    Serial.println("Configuring UDP socket");
    udp.begin(udpListenPort);
    
}

void loop()
{
     delay(500);
     sendUdpPacket("Hej hej bla bla\n");

}

