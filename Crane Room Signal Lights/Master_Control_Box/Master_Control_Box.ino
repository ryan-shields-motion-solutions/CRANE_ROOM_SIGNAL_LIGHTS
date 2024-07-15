#include <ArduinoBLE.h>
#include <time.h>
#include <Arduino_BuiltIn.h>

BLEService LEDservice("1234");  //BLE LED Service

//BLE Characteristic for master control box arduino. Creates a boolean that remote devices can read to determine whether the switch is on or off
BLECharCharacteristic SwitchCharacteristic("1234", BLERead | BLENotify | BLEBroadcast);
char switchflag[1];        //Last state of the LED switch
long previousMillis = 0;  // Last time the LED switch was checked in ms
int D2Pin=2;
int switchstateold=0;
int switchstatenew=0;


void setup() {
  // put your setup code here, to run once:
  delay(10);
  Serial.begin(9600);  //initialize Serial communication
  delay(10);
  Serial.print("Starting Program");
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);  //Initialize the built-in LED pin to indicate when a central is connected
pinMode(D2Pin,INPUT_PULLUP);
  while (!Serial) {
  }
  if (!BLE.begin()) {
    Serial.println("Failed to initialiaze BLE!");
    while (true) {
    }
  }



  /* Set a local name for the BLE device. This will appear in the advertising packets
   and can be used by remote devices to identify the BLE device.
*/
  BLE.setLocalName("LED Switch");
  BLE.setAdvertisedService(LEDservice);                //add the service UUID
  LEDservice.addCharacteristic(SwitchCharacteristic);  //add the switch status service
  BLE.addService(LEDservice);                          //add the LED service
  SwitchCharacteristic.writeValue(switchflag[0]);           //set the intial value of the switch characteristic

  /* Start advertising Bluetooth® Low Energy.  It will start continuously transmitting Bluetooth® Low Energy
     advertising packets and will be visible to remote Bluetooth® Low Energy central devices
     until it receives a new connection.
*/
  //Start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}


void loop() {
  //wait for a BLE central
  BLEDevice central = BLE.central();

  //if  a central is coonnected to the peripheral
  if (central) {
    Serial.print("Connected to central: ");
    //print the central'sBT address:
    Serial.println(central.address());
    //turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    //check the switch status every 500 ms while the central is connected:
    while (central.connected()) {
      long currentMillis = millis();
      //if 500 ms have passed, then check the switch status
      if (currentMillis-previousMillis >=500){
        previousMillis=currentMillis;
        updateSwitchStatus();
      }
    }

    //when the central disconnects, turn off the LED
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print ("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void updateSwitchStatus(){

switchstatenew=digitalRead(D2Pin);
if (switchstatenew!=switchstateold){
  Serial.print("Switch state is now: ");
  Serial.println(switchstatenew);
  if(switchstatenew==1){
    switchflag[0]=(byte)0x30;
  }
  else{
    switchflag[0]=(byte)0x31;
  }
  SwitchCharacteristic.writeValue(switchflag[0]);
  switchstateold=switchstatenew;
}






}
