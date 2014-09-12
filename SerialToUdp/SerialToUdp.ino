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

enum SERIAL_MODE {
  SERIAL_DEBUG = 0,
  SERIAL_MEK = 1
};

enum REPLY_CODE {
  REPLY_PNAK = 0xFF
};

int serialMode = SERIAL_MEK;
int serialVariant = SERIAL_8E1;

void sendUdpPacket(char *buf)
{
    udp.beginPacket(server, udpServerPort);
    udp.write(buf, strlen(buf));
    udp.endPacket();
}

void sendSerialPacket(char *buf)
{
  Serial.println(buf);
}

void debug(char *str)
{
  if (serialMode == SERIAL_DEBUG)
    sendSerialPacket(str);
  else if (serialMode == SERIAL_MEK)
    sendUdpPacket(str);
}

void serialDebug(char *str)
{
  if (serialMode == SERIAL_DEBUG)
    sendSerialPacket(str);
}

void sendMekPacket(char *str)
{
  if (serialMode == SERIAL_DEBUG) {
    sendSerialPacket("TO MEK: ");
    sendSerialPacket(str);
  } else if (serialMode == SERIAL_MEK) {
    sendSerialPacket(str);
  }
}

void sendReply(int code)
{
  char buf[5];
  buf[0] = code;
  buf[1] = 0;
  
  sendMekPacket(buf);
}

char hexDigitToChar(int hexDigit)
{
    char base = '?';
    int offset = 0;
    
    if (hexDigit >= 0 && hexDigit <= 9) {
        base = '0';
        offset = hexDigit;
    } else if (hexDigit >= 0xA && hexDigit <= 0xF) {
        base = 'A';
        offset = hexDigit - 10;
    }
    return base + offset;
}

void setup(void) {
  // Open serial communications and wait for port to open:
  Serial.begin(9600, serialVariant);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
//  if (Ethernet.begin(mac) == 0) {
    serialDebug("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    serialDebug("Configuring ethernet");
  
    Ethernet.begin(mac, ip);
    
 // }
  
  // Short delay before sending the initial PNAK to the link master
  serialDebug("delay...");
  delay(500);
  
  sendReply(REPLY_PNAK);
  
  // give the Ethernet shield a second to initialize:
  serialDebug("delay...");
  delay(500);
  serialDebug("connecting...");

  serialDebug("Configuring UDP socket");
  udp.begin(udpListenPort);
  
}

void loop(void)
{
     char msgTemplate[] = "Read character <?> <0x??>\n";
     char *message;
     int charOffset = 16;
     int hexOffset = 22;
     delay(500);
     debug("Hej hej bla bla\n");

  // if there are incoming bytes available
  // from the server, read them and print them:
  int done = 0;
  while (Serial.available()) {
    char c = Serial.read();
    
     int hexDigit;
     char hexChar;
     hexDigit = (c >> 4) & 0x0F;
     hexChar = hexDigitToChar(hexDigit);
     msgTemplate[hexOffset] = hexChar;
     
     hexDigit = c & 0x0f;
     hexChar = hexDigitToChar(hexDigit);
     msgTemplate[hexOffset + 1] = hexChar;
     
     msgTemplate[charOffset] = c;
     
     message = msgTemplate;
     
     debug(message);
     
     done = 1;
  }
  if (!done) {
     message = "Could not read from serial";
     
     debug(message);
  }
}

