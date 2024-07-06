#include "ArduinoLowPower.h"

#define LED_RED 9
#define LED_BLUE 8

#define BTN_TIMER 2
#define BTN_START 1
#define BTN_3 4
#define BTN_4 3

#define MODE_DEFAULT 0
#define MODE_SWITCH 1
#define MODE_TIMER 2
#define MODE_WILD 3

#define STATE_TIMER_WORK_1 0
#define STATE_TIMER_WORK_2 1
#define STATE_TIMER_PAUSE 2

int counter = 0;
int mode = 0;
int timerState = 0;
int timerCounter = 0;

void setup() {
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_TIMER, INPUT_PULLUP);
  pinMode(BTN_3, INPUT_PULLUP);
  pinMode(BTN_4, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  delay(1000);

  startup();
}

void redOn(){
  analogWrite(LED_RED, 255);
}

void redSet(uint intensity){
  if (intensity > 255){
    intensity = 255;
  }
  analogWrite(LED_RED, intensity);
}

void redOff(){
  analogWrite(LED_RED, 0);
}

void blueOn(){
  digitalWrite(LED_BLUE, HIGH);
}

void blueOff(){
  digitalWrite(LED_BLUE, LOW);
}

void startup(){
  blueOff();
  for (uint i = 0; i <= 50; i++){
    redSet(i);
    delay(250);
  }
  for (uint i = 51; i <= 150; i++){
    redSet(i);
    delay(50);
  }
  for (uint i = 151; i <= 255; i++){
    redSet(i);
    delay(10);
  }
  redOn();
  delay(1000);
  redOff();
}

void loop() {
  if (digitalRead(BTN_TIMER) == LOW){
    redOff();
    blueOff();
    mode = MODE_TIMER;
    timerState = STATE_TIMER_WORK_1;
    timerCounter = 9;
    delay(1000);
  }

  if (digitalRead(BTN_START) == LOW){
    startup();
  }

  if (digitalRead(BTN_3) == LOW){
    if (mode == MODE_DEFAULT){
      mode = MODE_SWITCH;
    } else {
      mode = MODE_DEFAULT;
    }
    redOff();
    blueOff();
    delay(1000);
  }

  if (digitalRead(BTN_4) == LOW){
    if (mode == MODE_DEFAULT){
      mode = MODE_WILD;
    } else {
      mode = MODE_DEFAULT;
    }
    redOff();
    blueOff();
    delay(1000); 
  }

  if (mode == MODE_DEFAULT){
    if (counter == 0){
      digitalWrite(LED_BLUE, HIGH);
      counter = 3;
    } else {
      digitalWrite(LED_BLUE, LOW);
      counter -= 1;
    }
  }

  if (mode == MODE_SWITCH){
    if (counter == 0){
      redOn();
      blueOff();
      counter = 1;
    } else {
      redOff();
      blueOn();
      counter = 0;
    }
  }

  if (mode == MODE_WILD){
    if (counter == 0){
      for (int i = 0; i <= 10; i++){
        redOn();
        delay(50);
        redOff();
        delay(50);
      }
      counter = 1;
    } else {
      for (int i = 0; i <= 10; i++){
        blueOn();
        delay(50);
        blueOff();
        delay(50);
      }
      counter = 0;
    }
    return;
  }

  if (mode == MODE_TIMER){
    if (timerCounter == 0){
      switch (timerState){
        case STATE_TIMER_WORK_1:
          timerState = STATE_TIMER_WORK_2;
          timerCounter = 9;
          break;
        case STATE_TIMER_WORK_2:
          timerState = STATE_TIMER_PAUSE;
          timerCounter = 6;
          break;
        default:
          mode = MODE_DEFAULT;
          redOff();
          blueOff();
          return;
      }
    }

    switch (timerState){
      case STATE_TIMER_WORK_1:
        redOff();
        blueOn();
        break;
      case STATE_TIMER_WORK_2:
        redOn();
        blueOn();
        break;
      default:
        redOn();
        blueOff();
        break;
    }

    //LowPower.deepSleep(500);
    LowPower.deepSleep(1000*60*5);
    timerCounter -= 1;
    return;
  }

  delay(500);
}


