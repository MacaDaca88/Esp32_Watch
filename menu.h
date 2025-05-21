#ifndef MENU_H
#define MENU_H



// Function to display the clock
void Clock() {
  int hours = timeClient.getHours();
  menu = false;
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
  time_t rawTime = timeClient.getEpochTime();
  struct tm* timeInfo = localtime(&rawTime);

  int date = timeInfo->tm_mday;
  int month = timeInfo->tm_mon;  // 0 = Jan, 3 = April, etc.

  String suffix = "th";
  if (date % 10 == 1 && date != 11) suffix = "st";
  else if (date % 10 == 2 && date != 12) suffix = "nd";
  else if (date % 10 == 3 && date != 13) suffix = "rd";

  const char* months[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };

  String fullDate = String(date) + suffix + " " + months[month];

  u8g2.setFont(u8g2_font_osr18_tf);

  u8g2.setCursor(5, 30);
  u8g2.print(timeBuffer);

  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.print(period);
  u8g2.setCursor(5, 59);
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.print(dayOfWeek);
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.setCursor(65, 59);
  u8g2.print(fullDate);
  u8g2.setFont(u8g2_font_timR10_tf);
  u8g2.setCursor(50, 45);
  u8g2.print("Folex ");


  if (hours == 4 && minutes == 20) {

    u8g2.setDrawColor(1);

    u8g2.drawBox(0, 0, 127, 63);
    u8g2.setCursor(30, 25);
    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_fub14_tf);
    u8g2.print("4:20");
    u8g2.setCursor(30, 40);
    u8g2.print("Bitches");
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Green
    pixels.show();
    u8g2.sendBuffer();
    return;
  }
  if (hours == 7 && minutes == 30 && period == "AM") {

    u8g2.setDrawColor(1);

    u8g2.drawBox(0, 0, 127, 63);
    u8g2.setCursor(30, 20);
    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_fub14_tf);
    u8g2.print("Wakey");
    u8g2.setCursor(30, 30);
    u8g2.print("Wakey");
    pixels.setPixelColor(0, pixels.Color(255, 255, 255));  // White
    pixels.show();
    u8g2.sendBuffer();
    return;
  }
}

// Menu function
void Menu() {

  if (menu) {
    switch (menuState) {
      case 0:
        drawMenu();
        menu = true;
        break;
      case 1:

        break;
    }
  } else {
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, 0, 128, 64);
    u8g2.setDrawColor(1);
    u8g2.drawFrame(0, 0, 127, 63);
    menu = false;

    updateLEDMode();
    drawWifiSymbol();  // Show Wi-Fi symbol if connected
    Clock();
    Batt();
    light();

    // Enter light sleep for short periods if no Wifi
    if (!wifiState) {
      setCpuFrequencyMhz(80);

      // Sleep for 1 seconds
      Serial.println("Entering light sleep...");
      esp_sleep_enable_timer_wakeup(1 * 1000000);  // Wake up after 1s
      esp_light_sleep_start();                     // Go into light sleep
      // Update the display if Asleep
      //  u8g2.setCursor(10, 10);
      //  u8g2.setFont(u8g2_font_5x7_tr);
      //  u8g2.drawStr(0, 10, " Power Save ");
    } else {
      setCpuFrequencyMhz(240);

      // Update the display if Awake
      // u8g2.setCursor(10, 10);
      // u8g2.setFont(u8g2_font_5x7_tr);
      // u8g2.drawStr(0, 10, " Awake ");
    }
  }
/*  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);  // Connect to the weather API

    int httpCode = http.GET();  // Send GET request

    if (httpCode == HTTP_CODE_OK) {       // If HTTP request is successful
      String payload = http.getString();  // Get the response as a string

      // Parse JSON data
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      temperature = doc["main"]["temp"].as<float>();  // Temperature

      u8g2.setFont(u8g2_font_helvR08_tr);
      u8g2.setCursor(10, 45);
      u8g2.print(String(temperature) + " C");
    }
  }*/
  u8g2.sendBuffer();
}

#endif
