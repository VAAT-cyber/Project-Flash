/* PROJECT-FLASH OS V2
 * AUTHOR: MICAH BOWONTHAMACHAKR
 * LAST EDITED: OCTOBER 12, 2021.
 */

//Remote passcode array:
/* 1000 = C
 * 0100 = A
 * 0010 = D
 * 0001 = B
 */
//No checks, but length should be at least 2 with no consecutive repetitions.
static const int PASSCODE[] = {B1000,B0001,B0100,B0010};
static const int PASSCODE_LENGTH = 4; 
//Constants
#define ALARM_SET 1
#define PING_SET 1
#define UNLOCK_SET 1
#define COOLDOWN_SET 100
#define SPEED_INCREMENT 500
#define MAX_POSSIBLE_SPEED 2000
#define MIN_POSSIBLE_SPEED 500
#define THROTTLE_INCREMENT 50

//BEGIN PITCHES DEFINITIONS
/*******NOTES LIBRARY BELOW*******/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
//END PITCHES

//State variables
static bool LOCKED = true;
static unsigned int PASSCODE_STATE = 0;
static unsigned int COOLDOWN = 0;

//Bound variables
static unsigned int MAXSPEED = MIN_POSSIBLE_SPEED;
static unsigned int THROTTLE = 0;

//Sound timers
static unsigned int ALARM = 0;
static unsigned int PING = 0;
static unsigned int UNLOCK = 0;

//D10 drives motor mosfet, D11 drives speaker transistor
void setup() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

//Lock-state routine
void LOCK_IO() {
  int da0 = digitalRead(2);
  int da1 = digitalRead(3);
  int da2 = digitalRead(4);
  int da3 = digitalRead(5);
  int da4 = digitalRead(6);
  int da5 = digitalRead(7);

  //Some bit manipulation to view all code pin inputs
  int currentCode = 0;
  currentCode = currentCode | da0<<0;
  currentCode = currentCode | da1<<1;
  currentCode = currentCode | da2<<2;
  currentCode = currentCode | da3<<3;

  //Special case for ping
  if((da4 == 1 || da5 == 1) && ALARM == 0) {
    ALARM = ALARM_SET;
    PASSCODE_STATE = 0;
  }
  //Special case for first state
  else if(PASSCODE_STATE == 0) {
    if (currentCode == B0001 && PING == 0) {PING = PING_SET;} 
    else if(currentCode == PASSCODE[0]) {PASSCODE_STATE++;}
  }
  //Special case for last state
  else if(PASSCODE_STATE == PASSCODE_LENGTH-1) {
    if (currentCode == PASSCODE[PASSCODE_LENGTH-1]) {
      PASSCODE_STATE = 0;
      LOCKED = false;
      UNLOCK = UNLOCK_SET;
      ALARM = 0;
    }
    else if (currentCode = PASSCODE[PASSCODE_LENGTH-2] || currentCode == 0) {}
    else {PASSCODE_STATE = 0;}
  }
  //General case for middle-states
  else {
    if (currentCode == PASSCODE[PASSCODE_STATE]) {
      PASSCODE_STATE++;
    }
    else if (currentCode = PASSCODE[PASSCODE_STATE-1] || currentCode == 0) {}
    else {PASSCODE_STATE = 0;}
  }
}

//Drive-state routine
void DRIVE_IO() {
  int da0 = digitalRead(2);
  int da4 = digitalRead(6);
  int da5 = digitalRead(7);

  //For pinging the car, should not be driving!
  if(da0 == 1 && PING == 0) {
    PING = PING_SET;
    LOCKED = true;
    THROTTLE = 0;
  }

  //For driving throttle/speed adjustments
  else {
    if(COOLDOWN!=0) {COOLDOWN--;}
    if(da4 == 1 && COOLDOWN == 0) {
      if(MAXSPEED+SPEED_INCREMENT <= MAX_POSSIBLE_SPEED) {MAXSPEED+=SPEED_INCREMENT;}
      else{MAXSPEED = MIN_POSSIBLE_SPEED;}
      COOLDOWN = COOLDOWN_SET;
    }
    if(da5 == 1) {
      if(THROTTLE+THROTTLE_INCREMENT<=MAXSPEED) {THROTTLE+=THROTTLE_INCREMENT;}
    }
    else {THROTTLE = 0;}
  }
  analogWrite(10,THROTTLE);
}

void PLAY_SOUNDS() {
  //Using timer value 0 or 1 since they should all play and finish immediately.
  //Alarm sound has highest priority
  if(ALARM!=0) {
    ALARM--;
    /****ALARM SOUND CODE****/
    for(int i=0; i<255; i++) {
    analogWrite(11, i);
    delay(1);
    }

    for(int i=255; i>0; i--) {
      analogWrite(11, i);
      delay(1);
    
    }
    noTone(11);
    /****END ALARM SOUND CODE****/
  }
  else if(PING!=0) {
    PING--;
    /****PING SOUND CODE****/
    tone(11,NOTE_G3+150,35);
    delay(35);
    tone(11,NOTE_G4+175,35);
    delay(35);
    tone(11,NOTE_G5+200,35);
    delay(35);
    noTone(11);
    delay(50);
    tone(11,NOTE_G3+150,35);
    delay(35);
    tone(11,NOTE_G4+175,35);
    delay(35);
    tone(11,NOTE_G5+200,35);
    delay(35);
    noTone(11);
    delay(100); 
    /****END PING SOUND CODE****/
  }
  //Unlock sound has lowest priority
  else if(UNLOCK!=0) {
    UNLOCK--;
    /****UNLOCK SOUND CODE****/
    tone(11,NOTE_E6,125);
    delay(70);
    tone(11,NOTE_G6,125);
    delay(70);
    tone(11,NOTE_E7,125);
    delay(70);
    tone(11,NOTE_G7,125);
    delay(150);
    noTone(11);
    delay(500);  
    /****END UNLOCK SOUND CODE****/
  }
  
}

//MAIN_LOOP
void loop() {
  if(!LOCKED) {DRIVE_IO();}
  else {LOCK_IO();}
  PLAY_SOUNDS();
}
