 #include <Wire.h>
#include <BH1750.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

String devicename = "Smart Vase";

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int ploop;
boolean boot;
String getsoftserial;
/* /////////////////////////// Public status Variables \\\\\\\\\\\\\\\\\\\\*/
String svip;
String stime;
String sdate;
String wifistatus;

/* \\\\\\\\\\\\\\\\\\\\\\\\\\\ Public status Variables ////////////////////*/
const unsigned char wifiicon[] PROGMEM =  {
  0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x1f, 0xf8, 0x38, 0x1c, 0x60, 0x06, 0xc7, 0xe3, 0x0f, 0xf0,
  0x18, 0x18, 0x10, 0x08, 0x07, 0xe0, 0x06, 0x60, 0x04, 0x20, 0x00, 0x00, 0x01, 0x80, 0x01, 0x80
};


const unsigned char irrigationicon [] PROGMEM= {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x01, 0xfc, 0x00, 
  0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x3f, 0x00, 
  0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x0f, 0x80, 
  0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x07, 0xc0, 
  0x00, 0x00, 0x00, 0x1f, 0x00, 0x0f, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x1f, 0xc3, 0xe0, 
  0x00, 0x00, 0x00, 0x1e, 0x00, 0x3f, 0xe3, 0xe0, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x7f, 0xf1, 0xe0, 
  0x00, 0x00, 0x00, 0x1e, 0x00, 0xff, 0xf9, 0xe0, 0x00, 0x00, 0x00, 0x1e, 0x01, 0xff, 0xff, 0xe0, 
  0x00, 0x00, 0x00, 0x1f, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0x07, 0xff, 0xff, 0xe0, 
  0x00, 0x00, 0x00, 0x1f, 0x0f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0x9f, 0xff, 0xff, 0xe0, 
  0x00, 0x00, 0x00, 0x0f, 0xbf, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xf8, 
  0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xfe, 
  0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xfe, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfc, 
  0x00, 0x00, 0x20, 0x00, 0x3f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x30, 0x00, 0x1f, 0xff, 0xff, 0xf0, 
  0x00, 0x00, 0x38, 0x01, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x1c, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xc0, 
  0x00, 0x3c, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x0e, 0x38, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x00, 
  0x1e, 0x10, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0c, 0x00, 0x3d, 0xff, 0xff, 0xff, 0xfc, 0x00, 
  0x00, 0x00, 0x38, 0x01, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x30, 0x00, 0x01, 0xff, 0xf0, 0x00, 
  0x00, 0x38, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 
  0x38, 0x78, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x78, 0x30, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 
  0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char humidityicon [] PROGMEM = {
  0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x01, 0x06, 0x00, 0x02, 0x89, 0x00, 0x02, 0x91, 0x00, 0x04, 
  0x40, 0x80, 0x08, 0x20, 0x40, 0x10, 0x10, 0x40, 0x20, 0x00, 0x40, 0x20, 0x08, 0x40, 0x40, 0x04, 
  0x40, 0x40, 0x04, 0x80, 0x40, 0x05, 0x00, 0x50, 0x04, 0x00, 0x50, 0x04, 0x00, 0x48, 0x00, 0x00, 
  0x24, 0x08, 0x00, 0x11, 0x10, 0x00, 0x0c, 0x60, 0x00, 0x00, 0x00, 0x00
};

const unsigned char tempicon [] PROGMEM =  {
  0x01, 0xc0, 0x00, 0x01, 0x20, 0x00, 0x02, 0x3e, 0x00, 0x02, 0x20, 0x00, 0x02, 0x20, 0x00, 0x02, 
  0xa0, 0x00, 0x02, 0xa0, 0x00, 0x02, 0xae, 0x00, 0x02, 0xa0, 0x00, 0x02, 0xb8, 0x00, 0x02, 0xa0, 
  0x00, 0x02, 0xa0, 0x00, 0x02, 0xa0, 0x00, 0x02, 0xa0, 0x00, 0x02, 0xa0, 0x00, 0x05, 0xd0, 0x00, 
  0x05, 0xd0, 0x00, 0x05, 0xd0, 0x00, 0x02, 0x20, 0x00, 0x01, 0xc0, 0x00
};

const unsigned char sunicon [] PROGMEM = {
  0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x10, 0x00, 0x80, 0x08, 0xe1, 0x00, 0x03, 
  0xfa, 0x00, 0x06, 0x0c, 0x00, 0x04, 0x06, 0x00, 0x0c, 0x02, 0x00, 0x0c, 0x03, 0x00, 0xec, 0x03, 
  0x70, 0x0c, 0x03, 0x00, 0x04, 0x02, 0x00, 0x06, 0x04, 0x00, 0x03, 0xfc, 0x00, 0x08, 0xf1, 0x00, 
  0x10, 0x00, 0x80, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00
};


String pubcommand;
int flwlightvalue;
int i; /* for counter*/
int counter;
int minimum_humidity = 50;
float minimum_light = 500;


BH1750 lightMeter;
const int flinepixel = 16; /*first line pixel for LED*/
const int soil_pin = A0;  /* Soil moisture sensor O/P pin */
const int flwlightsens = A0; /* set A1 pin for geting flower Light resived*/
const int pumprelayPin = 3; /* set Digital port 3 to set waterpump relay switch */
const int lightPin = 4; /* set Digital port 4 to set Lighting relay switch*/
/*const int tempPin = 2;  set Digital port 5 to set temperture with a 4.7 K ohm resistor*/
const int Resetpin = 2; /* Set Digital port 2 to restart Arduino */
int moisture_percentage; /*for save humidity percentage */
int humidPPin = 10;
int lightingPPin = 9;
//******************************************************* Temp Sensor aconfig ****************************************************
//   for setup pin for get serial of temperture sensors
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 0
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
//********************************************************************************************************************************

WiFiServer server(80); //set Web server port

// NTP configuration
const long utcOffsetInSeconds = 16200; // set Tehran Timezone

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
//  get NTP Time


void setup() {
  Serial.begin(115200); /* Define baud rate for serial communication */

  /* set analog pins */
  pinMode(pumprelayPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, LOW);
  digitalWrite(pumprelayPin, LOW);
  pinMode(humidPPin, OUTPUT);
  pinMode(lightingPPin, OUTPUT);
  digitalWrite(humidPPin, LOW);
  digitalWrite(lightingPPin, LOW);
    
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  // On esp8266 devices you can select SCL and SDA pins using Wire.begin(D4, D3);
  //*********************************************** setup I2C ID and recive event **************************************************

  Wire.begin();
  // begin returns a boolean that can be used to detect setup problems.
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    //Serial.println(F("BH1750 Advanced begin"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
  }

  sensors.begin(); /* for geting temperture sensors data*/

  /*                                                setup display                              */
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.clearDisplay();

  /* setup Wifi Module */
  //SoftwareSerial toESP(8, 9);//12==>TX , 13==>RX
  //toESP.begin(115200);
  String WifiName = "3rfan"; //for join to wifi modem
  String WifiPass = "3rfan3056";
//////////////////////////////////////////////////////// load First Boot Functions \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

firstboot();
}



