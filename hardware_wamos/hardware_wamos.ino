#include <SoftwareSerial.h>
#include <NewPing.h>
// IMPORT ALL REQUIRED LIBRARIES
#ifndef STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef ARDUINO_H
#include <Arduino.h>
#endif 

#include <math.h>
#include <ArduinoJson.h>

   
//**********ENTER IP ADDRESS OF SERVER******************//

#define HOST_IP     ""       // REPLACE WITH IP ADDRESS OF SERVER ( IP ADDRESS OF COMPUTER THE BACKEND IS RUNNING ON) mona ip:172.16.193.152
#define HOST_PORT   "8080"            // REPLACE WITH SERVER PORT (BACKEND FLASK API PORT)
#define route       "api/update"      // LEAVE UNCHANGED 
#define idNumber    "620152241"       // REPLACE WITH YOUR ID NUMBER 

// WIFI CREDENTIALS
#define SSID        "Digicel_WiFi_A8eG"      // "REPLACE WITH YOUR WIFI's SSID" MonaConnect Digicel_WiFi_A8eG 
#define password    "Nu5TSQ5h"  // "REPLACE WITH YOUR WiFi's PASSWORD" Nu5TSQ5h

#define stay        100
 
//**********PIN DEFINITIONS******************//

 
#define espRX         10
#define espTX         11
#define espTimeout_ms 300
#define ARDUINOJSON_USE_DOUBLE 1

#define trigPin  9   // Trigger
#define echoPin  12  // Echo
#define tank_height 94.5 // Ultrasonic sensor height from the base of the tank
#define max_height 77.763 // Maximum water level/height from the base of the tank
#define tank_diameter 61.5 // Diameter of the tank
#define max_capacity  1000 // Maximum capacity of the tank in US Gallons


float measureRadar();
float calculateWaterHeight(float radar);
float calculateWaterReserve(float height);


/* Declare your functions below */
SoftwareSerial esp(espRX, espTX); 
NewPing sonar(trigPin, echoPin,1000); 

void setup(){

  Serial.begin(115200); 
  // Configure GPIO pins here
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //esp.begin(115200); // Initialize ESP module

  espInit();  
 
}

void loop(){ 
   // Perform radar measurement and return the value

  double radar= sonar.ping_in();// Speed of sound in inches per microsecond formula:
  double waterHeight = calculateWaterHeight(radar); // Calculate water height
  double reserve = calculateWaterReserve(waterHeight); // Calculate water reserve
  double percentage = (waterHeight / max_height) * 100; // Calculate percentage

   
  // send updates with schema ‘{"id": "student_id", "type": "ultrasonic", "radar": 0, "waterheight": 0, "reserve": 0, "percentage": 0}’
      // 1. Create JSon object
      StaticJsonDocument<290> doc; // Create JSon object
      char message[290]  = {0};
      // 3. Add key:value pairs to JSon object based on above schema

      doc["id"] = "620152241";
      doc["type"] = "ultrasonic";
      doc["radar"] = radar;
      doc["waterheight"] = waterHeight;
      doc["reserve"] = reserve;
      doc["percentage"] = percentage;

      // 4. Seralize / Covert JSon object to JSon string and store in message array
      serializeJson(doc, message);

      // 5. Publish message to a topic sobscribed to by both backend and frontend
       espUpdate(message);
      

      // snprintf(message, sizeof(message), "{\"id\": \"%s\", \"type\": \"ultrasonic\", \"radar\": %.2f, \"waterheight\": %.2f, \"reserve\": %.2f, \"percentage\": %.2f}", idNumber, radarMeasurement, waterHeight, reserve, percentage);

  delay(1000);  
}

 
void espSend(char command[] ){   
    esp.print(command); // send the read character to the esp    
    while(esp.available()){ Serial.println(esp.readString());}    
}


void espUpdate(char mssg[]){ 
    char espCommandString[50] = {0};
    char post[290]            = {0};

    snprintf(espCommandString, sizeof(espCommandString),"AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",HOST_IP,HOST_PORT); 
    espSend(espCommandString);    //starts the connection to the server
    delay(stay);

    // GET REQUEST 
    // snprintf(post,sizeof(post),"GET /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s\r\n\r\n",route,HOST_IP,strlen(mssg),mssg);

    // POST REQUEST
    snprintf(post,sizeof(post),"POST /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s\r\n\r\n",route,HOST_IP,strlen(mssg),mssg);
  
    snprintf(espCommandString, sizeof(espCommandString),"AT+CIPSEND=%d\r\n", strlen(post));
    espSend(espCommandString);    //sends post length
    delay(stay);
    Serial.println(post);
    espSend(post);                //sends POST request with the parameters 
    delay(stay);
    espSend("AT+CIPCLOSE\r\n");   //closes server connection
   }

void espInit(){
    char connection[100] = {0};
    esp.begin(115200); 
    Serial.println("Initiallizing");
    esp.println("AT"); 
    delay(1000);
    esp.println("AT+CWMODE=1");
    delay(1000);
    while(esp.available()){ Serial.println(esp.readString());} 

    snprintf(connection, sizeof(connection),"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,password);
    esp.print(connection);

    delay(3000);  //gives ESP some time to get IP

    if(esp.available()){   Serial.print(esp.readString());}
    
    Serial.println("\nFinish Initializing");    
   
}

double calculateWaterHeight(double radar){
  // Calculate water height using the radar measurement
  double val=radar+ 16.737 ;
  return tank_height- val; // Placeholder, replace with actual calculation
}

double calculateWaterReserve(double height){
  // Calculate water reserve using the radar measurement
  double tank_radius= tank_diameter/2.0;
 // Convert height to gallons using the volume formula for a cylinder
  double volume = M_PI * tank_radius * tank_radius * height / 231.0; // 231 cubic inches in a gallon

  return volume;
  }


