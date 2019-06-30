#include <Arduino.h>
#include <USBComposite.h>
#include <EEPROM.h>

#define PIN_A PA0
#define PIN_B PA1
#define BUTTON PA2

USBHID HID;
HIDKeyboard Keyboard(HID);
USBCompositeSerial CompositeSerial;
RotaryEncoder encoder(PIN_A, PIN_B, BUTTON);

int16_t position = 0;
int8_t buttonDown = 0;

int8_t upKey = KEY_F1;
int8_t downKey = KEY_F2;
int8_t buttonKey = KEY_F3;

void encoderISR()
{
  encoder.readAB();
}

void encoderButtonISR()
{
  encoder.readPushButton();
}

void setup() {
  HID.begin(CompositeSerial, HID_KEYBOARD);
  Keyboard.begin();

  encoder.begin();                                                           //set encoders pins as input & enable built-in pullup resistors

  attachInterrupt(digitalPinToInterrupt(PIN_A),  encoderISR,       CHANGE);  //call encoderISR()    every high->low or low->high changes
  attachInterrupt(digitalPinToInterrupt(BUTTON), encoderButtonISR, FALLING); //call pushButtonISR() every high->low              changes

  delay(1000);

  // Read current key settings via EEPROM library
}

void loop()
{
  if(CompositeSerial.available()) {
    // Read and reconfigure key settings
    // Store via EEPROM library
  }

  if (position != encoder.getPosition()) {
    int16_t newPosition = encoder.getPosition();
    if(newPosition > position) {
      Keyboard.press(upKey);
    } else {
      Keyboard.press(downKey);
    }
    position = newPosition;
  }
  
  if (encoder.getPushButton() == true) {
    if(buttonDown == 0) {
      Keyboard.press(buttonKey);
      buttonDown = 1;
    }
  } else {
    buttonDown = 0;
  }
}