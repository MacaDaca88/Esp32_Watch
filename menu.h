#ifndef MENU_H
#define MENU_H

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

  // Display the battery voltage in the serial monitor
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.println(" V");

  // Display the battery voltage on the screen
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.setCursor(60, 55);
  u8g2.print("B+ ");
  u8g2.print(batteryVoltage, 2);  // Display voltage with two decimal points
  u8g2.print(" V");
}

// Function to display the clock
void Clock() {
  int hours = timeClient.getHours();

  if (hours >= 24) {
    hours -= 24;
  }

  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();

  String period = "AM";
  if (hours >= 12) {
    period = "PM";
    if (hours > 12) hours -= 12;
  } else if (hours == 0) {
    hours = 12;
  }

  char timeBuffer[9];
  sprintf(timeBuffer, "%02d:%02d:%02d", hours, minutes, seconds);

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

  u8g2.setFont(u8g2_font_fub14_tf);
  u8g2.drawFrame(1, 1, 127, 63);
  u8g2.setCursor(10, 25);
  u8g2.print(timeBuffer);
  u8g2.print(" ");
  u8g2.print(period);
  u8g2.setCursor(10, 50);
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.print(dayOfWeek);
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.setCursor(30, 40);
  u8g2.print("Fake Rolex ");


  if (hours == 4 && minutes == 20) {
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);

    u8g2.drawBox(0, 0, 127, 63);
    u8g2.setCursor(30, 20);
    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_fub11_tf);
    u8g2.print("4:20");
    u8g2.setCursor(30, 30);
    u8g2.print("Bitches");
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Green
    pixels.show();
    u8g2.sendBuffer();
  }
    if (hours == 7 && minutes == 30 && period == "AM") {
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);

    u8g2.drawBox(0, 0, 127, 63);
    u8g2.setCursor(30, 20);
    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_fub11_tf);
    u8g2.print("Wakey");
    u8g2.setCursor(30, 30);
    u8g2.print("Wakey");
    pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // Green
    pixels.show();
    u8g2.sendBuffer();
  }
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
