

int button1 = 0;
int button2 = 0;
int button3 = 0;

int button1State = 0;
int button2State = 0;
int button3State = 0;

unsigned long buttonTime = 0;
unsigned long oldButtonTime = 0;

void button() {
  buttonTime = millis();

  button1 = digitalRead(BUTTON1);
  button2 = digitalRead(BUTTON2);
  button3 = digitalRead(BUTTON3);

  if (button2 != button2State && button2 == LOW) {
    led = !led;
  }
  button2State = button2;

  if (button2 == LOW && (buttonTime - oldButtonTime >= 2000)) {
    ledFlash = !ledFlash; // Toggle flashing mode
    oldButtonTime = buttonTime; // Reset to avoid multiple toggles
  }
  oldButtonTime = buttonTime;
}