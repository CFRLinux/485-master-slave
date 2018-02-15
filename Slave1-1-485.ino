/* Slave1-1-485.ino
svare med  data når a b er modtaget.

*/

#include <SoftwareSerial.h>

#define SSerialRX        10   //Serial Receive pin
#define SSerialTX        11   //Serial Transmit pin

#define SSerialTxControl 3    //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define Pin13LED         13

SoftwareSerial RS485Serial(SSerialRX, SSerialTX);  // RX, TX


int byteReceived;
int byteSend;

int IBat1 =1023;  // Batteri spænding Bat1
int IBat2 =1022;  // Batteri spænding Bat2
int IVL1 =1021;   // Vandnivau 1
int IVL2 =1020;   // Vandnivau 2
int IDis =1019;   // Diesel nivau
int IUsTemp =614; // Udstødnings temperatur

String Bat1, Bat2, VL1, VL2, Dis, UsTemp;


bool Q1 = false;
bool Q2 = false;


void setup()
{
  
  pinMode(Pin13LED, OUTPUT);   
  pinMode(SSerialTxControl, OUTPUT);  
  digitalWrite(SSerialTxControl, RS485Receive); // Init Transceiver

  RS485Serial.begin(4800); // set the data rate 

}


void loop() /****** LOOP: RUNS CONSTANTLY ******/
{
	if (RS485Serial.available()) 
	 {
		byteSend = RS485Serial.read();   // Read the byte 
		if (byteSend == 97)  // a
			{
			 Q1 = true;
			}	
		else if (Q1 == true && byteSend == 98)  // a & b
			{
			 Q2 = true;
			}	
		else 
			{
			 Q1 = false; Q2 = false;
			}

	if (Q2 == true)
	 {
  	delay(10);
		IntToString(); // konvatere Int til string, eller find lave data
 	  digitalWrite(Pin13LED, HIGH);  // Show activity
	  delay(5);              
	  digitalWrite(SSerialTxControl, RS485Transmit);   // Enable RS485 Transmit    
	  RS485Serial.print("a"+Bat1);   // Send the byte back
	  RS485Serial.print("b"+Bat2);   // Send the byte back
	  RS485Serial.print("c"+VL1);    // Send the byte back
	  RS485Serial.print("d"+VL2);    // Send the byte back
	  RS485Serial.print("e"+Dis);    // Send the byte back
	  RS485Serial.print("f"+UsTemp); // Send the byte back
	  RS485Serial.write("\n");       // Send the byte back
	  delay(20);   
	  digitalWrite(SSerialTxControl, RS485Receive); // Disable RS485 Transmit      
	  digitalWrite(Pin13LED, LOW);
//	  delay(10);	 
	  Q1 = false; Q2 = false;
//	  delay(100);

	 }
	}// End If RS485SerialAvailable

// Her skal der være noget kode der henter data fra sensore !!!

 
}//--(end main loop )---

void IntToString()   // konvatere Int til string, eller find lave data
 {
	Bat1 = String(IBat1,DEC);  // spænding mellem 10,7 & 13,6 Volt 0 to 1023
	Bat2 = String(IBat2,DEC);  // spænding mellem 10,7 & 13,6 Volt 0 to 1023
	VL1 = String(IVL1,DEC);    // 0-100% 0-1023
	VL2 = String(IVL2,DEC);    // 0-100% 0-1023
	Dis = String(IDis,DEC);    // 0-100% 0-1023
	UsTemp =String(IUsTemp,DEC);  // 0-100 grader 0-1023
 }

