#include <PZEM004Tv30.h> 
#include <SoftwareSerial.h> 
#include "EmonLib.h" // Include Emon Library 
EnergyMonitor emon1; // Create an instance 
EnergyMonitor emon2; // Create an instance 
EnergyMonitor emon3; // Create an instance 
EnergyMonitor emon4; // Create an instance 
PZEM004Tv30 pzem(10,11); // inisiasi pin RX=11 ,TX=10 
#define VIN A4 
const float vcc = 5.00;// supply v1 5V or 3.3V 
const float factor = 0.02;// 20mV/A is the factor 
float vs; 
float vin; 
double IrmsNock1 = 0,IrmsNock2 = 0,IrmsNock3 = 0,IrmsNock4 = 0; 
int analogPin = A5; // pin arduino yang terhubung dengan pin S modul sensor  tegangan
float Vmodul = 0.0;  
float hasil = 0.0; 
float R1 = 100000.0; //100k 
float R2 = 5260.0; //5260 ohm resistor,  int value = 0; 
void setup() { 
 Serial.begin(9600); 
 Serial1.begin(9600); 
//ACS 
 pinMode(VIN, INPUT); 
//SCT 
 for (int i=0; i<5; ++i) 
 { 
 emon1.current(A0, 48);
emon2.current(A1, 0.5); 
 emon3.current(A2, 50); 
 emon4.current(A3, 48); 
  
 IrmsNock1 += emon1.calcIrms(1480);  IrmsNock2 += emon2.calcIrms(1480);  IrmsNock3 += emon3.calcIrms(1480);  IrmsNock4 += emon4.calcIrms(1480);  } 
 IrmsNock1 = IrmsNock1/5; 
 IrmsNock2 = IrmsNock2/5; 
 IrmsNock3 = IrmsNock3/5; 
 IrmsNock4 = IrmsNock4/5; 
//pembagi tegangan  
pinMode(analogPin, INPUT); 
} 
void loop() {
Serial.print ("IRMS NOCK ="); Serial.println (IrmsNock1);  Serial.print ("IRMS NOCK 2="); Serial.println (IrmsNock2);  Serial.print ("IRMS NOCK 3="); Serial.println (IrmsNock3);  Serial.print ("IRMS NOCK 4="); Serial.println (IrmsNock4);   
 Serial1.print("1"); 
 Serial1.print(","); 
//================Pzem 01================// 
 float v1 = pzem.voltage(); 
 if(v1 != NAN){ 
 Serial.print("Voltage PZEM : "); 
 Serial.println(v1); 
 Serial1.print(v1); 
 } else { 
 Serial.println("Error reading v1"); 
 } 
Serial1.print(","); 
 float current = pzem.current(); 
 if(current != NAN){
Serial.print("Current PZEM : ");  Serial.println(current,2); 
 Serial1.print(current,2); 
 } else { 
 Serial.println("Error reading current");  } 
  
Serial1.print(","); 
 float power = pzem.power(); 
 if(current != NAN){ 
 Serial.print("Power PZEM : ");  Serial.println(power); 
 Serial1.print(power); 
 } else { 
 Serial.println("Error reading power");  } 
Serial1.print(","); 
 float energy = pzem.energy(); 
 if(current != NAN){ 
 Serial.print("Energy PZEM : ");  Serial.println(energy,2);
Serial1.print(energy,2); 
 } else { 
 Serial.println("Error reading energy"); 
 } 
Serial1.print(","); 
 float pf1 = pzem.pf(); 
 if(pf1 != NAN){ 
 Serial.print("Power factor PZEM : "); 
 Serial.println(pf1); 
 Serial1.print(pf1); 
 }  
 else { 
 Serial.println("Error reading power factor"); 
 } 
Serial1.print(","); 
  
// ===========sct01=========== 
 double Irms1 = emon1.calcIrms(1480) - IrmsNock1; // Calculate Irms only // float watt1 = Irms1*v1*pf1; 
 // Just to ignore small fluctuations
if ( Irms1 < 0 ) Irms1 = 0; 
 Serial.print("Current SCT1 : "); 
 Serial.print(Irms1,2); // Irms 
 Serial1.print(Irms1,2); // Irms 
  
Serial1.print(","); 
  
 //Watt sct 1 
 Serial.print(", Power SCT1 : "); 
 Serial.println(Irms1*v1*pf1); 
 Serial1.print(Irms1*v1*pf1); 
Serial1.print(","); 
  
// ===========sct02=========== 
 double Irms2 = emon2.calcIrms(1480) ; // Calculate Irms only // float watt2 = Irms2*v1*pf1; 
 // Just to ignore small fluctuations 
 if ( Irms2 < 0 ) Irms2 = 0; 
 Serial.print("Current SCT2 : "); 
 Serial.print(Irms2,2); // Irms
Serial1.print(Irms2,2); // Irms 
Serial1.print(","); 
//Watt sct 2 
 Serial.print(", Power SCT2 : "); 
 Serial.println(Irms2*v1*pf1); 
 Serial1.print(Irms2*v1*pf1); 
Serial1.print(","); 
  
// ===========sct03=========== 
 double Irms3 = emon3.calcIrms(1480) -IrmsNock3; // Calculate Irms only // float watt3 = Irms3*v1*pf1; 
 // Just to ignore small fluctuations 
 if ( Irms3 < 0 ) Irms3 = 0; 
 Serial.print("Current SCT3 : "); 
 Serial.print (Irms3,2); // Irms 
 Serial1.print (Irms3,2); // Irms 
Serial1.print(",");
//Watt sct 3 
 Serial.print(", Power SCT3 : "); 
 Serial.println(Irms3*v1*pf1); 
 Serial1.print(Irms3*v1*pf1); 
Serial1.print(","); 
// ===========sct04=========== 
 double Irms4 = emon4.calcIrms(1480) - IrmsNock4; // Calculate Irms only // float watt4 = Irms4*v1*pf1; 
 // Just to ignore small fluctuations 
 if ( Irms4 < 0 ) Irms4 = 0; 
 Serial.print("Current SCT4 : "); 
 Serial.print(Irms4,2); // Irms 
 Serial1.print(Irms4,2); // Irms 
Serial1.print(","); 
 //Watt sct 4 
 Serial.print(", Power SCT4 : ");
Serial.println(Irms4*v1*pf1); 
 Serial1.print(Irms4*v1*pf1); 
Serial1.print(","); 
//ACS 
 vs = (5.0 / 1023.0)* analogRead(VIN);// Read the v1 from sensor 
 vin = (vs - (vcc * 0.5)) + 0.007;// 0.007 is a value to make v1 zero when there is  no current 
 float arus = vin / factor; 
 if (vin <= 0.0055 && vin >= -0.0055) 
 {vin = 0; arus = 0;} 
 Serial.print("Current Baterai : "); 
 Serial.println(arus);  
 Serial1.print(arus);  
Serial1.print(","); 
//pembagi tegangan 
 value = analogRead(analogPin); 
 Vmodul = (value * 4.997) / 987.0;
hasil = Vmodul / (R2/(R1+R2)); 
 Serial.print("Pembagi Tegangan : ");  Serial.println(hasil,2); 
 Serial1.print(hasil,2); 
  
Serial1.print("#"); 
  
 Serial1.println(); 
// delay(70000); 
delay(4000); 
}
