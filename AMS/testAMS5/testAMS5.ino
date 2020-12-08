#include <SD.h>
#include <ArduinoJson.h>
#include <SimpleComm.h>
#include <WifiModule.h>

typedef struct {
  //preset
  char machine[20];
  char tool[20];
  char productPart[20];
  char productDataParam[20];
  char paramUM[5];
  uint8_t minVal;
  uint8_t maxVal;

  //data
  uint8_t realVal;
} data_t;


SimplePacket packet;
data_t pumpData;
data_t levelAlarmData;
data_t tempA;
data_t tempB;

int pumpRunLast = 0;
int levelAlarmLast = 0;
int tempALast = 0;
int tempBLast = 0;

//general presets that never change 
const char MACHINE[20] = "M1";
const char TOOL[20] = "T1";
const char PRODUCT_PART[20] = "P123";

////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600L);
  WifiModule.begin(115200UL);
  SimpleComm.begin();
  pinMode(I0_1, INPUT);
  pinMode(I0_0, INPUT);
  Serial.println("Wifi Module started");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  //serializeJsonPretty(pumpRunDoc, Serial);

  int pumpRunValue = digitalRead(I0_0);
  int levelAlarmValue = digitalRead(I0_1);
  int tempAValue = analogRead(I0_2);
  int tempBValue = analogRead(I0_3);
  
  if (pumpRunValue != pumpRunLast) {
    pumpRunLast = pumpRunValue;

    // Init data
    //general preset
    sprintf(pumpData.machine, MACHINE);
    sprintf(pumpData.tool, TOOL);
    sprintf(pumpData.productPart, PRODUCT_PART);

    //specific preset
    sprintf(pumpData.productDataParam, "Cycle Duration A");
    sprintf(pumpData.paramUM, "sec");
    pumpData.minVal = 0;
    pumpData.maxVal = 20;
    
    //real data
    pumpData.realVal = pumpRunValue;

    Serial.println(pumpData.machine);
    Serial.println(pumpData.tool);
    Serial.println(pumpData.productPart);
    Serial.println(pumpData.productDataParam);
    Serial.println(pumpData.paramUM);
    Serial.println(pumpData.minVal);
    Serial.println(pumpData.maxVal);
    Serial.println(pumpData.realVal);

    
    // Create packet from data
    packet.setData(&pumpData, sizeof(pumpData));


    if (!SimpleComm.send(WifiModule, packet, 0)) {
      Serial.println("Send packet error");
    } else {
      Serial.println("sent");
    }
  }


  

  if (levelAlarmValue != levelAlarmLast) {
    levelAlarmLast = levelAlarmValue;

    // Init data
    //general preset
    sprintf(levelAlarmData.machine, MACHINE);
    sprintf(levelAlarmData.tool, TOOL);
    sprintf(levelAlarmData.productPart, PRODUCT_PART);

    //specific preset
    sprintf(levelAlarmData.productDataParam, "Cycle Duration B");
    sprintf(levelAlarmData.paramUM, "sec");
    levelAlarmData.minVal = 5;
    levelAlarmData.maxVal = 10;
    
    //real data
    levelAlarmData.realVal = levelAlarmValue;

    Serial.println(levelAlarmData.machine);
    Serial.println(levelAlarmData.tool);
    Serial.println(levelAlarmData.productPart);
    Serial.println(levelAlarmData.productDataParam);
    Serial.println(levelAlarmData.paramUM);
    Serial.println(levelAlarmData.minVal);
    Serial.println(levelAlarmData.maxVal);
    Serial.println(levelAlarmData.realVal);

    
    // Create packet from data
    packet.setData(&levelAlarmData, sizeof(levelAlarmData));


    if (!SimpleComm.send(WifiModule, packet, 0)) {
      Serial.println("Send packet error");
    } else {
      Serial.println("sent");
    }
  }

//  if (tempAValue >= tempALast + 100 || tempAValue >= tempALast - 100) {
//    tempALast = tempAValue;
//
//    // Init data
//    //general preset
//    sprintf(tempA.machine, MACHINE);
//    sprintf(tempA.tool, TOOL);
//    sprintf(tempA.productPart, PRODUCT_PART);
//
//    //specific preset
//    sprintf(tempA.productDataParam, "tempB");
//    sprintf(tempA.paramUM, "degrees_C");
//    tempA.minVal = 50;
//    tempA.maxVal = 70;
//    
//    //real data
//    pumpData.realVal = tempAValue;
//
//    Serial.println(tempA.machine);
//    Serial.println(tempA.tool);
//    Serial.println(tempA.productPart);
//    Serial.println(tempA.productDataParam);
//    Serial.println(tempA.paramUM);
//    Serial.println(tempA.minVal);
//    Serial.println(tempA.maxVal);
//    Serial.println(tempA.realVal);
//
//    
//    // Create packet from data
//    packet.setData(&tempA, sizeof(tempA));
//
//
//    if (!SimpleComm.send(WifiModule, packet, 0)) {
//      Serial.println("Send packet error");
//    } else {
//      Serial.println("sent");
//    }
//  }
//  if (tempBValue >= tempBLast + 70 || tempBValue >= tempBLast - 70) {
//    tempBLast = tempBValue;
//
//    // Init data
//    //general preset
//    sprintf(tempB.machine, MACHINE);
//    sprintf(tempB.tool, TOOL);
//    sprintf(tempB.productPart, PRODUCT_PART);
//
//    //specific preset
//    sprintf(tempB.productDataParam, "tempB");
//    sprintf(tempB.paramUM, "degrees_C");
//    tempA.minVal = 80;
//    tempA.maxVal = 120;
//    
//    //real data
//    pumpData.realVal = tempBValue;
//
//    Serial.println(tempB.machine);
//    Serial.println(tempB.tool);
//    Serial.println(tempB.productPart);
//    Serial.println(tempB.productDataParam);
//    Serial.println(tempB.paramUM);
//    Serial.println(tempB.minVal);
//    Serial.println(tempB.maxVal);
//    Serial.println(tempB.realVal);
//
//    
//    // Create packet from data
//    packet.setData(&tempB, sizeof(tempB));
//
//
//    if (!SimpleComm.send(WifiModule, packet, 0)) {
//      Serial.println("Send packet error");
//    } else {
//      Serial.println("sent");
//    }
//  }

}
