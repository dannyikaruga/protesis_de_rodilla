//conectar la segunda mpu en ADO en lugar de vcc
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <Wire.h>

const char* ssid = "DESKTOP";
const char* password = "Microprots";
 
WiFiServer server(26);
WiFiClient client;

//#define MPU 0x68
//#define MPU2 0x69

Adafruit_MPU6050 MPU;
Adafruit_MPU6050 MPU2;
 
//Ratios de conversion
#define A_R 16384.0 // 32768/2
#define G_R 131.072 // 32768/250
 
//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.29577951

#define ANALOG_PIN_0 36




int analog_value1 = 0;
 
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ; 
int16_t AcX2, AcY2, AcZ2, GyX2, GyY2, GyZ2; 
 
float Acc[2],Acc2[2];
float Gy[3],Gy2[3];
float Angle[3],Angle2[3];
String valores,valores2;
long tiempo_prev,tiempo_prev2;
float dt,dt2;

int variable;
 
void setup() {

  Serial.begin(115200);
  
  WiFi.begin(ssid,password);
  Serial.println("Connecting");
 
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.print("Connected to "); 
  Serial.println(ssid);
  
  Serial.print("IP Address: "); 
  Serial.println(WiFi.localIP());
 
  server.begin();

  // Try to initialize!
  if (!MPU.begin(0x68) || !MPU2.begin(0x69)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
}
 
void loop() {
  // Listen for connecting clients
  client = server.available();
  if (client){
   Serial.println("Client connected");
    while (client.connected()){


     delay(50);
   }
  }
 } 
