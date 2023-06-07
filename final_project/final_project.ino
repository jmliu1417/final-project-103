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


int sadSong[4][2] = {
  { 58, 100 },
  { 55, 100 },
  { 60, 200 },
  { 57, 200 },
};

int happySong[4][2] = {
  { 58, 100 },
  { 55, 100 },
  { 60, 200 },
  { 57, 200 },
};

int startUp[4][2] = {
  { 58, 100 },
  { 55, 100 },
  { 60, 200 },
  { 57, 200 },
};

int turnOff[4][2] = {
  { 58, 100 },
  { 55, 100 },
  { 60, 200 },
  { 57, 200 },
};

int buttonPressed;

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
    level1();
    if(level1() == true){
      Serial.println("okay");
    }else{
      Serial.println("uhhh");
    }
//this is the initial startup code, could probably fit in a function.
    // for(int i=0; i<3; i++){
    //   for(int j=0; j<10; j++){
    //     CircuitPlayground.setPixelColor(j, 0, 0, 255);
    //   }
    //   delay(300);
    //   for(int j=0; j<10; j++){
    //     CircuitPlayground.setPixelColor(j, 0, 0, 0);
    //   }
    //   delay(300);
    // }

  }else if(!switchVal){
    delay(50);
    
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
        return true;
      }else{
        return false;
        break;
      }
    }

    
  }

}
