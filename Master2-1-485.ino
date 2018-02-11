/* 
Maste2-1-485.ino

sender a eftefulgt af b
modtager data fra slave1,
data gemmes i aray
laver Float værdier af dataene. 

sender a eftefulgt af c
modtager data fra slave1,
data gemmes i aray
laver Float værdier af dataene. 

*/

#include <SoftwareSerial.h>

#define RS485RX        10  //Serial Receive pin
#define RS485TX        11  //Serial Transmit pin
#define RS485Control	 3   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define Pin13LED         13

SoftwareSerial RS485Serial(RS485RX, RS485TX); // RX, TX

int byteReceived;
int byteSend;
int data[2][9]; // af uransagelige orsager skal den være på 9 selv om der kun bruges 8 !!

float Bat1 =0; 	// Bat1 spending
float Bat2 =0;	// Bat1 spending
float VL1 =0;		// Vand level 1
float VL2 =0;		// Vand level i vandtank
float Die =0;		// Diesel level i Diesel tank
float UsTemp =0;	// Udstødnings temperatur
float Amp1 =0;
float Amp2 =0;
float Amp3 =0;
float Amp4 =0;

const byte numChars = 40;		//Længden på CharBuffer
char CharBuffer[numChars];	// an array to store the received data

char* Slave[]={"ab", "ac", "ad","ae"};

bool newData = false;


void setup()
 {
  Serial.begin(9600);
  Serial.println("Master2-1-485.ino test program");

  pinMode(Pin13LED, OUTPUT);   
  digitalWrite(Pin13LED, LOW); 
  pinMode(RS485Control, OUTPUT);    
  RS485Serial.begin(4800);   // set the data rate 
	delay(100);
  clearData();
 }


void loop()
 {
  newData = false;

	TransMitSlave(0);
	delay(50);
	RecvWithEndMarker();												// modtager data til \n
	if (newData == true){	FindDataIBufferX(0);} // udtræk data fra buffer og gem i Array

	digitalWrite(Pin13LED, LOW);								// Show activity
	delay(1000);


	TransMitSlave(1);
	delay(50);
	RecvWithEndMarker();												// modtager data til \n
	if (newData == true){	FindDataIBufferX(1);} // udtræk data fra buffer og gem i Array
	digitalWrite(Pin13LED, LOW);								// Show activity

	ConvDataToFload();
	PrintFloatData();

	delay(1000);

 }



void TransMitSlave(int X)
{
  digitalWrite(RS485Control, RS485Transmit);		// Enable RS485 Transmit   
	RS485Serial.write(Slave[X]);					 				// Send byte to Remote Arduino (Slave)
	digitalWrite(RS485Control, RS485Receive);			// Enable RS485 Receive
}

void RecvWithEndMarker()  			//universal
{
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (RS485Serial.available() > 0 && newData == false)
		 {
     rc = RS485Serial.read();    // Read received byte   
     if (rc != endMarker)			// hvis vi ikke er nået til \n
			{
        CharBuffer[ndx] = rc; // putter RC i buffer
        ndx++;								// tæller en op
        if (ndx >= numChars)  // er støre en eller lig med buffer længde
					{
            ndx = numChars - 1;
          }
       }
     else 											// Nåede \n 
			{
       CharBuffer[ndx] = '\0';	// terminate the string
       ndx = 0;									// nuldstil tæller
       newData = true;					// NewData JA
      }
    	}
	digitalWrite(Pin13LED, HIGH);  // Show activity

}


void showNewData()
{
    if (newData == true) {
        Serial.print("showNewData ... ");
	      Serial.println(CharBuffer); // CharBuffer indeholder hele stringen der er modtaget
    }
}


void FindDataIBufferX(int NX)           // afhængig af slaven
{
 char XA[]={'a','b','c','d','e','f','g','h','i'};
 int value[8];

 String StringOne =CharBuffer;
 String StringTo;

 for (int i=0; i <= 8; i++)
  {
   value[i] = StringOne.indexOf(XA[i]); //Finder positionen for de 8 bugstaver i XA & gemmer pos i Value[]
  }
 for (int i=0; i <= 8; i++)
  {
	 if (value[i] >= 0 )
	 {
    StringTo = StringOne.substring(value[i]+1, value[i+1]);	 
		data[NX][i]=StringTo.toInt();
	 }
	 else
	 {
		data[NX][i]=0;
	 }
	}

  newData = false;
}


void printData()
 {
 Serial.println("Data array ");
 for (int X=0; X <= 1; X++)	
	{
   for (int Y=0; Y <= 8; Y++)
		{
		 Serial.print(" X=");
		 Serial.print(X);
		 Serial.print(" Y=");
		 Serial.print(Y);
		 Serial.print(" D= ");
		 Serial.print(data[X][Y]);
		Serial.print("; ");
		}
	 Serial.println();
	}
 }


void clearData()
{
 Serial.println("Clear Data array ");

 for (int X=0; X <= 1; X++)	
	{
   for (int Y=0; Y <= 8; Y++)
		{		
		data[X][Y] =0;
		Serial.print(" X=");
		Serial.print(X);
		Serial.print(" Y=");
		Serial.print(Y);
		Serial.print(" D= ");
		Serial.print(data[X][Y]);
		Serial.print("; ");
		}	
		Serial.println();
	}
}


void ConvDataToFload()
{

	 Bat1 = 10.7+((2.9/1023)*data[0][0]);		// Bat1 spending
	 Bat2 = 10.7+((2.9/1023)*data[0][1]);		// Bat1 spending
	 VL1 = ((100.00/1023.00)*data[0][2]);		// Vand level 1
	 VL2 = ((100.00/1023.00)*data[0][3]);		// Vand level i vandtank
	 Die = ((100.00/1023.00)*data[0][4]);		// Diesel level i Diesel tank
	 UsTemp = ((100.00/1023.00)*data[0][5]);	// Ustødnings temperatur     

	 Amp1 = ((100.00/1023)*data[1][0]);		// Bat1 spending
	 Amp2 = ((100.00/1023)*data[1][1]);		// Bat1 spending
	 Amp3 = ((100.00/1023.00)*data[1][2]);		// Vand level 1
	 Amp4 = ((100.00/1023.00)*data[1][3]);		// Vand level i vandtank
}

void PrintFloatData()
{
	 Serial.println("Følgende data er modtaget fra Slaver");
	 Serial.print("Bat1= ");
	 Serial.print(Bat1);
	 Serial.print(" Bat2= ");
	 Serial.print(Bat2);
 	 Serial.print(" VL1= ");
	 Serial.print(VL1);
	 Serial.print(" VL2= ");
	 Serial.print(VL2);
	 Serial.print(" Diesel= ");
	 Serial.print(Die);
	 Serial.print(" Udstødnings temp= ");
	 Serial.print(UsTemp);
	 Serial.print(" Amp1= ");
	 Serial.print(Amp1);
	 Serial.print(" Amp2= ");
	 Serial.print(Amp2);
 	 Serial.print(" Amp3= ");
	 Serial.print(Amp3);
	 Serial.print(" Amp4= ");
	 Serial.println(Amp4);
}

