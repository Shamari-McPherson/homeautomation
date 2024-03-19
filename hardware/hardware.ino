//##################################################################################################################
//##                                      ELET2415 DATA ACQUISITION SYSTEM CODE                                   ##
//##                                                                                                              ##
//##################################################################################################################

 
#include "Arduino.h"
// IMPORT ALL REQUIRED LIBRARIES
#include <rom/rtc.h>


#ifndef _WIFI_H 
#include <WiFi.h>
#include <HTTPClient.h>
#endif

#ifndef STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef ARDUINO_H
#include <Arduino.h>
#endif 
 
#ifndef ARDUINOJSON_H
#include <ArduinoJson.h>
#endif

#ifndef ARDUINOJSON_H
#include "forecast.h"
#endif

//LIBRARY IMPORTS
#include <rom/rtc.h>
#include <math.h>  // https://www.tutorialspoint.com/c_standard_library/math_h.htm
#include <ctype.h>

#include "DHT.h"

#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "time.h"
#include "sntp.h"
#include <Adafruit_BMP280.h>
#include <Wire.h>




// DEFINE VARIABLES
#define DHTTYPE DHT22 


#define SOIL       33
#define TFT_DC    17
#define TFT_CS    5
#define TFT_RST   16
#define TFT_CLK   18
#define TFT_MOSI  23
#define TFT_MISO  19

#define DHTPIN    4
#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

// Meter colour schemes
#define RED2RED 0
#define GREEN2GREEN 1
#define BLUE2BLUE 2
#define BLUE2RED 3
#define GREEN2RED 4
#define RED2GREEN 5

#define espTimeout_ms 300
#define ARDUINOJSON_USE_DOUBLE 1

// #define trigPin  9   // Trigger
// #define echoPin  12  // Echo

#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define GREY    0x2108



/* Init class Instances for the DHT22 etcc */
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp; // use I2C interface


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);


// IMPORT FONTS FOR TFT DISPLAY
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h> 



static const char* pubtopic      = "620152241";                    // Add your ID number here
static const char* subtopic[]    = {"620152241_sub","/elet2415"};  // Array of Topics(Strings) to subscribe to
static const char* mqtt_server   = "dbs.msjrealtms.com";         // Broker IP address or Domain name as a String 
static uint16_t mqtt_port        = 1883;

// WIFI CREDENTIALS
const char* ssid       = "Digicel_WiFi_A8eG"; // Add your Wi-Fi ssid MonaConnect Digicel_WiFi_A8eG
const char* password   = "Nu5TSQ5h"; // Add your Wi-Fi password Nu5TSQ5h




// TASK HANDLES 
TaskHandle_t xMQTT_Connect          = NULL; 
TaskHandle_t xNTPHandle             = NULL;  
TaskHandle_t xLOOPHandle            = NULL;  
TaskHandle_t xUpdateHandle          = NULL;
TaskHandle_t xButtonCheckeHandle    = NULL; 


// FUNCTION DECLARATION   
void checkHEAP(const char* Name);   // RETURN REMAINING HEAP SIZE FOR A TASK
void initMQTT(void);                // CONFIG AND INITIALIZE MQTT PROTOCOL
unsigned long getTimeStamp(void);   // GET 10 DIGIT TIMESTAMP FOR CURRENT TIME
void callback(char* topic, byte* payload, unsigned int length);
void initialize(void);
bool publish(const char *topic, const char *payload); // PUBLISH MQTT MESSAGE(PAYLOAD) TO A TOPIC
void vButtonCheck( void * pvParameters );
void vUpdate( void * pvParameters ); 

 

//############### IMPORT HEADER FILES ##################
#ifndef NTP_H
#include "NTP.h"
#endif

#ifndef MQTT_H
#include "mqtt.h"
#endif


/* Initialize class objects*/

/* Declare your functions below */
uint32_t runTime = -99999;       // time for next update
int reading = 0; // Value to be displayed
int d = 0; // Variable used for the sinewave test waveform
boolean alert = 0;
int8_t ramp = 1;
int tesmod = 0;
       




