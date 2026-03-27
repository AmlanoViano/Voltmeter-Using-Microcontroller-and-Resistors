/*
  Universal DC Voltmeter
  External divider:
    R_TOP = your top resistor in ohms
    R_BOTTOM = your bottom resistor in ohms
  Divider ratio:
    V_ADC = V_in * (R_BOTTOM / (R_TOP + R_BOTTOM))
    V_in  = V_ADC * ((R_TOP + R_BOTTOM) / R_BOTTOM)
  
  Change analogPin to match your board's ADC pin
  Change A0_MAX_VOLTAGE to your board's ADC reference voltage
  Change ADC_MAX to match your board's ADC bit depth (255 / 1023 / 4095)
  Measure DC voltages only
  Connect source GND to board GND
  Board reference:
    Arduino Uno/Nano : analogPin=A0, A0_MAX_VOLTAGE=5.0, ADC_MAX=1023
    NodeMCU ESP8266  : analogPin=A0, A0_MAX_VOLTAGE=3.3, ADC_MAX=1023
    ESP32            : analogPin=34, A0_MAX_VOLTAGE=3.3, ADC_MAX=4095
    Arduino Due      : analogPin=A0, A0_MAX_VOLTAGE=3.3, ADC_MAX=4095
*/

// ---- CONFIGURE THESE FOR YOUR BOARD AND CIRCUIT ----
const int analogPin = A0;
// Divider values in ohms
const float R_TOP = 45000.0;
const float R_BOTTOM = 15000.0;
// ADC reference voltage for your board in volts
const float A0_MAX_VOLTAGE = 3.3;
// ADC raw range: 10-bit = 1023, 12-bit = 4095, 8-bit = 255
const int ADC_MAX = 1023;
// Calibration factor
const float CALIBRATION_FACTOR = 1.000;
// Averaging
const int NUM_SAMPLES = 100;
// -----------------------------------------------------

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("NodeMCU ESP8266 DC Voltmeter Starting...");
  Serial.println("Designed for approximately 0V to 12V input");
}
void loop() {
  unsigned long total = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    total += analogRead(analogPin);
    delay(2);
  }
  float adcRaw = total / (float)NUM_SAMPLES;
  float vA0 = (adcRaw / ADC_MAX) * A0_MAX_VOLTAGE;
  float vIn = vA0 * ((R_TOP + R_BOTTOM) / R_BOTTOM);
  vIn *= CALIBRATION_FACTOR;
  Serial.print("ADC Raw: ");
  Serial.print(adcRaw, 1);
  Serial.print(" | A0 Voltage: ");
  Serial.print(vA0, 3);
  Serial.print(" V | Input Voltage: ");
  Serial.print(vIn, 3);
  Serial.println(" V");
  delay(500);
}
