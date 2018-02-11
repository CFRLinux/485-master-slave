/* Slave1-1-485.ino
svare med (#1 data) når a b er modtaget.

*/

#include <SoftwareSerial.h>

#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define Pin13LED         13

SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX


int byteReceived;
int byteSend;
bool Q1 = false;
bool Q2 = false;


void setup()
{
  
  pinMode(Pin13LED, OUTPUT);   
  pinMode(SSerialTxControl, OUTPUT);  
  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver

  RS485Serial.begin(4800);   // set the data rate 

}


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
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
 	  digitalWrite(Pin13LED, HIGH);  // Show activity
	  delay(5);              
	  digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit    
	  RS485Serial.write("BAX:1023"); // Send the byte back
	  RS485Serial.write("BAY:1018"); // Send the byte back
	  RS485Serial.write("VLX:0"); // Send the byte back
	  RS485Serial.write("VLY:512"); // Send the byte back
	  RS485Serial.write("DIX:412"); // Send the byte back
	  RS485Serial.write("\n"); // Send the byte back
	  delay(10);   
	  digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit      
	  digitalWrite(Pin13LED, LOW);
//	  delay(10);	 
	  Q1 = false; Q2 = false;
//	  delay(100);

	 }

	}// End If RS485SerialAvailable
 
}//--(end main loop )---

