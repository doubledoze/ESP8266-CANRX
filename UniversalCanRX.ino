/* 
 * SecSea 2K23 - Hack in Provence - Can Bus Reader @Samy
 * Utilisation de la librairie : https://github.com/coryjfowler/MCP_CAN_lib
 */
 
 /*
  * Cablage ESP8266 <=> MCP2515
  * 3V3         - VCC
  * GND         - GND
  * D8 (GPIO15) - CS
  * D6          - SO
  * D7          - SI
  * D5          - SCK
  * D4 (GPIO2)  - INT
  */

#include <mcp_can.h>
#include <SPI.h>

// CAN RX Variables
long unsigned int rxId;
unsigned char len;
byte rxBuf[8];

// CAN0 INT et CS
#define CAN0_INT 2  // INT = pin D4 (GPIO2)
MCP_CAN CAN0(15);   // CS = pin D8 (GPIO15)

void setup() {
  Serial.begin(115200);
  Serial.println();

  startCan();
  
  Serial.println("Setup done.");
}

void loop() {
  if(!digitalRead(CAN0_INT)) {  // If CAN0_INT pin is low, read receive buffer
    CAN0.readMsgBuf(&rxId, &len, rxBuf);  // Read data: len = data length, buf = data byte(s)
    
    // Affichage des informations du message CAN sur la console série
    String message = "ID: 0x" + String(rxId, HEX) + " Data:";
    for(byte i = 0; i < len; i++) {  // Affichage de la trame complète (non nettoyée)
      message += " 0x" + String(rxBuf[i], HEX);
    }
    Serial.println(message);
  }
}

void startCan() {
  if(CAN0.begin(MCP_LISTENONLY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  pinMode(CAN0_INT, INPUT);  // Configuration du pin INT
}
