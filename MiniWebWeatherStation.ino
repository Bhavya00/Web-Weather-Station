#include <ESP8266WiFi.h>
#include<DHT.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);


#define DHTPIN 3
#define DHTTYPE DHT11 
DHT dht(DHTPIN,DHTTYPE);
float h;
float t;

const char* ssid="Your SSID";
const char* password="Your WiFi Password";
char status;
WiFiServer server(80);

void setup() {
Serial.begin(115200);
delay(100);
Wire.begin(2,0);
lcd.init();   // initializing the LCD
lcd.backlight();
lcd.setCursor(0,0);
dht.begin();
 Serial.print("Connecting to");
 Serial.println(ssid);
 WiFi.begin(ssid,password);

while(WiFi.status() !=WL_CONNECTED){
  delay(500);
  Serial.print(".");
  }
Serial.println("");
Serial.println("WiFi is connected");
server.begin();
Serial.println("Server started");
Serial.println(WiFi.localIP());
}

void loop() {
 h=dht.readHumidity();
 t=dht.readTemperature();
 Serial.println(t);
 lcd.print(t);
 lcd.print(" C");
 lcd.setCursor(0,1);
 delay(50);
 lcd.clear();
 
 float f=dht.readTemperature(true);

 WiFiClient client =server.available();
 client.println("HTTP/1.1 200 OK");
 client.println("Content-Type: text/html");
 client.println("Connection: close");
 client.println("Refresh: 10");
 client.println();
 client.println("<!DOCTYPE HTML>");
 client.println("<html>");
 client.println("<style>html{font-family: aerial;display:block;margin: 0px auto; text-align: centre; color: #333333;background-color: #a4d3ed;}");
 client.println("body{margin-top: 50px}");
 client.println("h1{margin 50px auto 30px; font-size: 50px; text-align: centre;}");
 client.println(".side_adjust{display: inline-block;vertical-align: middle;position: relative;}");
client.println(".text1{font-weight: 180; padding-left: 15px; font-size: 50px; width: 170px; text-align: left; color: #3498db;}");
client.println(".data1{font-weight: 180; padding-left: 80px; font-size: 50px;color: #3498db;}");
client.println(".text2{font-weight: 180; font-size: 50px; width: 170px; text-align: left; color: #ff6600;}");
client.println(".data2{font-weight: 180; padding-left: 150px; font-size: 50px;color: #ff6600;}");
client.println(".data{padding: 10px;}");
client.println("</style>");
client.println("</head>");
client.println("<body>");
client.println("<div id=\"webpage\">");   
client.println("<h1>Temperature server</h1>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text1\">Humidity:</div>");
client.println("<div class=\"side_adjust data1\">");
client.print(h);
client.println("<div class=\"side_adjust text1\">%</div>");
client.println("</div>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text2\">Temperature:</div>");
client.println("<div class=\"side_adjust data2\">");
client.print(t);
client.println("<div class=\"side_adjust text2\">*C</div>");
client.print(f);
client.println("<div class=\"side_adjust text2\">F</div>");
client.println("</div>");
client.println("</body>");
client.println("</html>");
delay(4000);
}
