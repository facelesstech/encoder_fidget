#include <EncoderStepCounter.h>

#include <FastLED.h>

// Pins for the encoder
const int encoderPinA = 2;
const int encoderPinB = 3;

// Create an instance of EncoderStepCounter
EncoderStepCounter encoder(encoderPinA, encoderPinB);

// Variables to store encoder values
long previousPosition = 0;
int direction = 0;
int counter = 0;
int button_mode = 0;

// Number of LEDs in the strip
const int numLeds = 16;
// LED strip data pin
const int dataPin = 6;
// Create an array of CRGB for the LED strip
CRGB leds[numLeds];
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

// Button stuff
const int buttonPin = 7;    // the number of the pushbutton pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  // Button stuff
  pinMode(buttonPin, INPUT_PULLUP);
//  digitalWrite(buttonPin, HIGH); // initiate the internal pull up resistor

  Serial.begin(9600);
  encoder.begin();
  encoder.setPosition(0);

  // Initialize FastLED library
  FastLED.addLeds<WS2812, dataPin, GRB>(leds, numLeds);
//  FastLED.setBrightness(64);
  FastLED.setBrightness(20);
}

void interrupt() {
  encoder.tick();
}

 // Sinelon stuff
void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, numLeds, 2);
  int pos = beatsin16( 13, 0, numLeds-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void loop() {
  encoder.tick();
  // Read the current encoder position
  long currentPosition = encoder.getPosition();
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // WHATEVER THE READING IS AT, IT'S BEEN THERE FOR LONGER
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        Serial.println("button press");
//        Serial.println(button_mode);

        button_mode++;
        if (button_mode == 4) {
          button_mode = 0;
        }

      }
    }
  }
  lastButtonState = reading;


  if (currentPosition != previousPosition) {
    // Check the direction of rotation
    if (currentPosition > previousPosition) {
      direction = 1; // Clockwise
    } else {
      direction = -1; // Counterclockwise
    }

    // Update the counter value
    counter += direction;

    // Limit the counter between 0 and 16
    if (counter > 16) {
      counter = 16;
    } else if (counter < 0) {
      counter = 0;
    }
    
    if (button_mode == 0){
    // Light up LEDs based on the direction
    if (direction == 1) {
      // Clockwise: Set LEDs to white
      fill_solid(leds, counter, CRGB::Red);
      fill_solid(leds + counter, numLeds - counter, CRGB::Black);
    } else {
      // Counterclockwise: Set LEDs to blue
      fill_solid(leds, counter, CRGB::Green);
      fill_solid(leds + counter, numLeds - counter, CRGB::Black);
    }

    // Show the LED strip
    FastLED.show();

    // Update the previous position
    previousPosition = currentPosition;
  }
    if (button_mode == 1){
    // Light up LEDs based on the direction
    if (direction == 1) {
      // Clockwise: Set LEDs to white
      fill_solid(leds, counter, CRGB::Purple);
      fill_solid(leds + counter, numLeds - counter, CRGB::Black);
    } else {
      // Counterclockwise: Set LEDs to blue
      fill_solid(leds, counter, CRGB::Blue);
      fill_solid(leds + counter, numLeds - counter, CRGB::Black);
    }

    // Show the LED strip
    FastLED.show();

    // Update the previous position
    previousPosition = currentPosition;
  }

    if (button_mode == 2){
  sinelon();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
//  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through t    he rainbow

  }
    if (button_mode == 3)
    {
    if (direction == 1) 
    {
      gHue++;
      // Clockwise: Set LEDs to white
//      fill_solid(leds, counter, CHSV( gHue, 255, 192));
      fill_solid(leds, counter, CHSV( gHue, 254, 254));
      fill_solid(leds + counter, numLeds - counter, CRGB::Black);
    }

    else {
      gHue--;
      // Counterclockwise: Set LEDs to blue
      fill_solid(leds, counter, CRGB::White);
      fill_solid(leds + counter, numLeds - counter, CRGB::Black);
    }

    // Show the LED strip
    FastLED.show();

    // Update the previous position
    previousPosition = currentPosition;
    }
    }
  }
//}
