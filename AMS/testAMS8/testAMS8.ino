#include <SPIFFS.h>
#include <SimpleComm.h>
#include <WiFi.h>
#include <WiFiClient.h>  
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "ESP32_FTPClient.h"

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

unsigned long previousMillis;
unsigned long currentMillis;  
const long interval = 10000;

SimplePacket packet;
char json[100];

#define WIFI_SSID "Hendo"
#define WIFI_PASS "Bunny123"

char ftp_server[] = "52.233.44.204";
char ftp_user[]   = "gcftptest";
char ftp_pass[]   = "Access4FTP";

char working_dir[]   = "";

ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass, 5000, 2);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  previousMillis = millis();
  Serial.begin(115200UL);
  Serial2.begin(115200UL);
  SimpleComm.begin();

  Serial.println("esp32 started");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  Serial.println("Connecting Wifi...");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
      
  }
  Serial.println("IP address: ");
      
  Serial.println(WiFi.localIP());

  timeClient.begin();
  timeClient.setTimeOffset(-18000);

  ftp.OpenConnection();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {

  currentMillis = millis();
  
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  if (SimpleComm.receive(Serial2, packet)) {
    const data_t *data = (const data_t *) packet.getData();
    if (data != nullptr) {
//      Serial.println("Received data: ");
//      Serial.println(data->machine);
//      Serial.println(data->tool);
//      Serial.println(data->productPart);
//      Serial.println(data->productDataParam);
//      Serial.println(data->paramUM);
//      Serial.println(data->minVal);
//      Serial.println(data->maxVal);
//      Serial.println(data->realVal);

      // The formattedDate comes with the following format:
      // 2018-05-28T16:00:13Z
      // We need to extract date and time
      formattedDate = timeClient.getFormattedDate();
      Serial.println(formattedDate);

      // Extract date
      int splitT = formattedDate.indexOf("T");
      dayStamp = formattedDate.substring(0, splitT);
      dayStamp = dayStamp;
      Serial.print("DATE: ");
      Serial.println(dayStamp);
      // Extract time
      timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
      timeStamp.replace(":","-");
      timeStamp = timeStamp;
      Serial.print("HOUR: ");
      Serial.println(timeStamp);


      char json[300];
      sprintf(json, "{\"machine\":\"%s\",\"tool\":\"%s\",\"productPart\":\"%s\",\"productDataParameter\":\"%s\",\"parameterUM\":\"%s\",\"minValue\":%d,\"maxValue\":%d,\"date\":\"%s\",\"time\":\"%s\",\"realValue\":%d}", data->machine, data->tool, data->productPart, data->productDataParam, data->paramUM, data->minVal, data->maxVal, dayStamp, timeStamp, data->realVal);
      Serial.print("JSON: ");
      
      Serial.println(json);

      Serial.println(sizeof(json));


      if(!SPIFFS.begin(true)){
        Serial.println("error");
        return;
      }

      File file = SPIFFS.open("/test.txt", FILE_APPEND);

      if(!file){
        Serial.println("error opening file");
        return;
      }

      if(file.println(json)){
        Serial.println("Written");
      } else{
        Serial.println("not writeen");
      }

      file.close();
    }
  }
    if(currentMillis - previousMillis >= interval){
      previousMillis = millis();


      // The formattedDate comes with the following format:
      // 2018-05-28T16:00:13Z
      // We need to extract date and time
      formattedDate = timeClient.getFormattedDate();
      Serial.println(formattedDate);

      // Extract date
      int splitT = formattedDate.indexOf("T");
      dayStamp = formattedDate.substring(0, splitT);
      dayStamp = dayStamp;
      Serial.print("DATE: ");
      Serial.println(dayStamp);
      // Extract time
      timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
      timeStamp.replace(":","-");
      timeStamp = timeStamp;
      Serial.print("HOUR: ");
      Serial.println(timeStamp);


      char fileName[100];
      const char *text = ".txt";
      const char *dash = "-";
      char timeS[100];
      timeStamp.toCharArray(timeS, 100);
      char dayS[100];
      dayStamp.toCharArray(dayS, 100);
      strcpy(fileName,dayS);
      strcat(fileName, dash);
      strcat(fileName,timeS);
      strcat(fileName,text);
      Serial.print("file name: ");
      Serial.println(fileName);
      ftp.InitFile("Type A");
      ftp.ChangeWorkDir(working_dir);
      ftp.NewFile(fileName);

      char content[5000] = {'\0'};
      
      File file2 = SPIFFS.open("/test.txt");

      Serial.println(fileName);
      
      if(!file2){
        Serial.println("error opening file");
        return;
      }
      
      uint16_t i = 0;
      while(file2.available()){
        content[i] = file2.read();
        i++;
      }
      content[i] = '\0';
      file2.close();
      if(SPIFFS.remove("/test.txt")){
        Serial.println("removed");
      }
      Serial.println(content);
      ftp.Write(content);
      ftp.CloseFile();
  }
}
