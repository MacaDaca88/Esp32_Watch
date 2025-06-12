

int button1 = 1;
int button2 = 1;
int button3 = 1;

int button1State = 0;
int button2State = 0;
int button3State = 0;

bool button1Pressed = false;
unsigned long pressStartTime = 0;
const unsigned long shortHold = 1500;  // 1.5 seconds
const unsigned long longHold = 5000;   // 5 seconds

// Variables for menu navigation
int menuState = 0;
const char* menuItems[] = { "Ip Address", "Scan Wifi ", "Up-Time", "Clock Sync", "Weather" };
const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
int currentSelection = 0;
int menu = 0;

void drawMenu() {
  u8g2.clearBuffer();
  menu = true;
  // Draw each menu item
  for (int i = 0; i < menuLength; i++) {
    if (i == currentSelection) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, i * 10, 128, 10);  // Highlight selected item
      u8g2.setDrawColor(0);
    } else {
      u8g2.setDrawColor(1);
    }
    u8g2.setCursor(2, (i + 1) * 10 - 2);
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.print(menuItems[i]);
  }
  u8g2.sendBuffer();
}
void handleMenuSelection() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 30);
  u8g2.print("Selected:");
  u8g2.setCursor(0, 50);
  u8g2.print(menuItems[currentSelection]);
  u8g2.sendBuffer();

  delay(500);  // Show selected item for 1 second
  switch (currentSelection) {
    case 0:  // Show IP Address
      {
        u8g2.clearBuffer();
        u8g2.setDrawColor(0);
        u8g2.drawBox(0, 0, 128, 64);
        u8g2.setDrawColor(1);
        u8g2.setCursor(10, 10);
        u8g2.print("IP Address:");
        u8g2.setCursor(10, 40);
        u8g2.print(WiFi.localIP());
        u8g2.sendBuffer();

        delay(1000);
        break;
      }

    case 1:  // Scan Wi-Fi networks
      {
        u8g2.clearBuffer();
        u8g2.setCursor(0, 10);
        u8g2.print("Scanning Wi-Fi...");
        u8g2.sendBuffer();

        int n = WiFi.scanNetworks();  // Declare inside a block
        u8g2.clearBuffer();

        if (n == 0) {
          u8g2.setCursor(0, 10);
          u8g2.print("No networks found");
        } else {
          for (int i = 0; i < n && i < 5; ++i) {  // Limit to first 5 networks
            u8g2.setCursor(0, (i + 1) * 10);
            String networkInfo = String(i + 1) + ": " + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ")";
            u8g2.print(networkInfo);
            yield();  // Prevent watchdog reset
          }
        }
        u8g2.sendBuffer();

        delay(3000);  // Allow the user time to see the output
        break;
      }
    case 2:  // uptime display
      {
        u8g2.clearBuffer();

        unsigned long seconds = Time / 1000;   // Convert milliseconds to seconds
        unsigned long minutes = seconds / 60;  // Convert seconds to minutes
        unsigned long hours = minutes / 60;    // Convert minutes to hours

        seconds = seconds % 60;  // Remaining seconds after minutes
        minutes = minutes % 60;  // Remaining minutes after hours
        u8g2.setDrawColor(1);
        u8g2.setFont(u8g2_font_timR10_tf);
        u8g2.setCursor(10, 20);

        u8g2.print("Uptime = ");
        u8g2.print(hours);
        u8g2.print(":");
        u8g2.print(minutes);
        u8g2.print(":");
        u8g2.print(seconds);
        u8g2.sendBuffer();

        delay(3000);  // Allow the user time to see the output
      }

    case 3:  // Display Last Time updated
      {
        u8g2.clearBuffer();
        u8g2.setDrawColor(1);
        u8g2.setFont(u8g2_font_timR10_tf);
        u8g2.setCursor(5, 10);

        // Convert lastNtpEpoch to time struct
        struct tm* timeinfo;
        time_t rawtime = lastNtpEpoch;   // Your last sync time
        timeinfo = localtime(&rawtime);  // Convert to time structure

        // Create buffer for formatted date and time
        char buffer[20];                                                  // Buffer to hold the formatted date and time string
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);  // Format as YYYY-MM-DD HH:MM:SS

        // Display the formatted date and time
        u8g2.print(buffer);

        u8g2.sendBuffer();
        delay(3000);  // Allow the user time to see the output

        break;
      }
    case 4:  // Display Weather
      {
        if (WiFi.status() == WL_CONNECTED) {
          HTTPClient http;
          http.begin(url);  // Connect to the weather API

          int httpCode = http.GET();  // Send GET request

          if (httpCode == HTTP_CODE_OK) {       // If HTTP request is successful
            String payload = http.getString();  // Get the response as a string

            // Parse JSON data
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);

            weatherDescription = doc["weather"][0]["description"].as<String>();  // Weather description
            temperature = doc["main"]["temp"].as<float>();                       // Temperature
            humidity = doc["main"]["humidity"].as<float>();                      // Humidity

            // Display the weather data
            u8g2.clearBuffer();
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 127, 63);
            u8g2.setDrawColor(1);
            u8g2.setFont(u8g2_font_6x10_mf);
            u8g2.setCursor(5, 5);
            u8g2.print("Weather: ");
            u8g2.setCursor(5, 15);
            u8g2.print(weatherDescription);
            
            u8g2.setFont(u8g2_font_timR10_tf);

            u8g2.setCursor(10, 30);
            u8g2.print("Temp: " + String(temperature) + " C");

            u8g2.setCursor(10, 50);
            u8g2.print("Humidity: " + String(humidity) + " %");

            u8g2.sendBuffer();
          } else {
            u8g2.setCursor(10, 10);
            u8g2.print("Failed to fetch");
            u8g2.setCursor(10, 30);
            u8g2.print("weather data.");
            u8g2.sendBuffer();
          }
          http.end();   // Close the connection
          delay(5000);  // Allow the user time to see the output

        } else {
          u8g2.clearBuffer();
          u8g2.setDrawColor(1);
          u8g2.setFont(u8g2_font_timR10_tf);
          u8g2.setCursor(10, 10);
          u8g2.print("No Wi-Fi connection");
          u8g2.sendBuffer();
          delay(2000);  // Allow the user time to see the output
        }
        break;
      }

    default:
      break;
  }
}