void loop() {

  // get soil moistor
  int soilhumid = soildata();
  // get room Temp
  int temp = gettemp();
  //Serial.println(temp);
  // get room light
  int roomlight = getroomlight();
  //get Radiant light analog value
  int flwlightvalue = getlightingstat();
  makeweb(temp, roomlight, soilhumid, 1, 1);
  ploop++;
  Serial.println(ploop);
  if (ploop <= 10) {
    showdisplay(1);
  } else if (ploop <= 15) {
    showdisplay(2);
  } else if (ploop <= 17) {
    showdisplay(4);
  } else if (ploop <= 19) {
    showdisplay(4);
    ploop = 0;
  }



  //Serial.print("Moisture Percentage = ");
  //Serial.println(moisture_percentage);
  //  Serial.println("%\n\n");
  //  Serial.println(sensor_analog); /* for View humidity analog data*/
  if (moisture_percentage < minimum_humidity) {
    irrigation();
  }
  // get Light data from digital (I2C) sensore

  //  Serial.print("Light: ");
  //  Serial.print(lux);
  //  Serial.println("lux");



  delay(1000);
}

int getroomlight(){
  int lroomlight = lightMeter.readLightLevel();
  //lroomlight = lroomlight/54612*100;
    return lroomlight;
}

int getlightingstat(){
  pinMode(lightingPPin, OUTPUT);
  digitalWrite(lightingPPin, HIGH);
  delay(50);
  int lightedstat = analogRead(flwlightsens);
  Serial.println(lightedstat); 
  digitalWrite(lightingPPin, LOW);
  pinMode(lightingPPin, INPUT);
  return lightedstat;
}

