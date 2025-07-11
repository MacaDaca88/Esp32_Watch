// GamesMenu.h
#ifndef GAMES_MENU_H
#define GAMES_MENU_H

int button1 = 1;
int button2 = 1;
int button3 = 1;

int button1State = 0;
int button2State = 0;
int button3State = 0;

bool button1Pressed = false;
unsigned long pressStartTime = 0;
const unsigned long shortHold = 1500;
const unsigned long longHold = 5000;

int menuState = 0;
const char* menuItems[] = { "Ip Address", "Scan Wifi ", "Up-Time", "Games", "Weather", "QR: Maca3D" };
const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
int currentSelection = 0;

const char* gameItems[] = { "Snake", "Pacman" };
const int gameLength = sizeof(gameItems) / sizeof(gameItems[0]);
int gameSelection = 0;

void pacmanGame() {
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(10, 30);
  u8g2.print("Pacman Coming Soon!");
  u8g2.sendBuffer();
  delay(5000);
}
void drawMenu() {
  u8g2.clearBuffer();
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 127, 63);
  menu = true;
  for (int i = 0; i < menuLength; i++) {
    if (i == currentSelection) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, i * 10, 128, 10);
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

void drawGameMenu() {
  u8g2.clearBuffer();
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, 127, 63);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_ncenB08_tr);
  for (int i = 0; i < gameLength; i++) {
    if (i == gameSelection) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, i * 10, 128, 10);
      u8g2.setDrawColor(0);
    } else {
      u8g2.setDrawColor(1);
    }
    u8g2.setCursor(2, (i + 1) * 10 - 2);
    u8g2.print(gameItems[i]);
  }
  u8g2.sendBuffer();
}

void handleGameSelection() {
  switch (gameSelection) {
    case 0: snakeGame(); break;
    case 1: pacmanGame(); break;
  }
}

void handleMenuSelection() {
  switch (currentSelection) {
    case 0: { // IP
      u8g2.clearBuffer();
      u8g2.setCursor(10, 10);
      u8g2.print("IP Address:");
      u8g2.setCursor(10, 40);
      u8g2.print(WiFi.localIP());
      u8g2.sendBuffer();
      delay(1000);
      break;
    }
    case 1: { // Scan Wi-Fi
      u8g2.clearBuffer();
      u8g2.setCursor(0, 10);
      u8g2.print("Scanning Wi-Fi...");
      u8g2.sendBuffer();
      int n = WiFi.scanNetworks();
      u8g2.clearBuffer();
      if (n == 0) {
        u8g2.setCursor(0, 10);
        u8g2.print("No networks found");
      } else {
        for (int i = 0; i < n && i < 5; ++i) {
          u8g2.setCursor(0, (i + 1) * 10);
          u8g2.print(String(i + 1) + ": " + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ")");
          yield();
        }
      }
      u8g2.sendBuffer();
      delay(3000);
      break;
    }
    case 2: { // Uptime
      u8g2.clearBuffer();
      unsigned long s = Time / 1000;
      unsigned long m = s / 60;
      unsigned long h = m / 60;
      s = s % 60;
      m = m % 60;
      u8g2.setCursor(10, 20);
      u8g2.print("Uptime = " + String(h) + ":" + String(m) + ":" + String(s));
      struct tm* tinfo;
      tinfo = localtime((time_t*)&lastNtpEpoch);
      char buf[20];
      strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tinfo);
      u8g2.setCursor(5, 40);
      u8g2.print(buf);
      u8g2.sendBuffer();
      delay(3000);
      break;
    }
    case 3: { // Games
  bool inGameMenu = true;
  drawGameMenu();

  bool button1Prev = HIGH;
  bool button2Prev = HIGH;
  bool button3Prev = HIGH;

  while (inGameMenu) {
    button1 = digitalRead(BUTTON1);
    button2 = digitalRead(BUTTON2);
    button3 = digitalRead(BUTTON3);

    if (button1 == LOW && button1Prev == HIGH) {
      gameSelection--;
      if (gameSelection < 0) gameSelection = gameLength - 1;
      drawGameMenu();
      delay(150);
    }

    if (button3 == LOW && button3Prev == HIGH) {
      gameSelection++;
      if (gameSelection >= gameLength) gameSelection = 0;
      drawGameMenu();
      delay(150);
    }

    if (button2 == LOW && button2Prev == HIGH) {
      unsigned long pressStart = millis();
      while (digitalRead(BUTTON2) == LOW) {
        if (millis() - pressStart > 3000) {
          // Long press exits
          inGameMenu = false;
          drawMenu();
          delay(200);
          break;
        }
      }
      if (inGameMenu) {
        // Short press selects game
        handleGameSelection();
        inGameMenu = false;
        drawMenu();
        delay(200);
      }
    }

    button1Prev = button1;
    button2Prev = button2;
    button3Prev = button3;
  }
  break;
}

    case 4: { // Weather
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(url);
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          DynamicJsonDocument doc(1024);
          deserializeJson(doc, payload);
          weatherDescription = doc["weather"][0]["description"].as<String>();
          temperature = doc["main"]["temp"].as<float>();
          humidity = doc["main"]["humidity"].as<float>();
          u8g2.clearBuffer();
          u8g2.setCursor(5, 12);
          u8g2.print(weatherDescription);
          u8g2.setCursor(10, 30);
          u8g2.print("Temp: " + String(temperature) + " C");
          u8g2.setCursor(10, 50);
          u8g2.print("Humidity: " + String(humidity) + " %");
        } else {
          u8g2.setCursor(10, 10);
          u8g2.print("Failed to fetch");
          u8g2.setCursor(10, 30);
          u8g2.print("weather data.");
        }
        http.end();
        u8g2.sendBuffer();
        delay(3000);
      } else {
        u8g2.clearBuffer();
        u8g2.setCursor(10, 10);
        u8g2.print("No Wi-Fi connection");
        u8g2.sendBuffer();
        delay(2000);
      }
      break;
    }
    case 5:
      qr();
      break;
  }
}


