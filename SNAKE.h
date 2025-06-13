#ifndef SNAKE_H
#define SNAKE_H



// === Pins (Set according to your hardware) ===
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
unsigned int snakeSpeed = 100;  // Lower is faster

unsigned long snakeButtonPressTime = 0;
bool snakeButtonHeld = false;

// === Game State ===
bool snakeGameOver = false;
bool snakeInitDone = false;
bool snakePlay = false;
bool reset = false;

// === Function Prototypes ===
void snakeReset();
void snakePlaceFood();
void snakeDraw();
bool snakeUpdate();
void snakeHandleInput();
void snakeGame();

void snakePlaceFood() {
  foodX = random(0, GRID_WIDTH);
  foodY = random(0, GRID_HEIGHT);
}

void snakeReset() {
  snakeLength = 3;
  snakeX[0] = 5;
  snakeY[0] = 5;
  snakeX[1] = 4;
  snakeY[1] = 5;
  snakeX[2] = 3;
  snakeY[2] = 5;
  dir = DIR_RIGHT;
  snakeGameOver = false;
  snakePlaceFood();
}

void snakeDraw() {
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.drawFrame(0, 0, 127, 63);

  for (int i = 0; i < snakeLength; i++) {
    u8g2.drawBox(snakeX[i] * CELL_SIZE, snakeY[i] * CELL_SIZE, CELL_SIZE, CELL_SIZE);
  }

  // Draw food
  u8g2.drawBox(foodX * CELL_SIZE, foodY * CELL_SIZE, CELL_SIZE, CELL_SIZE);

  u8g2.sendBuffer();
}

bool snakeUpdate() {
  // Move body
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  // Move head
  switch (dir) {
    case DIR_UP: snakeY[0]--; break;
    case DIR_DOWN: snakeY[0]++; break;
    case DIR_LEFT: snakeX[0]--; break;
    case DIR_RIGHT: snakeX[0]++; break;
  }

  // Wall collision
  if (snakeX[0] < 0 || snakeX[0] >= GRID_WIDTH || snakeY[0] < 0 || snakeY[0] >= GRID_HEIGHT)
    return false;

  // Self collision
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
      return false;
  }

  // Food collision
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeLength++;
    if (snakeLength >= 128) snakeLength = 127;  // prevent overflow
    snakePlaceFood();
  }

  return true;
}

void snakeHandleInput() {
  if (digitalRead(buttonLeftPin) == LOW) {
    dir = (dir + 3) % 4;  // Turn left
    delay(150);
  }
  if (digitalRead(buttonRightPin) == LOW) {
    dir = (dir + 1) % 4;  // Turn right
    delay(150);
  }
}

void snakeGame() {
  if (!snakeInitDone) {
    pinMode(buttonLeftPin, INPUT_PULLUP);
    pinMode(buttonRightPin, INPUT_PULLUP);
    pinMode(buttonStartPin, INPUT_PULLUP);
    snakeReset();
    snakeInitDone = true;
    snakeButtonHeld = false;
  }
  // ==== Handle Quick Press for Reset ====
  if (snakeGameOver && digitalRead(buttonStartPin) == LOW && !snakeButtonHeld) {
    snakeReset();
    reset = true;
    snakeGameOver = false;
    delay(300);  // debounce
  }
  // ==== Handle Button Hold to Exit Game ====

  if (digitalRead(buttonStartPin) == LOW) {
    if (!snakeButtonHeld) {
      snakeButtonPressTime = millis();
      snakeButtonHeld = true;
    } else if (millis() - snakeButtonPressTime >= 2000) {
      // Exit condition met
      snakePlay = false;
      menu = false;
      snakeInitDone = false;
      delay(500);
      return;
    }
  } else {
    snakeButtonHeld = false;
  }

  // ==== Game Logic ====
  if (!snakeGameOver && millis() - lastMove >= snakeSpeed) {
    lastMove = millis();
    snakeHandleInput();
    if (!snakeUpdate()) {
      snakeGameOver = true;
    }
    snakeDraw();
  }

  // ==== Game Over Display ====
  if (snakeGameOver) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(25, 30, "Game Over!");
    u8g2.drawStr(10, 50, "Hold to Exit");
    u8g2.sendBuffer();
  }
}


#endif
