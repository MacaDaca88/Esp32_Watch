
const float resistorRatio = 1.480;      // Voltage divider ratio (R1/R2)
const int adcResolution = 4095;         // 12-bit ADC resolution for ESP32
const float adcReferenceVoltage = 3.3;  // ESP32 ADC reference voltage (typically 3.3V)

// Function to read battery voltage
float readBatteryVoltage() {
  int analogValue = analogRead(BATT);  // Read ADC value
  // Calculate battery voltage using the voltage divider factor
  float batteryVoltage = (analogValue / (float)adcResolution) * adcReferenceVoltage * resistorRatio;
  return batteryVoltage;
}

// Function to read and display the battery voltage
void Batt() {
  float batteryVoltage = readBatteryVoltage();  // Get the battery voltage

  // Display the battery voltage on the screen
  u8g2.setFont(u8g2_font_5x7_tr);
  u8g2.setCursor(68, 59);
  u8g2.print("B+ ");
  u8g2.print(batteryVoltage, 2);  // Display voltage with two decimal points
  u8g2.print("V");
  if (batteryVoltage >= 4.20) {
    battCharged = true;
  } else {
    battCharged = false;
  }

  if (batteryVoltage <= 3.60) {
    battFlat = true;
  } else {
    battFlat = false;
  }
}