void button() {
  unsigned long currentTime = millis();
  button1 = digitalRead(BUTTON1);
  button2 = digitalRead(BUTTON2);
  button3 = digitalRead(BUTTON3);

  if (!menu) {
    if (button1 == LOW) {
      if (!button1Pressed) {
        button1Pressed = true;
        pressStartTime = currentTime;
      }
    } else {
      if (button1Pressed) {
        unsigned long pressDuration = currentTime - pressStartTime;
        if (pressDuration >= longHold) ledMode = 0;
        else if (pressDuration >= shortHold) ledMode = 4;
        button1Pressed = false;
      }
    }
    if (button1 != button1State) {
      button1State = button1;
      if (button1 == HIGH) {
        ledMode = (ledMode + 1) % 6;
        delay(10);
      }
    }
    if (button2 != button2State) {
      button2State = button2;
      if (button2 == HIGH) {
        menu = true;
        currentSelection = 0;
        drawMenu();
      }
    }
    if (button3 != button3State) {
      button3State = button3;
      if (button3 == HIGH) {
        wifiState = !wifiState;
        handleWiFiState();
        u8g2.clearBuffer();
        u8g2.setCursor(20, 40);
        u8g2.print(wifiState ? "Wi-Fi ON" : "Wi-Fi OFF");
        u8g2.sendBuffer();
        delay(2000);
      }
    }
  } else {
    if (button1 == LOW) {
      currentSelection--;
      if (currentSelection < 0) currentSelection = menuLength - 1;
      drawMenu();
      delay(200);
    }
    if (button2 == LOW) {
      handleMenuSelection();
      delay(200);
    }
    if (button3 == LOW) {
      currentSelection++;
      if (currentSelection >= menuLength) currentSelection = 0;
      drawMenu();
      delay(200);
    }
    if (button2 == LOW && Time - oldTime >= 4000) {
      menu = false;
      oldTime = Time;
    }
  }
}

#endif