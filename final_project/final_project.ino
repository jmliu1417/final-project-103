#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay delay_1s;

int switchPin = 7;
int rightPin = 5;
int leftPin = 4;

volatile bool switchFlag = 0;
volatile bool rightFlag = 0;
volatile bool leftFlag = 0;

bool switchVal = 0;
bool rightVal = 0;
bool leftVal = 0;

float midi[127];
int A_four = 440;  // a is 440 hz...


int sadSong[2][2] = {
  { 58, 100 },
  { 55, 100 },
};

int happySong[2][2] = {
  { 61, 200 },
  { 57, 200 },
};

int startSong[4][2] = {
  { 58, 100 },
  { 59, 100 },
  { 60, 100 },
  { 61, 100 },
};

int offSong[4][2] = {
  { 61, 100 },
  { 60, 100 },
  { 59, 100 },
  { 58, 100 },
};

int buttonPressed = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  CircuitPlayground.begin();
  generateMIDI();
  randomSeed(analogRead(0));

  pinMode(switchPin, INPUT_PULLUP);
  pinMode(leftPin, INPUT_PULLDOWN);
  pinMode(rightPin, INPUT_PULLDOWN);

  attachInterrupt(digitalPinToInterrupt(switchPin), onOff7, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightPin), onOff5, FALLING);
  attachInterrupt(digitalPinToInterrupt(leftPin), onOff4, FALLING);

  switchVal = digitalRead(switchPin);

  while (!Serial) {
    Serial.println("Starting timer");
  }

  delay_1s.start(2000, AsyncDelay::MILLIS);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(switchFlag){
    delay(5);
    switchVal = digitalRead(switchPin);
    switchFlag = 0;
  }
  // startup code
  if(switchVal){

    bool level1Return = false;

    for (int i = 0; i < sizeof(startSong) / sizeof(startSong[0]); i++) {
      CircuitPlayground.playTone(midi[startSong[i][0]], startSong[i][1]);
      }

      for(int i=0; i<3; i++){
      for(int j=0; j<10; j++){
        CircuitPlayground.setPixelColor(j, 0, 255, 0);
      }
      delay(300);
      for(int j=0; j<10; j++){
        CircuitPlayground.setPixelColor(j, 0, 0, 0);
      }
      delay(300);
    }


    level1();
    if(level1() == true){
      Serial.println("okay");
    }else{
      Serial.println("uhhh");
    }
//this is the initial startup code, could probably fit in a function.
    
  }else if(!switchVal){
    delay(5);

    for (int i = 0; i < sizeof(offSong) / sizeof(offSong[0]); i++) {
      CircuitPlayground.playTone(midi[offSong[i][0]], offSong[i][1]);
      }
    
    //turning off sequence (lights)
    for(int i=0; i<3; i++){
      for(int i=0; i<10; i++){
        CircuitPlayground.setPixelColor(i, 255, 0, 0);
      }
      delay(300);
      for(int i=0; i<10; i++){
        CircuitPlayground.setPixelColor(i, 0, 0, 0);
      }
      delay(300);
    }

    
  }

}

void onOff7() {
  switchFlag = 1;
}

void onOff5() {
  rightFlag = 1;
}

void onOff4() {
  leftFlag = 1;
}

void generateMIDI() {
  for (int x = 0; x < 127; ++x) {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }
}

bool level1(){
  int randomLED = random(10);

  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, 254, 150, 250);
  }

  CircuitPlayground.setPixelColor(randomLED, 255, 230, 0);
  delay(1000);

  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, 255, 0, 0);
    delay(1000);

    Serial.println(i);

    if(rightFlag || leftFlag){
      buttonPressed = i;
      if(buttonPressed == randomLED){

        for (int i = 0; i < sizeof(happySong) / sizeof(happySong[0]); i++) {
      CircuitPlayground.playTone(midi[happySong[i][0]], happySong[i][1]);
      }

        return true;
      }else{

        for (int i = 0; i < sizeof(sadSong) / sizeof(sadSong[0]); i++) {
      CircuitPlayground.playTone(midi[sadSong[i][0]], sadSong[i][1]);
      }
        return false;
        break;
      }
    }

    
  }

}
