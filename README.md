# 485-master-slave
Et simpelt forsøg, på at lave RS485 kommunikation mellem flere Arduinoer.
En master og et X antal slaver.

Masteren kontakter  slave  en af gangen, ved at sende adressen på slaven ud på bussen,
slaven lytter efter sin adresse på bussen,
& svare efterfølgende med de data den er i besiddelse af,

Masteren laver efterfølgende  behandling af dataene
og sender dem til PC.
 
 ******
 
A simple attempt to make RS485 communication between several Arduinoes.
A master and an X number of slaves.

The master contacts the slave one at a time by sending the address of the slave to the bus,
The slave listens for his address on the bus,
& respond subsequently with the data it holds,

The master subsequently processes the data
and send them to PC.
