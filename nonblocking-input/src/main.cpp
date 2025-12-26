#include <Arduino.h>

const int LED = 12;
const int BUTTON = 2;

bool state = false;
bool state_prev = false;
bool LED_state = false;
bool button_prev = HIGH;
bool press_expired = false;
static unsigned long start = 0;
const unsigned long debounce_t = 100;

void led_ctrl();
bool detect(int PIN);

void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  digitalWrite(LED, LOW);
}

void loop()
{
  led_ctrl();
}

void led_ctrl()
{
  if (detect(BUTTON))
    state = true;
  else
    state = false;
  if (state_prev != state)
  {
    if (state)
    {
      Serial.println("ON");
      state_prev = state;
      if (!LED_state)
      {
        digitalWrite(LED, HIGH);
        LED_state = true;
      }
      else
      {
        digitalWrite(LED, LOW);
        LED_state = false;
      }
    }
    else
    {
      Serial.println("OFF");
      state_prev = state;
    }
  }
}

bool detect(int PIN)
{
  bool button_state = digitalRead(PIN);
  if (button_prev != button_state)
  {
    start = millis();
    press_expired = false;
    button_prev = button_state;
  }

  if (button_state == LOW && millis() - start > debounce_t && !press_expired)
  {
    start = 0;
    press_expired = true;
    if (button_state == LOW)
      return true;
    else
      return false;
  }
  else
  {
    return false;
  }
}