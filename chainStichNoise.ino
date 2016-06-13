/*
* chainStitch Noise for ATtiny85
*
* Two capacitive sensors controlling a white noise generator.
*
* Links-references:
* https://code.google.com/p/greennoise/downloads/detail?name=prbsGen.pde&can=2&q=label%3AWhite+Noise
* https://en.wikipedia.org/wiki/Linear_feedback_shift_register
* http://www.electricdruid.net/index.php?page=techniques.practicalLFSRs
*
* Afroditi Psarra
* June 2016
*/

#include <CapacitiveSensor.h> // Use the capacitive sensing library.

CapacitiveSensor switchOn = CapacitiveSensor(1, 2); // 1MOhms resistor between digital pin 1 and 2, 2 is the sensor pin.
CapacitiveSensor touch = CapacitiveSensor(4, 3); // 1MOhms resistor between digital pin 4 and 3, 3 is the sensor pin.

long touchVal = 0; // Variable to store the touch sensor's value
int outVal = 0; // Variable to store the mapped touch sensor's value

long switchOnVal = 0; // Variable to store the switchOn sensor's value
int freqVal = 0; // Variable to store the mapped switchOn sensor's value - to be later used as frequency modulator

int speakerPin = 0; // Speaker connected to digital pin 0
// int speakerPin = 11; // Speaker connected to digital pin 11 - for debugging with Arduino UNO

unsigned long int reg;

void setup() {
  //  Serial.begin(9600); // for debugging with Arduino UNO.
  pinMode(speakerPin, OUTPUT); // initialize the speakerPin as an output
  reg = 0x55aa55aaL; //The seed for the bitstream. It can be anything except 0.
}

void loop() {
  touchVal = touch.capacitiveSensor(30); // read the touch sensor value
  outVal = map(touchVal, 0, 30000, 50, 0); // map the touch sensor value in milliseconds to be used as a delay between each grain repetition.

  switchOnVal = switchOn.capacitiveSensor(30); // read the SwitchOn sensor value - used to turn on the noise generator.
  freqVal = map(switchOnVal, 0, 15000, 400, 0); // map the touch sensor value in milliseconds to be used as a delay between each grain repetition.

  //  Serial.print("\t switchOnVal = ");
  //  Serial.print(switchOnVal);
  //  Serial.print("\t touchVal = ");
  //  Serial.print(touchVal);
  //  Serial.print("\t freqVal = ");
  //  Serial.println(freqVal);

  if (touchVal > 40) {
    for (int i = 0; i < 500; i++) {
      generateNoise();
    }
    delay(outVal);
  }

  if (switchOnVal > 40) {
    for (int i = 0; i < 150; i++) {
      generateNoise();
    }
    delay(outVal);
  }

  else {
    digitalWrite(speakerPin, LOW);
  }
}

// the noise producing function
void generateNoise() {
  unsigned long int newr;
  unsigned char lobit;
  unsigned char b31, b29, b25, b24;
  b31 = (reg & (1L << 31)) >> 31;
  b29 = (reg & (1L << 29)) >> 29;
  b25 = (reg & (1L << 25)) >> 25;
  b24 = (reg & (1L << 24)) >> 24;
  lobit = b31 ^ b29 ^ b25 ^ b24;
  newr = (reg << 1) | lobit;
  reg = newr;
  digitalWrite(speakerPin, reg & 1);

  delayMicroseconds(freqVal); // Changing this value changes the frequency.
}
