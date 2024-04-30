/* 
Project by Renus Global Indonesia 
Energy Monitoring  
*/ 
#include <ESP8266WiFi.h> 
#include <Firebase_ESP_Client.h> 
#include <WiFiManager.h> 
//Firebase Library 
#include "addons/TokenHelper.h" 
#include "addons/RTDBHelper.h" 
//Define WiFi 
#define WIFI_SSID "RenusGlobalAward" 
#define WIFI_PASSWORD "wifinyagaada" 
//Define Firebase Account 
#define USER_EMAIL "aditya@renergynusantara.com" #define USER_PASSWORD "renusglobalindonesia" 
//Define Firebase Key 
#define API_KEY "AIzaSyDsPeId501Ic5zJxpk3l5cmj92VT1Bvvmc"
//Define Firebase URL 
#define DATABASE_URL "https://energy-monitoring-renus-default-rtdb.asia southeast1.firebasedatabase.app" 
//Define Firebase Name 
FirebaseData fbdo; 
FirebaseAuth auth; 
FirebaseConfig config; 
//Variabel Data 
String dataIn,dt[16]; 
float  
Count,Vpzem1,Apzem1,Wpzem1,Epzem1,PFpzem1,Asct1,Wsct1,Asct2,Wsct2,A sct3,Wsct3,Asct4,Wsct4,Iacs,Vbatt; 
int i; 
bool koneksi = true; 
bool parsing = false; 
void setup() { 
 Serial.begin(9600); 
 Config_Wifi(); 
 Config_Firebase();
} 
void loop() { 
 while(Serial.available()!=0){ 
 char inChar = Serial.read(); 
 dataIn += char(inChar); 
 if(inChar == '\n'){ 
 parsing = true; 
 } 
 } 
 if(parsing){ 
 Parsing_Data(); 
 parsing = false; 
 dataIn = ""; 
 } 
} 
void Config_Wifi(){ 
 int i; 
 WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
 Serial.print("Connecting to Wi-Fi"); 
 while (WiFi.status() != WL_CONNECTED && koneksi == true){  Serial.print(".");
delay(300); 
 i++; 
 if(i>20){ 
 Manager(); 
 koneksi = false; 
 i=0; 
 } 
 } 
 Serial.println(); 
 Serial.print("Connected with IP: "); 
 Serial.println(WiFi.localIP()); 
 Serial.println(); 
} 
void Manager(){ 
 WiFiManager wm; 
 bool res; 
  
 res = wm.autoConnect("IoT Monitoring Energy Management","12345678"); 
 if(!res){ 
 Serial.println("Gagal Koneksi"); 
 }
else{ 
 Serial.println("Berhasil Terkoneksi"); 
 } 
} 
void Config_Firebase(){ 
 config.api_key = API_KEY; 
 config.database_url = DATABASE_URL; 
 auth.user.email = USER_EMAIL; 
 auth.user.password = USER_PASSWORD; 
 config.token_status_callback = tokenStatusCallback;  Firebase.begin(&config, &auth); 
 Firebase.reconnectWiFi(true); 
} 
void Parsing_Data(){ 
 int j=0; 
 //kirim data yang telah diterima sebelumnya  Serial.print("data masuk : "); 
 Serial.print(dataIn); 
 Serial.print("\n"); 
 //inisialisasi variabel, (reset isi variabel)
dt[j]=""; 
 //proses parsing data 
 for(i=1;i<dataIn.length();i++){ 
 //pengecekan tiap karakter dengan karakter (#) dan (,) 
 if ((dataIn[i] == '#') || (dataIn[i] == ',')){ 
 //increment variabel j, digunakan untuk merubah index array penampung  j++; 
 dt[j]=""; //inisialisasi variabel array dt[j] 
 } 
 else{ 
 //proses tampung data saat pengecekan karakter selesai.  dt[j] = dt[j] + dataIn[i]; 
 } 
 }  
 //Pzem 1 
 Vpzem1=dt[1].toFloat(); 
 Apzem1=dt[2].toFloat(); 
 Wpzem1=dt[3].toFloat(); 
 Epzem1=dt[4].toFloat(); 
 PFpzem1=dt[5].toFloat(); 
  
 //SCT
Asct1=dt[6].toFloat(); 
 Wsct1=dt[7].toFloat(); 
 Asct2=dt[8].toFloat(); 
 Wsct2=dt[9].toFloat(); 
 Asct3=dt[10].toFloat(); 
 Wsct3=dt[11].toFloat(); 
 Asct4=dt[12].toFloat(); 
 Wsct4=dt[13].toFloat(); 
 //ACS 
 Iacs=dt[14].toFloat(); 
 Vbatt=dt[15].toFloat(); 
// Upload Data 
 Firebase_Send(); 
  
} 
void Firebase_Send(){ 
 //Kirim Vpzem1 
 if(Firebase.RTDB.setFloat(&fbdo, "data/pzem1/voltage", Vpzem1)){  Serial.println("Vpzem1 Passed"); 
 Serial.print("\n");
Serial.print("Volt pzem : "); 
 Serial.print(Vpzem1); 
 Serial.print("\n"); 
 } 
 else{ 
 Serial.println("Vpzem1 FAILED"); 
 Serial.println("REASON: " + fbdo.errorReason());  } 
 //Kirim Apzem1 
 if(Firebase.RTDB.setFloat(&fbdo, "data/pzem1/ampere", Apzem1)){  Serial.println("Apzem1 Passed"); 
 Serial.print("\n"); 
 Serial.print("Arus pzem : "); 
 Serial.print(Apzem1); 
 Serial.print("\n"); 
 } 
 else{ 
 Serial.println("Apzem1 FAILED"); 
 Serial.println("REASON: " + fbdo.errorReason());  }
