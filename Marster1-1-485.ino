/* 
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

//int arrayPointer = 0;
//int counter = 0;
int byteReceived;
int byteSend;
int data[2][8] = { { 0, 0, 0, 0, 0, 0, 0, 0}, { 0, 0, 0, 0, 0, 0, 0, 0} };

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

char* Slave[]={"aa", "ab", "ac","ad"};

bool newData = false;
bool PrintData = true; // False = ingen print på skærm
bool Slave1	= false;
bool Slave2	= false;
bool Slave3	= false;

void setup()
 {
  Serial.begin(9600);
  Serial.println("Master1-1-485.ino test program");
//	clearData();  
  pinMode(Pin13LED, OUTPUT);   
  digitalWrite(Pin13LED, LOW); 
  pinMode(RS485Control, OUTPUT);    
  RS485Serial.begin(4800);   // set the data rate 
	delay(100);
 }


void loop()
 {
	TransMitSlave(1);
	RecvWithEndMarker();			// modtager data til \n
	FindDataIBuffer1();				// udtrækker data fra buffer og laver Float værdiere ud af dem

	delay(50);
	digitalWrite(Pin13LED, LOW);  // Show activity
	delay(1000);

	TransMitSlave(2);
	RecvWithEndMarker();			// modtager data til \n
  FindDataIBufferX(1);
	FindDataIBuffer2();				// udtrækker data fra buffer og laver Float værdiere ud af dem
 
	delay(50);
	digitalWrite(Pin13LED, LOW);  // Show activity
	delay(1000);
	
	Serial.println();
	printData();

 }

void clearData()
{
 for (int X=1; X <= 2; X++)	
	{
   for (int Y=1; Y <= 8; Y++)
		{		
		data[X][Y] =0;
		}	
	}
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
        Serial.print("Modtaget ... ");
	      Serial.println(CharBuffer); // CharBuffer indeholder hele stringen der er modtaget
  //      newData = false;
    }
}


void FindDataIBuffer1()           // afhængig af slaven
{
	if (newData == true)
	{
	String StringOne =CharBuffer;
	String StringTo;

	int A = StringOne.indexOf('a'); // hvor står a
	int B = StringOne.indexOf('b'); // hvor står b
	int C = StringOne.indexOf('c'); // hvor står c
	int D = StringOne.indexOf('d'); // hvor står d
	int E = StringOne.indexOf('e'); // hvor står e
	int F = StringOne.indexOf('f'); // hvor står f
	int G = StringOne.indexOf('\0'); // hvor står \0


  StringTo = StringOne.substring(A+1,B);	// udtag data fra string mellem A+1 & B
	A = StringTo.toInt();										// konvater string til Int
	StringTo = StringOne.substring(B+1,C);	// udtag data fra string mellem B+1 & C
	B = StringTo.toInt();										// konvater string til Int
  StringTo = StringOne.substring(C+1,D);	// udtag data fra string mellem C+1 & D
	C = StringTo.toInt();										// konvater string til Int
  StringTo = StringOne.substring(D+1,E);	// udtag data fra string mellem D+1 & E
	D = StringTo.toInt();										// konvater string til Int
  StringTo = StringOne.substring(E+1,F);	// udtag data fra string mellem E+1 & F
	E = StringTo.toInt();										// konvater string til Int
  StringTo = StringOne.substring(F+1,G);	// udtag data fra string mellem F+1 & G
	F = StringTo.toInt();										// konvater string til Int

/*
	Serial.print("Bat1= ");
	Serial.print(A);
	Serial.print(" Bat2= ");
	Serial.print(A);
 	Serial.print(" VL1= ");
	Serial.print(C);
	Serial.print(" VL2= ");
	Serial.print(D);
	Serial.print(" Diesel= ");
	Serial.println(E);
*/

	 Bat1 = 10.7+((2.9/1023)*A);		// Bat1 spending
	 Bat2 = 10.7+((2.9/1023)*B);		// Bat1 spending
	 VL1 = ((100.00/1023.00)*C);		// Vand level 1
	 VL2 = ((100.00/1023.00)*D);		// Vand level i vandtank
	 Die = ((100.00/1023.00)*E);		// Diesel level i Diesel tank
	 UsTemp = ((100.00/1023.00)*F);	// Ustødnings temperatur     



if (PrintData == true)
	{
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
	 Serial.println(UsTemp);
	}
  newData = false;
	}
}



void FindDataIBuffer2()           // afhængig af slaven
{
	if (newData == true)
	{
	String StringOne =CharBuffer;
	String StringTo;

	int A = StringOne.indexOf('a'); // hvor står a
	int B = StringOne.indexOf('b'); // hvor står b
	int C = StringOne.indexOf('c'); // hvor står c
	int D = StringOne.indexOf('d'); // hvor står d
//	int E = StringOne.indexOf('e'); // hvor står e
//	int F = StringOne.indexOf('f'); // hvor står f
	int G = StringOne.indexOf('\0'); // hvor står \0

  StringTo = StringOne.substring(A+1,B);	// udtag data fra string mellem A+1 & B
	A = StringTo.toInt();										// konvater string til Int
	StringTo = StringOne.substring(B+1,C);	// udtag data fra string mellem B+1 & C
	B = StringTo.toInt();										// konvater string til Int
  StringTo = StringOne.substring(C+1,D);	// udtag data fra string mellem C+1 & D
	C = StringTo.toInt();										// konvater string til Int
  StringTo = StringOne.substring(D+1,G);	// udtag data fra string mellem D+1 & E
	D = StringTo.toInt();										// konvater string til Int
//  StringTo = StringOne.substring(E+1,F);	// udtag data fra string mellem E+1 & F
//	E = StringTo.toInt();										// konvater string til Int
//  StringTo = StringOne.substring(F+1,G);	// udtag data fra string mellem F+1 & G
//	F = StringTo.toInt();										// konvater string til Int


	 Amp1 = ((100.00/1023)*A);		// Bat1 spending
	 Amp2 = ((100.00/1023)*B);		// Bat1 spending
	 Amp3 = ((100.00/1023.00)*C);		// Vand level 1
	 Amp4 = ((100.00/1023.00)*D);		// Vand level i vandtank


if (PrintData == true)
	{
	 Serial.print("Amp1= ");
	 Serial.print(Amp1);
	 Serial.print(" Amp2= ");
	 Serial.print(Amp2);
 	 Serial.print(" Amp3= ");
	 Serial.print(VL1);
	 Serial.print(" Amp4= ");
	 Serial.println(Amp4);
	}
  newData = false;
	}
}




void FindDataIBufferX(int NX)           // afhængig af slaven
{
 char XA[]={'a','b','c','d','e','f','g','h','i'};
 int value[8];

// int eol;
// int counter;
 String StringOne =CharBuffer;
 String StringTo;

 for (int i=1; i <= 8; i++)
  {
   value[i] = StringOne.indexOf(XA[i]);
  }

 for (int i=1; i <= 8; i++)
  {
	 if (value[i] >= 0 )
	 {
    StringTo = StringOne.substring(value[i]+1, value[i+1]);	 
		data[NX][i]=StringTo.toInt();
	 }
	}
  newData = false;
}


void printData()
 {
 Serial.println();
 for (int X=1; X <= 2; X++)	
	{
	 Serial.println();
   for (int Y=1; Y <= 8; Y++)
		{
		 Serial.print("X=");
		 Serial.print(X);
		 Serial.print("Y=");
		 Serial.print(Y);
		 Serial.print(" Data= ");
		 Serial.print(data[X][Y]);
		}
	 Serial.println();
	}
	 Serial.println();
 }