int soildata() {
  float sensor_analog;
  pinMode(humidPPin, OUTPUT);
  digitalWrite(humidPPin, HIGH);
  delay(1000);
  sensor_analog = analogRead(soil_pin);
  /* for arduino uno betwen 300 and 625 , for arduino pro mini betwen 440 and 925*/
  moisture_percentage = 100 - ( ( (sensor_analog - 440) * 100 ) / 495 );
  if (moisture_percentage >= 100) {
    moisture_percentage = 99;
  }


  digitalWrite(humidPPin, LOW);
  pinMode(humidPPin, INPUT);
  
  return moisture_percentage;
}

void irrigation() {
  if (i >= 3) {
    //turn off LED For geting power
    //digitalWrite(lightPin, LOW);
    //start irrigation for 5 Sec
    //Serial.print("Start irrigation for 5 second !");
    /* show notification */
    showdisplay(3);
    digitalWrite(pumprelayPin, HIGH);
    delay(1000);
    //Serial.println("Stop irrigation !");
    digitalWrite(pumprelayPin, LOW);
    //Serial.print("Stoped irrigation !");
    //delay(3000);
    i = 0;
  } else {
    i += 1;
  }
}

int gettemp() {
  //get temperture digital data from configed port
  sensors.requestTemperatures();
  //  Serial.print("Temperature is: ");
  //  Serial.println(sensors.getTempCByIndex(0)); /* Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire */
  return sensors.getTempCByIndex(0);
}

void showdisplay(int page) {
  int retry = 0;
  int soilhumid;
  int roomlight;
  int temp;
  String stime;
  display.clearDisplay();
  switch (page) {
    case 0:
      page0(0);
      delay(1000);
      page0(1);
      while (wifistatus == "" and retry < 3) {
        retry++;
        wificonnection("3rfan", "3rfan3056");
      }
      //Serial.println(wifistatus);
      if (wifistatus == "Connected") {
        page0(2);
        delay(5000);
      } else {
        page0(3);
      }
      return;
    case 1:

      // get soil moistor
      soilhumid = soildata();
      // get room Temp
      temp = gettemp();
      //Serial.println(temp);
      // get room light
      roomlight = getroomlight();
      //get Radiant light analog value
      flwlightvalue = getlightingstat();
      page1(temp, roomlight, soilhumid, flwlightvalue, nowtime());
      return;
    case 2:
      page2(nowtime());
      return;
    case 3:
      page3();
      return;
  }
}

