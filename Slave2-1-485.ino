/* Slave2-1-485.ino
svare med  data når a c er modtaget.

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

int IAmp1 =412; 	// 
int IAmp2 =0;			// 
int IAmp3 =120;		//
int IAmp4 =1019;	//

String Amp1, Amp2, Amp3, Amp4;


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
		else if (Q1 == true && byteSend == 99)  // a & c
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
		IntToString(); 									// konvatere Int til string, eller find data
 	  digitalWrite(Pin13LED, HIGH);  // Show activity
	  delay(5);              
	  digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit    
	  RS485Serial.print("a"+Amp1); 	// Send the byte back
	  RS485Serial.print("b"+Amp2); 	// Send the byte back
	  RS485Serial.print("c"+Amp3); 	// Send the byte back
	  RS485Serial.print("d"+Amp4); 	// Send the byte back
    RS485Serial.write("\n"); // Send the byte back
	  delay(20);   
	  digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit      
	  digitalWrite(Pin13LED, LOW);
	  Q1 = false; Q2 = false;
	 }
	}

}

void IntToString()					// konvatere Int til string, eller find lave data
 {
	Amp1 = String(IAmp1,DEC); // 
	Amp2 = String(IAmp2,DEC); // 
	Amp3 = String(IAmp3,DEC);	// 
	Amp4 = String(IAmp4,DEC);	// 
 }

