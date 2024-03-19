#include <CapacitiveSensor.h>

class CapacitiveSensorReader {
  private:
    CapacitiveSensor cs;
    long threshold;

  public:
    // Constructor to initialize the capacitive sensor pin and the threshold.
    CapacitiveSensorReader(byte sendPin, byte receivePin, long thresholdValue) : cs(sendPin, receivePin), threshold(thresholdValue) {
    }

    // Begin method to set up the sensor (call this in setup()).
    void begin() {
      cs.set_CS_AutocaL_Millis(0xFFFFFFFF);  // Turn off auto-calibrate.
    }

    // Method to set a new threshold.
    void setThreshold(long newThreshold) {
      threshold = newThreshold;
    }

    // Method to read the capacitance value.
    long readCapacitance() {
      return cs.capacitiveSensor(30); // Read capacitance value (30 samples).
    }

    // Check if the capacitance is above the threshold.
    int isAboveThreshold() {
      long capacitanceValue = readCapacitance();
      return capacitanceValue > threshold ? 1 : 0;
    }
};

// Initialize all sensor reader objects
CapacitiveSensorReader sensorReader1(3, 2, 50);
CapacitiveSensorReader sensorReader2(5, 4, 50);
CapacitiveSensorReader sensorReader3(7, 6, 50);
CapacitiveSensorReader sensorReader4(9, 8, 50);
CapacitiveSensorReader sensorReader5(11, 10, 50);
CapacitiveSensorReader sensorReader6(13, 12, 50);
CapacitiveSensorReader sensorReader7(27, 26, 50);
CapacitiveSensorReader sensorReader8(29, 28, 50);
CapacitiveSensorReader sensorReader9(23, 22, 50);
CapacitiveSensorReader sensorReader10(25, 24, 50);
CapacitiveSensorReader sensorReader11(37, 36, 50);
CapacitiveSensorReader sensorReader12(35, 34, 50);
CapacitiveSensorReader sensorReader13(45, 44, 50);
CapacitiveSensorReader sensorReader14(43, 42, 50);
CapacitiveSensorReader sensorReader15(33, 32, 50);
CapacitiveSensorReader sensorReader16(31, 30, 50);
CapacitiveSensorReader sensorReader17(51, 50, 50);
CapacitiveSensorReader sensorReader18(53, 52, 50);
CapacitiveSensorReader sensorReader19(39, 38, 50);
CapacitiveSensorReader sensorReader20(41, 40, 50);
CapacitiveSensorReader sensorReader21(49, 48, 50);
CapacitiveSensorReader sensorReader22(47, 46, 50);

void setup() {
  Serial.begin(9600);
  sensorReader1.begin();
  sensorReader2.begin();
  sensorReader3.begin();
  sensorReader4.begin();
  sensorReader5.begin();
  sensorReader6.begin();
  sensorReader7.begin();
  sensorReader8.begin();
  sensorReader9.begin();
  sensorReader10.begin();
  sensorReader11.begin();
  sensorReader12.begin();
  sensorReader13.begin();
  sensorReader14.begin();
  sensorReader15.begin();
  sensorReader16.begin();
  sensorReader17.begin();
  sensorReader18.begin();
  sensorReader19.begin();
  sensorReader20.begin();
  sensorReader21.begin();
  sensorReader22.begin();
}

// Static variables to keep track of toggle states for vocal and drum loops.
static int toggleStateDrumLoops[4] = {0, 0, 0, 0}; // Sensors 19 to 22
static int toggleStateVocalLoops[5] = {0, 0, 0, 0, 0}; // Sensors 14 to 18

// Add a new array to track if the sensor was previously above the threshold
static int wasAboveThreshold[22] = {0}; // Initialized to 0 (not above threshold)

// Initialize previous state tracking for live drums sensors
static int wasPressedDrumSensor9 = 0;
static int wasPressedDrumSensor13 = 0;


void handleToggle(int &toggleState, CapacitiveSensorReader &sensor) {
  static unsigned long lastToggleTime[22] = {0}; // One for each sensor
  unsigned long currentTime = millis();
  int sensorIndex = (&sensor - &sensorReader1); // Calculate index based on address difference

  int isCurrentlyAbove = sensor.isAboveThreshold();

  // Check for transition from above threshold to not above (sensor release)
  if (wasAboveThreshold[sensorIndex] && !isCurrentlyAbove && currentTime - lastToggleTime[sensorIndex] > 500) {
    toggleState = !toggleState; // Toggle state
    lastToggleTime[sensorIndex] = currentTime;
  }

  wasAboveThreshold[sensorIndex] = isCurrentlyAbove; // Update previous state
}

