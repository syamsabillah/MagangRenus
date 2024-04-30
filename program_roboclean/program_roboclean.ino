//Library Radio Module nRF24L01 
#include <SPI.h> 
#include <nRF24L01.h> 
#include <RF24.h> 
//Define Radio Module nRF24L01 RF24 radio(7,8);//CE CSN 
//Variabel Radio Module nRF24L01 const byte address[6] = "00001"; int receivedData[2]; 
int xAxis = 127;  
int yAxis = 127; 
//Define Pin Motor Kiri 
#define Kiri_A 25 
#define Kiri_B 24 
#define pwm_Kiri 5 
//Define Pin Motor Kanan 
#define Kanan_A 22 
#define Kanan_B 23
#define pwm_Kanan 4 
//Variabel PWM Motor int kiri = 0; 
int kanan = 0; 
//Define Encoder Motor #define Enc_Kiri_A 3 
#define Enc_Kiri_B 2 
#define Enc_Kanan_A 19 #define Enc_Kanan_B 18 
//Variabel Encoder 
int kiriState,kiriLast; 
int kananState,kananLast; long int pulse_kiri = 0; long int pulse_kanan = 0; float gear = 13.7; 
float pulse_revolution = 95.9; float jarak_tempuh; 
int selisih; 
float sudut = 57.2727;
float heading; 
float Enc_mm = 0.3246; 
//Define Ultrasonic 
#define trigPin 30 
#define echoPin 31 
//Variabel Ultrasonic 
long duration; 
int distance; 
void setup() { 
 //Inisiasi Serial 
 Serial.begin(9600); 
 //Inisiasi Ultrasonic 
 pinMode(trigPin, OUTPUT);  pinMode(echoPin, INPUT);   
 //Inisiasi Motor Kiri 
 pinMode(Kiri_A, OUTPUT);  pinMode(Kiri_B, OUTPUT);
pinMode(pwm_Kiri, OUTPUT); 
 //Inisiasi Motor Kanan 
 pinMode(Kanan_A, OUTPUT); 
 pinMode(Kanan_B, OUTPUT); 
 pinMode(pwm_Kanan, OUTPUT); 
 //Inisiasi Encoder Motor 
 pinMode(Enc_Kiri_A, INPUT); 
 pinMode(Enc_Kiri_B, INPUT); 
 pinMode(Enc_Kanan_A, INPUT); 
 pinMode(Enc_Kanan_B, INPUT); 
 //Inisiasi Interrupt Encoder 
 attachInterrupt(digitalPinToInterrupt(Enc_Kiri_A),bacaEncoderKiri,CHANGE); 
  
attachInterrupt(digitalPinToInterrupt(Enc_Kanan_A),bacaEncoderKanan,CHAN GE); 
 //Inisiasi Radio Module nRF24L01 
 radio.begin(); 
 radio.openReadingPipe(0,address); 
 radio.setPALevel(RF24_PA_MIN);
radio.startListening(); 
  
} 
void loop() { 
 //Komunikasi Radio 
 while(radio.available()){ 
 radio.read(&receivedData, sizeof(receivedData));  xAxis = receivedData[0]; 
 yAxis = receivedData[1]; 
 } 
 Serial.print("X : "); 
 Serial.print(xAxis); 
 Serial.print(" || Y : "); 
 Serial.println(yAxis); 
 RemoteControl_Mode(); 
  
 delay(30); 
}
void RemoteControl_Mode() { //Fungsi Control  if(yAxis < 115) { //Analog Keatas 
 Maju(); 
 kiri = map(yAxis, 115, 0, 0, 255); 
 kanan = map(yAxis, 115, 0, 0, 255); 
 } 
 else if (yAxis > 140) { //Analog Kebawah  Mundur(); 
 kiri = map(yAxis, 140, 255, 0, 255); 
 kanan = map(yAxis, 140, 255, 0, 255); 
 } 
 else { //Analog Steady 
 kiri = 0; 
 kanan = 0; 
 } 
 if(xAxis < 115) { //Analog ke kiri, Motor kiri melambat  int xMap = map(xAxis, 115, 0, 0, 255); 
 kiri = kiri - xMap; 
 kanan = kanan + xMap;
if(kanan < 0) { 
 kanan = 0; 
 } 
 if(kiri > 255) { 
 kiri = 255; 
 } 
 } 
 if(xAxis > 140) { //Analog ke kanan, Motor kanan melambat  int xMap = map(xAxis, 140, 255, 0, 255); 
 kiri = kiri + xMap; 
 kanan = kanan - xMap; 
 if(kiri < 0) { 
 kiri = 0; 
 } 
 if(kanan > 255) { 
 kanan = 255; 
 } 
} 
 analogWrite(pwm_Kiri, kiri);