void page0(int stat) { // 0=first  1=wificonnecting  2=wificonnected  3=wifierror  4=checksensors

  switch (stat) {
    case  0:
      // ========================================= Line 1 Font 2 color yellow
      display.setCursor(2, 0);
      display.setTextSize(2);
      display.print(devicename); // show Device name on top of LCD
      display.display();
      return;
    // ========================================= Line 1 Font 2 color yellow
    case 1:
      // ========================================= Line 2 Font 1 color white
      display.setCursor(0, 16);
      display.setTextSize(1);
      display.print("Connecting to WIFI..."); // show connecting massage
      display.display();
      return;
    // ========================================= Line 2 Font 1 color white
    case 2:
      // ========================================= Line 3 Font 1 color white
      display.setCursor(0, 24);
      display.setTextSize(1);
      display.print("WIFI Connected !"); // show Connected massage
      // ========================================= Line 3 Font 1 color white
      // ========================================= Line 4 Font 1 color white
      display.setCursor(0, 32);
      display.setTextSize(1);
      display.print("IP : "); // show IP address
      display.print(svip); // show IP address
      display.display();
      return;
    // ========================================= Line 4 Font 1 color white
    case 3:
      // ========================================= Line 3 Font 1 color white
      display.setCursor(0, 32);
      display.setTextSize(2);
      display.print("WIFI Error"); // show Connected massage
      display.display();
      return;
    // ========================================= Line 3 Font 1 color white
    case 4:
      return;

  }
}

void page1(int ltemp, int lroomlight, int lsoilhumid, int lflwlightvalue, String stime) {
  display.clearDisplay();
  // ========================================= Line 1 show Wifi Icon color yellow
  display.drawBitmap(0, 0, wifiicon, 16, 16, 1);
  // ========================================= Line 1 show Wifi Icon color yellow
  // ========================================= Line 1 Font 1 color yellow
  display.setCursor(32, 0);
  display.setTextSize(1);
  display.print(sdate); // show Date on top of LCD
  // ========================================= Line 1 Font 1 color yellow
  display.fillRect(0, 15, 127, 1, WHITE);
  // ========================================= Line 2 Font 1 color yellow
  display.setCursor(45, 6);
  display.print(stime); // show Time on top of LCD
  // ========================================= Line 2 Font 1 color yellow
  // ========================================= Line 2 Font 2 color white
  display.setCursor(0, 16);
  display.setTextSize(2);
  display.drawBitmap(0, 16, tempicon, 20, 20, 1);
  //display.print("Temp"); // Temp Title
  display.setCursor(65, 16);
  display.drawBitmap(65, 16, humidityicon, 20, 20, 1);
  //display.print("Humid"); // Humid Title
  display.setCursor(23, 18);
  display.setTextSize(2);
  display.print(ltemp); // Show Temp Degrees
  display.setCursor(45, 18);
  display.setTextSize(1);
  display.print("C"); // Show Degrees Sign
  display.setTextSize(2);
  display.setCursor(87, 18);
  display.print(lsoilhumid); // show humidity percentage
  display.print("%"); // Show Degrees Sign
  // ========================================= Line 2 Font 2 color white
  // ========================================= Line 3 Font 2 color white
  display.drawBitmap(1, 40, sunicon, 20, 20, 1);
  //display.fillRect(0, 48, 127, 63, WHITE);
  //display.setTextSize(2);
  //display.setTextColor(BLACK, WHITE);
  display.setCursor(23, 45);
  //display.print("Light:"); // 
  display.print(lroomlight); // show humidity percentage
  display.setTextSize(1);
  display.print("LUX"); // 
  // ========================================= Line 3 Font 2 color white
  display.display();
}

void page2(String stime) {
  display.clearDisplay();
  display.stopscroll();
  // ========================================= Line 1 show Wifi Icon color yellow
  display.drawBitmap(0, 0, wifiicon, 16, 16, 1);
  // ========================================= Line 1 show Wifi Icon color yellow
  // ========================================= Line 1 Font 1 color yellow
  display.setCursor(32, 0);
  display.setTextSize(1);
  display.print(sdate); // show Date on top of LCD
  // ========================================= Line 1 Font 1 color yellow
  // ========================================= Line 2 Font 4 color white
  display.setCursor(6, 22);
  display.setTextSize(4);
  display.print(stime); // Time
  // ========================================= Line 2 Font 4 color white
  display.display();
}


