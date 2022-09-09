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

     IMU(); 
     delay(50);
   }
  }
 } 

 int IMU (void){

       
         //Calculo del angulo del Giroscopio
         Gy[0] = GyX/G_R;
         Gy[1] = GyY/G_R;
         Gy[2] = GyZ/G_R;
      
         dt = (millis() - tiempo_prev) / 1000.0;
         tiempo_prev = millis();
       
         Angle[0] = 0.02 *(Angle[0]+Gy[0]*dt) + 0.98*Acc[0];
         
          if(Acc[0]>0 && Acc[1]<0){
          Angle[0]=Angle[0]+90;
          }
          if(Acc[0]<0 && Acc[1]<0){
          Angle[0]=Angle[0]+90;
          }
          if(Acc[0]>0 && Acc[1]>0){
          Angle[0]= 270-Angle[0];
          }
          if(Acc[0]<0 && Acc[1]>0){
          Angle[0]=270-Angle[0];
          }
          
         Angle[1] = 0.02 *(Angle[1]+Gy[1]*dt) + 0.98*Acc[1];   //revisar

          if(Acc[0]<0 && Acc[1]<0){
          Angle[1]=-Angle[1];
          }
          if(Acc[0]>0 && Acc[1]<0){
          Angle[1]=Angle[1]+180;
          }
          if(Acc[0]>0 && Acc[1]>0){
          Angle[1]=Angle[1]+180;
          }
          if(Acc[0]<0 && Acc[1]>0){
          Angle[1]=-Angle[1]+360;
          }
          
         //Integración respecto del tiempo paras calcular el YAW
         Angle[2] = Angle[2]+Gy[2]*dt;
       
         //Mostrar los valores por consola

         //Serial.println(valores);
 //        client.print("Valor_X");
 //        client.print(valores);
 //        client.print('\r');
         


 
   //Calculo del angulo del Giroscopio
   Gy2[0] = GyX2/G_R;
   Gy2[1] = GyY2/G_R;
   Gy2[2] = GyZ2/G_R;

   dt2 = (millis() - tiempo_prev2) / 1000.0;
   tiempo_prev2 = millis();
 
   //Aplicar el Filtro Complementario
   Angle2[0] = 0.02 *(Angle2[0]+Gy2[0]*dt2) + 0.98*Acc2[0];
   Angle2[1] = 0.02 *(Angle2[1]+Gy2[1]*dt2) + 0.98*Acc2[1];

   //Integración respecto del tiempo paras calcular el YAW
   Angle2[2] = Angle2[2]+Gy2[2]*dt2;
 
   int analog_value1 = map(analogRead(ANALOG_PIN_0), 0, 4096, 0, 40);  // escalamos la lectura a un valor entre 0 y 180 
   //int analog_value2 = map(analogRead(ANALOG_PIN_1), 0, 4096, 0, 180);  // escalamos la lectura a un valor entre 0 y 180 

   valores = String(Angle[0]) + "," + String(Angle[1]) + "," + String(Angle[2]) + "," + String(Angle2[0]) + "," + String(Angle2[1]) + "," + String(Angle2[2]) +","+ String(analog_value1);
  //valores = String(Angle[0]) + "," + String(Angle[1]) + "," + String(Angle[2]);
   client.print(valores);
   //Serial.println(analog_value1);
   client.print('\r');
  }