// Ensure the rest of your setup and loop functions remain the same
// This modification only affects how the handleToggle function behaves


void loop() {
  String sensorStates = "";
  static String bpm = "20"; // Initialize BPM string representation to "20"
  static int lastBpmSensor = 0; // Remember the last sensor that changed BPM
  static int position = 10; // Initialize position to minimum value of 10

  // Include your existing sensor reading and processing code here
  // This should involve reading from the sensors and adjusting BPM/position as needed

  sensorStates += "1";

  // Toggle functionality for drum loops
  handleToggle(toggleStateDrumLoops[0], sensorReader19);
  handleToggle(toggleStateDrumLoops[1], sensorReader20);
  handleToggle(toggleStateDrumLoops[2], sensorReader21);
  handleToggle(toggleStateDrumLoops[3], sensorReader22);

  // Toggle functionality for vocal loops
  handleToggle(toggleStateVocalLoops[0], sensorReader14);
  handleToggle(toggleStateVocalLoops[1], sensorReader15);
  handleToggle(toggleStateVocalLoops[2], sensorReader16);
  handleToggle(toggleStateVocalLoops[3], sensorReader17);
  handleToggle(toggleStateVocalLoops[4], sensorReader18);

  //Synths
  sensorStates += sensorReader1.isAboveThreshold();
  sensorStates += sensorReader2.isAboveThreshold();
  sensorStates += sensorReader3.isAboveThreshold();

  // Concatenate the states of drum and vocal loop toggles to sensorStates
  for (int i = 0; i < 4; i++) {
    sensorStates += String(toggleStateDrumLoops[i]);
  }
  for (int i = 0; i < 5; i++) {
    sensorStates += String(toggleStateVocalLoops[i]);
  }

  // Handle live drums sensor logic
  int liveDrumState9 = 0; // Default state is 0
  int liveDrumState13 = 0; // Default state is 0

  if (sensorReader9.isAboveThreshold() && !wasPressedDrumSensor9) {
    liveDrumState9 = 1; // Set state to 1 on new press
    wasPressedDrumSensor9 = 1; // Mark as pressed
  } else if (!sensorReader9.isAboveThreshold() && wasPressedDrumSensor9) {
    wasPressedDrumSensor9 = 0; // Reset on release
  }

  if (sensorReader13.isAboveThreshold() && !wasPressedDrumSensor13) {
    liveDrumState13 = 1; // Set state to 1 on new press
    wasPressedDrumSensor13 = 1; // Mark as pressed
  } else if (!sensorReader13.isAboveThreshold() && wasPressedDrumSensor13) {
    wasPressedDrumSensor13 = 0; // Reset on release
  }

  // Update how you concatenate the live drums states into sensorStates string
  sensorStates += liveDrumState9;
  sensorStates += liveDrumState13;

  // BPM control with state retention
  int currentSensorTriggered = 0; // Tracks the current sensor that might trigger a BPM change
  if (sensorReader4.isAboveThreshold()) currentSensorTriggered = 4;
  else if (sensorReader5.isAboveThreshold()) currentSensorTriggered = 5;
  else if (sensorReader6.isAboveThreshold()) currentSensorTriggered = 6;
  else if (sensorReader7.isAboveThreshold()) currentSensorTriggered = 7;
  else if (sensorReader8.isAboveThreshold()) currentSensorTriggered = 8;
  else if (sensorReader10.isAboveThreshold()) currentSensorTriggered = 10;

  // Check if a different sensor is triggered to update BPM
  if (currentSensorTriggered != 0 && currentSensorTriggered != lastBpmSensor) {
    switch (currentSensorTriggered) {
      case 4: bpm = 20; break;
      case 5: bpm = 116; break;
      case 6: bpm = 212; break;
      case 7: bpm = 308; break;
      case 8: bpm = 404; break;
      case 10: bpm = 500; break;
    }
    lastBpmSensor = currentSensorTriggered; // Remember the last sensor that changed BPM
  }

// Position adjustment with minimum and maximum limits
  if (sensorReader11.isAboveThreshold()) {
    position = min(position + 5, 120); // Increase position, but not above 120
  }
  if (sensorReader12.isAboveThreshold()) {
    position = max(position - 5, 10); // Decrease position, but not below 10
  }

  // Concatenate sensor states with BPM and position
  sensorStates += ":" + bpm; // Add BPM string
  sensorStates += ":" + String(position); // Append position string

  Serial.println(sensorStates); // Print the combined sensor states, BPM, and position

  delay(100); // Adjust delay as needed.
}