void setup() {
    Serial.begin(115200);  // INIT SERIAL 
    // tft.reset();
    tft.begin();
    tft.setRotation(3); // 
    tft.fillScreen(GREY);
    // tft.setTextColor(ILI9341_BLACK); 
    tft.setCursor (248, 57);
    tft.setTextSize (2);
    tft.setTextColor (ILI9341_WHITE, GREY);
    tft.print ("HEAT ");
    tft.setCursor (250, 5);
    tft.setTextSize (2);
    tft.setTextColor (ILI9341_WHITE,GREY);
    tft.print ("HUM");
    
    //Design Interface (lines)
    tft.fillRect(0, 197, 217, 4, ILI9341_MAGENTA);
    tft.fillRect(217, 98, 320, 4, ILI9341_MAGENTA);
    tft.fillRect(217, 148, 320, 4, ILI9341_MAGENTA);
    tft.fillRect(217, 0, 4, 240, ILI9341_MAGENTA);
    tft.fillRect(221, 50, 320, 4, ILI9341_MAGENTA);
    tft.fillRect(221, 197, 320, 4, ILI9341_MAGENTA);



    dht.begin(); 
    bmp.begin(0x76);
    Wire.begin(21, 22); 
    pinMode(SOIL, INPUT);
  
  /* Configure all others here */

  initialize();           // INIT WIFI, MQTT & NTP 
  vButtonCheckFunction(); // UNCOMMENT IF USING BUTTONS THEN ADD LOGIC FOR INTERFACING WITH BUTTONS IN THE vButtonCheck FUNCTION

}
  
  
void loop() {

      vTaskDelay(1000 / portTICK_PERIOD_MS);  
}



  
//####################################################################
//#                          UTIL FUNCTIONS                          #       
//####################################################################
void vButtonCheck( void * pvParameters )  {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );     
      
    for( ;; ) {
       
        vTaskDelay(200 / portTICK_PERIOD_MS);  
    }
}


void vUpdate( void * pvParameters )  {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );    
 
    for( ;; ) {

    double dHumidity = dht.readHumidity();
    double dTemp_C = dht.readTemperature();
    double f = dht.readTemperature(true);
    double hif = dht.computeHeatIndex(f, dHumidity);
    double bTemp = bmp.readTemperature();
    double bPress = bmp.readPressure();
    double bAlt = bmp.readAltitude(1013.25);
    double a = (- 8.3144*(bTemp+273.15) / (28.97*9.806))-12*(bPress/1012.2);
    //a = abs(a); //absolute value of a
    int soilValue = analogRead(SOIL);
    int mappedValue = map(soilValue, 1150, 3410, 100, 0); //or put back to 2500 4095


      Serial.println(mappedValue);
      Serial.println(soilValue);

      if (mappedValue < 0)
        {
        mappedValue = 0;
        }
        else if (mappedValue >= 100){
        mappedValue = 100;
        }

      show(dHumidity, dTemp_C, f, hif, bTemp, bPress, bAlt, mappedValue);

       // Task code goes here.   
        // PUBLISH to topic every second. 
        // 1. Create JSon object
    
      StaticJsonDocument<1100> doc;

      // 2. Create message buffer/array to store serialized JSON object
      char message[1100] = { 0 };

      // 3. Add key:value pairs to JSon object based on above schema


      doc["type"] = "sensor";
      doc["id"] = "620152241";
      doc["temperature"] = dTemp_C;
      doc["humidity"] = dHumidity;
      doc["farTemperature"] = f;
      doc["hindex"] = hif;
      doc["soil moisture"] = mappedValue;
      doc["BMP temperature"] = bTemp;
      doc["pressure"] = bPress;
      doc["Altitude"] = bAlt;
      doc["value"] = soilValue;
      doc["timestamp"] = getTimeStamp();

    
        serializeJson(doc, message);  // Seralize / Covert JSon object to JSon string and store in char* array

        if (mqtt.connected()) {
          publish(pubtopic, message);
        }

       vTaskDelay(1000 / portTICK_PERIOD_MS);  
 
    }   
  }


