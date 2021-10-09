/*************************************************
 * Public Constants
 *************************************************/

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

//_______________________________REAL CODE
float speed = 0;
int state = 0;
int topSpeed = 50;
int cooldown = 0;
float xSound = 0;
float sinVal = 0;
float alarm = 0;
int toneVal = 0;
int failFac = 1;
bool locked = true;
int passState = 0;
bool calling = false;

void setup();
void setSpeed(int throttle);
void setAlarm();
void getTopSpeed(int selector);
void playAlarmTone();
void loop();
void soundAlarm();
void playCustomSound();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  pinMode(11, OUTPUT);
}

void playCustomSound(int selection) {
   /*// Play coin sound   tone(8,NOTE_B5,100);
  delay(100);
  tone(11,NOTE_E6,850);
  delay(800);
  noTone(11);
  
  delay(2000);  // pause 2 seconds */

  if(selection == 1){
  // Play 1-up sound
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
  }
  else if (selection == 2){

  // Play Fireball sound
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
  }
  else {};
  /*
  for(int y =0; y<2; y++){
      	for (int x=30; x<60; x++) {
			// convert degrees to radians then obtain sin value
			sinVal = (sin(x*(3.1412/180)));
			// generate a frequency from the sin value
			toneVal = (int(sinVal*1500))*1.5-250;
			tone(11, toneVal);
          	delay(8);
        }
        delay(50);
      }
    */
  /*
  analogWrite(11,400);
  delay(100);
  analogWrite(11,0);
  delay(200);
  analogWrite(11,400);
  delay(100);
  analogWrite(11,0);
  */
}
              
void setAlarm() {
  if (alarm == 0) {
    alarm = 100*failFac;
    failFac++;
  }
}

void setSpeed(int throttle)
{
 if (throttle==1)
  {
    if (speed+4 <topSpeed) {speed += 4;}
    else {speed = topSpeed;}
  }
  else
  {
    speed = 0;
  }
}

void getTopSpeed(int selector)
{
  if (cooldown > 0) {
    cooldown--;
  }
  else {
    cooldown = 0;
  	if (selector == 1) {
      	cooldown = 2000;
  		switch(state){
   		case(0): state = 1; topSpeed = 100; analogWrite(11,20); delay(100); analogWrite(11,0); delay(50); analogWrite(11,20); delay(100); analogWrite(11,0); break;
   		case(1): state = 2; topSpeed = 150; analogWrite(11,20); delay(100); analogWrite(11,0); delay(50); analogWrite(11,20); delay(100); analogWrite(11,0);delay(50); analogWrite(11,20); delay(100); analogWrite(11,0);break;
  	 	case(2): state = 3; topSpeed = 200; analogWrite(11,20); delay(100); analogWrite(11,0);delay(50); analogWrite(11,20); delay(100); analogWrite(11,0);delay(50); analogWrite(11,20); delay(100); analogWrite(11,0);delay(50); analogWrite(11,20); delay(100); analogWrite(11,0);break;
   		case(3): state = 0; topSpeed = 50; analogWrite(11,20); delay(100); analogWrite(11,0);break;
  		}
 	 }
  }
  analogWrite(10,speed);
}

void playAlarmTone() {
  if (alarm-0.1>0){
  	if (xSound <=180){
		sinVal = (sin(xSound*(3.1412/180)));
		// generate a frequency from the sin value
		toneVal = 2000+(int(sinVal*1000));
    	xSound+=0.5;
 	 }
  	else if (xSound <=250) {xSound+=1;}
  	else {xSound = 0;}
  	tone(11, toneVal);
    alarm -=0.1;
  }
  else {alarm = 0; noTone(11);}
}

void loop() {
  //Serial.print(passState);
  //Serial.print('\n');
  if (locked) {
    setSpeed(0);
    int da0 = digitalRead(3); //D0=B
    int da1 = digitalRead(4); //D1=D
    int da2 = digitalRead(5); //D2=A
    int da3 = digitalRead(6); //D3=C
    //3021 password CBAD
    switch(passState) {
     case(0): if((da3==1)&&(da0==da1==da2==0)){passState = 1;}
      else if((da0==1)&&(da3==da1==da2==0)) {playCustomSound(2);}
      break;
     case(1):if((da0==1)&&(da3==da1==da2==0)){passState = 2;}
      else if((da0==da1==da2==0)){}
      else {passState=0;}break;
     case(2): if((da2==1)&&(da3==da1==da0==0)){passState = 3;}
      else if ((da3==da1==da2==0)) {}
      else{passState=0;}break;
     case(3): if((da1==1)&&(da3==da0==da2==0)){
       passState = 0; locked = false; failFac=1;alarm=0;
       playCustomSound(1);
       }
      else if ((da3==da1==da0==0)) {}
      else {passState=0;}break;  
    }    
  }
  else if (!calling) //remote functions while car unlocked
  {
    if(digitalRead(3) == 1) //PAGE THE CAR (B)
    {
      /*for(int y = 0; y < 10; y++){
      	for (int x=0; x<90; x++) {
			// convert degrees to radians then obtain sin value
			sinVal = (sin(x*(3.1412/180)));
			// generate a frequency from the sin value
			toneVal = 2000+(int(sinVal*1000));
			tone(11, toneVal);
			delay(5);
		} cool alarm sound
      }*/
      calling = true;
      //custom pager sound attempt here
      playCustomSound(2);
      calling = false;
    }
    if(digitalRead(6) == 1) //LOCK THE CAR (C)
    {
      calling = true;
      locked = true;
      playCustomSound(2);
      calling = false;
    }
    
  }
  int dr7 = digitalRead(7);
  int dr2 = digitalRead(2);
  if((dr7==1 || dr2==1)&&(locked || (alarm >0))){
    setAlarm();
  }
  else if (!locked) {
    getTopSpeed(dr7);
    setSpeed(dr2);
  }
  playAlarmTone();
}


