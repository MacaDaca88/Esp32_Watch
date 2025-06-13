#ifndef SNAKE_H
#define SNAKE_H


// === Pins ===
const int buttonLeftPin = 0;
const int buttonRightPin = 6;
const int buttonStartPin = 1;

// === Grid settings ===
const int CELL_SIZE = 4;
const int GRID_WIDTH = 126 / CELL_SIZE;
const int GRID_HEIGHT = 62 / CELL_SIZE;

// === Snake settings ===
int snakeX[126];
int snakeY[62];
int snakeLength;
int dir;
#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3

// === Food ===
int foodX, foodY;

// === Timing ===
unsigned long lastMove = 0;
unsigned int snakeSpeed = 100;
unsigned long snakeButtonPressTime = 0;
bool snakeButtonHeld = false;

// === Game State ===
bool snakeGameOver = false;
bool snakeInitDone = false;
bool snakePlay = false;
bool reset = false;
int score = 0;

// === EEPROM High Score Storage ===
struct HighScore {
  char initials[3];
  uint8_t score;
};
const int MAX_SCORES = 5;
HighScore highScores[MAX_SCORES];

// === Function Prototypes ===
void snakeReset();
void snakePlaceFood();
void snakeDraw();
bool snakeUpdate();
void snakeHandleInput();
void snakeGame();
void loadScores();
void saveScores();
void insertScore(char* initials, uint8_t score);
void enterInitials(char* initials);
void displayScores();

void snakePlaceFood() {
  foodX = random(0, GRID_WIDTH);
  foodY = random(0, GRID_HEIGHT);
}

void snakeReset() {
  snakeLength = 3;
  snakeX[0] = 5; snakeY[0] = 5;
  snakeX[1] = 4; snakeY[1] = 5;
  snakeX[2] = 3; snakeY[2] = 5;
  dir = DIR_RIGHT;
  snakeGameOver = false;
  snakePlaceFood();
  score = 0;
}

void snakeDraw() {
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.drawFrame(0, 0, 127, 63);
  for (int i = 0; i < snakeLength; i++) {
    u8g2.drawBox(snakeX[i] * CELL_SIZE, snakeY[i] * CELL_SIZE, CELL_SIZE, CELL_SIZE);
  }
  u8g2.drawBox(foodX * CELL_SIZE, foodY * CELL_SIZE, CELL_SIZE, CELL_SIZE);
  u8g2.sendBuffer();
}

bool snakeUpdate() {
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }
  switch (dir) {
    case DIR_UP: snakeY[0]--; break;
    case DIR_DOWN: snakeY[0]++; break;
    case DIR_LEFT: snakeX[0]--; break;
    case DIR_RIGHT: snakeX[0]++; break;
  }
  if (snakeX[0] < 0 || snakeX[0] >= GRID_WIDTH || snakeY[0] < 0 || snakeY[0] >= GRID_HEIGHT)
    return false;
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
      return false;
  }
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    if (snakeLength < 126) snakeLength++;
    score++;
    snakePlaceFood();
  }
  return true;
}

void snakeHandleInput() {
  if (digitalRead(buttonLeftPin) == LOW) {
    dir = (dir + 3) % 4;
    delay(150);
  }
  if (digitalRead(buttonRightPin) == LOW) {
    dir = (dir + 1) % 4;
    delay(150);
  }
}

void enterInitials(char* initials) {
  int letterIndex = 0;
  for (int i = 0; i < 3; i++) initials[i] = 'A';

  while (true) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(10, 20, "Enter Initials:");

    char display[4];
    memcpy(display, initials, 3);
    display[3] = '\0';
    u8g2.drawStr(45, 40, display);
    u8g2.drawStr(0, 60, "Left/Right = A-Z, Start = Next");

    u8g2.sendBuffer();

    if (digitalRead(buttonLeftPin) == LOW) {
      initials[letterIndex] = (initials[letterIndex] - 'A' + 25) % 26 + 'A';
      delay(200);
    }
    if (digitalRead(buttonRightPin) == LOW) {
      initials[letterIndex] = (initials[letterIndex] - 'A' + 1) % 26 + 'A';
      delay(200);
    }
    if (digitalRead(buttonStartPin) == LOW) {
      delay(300);
      letterIndex++;
      if (letterIndex >= 3) break;
    }
  }
}

void loadScores() {
  EEPROM.get(0, highScores);
}

void saveScores() {
  EEPROM.put(0, highScores);
  EEPROM.commit();
}

void insertScore(char* initials, uint8_t scoreVal) {
  for (int i = 0; i < MAX_SCORES; i++) {
    if (scoreVal > highScores[i].score) {
      for (int j = MAX_SCORES - 1; j > i; j--) {
        highScores[j] = highScores[j - 1];
      }
      for (int k = 0; k < 3; k++) {
        highScores[i].initials[k] = initials[k];
      }
      highScores[i].score = scoreVal;
      break;
    }
  }
  saveScores();
}

void displayScores() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(20, 10, "High Scores");

  for (int i = 0; i < MAX_SCORES; i++) {
    char buf[20];
    snprintf(buf, sizeof(buf), "%d. %c%c%c - %d", i + 1,
             highScores[i].initials[0],
             highScores[i].initials[1],
             highScores[i].initials[2],
             highScores[i].score);
    u8g2.drawStr(5, 20 + i * 10, buf);
  }
  u8g2.sendBuffer();
  delay(3000);
}

void snakeGame() {
  if (!snakeInitDone) {
    pinMode(buttonLeftPin, INPUT_PULLUP);
    pinMode(buttonRightPin, INPUT_PULLUP);
    pinMode(buttonStartPin, INPUT_PULLUP);
    loadScores();
    snakeReset();
    snakeInitDone = true;
    snakeButtonHeld = false;
  }

  // === Reset on tap after Game Over ===
  if (snakeGameOver && digitalRead(buttonStartPin) == LOW && !snakeButtonHeld) {
    snakeReset();
    delay(300);
  }

  // === Hold to Exit ===
  if (digitalRead(buttonStartPin) == LOW) {
    if (!snakeButtonHeld) {
      snakeButtonPressTime = millis();
      snakeButtonHeld = true;
    } else if (millis() - snakeButtonPressTime >= 2000) {
      snakePlay = false;
      menu = false;
      snakeInitDone = false;
      return;
    }
  } else {
    snakeButtonHeld = false;
  }

  // === Game Loop ===
  if (!snakeGameOver && millis() - lastMove >= snakeSpeed) {
    lastMove = millis();
    snakeHandleInput();
    if (!snakeUpdate()) {
      snakeGameOver = true;

      // Save new high score
      char initials[3];
      enterInitials(initials);
      insertScore(initials, score);
      displayScores();
    }
    snakeDraw();
  }

  // === Game Over Message ===
  if (snakeGameOver) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(25, 30, "Game Over!");
    u8g2.drawStr(10, 50, "Hold to Exit");
    u8g2.sendBuffer();
  }
}

#endif
