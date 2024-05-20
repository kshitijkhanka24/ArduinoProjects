#include <LiquidCrystal_I2C.h>

#define REST      0
#define NOTE_AS4 466
#define NOTE_F5  698
#define NOTE_C6  1047
#define NOTE_AS5 932
#define NOTE_A5  880
#define NOTE_G5  784
#define NOTE_F6  1397

int buzzer = 11;
const int buttonPin = 2;
const int ledPin = 13;

const int tempo = 108;
const int totalSeconds = 60;
const int shortPresentationMinutes = 1;
const int longPresentationMinutes = 15;

int melody[] = {
  NOTE_AS4,8, NOTE_AS4,8, NOTE_AS4,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2 
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;

void playTune() {
  int ledState = 0;
  unsigned long prevMillis = millis();
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    unsigned long currMillis = millis();
    if(ledState == 0 && (currMillis - prevMillis) >= 250){
      digitalWrite(ledPin, HIGH);
      prevMillis = currMillis;
      ledState = 1;
    }else if(ledState == 1 && (currMillis - prevMillis) >= 250){
      digitalWrite(ledPin, LOW);
      prevMillis = currMillis;
      ledState = 0;
    }
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }
    tone(buzzer, melody[thisNote], noteDuration*0.9);

    delay(noteDuration);

    noTone(buzzer);
  }
}

LiquidCrystal_I2C mainLCD(0x27,20,4);

void PresentationCounter(int duration){
  for(int minutes = duration - 1; minutes >= 0; minutes--){
    if(minutes < 10){
      mainLCD.setCursor(5, 1);
      mainLCD.print("0");
      mainLCD.setCursor(6, 1);
      mainLCD.print(minutes);
    }else{
      mainLCD.setCursor(5, 1);
      mainLCD.print(minutes);
    }
    mainLCD.setCursor(7, 1);
    mainLCD.print(":");
    bool toggle = false;
    for(int seconds = totalSeconds - 1; seconds >= 0; seconds--){
      if(seconds < 10){
        mainLCD.setCursor(8, 1);
        mainLCD.print("0");
        mainLCD.setCursor(9, 1);
        mainLCD.print(seconds);
      }else{
        mainLCD.setCursor(8, 1);
        mainLCD.print(seconds);
      }
      if(minutes < 1){
        if(toggle){
          mainLCD.setBacklight(1);
          delay(1000);
          toggle = false;
        }else{
          mainLCD.setBacklight(0);
          delay(1000);
          toggle = true;
       } 
      }else{
        delay(1000);
      }
    }
  }
  mainLCD.clear();
  mainLCD.setCursor(5,0);
  mainLCD.print("Times");
  mainLCD.setCursor(6,1);
  mainLCD.print("Up!");
  playTune();
}

int durationOfTimer(){
  mainLCD.setCursor(0, 0);
  mainLCD.print("Select");
  mainLCD.setCursor(0, 1);
  mainLCD.print("Duration...");
  delay(2000);
  mainLCD.clear();
  mainLCD.setCursor(0, 0);
  mainLCD.print("Short / Long");
  mainLCD.setCursor(0, 1);
  mainLCD.print("(Hold for Long)");
  delay(1500);
  int buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH){
    return 1;
  }else{
    return 0;
  }
  delay(2000);
}

void execute()
{
  mainLCD.clear();
  delay(1000);
  int type = durationOfTimer();
  delay(1000);
  mainLCD.clear();
  mainLCD.setCursor(0, 0);
  mainLCD.print("Getting Ready...");
  delay(1000);
  for(int countdown = 3; countdown > 0; countdown--){
    mainLCD.setCursor(0, 1);
    mainLCD.print("Starts in : ");
    mainLCD.setBacklight(0);
    delay(750);
    mainLCD.setCursor(12,1);
    mainLCD.print(countdown);
    mainLCD.setBacklight(1);
    delay(750);
  }
  delay(1000);
  mainLCD.clear();
  mainLCD.setCursor(0, 0);
  mainLCD.print("Time Left");
  switch(type){
    case 0:
      PresentationCounter(shortPresentationMinutes);
      break;
    case 1:
      PresentationCounter(longPresentationMinutes);
      break;
  }
  mainLCD.setCursor(0, 0);
  mainLCD.print("Hold button");
  mainLCD.setCursor(0, 1);
  mainLCD.print("to continue..");
}

void setup()
{
  pinMode(buzzer, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  mainLCD.init();
  mainLCD.clear();
  mainLCD.setBacklight(1);
  mainLCD.setCursor(0, 0);
  mainLCD.print("Presentation");
  mainLCD.setCursor(0, 1);
  mainLCD.print("Counter!");
  delay(3000);
  mainLCD.clear();
  execute();
}

void loop(){
  delay(3000);
  int buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH){
    execute();
  }
}