analogWrite(pwm_Kanan, kanan); } 
void Ultrasonic(){ 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin, HIGH); 
 delayMicroseconds(10); 
 digitalWrite(trigPin, LOW); 
 //Formula Jarak 
 duration = pulseIn(echoPin, HIGH);  distance = duration * 0.034 / 2; 
} 
void bacaEncoderKiri(){ 
 kiriState = digitalRead(Enc_Kiri_A);  if(kiriState != kiriLast){ 
 if(digitalRead(Enc_Kiri_B) != kiriState){  pulse_kiri++; 
 } 
 else{ 
 pulse_kiri--; 
 }
} 
 kiriLast = kiriState; 
} 
void bacaEncoderKanan(){ 
 kananState = digitalRead(Enc_Kanan_A);  if(kananState != kananLast){ 
 if(digitalRead(Enc_Kanan_B) != kananState){  pulse_kanan++; 
 } 
 else{ 
 pulse_kanan--; 
 } 
 } 
 kananLast = kananState; 
} 
void Maju(){ 
 digitalWrite(Kiri_A, HIGH); 
 digitalWrite(Kiri_B, LOW); 
 digitalWrite(Kanan_A, HIGH); 
 digitalWrite(Kanan_B, LOW); 
 Serial.println("Maju"); 
}
void Kanan(){ 
 digitalWrite(Kiri_A, HIGH);  digitalWrite(Kiri_B, LOW);  digitalWrite(Kanan_A, LOW);  digitalWrite(Kanan_B, HIGH);  Serial.println("Kanan"); } 
void Kiri(){ 
 digitalWrite(Kiri_A, LOW);  digitalWrite(Kiri_B, HIGH);  digitalWrite(Kanan_A, HIGH);  digitalWrite(Kanan_B, LOW);  Serial.println("Kiri"); 
} 
void Berhenti(){ 
 digitalWrite(Kiri_A, LOW);  digitalWrite(Kiri_B, LOW);  digitalWrite(Kanan_A, LOW);  digitalWrite(Kanan_B, LOW);  Serial.println("Berhenti"); } 
void Mundur(){
digitalWrite(Kiri_A, LOW);  digitalWrite(Kiri_B, HIGH);  digitalWrite(Kanan_A, LOW);  digitalWrite(Kanan_B, HIGH);  Serial.println("Mundur"); }
Lampiran 6. Program Base Station //Library Bluetooth Serial 
#include <SoftwareSerial.h> 
//Library Radio Module nRF24L01 #include <SPI.h> 
#include <nRF24L01.h> 
#include <RF24.h> 
//Define Bluetooth Module 
SoftwareSerial BT(2, 3); // RX, TX 
//Pin Radio Module nRf24L01 RF24 radio(8,10); //CE CSN 
//Variabel Radio Module nRF24L01 const byte address[6] = "00001"; int xAxis=140, yAxis=140; 
int Array[2]; 
void setup() { 
 //Inisiasi Serial Monitor 
 Serial.begin(9600);
//Inisiasi Bluetooth Module  BT.begin(9600); 
 //Inisiasi Radio Module nRF24L01  radio.begin(); 
 radio.openWritingPipe(address);  radio.setPALevel(RF24_PA_MIN);  radio.stopListening(); 
 delay(500); 
} 
void loop() { 
 //Komunikasi Bluetooth 
 while (BT.available() >= 2) {  xAxis = BT.read(); 
 delay(10); 
 yAxis = BT.read(); 
 } 
 delay(10);
//Map Data Joystick 
 int x_sent = map(xAxis, 60, 220, 0, 255);  int y_sent = map(yAxis, 60, 220, 0, 255); 
 //Input ke Array 
 Array[0] = x_sent; 
 Array[1] = y_sent; 
 //Pengiriman Data Radio 
 radio.write(&Array, sizeof(Array)); 
 //Cetak Serial Monitor 
 Serial.print(xAxis); 
 Serial.print(","); 
 Serial.print(yAxis); 
 Serial.print(";"); 
 Serial.print(Array[0]); 
 Serial.print(","); 
 Serial.println(Array[1]); 
 delay(20); 
}