unsigned long getTimeStamp(void) {
          // RETURNS 10 DIGIT TIMESTAMP REPRESENTING CURRENT TIME
          time_t now;         
          time(&now); // Retrieve time[Timestamp] from system and save to &now variable
          return now;
}

 void callback(char* topic, byte* payload, unsigned int length) {
    // ############## MQTT CALLBACK  ######################################
    // RUNS WHENEVER A MESSAGE IS RECEIVED ON A TOPIC SUBSCRIBED TO

    Serial.printf("\nMessage received : ( topic: %s ) \n", topic);
    char* received = new char[length + 1]{ 0 };

    for (int i = 0; i < length; i++) {
      received[i] = (char)payload[i];
    }

    // PRINT RECEIVED MESSAGE
    Serial.printf("Payload : %s \n", received);


    // CONVERT MESSAGE TO JSON
    StaticJsonDocument<1100> doc;
    DeserializationError error = deserializeJson(doc, received);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }


    // PROCESS MESSAGE
    const char* type = doc["type"];

    if (strcmp(type, "sensor") == 0) {

    // const char* code= doc["code"];
  }

  

}

bool publish(const char *topic, const char *payload){   
     bool res = false;
     try{
        res = mqtt.publish(topic,payload);
         Serial.printf("\nres : %d\n",res);
        if(!res){
          res = false;
          throw false;
        }
     }
     catch(...){
      Serial.printf("\nError (%d) >> Unable to publish message\n", res);
     }
  return res;
}


