#include <max6675.h>
#include <ModbusRtu.h>
#include <SoftwareSerial.h>
//letak pin bluetooth
SoftwareSerial mySerial(1, 0); // RX, TX // dapat versi HC-05 terbalik
// letak pin DHT 22
#include <dht.h>
dht DHT;
#define DHT22_PIN 2

// data array for modbus network sharing

uint16_t au16data[16] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1 };
/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  u8serno : serial port (use 0 for Serial)
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus slave(1, mySerial, 0); // this is slave @1 and RS-232 or USB-FTDI

// Letak pin MAX6675
// pins for Bean Temperature 
int BTSO = 13;// SO=Serial Out pin 
int BTCS = 12;// CS = chip select CS pin
int BTCLK = 11;// SCK = Serial Clock pin
 
// pins for Environment Temperature 
int ETSO = 10;// SO=Serial Out pin
int ETCS = 9;// CS = chip select CS pin
int ETCLK = 8;// SCK = Serial Clock pin

MAX6675 BT(BTCLK, BTCS, BTSO);
MAX6675 ET(ETCLK, ETCS, ETSO);


void setup() {
  mySerial.begin(9600); // Soft Serial used for communication through Bluetooth module HC-05

  slave.start();
  // use Arduino pins
  au16data[2] = 0; // register input artisan untuk Bean Temperature
  au16data[3] = 0; // register input artisan untuk Environment Temperature
  au16data[4] = 0; // register input artisan untuk Suhu ruangan
  au16data[5] = 0; // register input artisan untuk Kelembapan Relatif
  delay(500);
}

void loop() {
// READ DATA
    uint32_t start = micros();
    int chk = DHT.read22(DHT22_PIN);
    uint32_t stop = micros();
  
   //write current thermocouple value
   au16data[2] = (uint16_t) (BT.readCelsius()*100); // Baca Bean Temperature
   au16data[3] = (uint16_t) (ET.readCelsius()*100); // Baca Environment Temperature 
   au16data[4] = (uint16_t) (DHT.temperature*100); // Baca Suhu ruangan
   au16data[5] = (uint16_t) (DHT.humidity*100); // Baca Kelembapan Relatif

   //poll modbus registers
   slave.poll( au16data, 16 );

   delay(500);
}
