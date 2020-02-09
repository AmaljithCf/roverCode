#include<Arduino.h>
#include "Communication.h"

int timeout = 100;

long readLong() {
  unsigned long startTime = millis();
  while ((Serial.available() < sizeof(long)) && (millis() - startTime) < timeout) {};
  if (Serial.available() > 0) {
    size_t bytesRead;
    byte longBuffer[sizeof(long)];
    bytesRead = Serial.readBytes(longBuffer, sizeof(long));
    if (bytesRead == sizeof(long)) {
      long longReceived;
      memcpy(&longReceived, longBuffer, sizeof(long));
      return longReceived;
    }
    Serial.println("partial long");
  }
  Serial.println("Couldn't parse long");
  return NULL;
}

int readInt() {
  unsigned long startTime = millis();
  while ((Serial.available() < sizeof(int)) && (millis() - startTime) < timeout) {};
  if (Serial.available() > 0) {
    size_t bytesRead;
    byte intBuffer[sizeof(int)];
    bytesRead = Serial.readBytes(intBuffer, sizeof(int));
    if (bytesRead == sizeof(int)) {
      int intReceived;
      memcpy(&intReceived, intBuffer, sizeof(int));
      return intReceived;
    }
    Serial.println("partial int");
    return NULL;
  }
  Serial.println("Couldn't read int");
  return NULL;
}

// BUGGY - DON'T USE
char* readString() {
  if (Serial.available() > 0) {
    size_t bytesRead;
    int stringLength = readInt();
    byte stringBuffer[stringLength * sizeof(char)];
    bytesRead = Serial.readBytes(stringBuffer, stringLength * sizeof(char));
    if (bytesRead == stringLength) {
      char stringReceived[stringLength];
      memcpy(&stringReceived, stringBuffer, stringLength * sizeof(char));
      return stringReceived;
    }
  }
  return;
}

bool readCommand(Command* command) {
  unsigned long startTime = millis();
  while ((Serial.available() < sizeof(Command)) && (millis() - startTime) < timeout) {};
  if (Serial.available() > 0) {
    byte startByte = Serial.read();
    if (startByte == '^') {
      COMMAND_TYPE commandType = readInt();
      if (commandType == NULL) {
        return false;
      }
      long commandParam = readLong();
      if (commandParam == NULL) {
        return false;
      }
      command->command = commandType;
      command->param = commandParam;
      return true;
    } else {
      byte t = Serial.peek();
      while (Serial.peek() != -1 && Serial.peek() != '^') {
        Serial.read();
      }
    }
  }
  return false;
}
