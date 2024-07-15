#include <ArduinoBLE.h>
#include <time.h>
#include <Arduino_BuiltIn.h>


// variables for button
//const int buttonPin = 2;
char Switch_value[1];

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  // configure the button pin as input
  // pinMode(buttonPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);  //Initialize the built-in LED pin to indicate when a central is connected
  // initialize the Bluetooth® Low Energy hardware
  BLE.begin();

  Serial.println("Bluetooth® Low Energy Central - LED control");

  // start scanning for peripherals
  BLE.scanForUuid("1234");
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "LED Switch") {
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("1234");
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic SwitchCharacteristic = peripheral.characteristic("1234");

  if (!SwitchCharacteristic) {
    Serial.println("Peripheral does not have Switch characteristic!");
    peripheral.disconnect();
    return;
  } else if (!SwitchCharacteristic.canRead()) {
    Serial.println("Peripheral does not have a writable LED characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) {
    // while the peripheral is connected

    // read the button pin
    //int buttonState = digitalRead(buttonPin);

    //if (oldButtonState != buttonState) {
    // button changed
    //oldButtonState = buttonState;

    //if (buttonState) {
    //Serial.println("button pressed");

    // button is pressed, write 0x01 to turn the LED on
    //ledCharacteristic.writeValue((byte)0x01);
    // } //else {
    // Serial.println("button released");

    // button is released, write 0x00 to turn the LED off
    //ledCharacteristic.writeValue((byte)0x00);
    //}
    //}

    delay(1000);
    SwitchCharacteristic.readValue(Switch_value,1);
    Serial.print("Current switch value is: ");
    //Switch_value[0]=(byte)0x30;
    Serial.print(Switch_value[0]);
    //Serial.print(SwitchCharacteristic.value());
    if(Switch_value[0]==(byte)0x30){
      digitalWrite(LED_BUILTIN, LOW);
    }
    else{
      digitalWrite(LED_BUILTIN, HIGH);
    }
    Serial.println();

  }


Serial.println("Peripheral disconnected");
}