void button() {
  unsigned long currentTime = millis();  // Get the current time

  // Read current button states
  button1 = digitalRead(BUTTON1);
  button2 = digitalRead(BUTTON2);
  button3 = digitalRead(BUTTON3);

  if (!menu) {  // Normal screen logic
    if (button1 == LOW) {
      if (!button1Pressed) {
        button1Pressed = true;
        pressStartTime = currentTime;
      }
    } else {
      if (button1Pressed) {
        unsigned long pressDuration = currentTime - pressStartTime;

        if (pressDuration >= longHold) {
          ledMode = 0;
        } else if (pressDuration >= shortHold) {
          ledMode = 4;
        }
        button1Pressed = false;
      }
    }


    if (button1 != button1State) {  // Toggle LED mode
      button1State = button1;
      if (button1 == HIGH) {          // Detect rising edge
        ledMode = (ledMode + 1) % 6;  // Cycle LED modes (0 to 4)
        delay(10);                   // Debounce delay
      }
    }
    if (button2 != button2State) {  // Toggle menu
      button2State = button2;
      if (button2 == HIGH) {   // Detect rising edge
        menu = true;           // Activate menu
        currentSelection = 0;  // Reset menu selection
        drawMenu();            // Draw the menu
      }
    }
    if (button3 != button3State) {  // Toggle Wi-Fi state
      button3State = button3;
      if (button3 == HIGH) {     // Detect rising edge
        wifiState = !wifiState;  // Toggle Wi-Fi state
        handleWiFiState();

        // Update Wi-Fi display
        u8g2.clearBuffer();
        u8g2.setDrawColor(0);
        u8g2.drawBox(0, 0, 127, 63);
        u8g2.setDrawColor(1);
        u8g2.setFont(u8g2_font_ncenB14_tf);
        u8g2.setCursor(20, 40);
        u8g2.print(wifiState ? "Wi-Fi ON" : "Wi-Fi OFF");
        u8g2.sendBuffer();

        delay(2000);  // Optional delay for button press effect
      }
    }
    // updateLEDMode();

  } else {                 // Menu active
    if (button1 == LOW) {  // Navigate up in the menu
      currentSelection--;
      if (currentSelection < 0) currentSelection = menuLength - 1;
      drawMenu();
      delay(200);  // Debounce delay
    }
    if (button2 == LOW) {  // Select menu item
      handleMenuSelection();
      delay(200);  // Debounce delay
    }
    if (button3 == LOW) {  // Navigate down in the menu
      currentSelection++;
      if (currentSelection >= menuLength) currentSelection = 0;
      drawMenu();
      delay(200);  // Debounce delay
    }
    // Exit menu if a long press of button 2 is detected
    if (button2 == LOW && Time - oldTime >= 4000) {
      menu = false;  // Exit menu
      oldTime = Time;
    }
  }
}