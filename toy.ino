// Pin Definitions
const int buttonPin = 2;             // Button input pin
const int RledPin = 9;               // Red LED pin (PWM)
const int GledPin = 10;              // Green LED pin (PWM)
const int BledPin = 11;              // Blue LED pin (PWM)

// Variables & Constants
int mood = 10;                       // The toy's mood state (starts neutral at 10, max 20, min 0)
const int neutralMood = 10;          // The neutral mood state
int buttonState = 0;                 // Button Current State
bool buttonPressed = false;          // Has the button been pressed or not
unsigned long touchedTimer = 0;      // The timer used to record the last touch time
unsigned long reducedTimer = 0;      // The timer used to record the last reduced point time
const long unTouchInterval = 5000;   // 5 second interval before mood starts decreasing
const long reducedInterval = 1000;   // Decrease mood every 1 second when untouched

void setup() {
  // Initialize serial monitor
  Serial.begin(9600);

  // Initialize pins
  pinMode(buttonPin, INPUT);
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);

  // Initialize the mood state and timer
  mood = neutralMood;
  touchedTimer = millis();
}

void loop() {
  // Show the current mood state on the RGB LED
  showLEDState(mood);

  // Check if the button is pressed or not
  buttonState = digitalRead(buttonPin);

  // If the button is clicked
  if (buttonState == HIGH && !buttonPressed) {
    // Increase the mood
    mood = mood + 1;
    if (mood > 20) {
      mood = 20;
    }
    // Update the touched timer and button flag
    touchedTimer = millis();
    buttonPressed = true;
    delay(50); // Simple debouncing delay
  }

  if (buttonState == LOW && buttonPressed) {
    buttonPressed = false;
  }

  // Check if the time since the last touch is longer than the untouche interval (5 seconds)
  unsigned long currentTime = millis();
  if (currentTime - touchedTimer > unTouchInterval) {
    // Reduce the point every 1 second
    if (currentTime - reducedTimer > reducedInterval) {
      mood = mood - 1;
      if (mood < 0) {
        mood = 0;
      }
      reducedTimer = currentTime;
    }
  }
}

// Function to control RGB LED color based on the mood value (0 to 20)
void showLEDState(int currentMood) {
  int redVal = 0;
  int greenVal = 0;
  int blueVal = 0;

  if (currentMood >= 10) {
    // Spectrum from Green (10) to Blue (20)
    // Red stays 0, Green decreases, Blue increases as mood gets higher
    redVal = 0;
    greenVal = map(currentMood, 10, 20, 255, 0);
    blueVal = map(currentMood, 10, 20, 0, 255);
  } else {
    // Spectrum from Red (0) to Green (10)
    // Red decreases, Green increases as mood gets higher, Blue stays 0
    redVal = map(currentMood, 0, 10, 255, 0);
    greenVal = map(currentMood, 0, 10, 0, 255);
    blueVal = 0;
  }

  // Write values to the RGB LED pins (assuming Common Anode or Common Cathode; 
  // adjust analogWrite values with 255 - val if using Common Anode)
  analogWrite(RledPin, redVal);
  analogWrite(GledPin, greenVal);
  analogWrite(BledPin, blueVal);
}