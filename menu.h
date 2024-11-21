#ifndef MENU_H
#define MENU_H

const float voltageDividerFactor = 2.0;  // Divider factor (R1/R2 ratio)
const int adcResolution = 4095;          // 12-bit ADC resolution for ESP32
const float adcReferenceVoltage = 3.3;   // ESP32 ADC reference voltage (typically 3.3V)

// Function to read and display the battery voltage
void Batt() {
  int adcValue = analogRead(BATT);  // Read ADC value
  // Convert ADC value to battery voltage
  float batteryVoltage = (adcValue / (float)adcResolution) * adcReferenceVoltage * voltageDividerFactor;

  // Display battery voltage
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.println(" V");

  u8g2.setCursor(80,55);
  u8g2.print("B+: ");
  u8g2.print(batteryVoltage);
  u8g2.print(" V");
}

// Function to display the clock
void Clock() {
  // Get hours, minutes, and seconds
  int hours = timeClient.getHours();

  if (hours >= 24) {  // Handle hour overflow
    hours -= 24;      // Wrap back to 00:00
  }

  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();

  // Convert to 12-hour format and determine AM/PM
  String period = "AM";
  if (hours >= 12) {
    period = "PM";
    if (hours > 12) hours -= 12;  // Convert to 12-hour time
  } else if (hours == 0) {
    hours = 12;  // Handle midnight as 12 AM
  }

  // Format the time with the adjusted hour (12-hour format)
  char timeBuffer[9];
  sprintf(timeBuffer, "%02d:%02d:%02d", hours, minutes, seconds);

  // Get the day of the week as a word (0 = Sunday, 1 = Monday, ...)
  String dayOfWeek;
  int day = timeClient.getDay();
  switch (day) {
    case 0: dayOfWeek = "Sunday"; break;
    case 1: dayOfWeek = "Monday"; break;
    case 2: dayOfWeek = "Tuesday"; break;
    case 3: dayOfWeek = "Wednesday"; break;
    case 4: dayOfWeek = "Thursday"; break;
    case 5: dayOfWeek = "Friday"; break;
    case 6: dayOfWeek = "Saturday"; break;
  }

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_courR10_tr);
  u8g2.drawFrame(1, 1, 127, 63);
  u8g2.setCursor(10, 25);
  u8g2.print(timeBuffer);
  u8g2.print(" ");
  u8g2.print(period);
  u8g2.setCursor(10, 50);
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.print(dayOfWeek);
}

// Menu function
void Menu() {
  if (button1 == 0) {
    Serial.println("Button 1 Pressed");
    u8g2.clearBuffer();
    u8g2.sendBuffer();

    u8g2.setDrawColor(1);
    u8g2.drawBox(1, 1, 127, 63);
    u8g2.setCursor(10, 10);
    u8g2.print("IP");

    u8g2.sendBuffer();
  } else {
    Clock();
  }
}

#endif