void page3() {
  display.clearDisplay();
  display.stopscroll();
  // ========================================= Line 1 show Wifi Icon color yellow
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.print("Irrigation"); // Irrigation Status
  // ========================================= Line 1 show Wifi Icon color yellow
  // ========================================= Line 1 Font 1 color yellow
  display.drawBitmap(32, 16, irrigationicon, 64, 54, 1);
  // ========================================= Line 1 Font 1 color yellow
  // ========================================= Line 2 Font 2 color white

  // ========================================= Line 2 Font 2 color white
  display.display();
}

void firstboot() {
  showdisplay(0);
  boot == true;
}

int wificonnection(String ssid, String password) {
  int count = 1;
  String cmd;
  String ipip;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED || count < 10) {
    delay ( 1000 );
    count++;
    //Serial.print ( "." );
  }
  if (WiFi.status() == WL_CONNECTED ) {
    svip = WiFi.localIP().toString();
    wifistatus = "Connected";

    Serial.print("Wifi connected#IP=");
    Serial.println(WiFi.localIP());

  } else {
    wifistatus = "";
  }
}

void makeweb(int ltemp, int llight, int lhumid, int llighting, int lgetlight) {
  // set local variables ********************************************************************************************************
  WiFiClient client = server.available();
  server.begin();

  // Check if a client has connected ********************************************************************************************


  if (!client) {
    return;
  }
  // Wait until the client sends some data %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //Serial.println("new client");


  // Read the first line of the request %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //String request = client.readStringUntil('\r');
  //Serial.println(request);

  // Return the response
  client.println("<HTML>");
  client.println("<h2> Welcome to my Smart Vase </h2>");
  client.println("<table class=\"sensorfataTable\">");
  client.println("<tbody>");
  client.println("<tr>");
  client.println("<td><strong>Sensor</strong></td>");
  client.println("<td><strong>Value</strong></td>");
  client.println("<td><strong>Unit</strong></td>");
  client.println("</tr>");
  client.println("<tr>");
  client.println("<td>Temperture</td>");
  client.print("<td style=\"text-align: center;\">");
  client.print(ltemp);
  client.println("</td>");
  client.println("<td>&deg;C</td>");
  client.println("</tr>");
  client.println("<tr>");
  client.println("<td>Humidity</td>");
  client.print("<td style=\"text-align: center;\">");
  client.print(lhumid);
  client.println("</td>");
  client.println("<td>%</td>");
  client.println("</tr>");
  client.println("<tr>");
  client.println("<td>Light</td>");
  client.print("<td style=\"text-align: center;\">");
  client.print(llight);
  client.println("</td>");
  client.println("<td>Lux</td>");
  client.println("</tr>");
  client.println("<tr>");
  client.println("<td>Lighting</td>");
  client.print("<td style=\"text-align: center;\">");
  client.print(llighting);
  client.println("</td>");
  client.println("<td>T/F</td>");
  client.println("</tr>");
  client.println("<tr>");
  client.println("<td>Get Light?</td>");
  client.print("<td style=\"text-align: center;\">");
  client.print(lgetlight);
  client.println("</td>");
  client.println("<td>T/F</td>");
  client.println("</tr>");
  client.println("</tbody>");
  client.println("</table>");
  client.println("</HTML>");
  // Stop Client Connection
  client.stop();
  //Serial.println("Client Disconnected");


}


String nowtime() {
  String timetime;
  timeClient.update();
  if (timeClient.getHours()<10){
    timetime = "0";
  }else{
    timetime = "";
  }
  timetime += timeClient.getHours();
  timetime += ":";
  if (timeClient.getMinutes()<10){
    timetime += "0";
  }
  timetime += timeClient.getMinutes();
  //  Serial.println(timetime);
  return timetime;
}


void restart() {
  digitalWrite(Resetpin, HIGH);
  digitalWrite(Resetpin, LOW);

}
