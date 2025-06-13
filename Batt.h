
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

  // Calculate percentage (assuming 3.3V = 0%, 4.2V = 100%)
  int percentage = constrain(map(batteryVoltage * 100, 330, 420, 0, 100), 0, 100);

  // Update status flags
  battCharged = batteryVoltage >= 4.20;
  battFlat = batteryVoltage <= 3.50;

  // Draw battery outline
  int x = 5, y = 50, w = 20, h = 10;
  u8g2.drawFrame(x, y, w, h);          // Outer battery body
  u8g2.drawBox(x + w, y + 3, 2, 4);    // Battery tip

  // Fill battery based on percentage
  int fillWidth = map(percentage, 0, 100, 0, w - 2);
  u8g2.drawBox(x + 1, y + 1, fillWidth, h - 2);

  // Display voltage and percentage
  u8g2.setFont(u8g2_font_6x10_tf);  // Clean and readable font
  u8g2.setCursor(28, 59);
  u8g2.printf(" %d%%", percentage);
}
