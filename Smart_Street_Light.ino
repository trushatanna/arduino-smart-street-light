/*
* Smart Street Light System
* Hardware: Arduino Nano, 2x PIR Sensors, 2x MOSFET Modules, 1x LDR, 6x LEDs
* Logic: 100% brightness on motion, 25% idle, 5s hold time.
*/

// --- PIN DEFINITIONS ---
const int PIR1_PIN = 2;	// Sensor 1 const int PIR2_PIN = 4;	// Sensor 2 const int LDR_PIN = A0;		// Light Sensor
const int MOS_GRP1 = 3; // MOSFET for LEDs 1-3 (PWM Pin) const int MOS_GRP2 = 5;  // MOSFET for LEDs 4-6 (PWM Pin - MUST move from pin 4 to 5)

// --- BRIGHTNESS SETTINGS (0-255) ---
const int BRIGHT_FULL = 255; // 100% const int BRIGHT_DIM = 64; // 25% const int BRIGHT_OFF = 0;	// 0%

// --- TIMING & THRESHOLDS ---
const int LDR_THRESHOLD = 500;
const unsigned long HOLD_TIME = 5000; // 5 Seconds
unsigned long lastMotion1 = 0; unsigned long lastMotion2 = 0;

void setup() { Serial.begin(9600);
 
pinMode(PIR1_PIN, INPUT); pinMode(PIR2_PIN, INPUT); pinMode(MOS_GRP1, OUTPUT); pinMode(MOS_GRP2, OUTPUT);
// Safety: Start with lights OFF analogWrite(MOS_GRP1, BRIGHT_OFF); analogWrite(MOS_GRP2, BRIGHT_OFF);

Serial.println("SYSTEM STARTING...");
Serial.println("Waiting 30s for PIR sensors to stabilize heat levels...");

// This delay prevents the "random" triggers at power-on delay(30000);
Serial.println("SYSTEM READY");
}
void loop() {
int ldrValue = analogRead(LDR_PIN);

// NIGHT TIME LOGIC
if (ldrValue >= LDR_THRESHOLD) {

// --- LED Group 1 Control ---
if (digitalRead(PIR1_PIN) == HIGH) { lastMotion1 = millis(); analogWrite(MOS_GRP1, BRIGHT_FULL);
}
else if (millis() - lastMotion1 > HOLD_TIME) { analogWrite(MOS_GRP1, BRIGHT_DIM);
 
}
// --- LED Group 2 Control (With Noise Filter) ---if (digitalRead(PIR2_PIN) == HIGH) {
// Noise filter: Check if signal stays HIGH for 50ms
// to avoid random electrical interference delay(50);
if (digitalRead(PIR2_PIN) == HIGH) { lastMotion2 = millis(); analogWrite(MOS_GRP2, BRIGHT_FULL);
}
}
else if (millis() - lastMotion2 > HOLD_TIME) { analogWrite(MOS_GRP2, BRIGHT_DIM);
}
}
// DAY TIME LOGIC
else {
analogWrite(MOS_GRP1, BRIGHT_OFF); analogWrite(MOS_GRP2, BRIGHT_OFF);
}
}