void show(double dHumidity, double dTemp_C, double f, double hif, double bTemp, double bPress, double bAlt, int mappedValue) {
        //delay(2000);

        tft.setCursor (235,172);
        tft.setTextSize(3);
        tft.setTextColor( CYAN,GREY);
        tft.printf("%03d",mappedValue);
        
        
        tft.setCursor(234, 78);
        tft.setTextSize (2);
        tft.setTextColor ( RED , GREY);
        tft.print (hif);
        tft.print ("F");
        tesmod=1;

        //  if(hif>0){  //Heat index Fahrenheit
          tft.setCursor (230,130);
          tft.setTextSize (2);
          tft.setTextColor (BLUE,GREY);
          tft.print (dTemp_C,0); 
          tft.print ("C");
          tesmod=1;

          tft.setCursor (275,130);
          tft.setTextSize (2);
          tft.setTextColor (MAGENTA,GREY);
          tft.print (f,0);
          tft.print ("F"); 
          tesmod=1;

          tft.setCursor (278,115);
          tft.setTextSize (1);
          tft.setTextColor (MAGENTA,GREY);
          tft.print ("Fahr");

          tft.setCursor (230,115);
          tft.setTextSize (1);
          tft.setTextColor (CYAN,GREY);
          tft.print ("Celsius");
          
          tft.setCursor (250,105);
          tft.setTextSize (1.5);
          tft.setTextColor (WHITE,GREY);
          tft.print ("DTemp");
          
        
          tft.setCursor (245,155);
          tft.setTextSize (1);
          tft.setTextColor (YELLOW,GREY);
          tft.print ("Moisture");


          tft.setCursor (245,205);
          tft.setTextSize (1);
          tft.setTextColor (GREEN,GREY);
          tft.print ("BMP Pressure");

          tft.setCursor (220,222);
          tft.setTextSize (2);
          tft.setTextColor (GREEN,GREY);
          tft.print (int(bPress));
          tft.print ("Pa");

         if (dHumidity >= 80) {
            tft.drawBitmap(80, 115, rain, 50, 50, ILI9341_WHITE, GREY);
        } else if (dHumidity >= 60) {
            tft.drawBitmap(80, 115, partlyCloudyDay, 50, 50, ILI9341_WHITE, GREY);
        } else if (dHumidity >= 40) {
            tft.drawBitmap(80, 115, sun, 50, 50, YELLOW, GREY);
        } else {
            tft.drawBitmap(80, 115, snow, 50, 50, ILI9341_WHITE, GREY);
        }
          

          
          tft.setCursor (10,214);
          tft.setTextSize (2);
          tft.setTextColor (WHITE,GREY);
          tft.print ("ALTITUDE");
          // tft.print ("C");
          
         // }
        //   if(bTemp>0){  //bmp temp
          tft.setCursor (150,210);
          tft.setTextSize (2);
          tft.setTextColor (BLUE,GREY);
          tft.print (int(bAlt)); 
          tft.print ("m");
          tesmod=1;


        //Used to print the temperature value, Pressure, and Altitude in the serial moniter
        Serial.println(" ");
        Serial.print(F("Temperature = "));
        Serial.print(dTemp_C);
        Serial.println(" C");

        Serial.print(F("Humidity = "));
        Serial.println(dHumidity);
        

        Serial.print(F("BMP Temperature = "));
        Serial.print(bTemp);
        Serial.println(" C");


        Serial.print(F("Soil Moisture = "));
        Serial.printf("%03d",mappedValue);
        Serial.println("%");

        Serial.print(F("Pressure = "));
        Serial.print(bPress);
        Serial.println("Pa");

        Serial.print(F("Approx altitude = "));
        Serial.print(bAlt); /* Adjusted to local forecast! */
        Serial.println(" m");

        Serial.print(F("Temp Fahr = "));
        Serial.print(f); /* Adjusted to local forecast! */
        Serial.println("F");

        Serial.print(F("Heat Index ="));
        Serial.print(hif); /* Adjusted to local forecast! */
        Serial.println("F");


        //checks if readings appropriate
        if (isnan(dHumidity) || isnan(dTemp_C)) {
        Serial.println(F("Fail! Colonel"));
        return;

        Serial.println(" ");
        Serial.println(" ");
    }

    int xpos = 0, ypos = 5, gap = 4, radius = 52;
    // Draw a large meter
    xpos = 320 / 2 - 160, ypos = 0, gap = 100, radius = 100;
    ringMeter(bTemp, 1, 99, xpos, ypos, radius, " BMP Celsius", BLUE2RED); 
   
    if (dHumidity > 0) { //Humidity %
      tft.setCursor (226, 25); //157,208
      tft.setTextSize (3);
      tft.setTextColor (BLUE, GREY);
      tft.print (dHumidity, 1); tft.print ('%');
      tesmod = 1;
    }
    
}


 int ringMeter(int value, int vmin, int vmax, int x, int y, int r, char *units, byte scheme)
{
  
  x += r; y += r;   
  int w = r / 3;    
  int angle = 150;  
  int v = map(value, vmin, vmax, -angle, angle); 
  byte seg = 3; 
  byte inc = 6; 
  
  int colour = GREY;
  
  for (int i = -angle + inc / 2; i < angle - inc / 2; i += inc) {
    
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (r - w) + x;
    uint16_t y0 = sy * (r - w) + y;
    uint16_t x1 = sx * r + x;
    uint16_t y1 = sy * r + y;
    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * 0.0174532925);
    float sy2 = sin((i + seg - 90) * 0.0174532925);
    int x2 = sx2 * (r - w) + x;
    int y2 = sy2 * (r - w) + y;
    int x3 = sx2 * r + x;
    int y3 = sy2 * r + y;
    if (i < v) { // Fill in coloured segments with 2 triangles
      switch (scheme) {
        case 0: colour = RED; break; // Fixed colour
        case 1: colour = GREEN; break; // Fixed colour
        case 2: colour = BLUE; break; // Fixed colour
        case 3: colour = rainbow(map(i, -angle, angle, 0, 127)); break; // Full spectrum blue to red
        case 4: colour = rainbow(map(i, -angle, angle, 70, 127)); break; // Green to red (high temperature etc)
        case 5: colour = rainbow(map(i, -angle, angle, 127, 63)); break; // Red to green (low battery etc)
        default: colour = BLUE; break; // Fixed colour
      }
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
      //text_colour = colour; // Save the last colour drawn
    }
    else // Fill in blank segments
    {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, GREY);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, GREY);
    }
  }
  // Convert value to a string
  char buf[10];
  byte len = 0;  if (value > 999) len = 5;
  dtostrf(value, 1, len, buf);
    //buf[len] = 1; 
  //buf[len] = 2; // Add blanking space and terminator, helps to centre text too!
  // Set the text colour to default
  if (value > 7) {
    tft.setTextColor(colour, GREY);
    tft.setCursor(x - 30, y - 40); tft.setTextSize(6);
    tft.print(buf);
  }
  if (value < 10) {
    tft.setTextColor(BLUE, GREY);
    tft.setCursor(x - 30, y - 40); tft.setTextSize(6);
    tft.print(buf);
  }
  tft.setTextColor(GREEN, GREY);
  tft.setCursor(x - 39, y + 75); tft.setTextSize(2);
  tft.print(units); // Units display
  // Calculate and return right hand side x coordinate
  return x + r;
}

unsigned int rainbow(byte value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to 127 = red
  byte red = 0; // Red is the top 5 bits of a 16 bit colour value
  byte green = 0;// Green is the middle 6 bits
  byte blue = 0; // Blue is the bottom 5 bits
  byte quadrant = value / 32;
  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  if (quadrant == 1) {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}