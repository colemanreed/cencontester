// Compiler Linkage
extern "C" {
  #include <stdbool.h>
  #include "commandSet.hpp"
}
#include <SoftwareSerial.h>
SoftwareSerial SoftSer(32,33);

// Pin Declaration
static const int Reset = 22;
static const int MAX_SYNC_ATTEMPTS = 100;
int syncAttempts = 0;
/*
 * Simple Test to assess the communication
 * capability of the uCAM-III device
 * (Designed for Arduino Uno or similar hardware)
 * NOTE: It is highly recommended that the Reset pin
 *       is connected, although it is optional
 *
 * Author: Haomin Yu
 */
void setup() {
//  Serial.begin(9600, SERIAL_8N1);
  Serial.begin(9600);
  SoftSer.begin(115200);
  pinMode(Reset, OUTPUT);
  digitalWrite(Reset, LOW);
  delay(5);
  digitalWrite(Reset, HIGH);
  delay(500); // STABILIZE Camera (you can remove if you want)
  if(initializeCamera()) {
    delay(1000);
    bool receivedSync = receiveSyncCommand();
    sendAckSyncCommand();
    if(receivedSync) {
      Serial.println("Initialization Successful!");
      Serial.print("Sync Attempts: ");
      Serial.println(syncAttempts);
    }
    else {
      Serial.println("Received ACK but not SYNC");
    }
  }
  else {
    Serial.println("Initialization Failed!");
  }
}

void loop() {}


bool initializeCamera() {
//  int syncAttempts = 0;
  bool ackReceived = false;
  do {
    sendSyncCommand();
    ackReceived = receiveAckCommand();
    delay(5 + syncAttempts++);
  } while((syncAttempts < MAX_SYNC_ATTEMPTS) && !ackReceived);

  return syncAttempts < MAX_SYNC_ATTEMPTS;
}

/*
 * Sends a SYNC command through serial
 * (With a small built-in delay)
 */
void sendSyncCommand() {
 //Serial.write(syncCommand, sizeof(syncCommand));
  SoftSer.write(syncCommand, sizeof(syncCommand));
  Serial.println("Syncing...");
  delay(1);
}

/*
 * Attempts to receive a SYNC command through serial
 * (Returns false if fails)
 */
bool receiveSyncCommand() {
  bool isSyncCommand = true;
  byte incoming = 0;
  // Checking if first byte is 0xAA
  //incoming = Serial.read();
  incoming =SoftSer.read();
  Serial.print(incoming);
  isSyncCommand = isSyncCommand && (incoming == 0xAA);
  // Checking if second byte is 0x0D
  //incoming = Serial.read();
  incoming =SoftSer.read();
   Serial.print(incoming);
  isSyncCommand = isSyncCommand && (incoming == 0x0D);
  // Checking if third byte is 0x00
  //incoming = Serial.read();
  incoming =SoftSer.read();
   Serial.print(incoming);
  isSyncCommand = isSyncCommand && (incoming == 0x00);
  // Checking if fourth byte is 0x00
 // incoming = Serial.read();
  incoming =SoftSer.read();
   Serial.print(incoming);
  isSyncCommand = isSyncCommand && (incoming == 0x00);
  // Checking if fifth byte is 0x00
  //incoming = Serial.read();
  incoming =SoftSer.read();
   Serial.print(incoming);
  isSyncCommand = isSyncCommand && (incoming == 0x00);
  // Checking if sixth byte is 0x00
  //incoming = Serial.read();
  incoming =SoftSer.read();
   Serial.print(incoming);
  isSyncCommand = isSyncCommand && (incoming == 0x00);
  Serial.println("");
  delay(1);
  return isSyncCommand;
}

/*
 * Sends a ACK command through serial, indicating
 * that the SYNC command has been registered
 * (With a small built-in delay)
 */
void sendAckSyncCommand() {
  //Serial.write(ackSyncCommand, sizeof(ackSyncCommand));
  SoftSer.write(ackSyncCommand, sizeof(ackSyncCommand));
  delay(1);
}

/*
 * Attempts to receive a ACK command through serial
 * (Returns false if fails)
 */
bool receiveAckCommand() {
  bool isAckCommand = true;
  byte incoming = 0;
  // Checking if first byte is 0xAA
 // incoming = Serial.read();
   incoming =SoftSer.read();
    Serial.print(incoming);
  isAckCommand = isAckCommand && (incoming == 0xAA);
  // Checking if second byte is 0x0E
  //incoming = Serial.read();
   incoming =SoftSer.read();
   Serial.print(incoming);
  isAckCommand = isAckCommand && (incoming == 0x0E);
  // Checking if third byte is 0x0D
  //incoming = Serial.read();
   incoming =SoftSer.read();
  Serial.print(incoming);
  isAckCommand = isAckCommand && (incoming == 0x0D);
  // Throwing away fourth byte (Debugging byte)
  //incoming = Serial.read();
   incoming =SoftSer.read();
  Serial.print(incoming);
  //  isAckCommand = isAckCommand && (incoming == 0x00);
  // Checking if fifth byte is 0x00
 // incoming = Serial.read();
   incoming =SoftSer.read();
  Serial.print(incoming);
  isAckCommand = isAckCommand && (incoming == 0x00);
  // Checking if sixth byte is 0x00
  //incoming = Serial.read();
    incoming =SoftSer.read();
   Serial.print(incoming);
  isAckCommand = isAckCommand && (incoming == 0x00);
   Serial.println("");
  delay(1);
  return isAckCommand;
}
