/*********************************************************************************
*    <센서의 종류> 
* ++++  아날로그  ++++
* 플레임(O-EFF-값) - A0 - 글쎄 - 50이상인경우 경고
* 가스 - A1 - 글쎄 - 800이상인경우 경고 // 미리 초기값을 맞추어 놓아야 한다.(예열?)
* 사운드 - A2- 1024까지 (5V인경우) - 500이상인경우 경고
* 온습도 - A3,A4 - 경고따윈없다...
* 진동센서 - A5 - 1024까지 -600이상되면 경고
* 
* ++++  디지털  ++++
* 적외선 - 4 - 0/1 - 1이면 감지된거임
* 모터 - 5,6,7,8 - 0도부터 180도까지임
* 
**********************************************************************************/

#include <Servo.h>
#include <SHT1x.h>
#include <Thread.h> 
#define OPEN 1
#define CLOSE 2
#define UP_DOWN     100
#define DOWN_DOWN   140
#define LEFT_DOWN    120
#define RIGHT_DOWN   120
#define UP_UP    10 
#define DOWN_UP  20
#define LEFT_UP  50
#define RIGHT_UP 0 
#define MOTOR_DELAY 1000
Servo myservoU;
Servo myservoD;
Servo myservoL;
Servo myservoR;
int static iUPosition, iDPosition, iLPosition, iRPosition;
int static BunkerState =0;
int static iCounter = 0 ;

// Ports for each threat
int flame_Pin = A0;
int gas_Pin = A5;
int sound_ain = A2;
int vib_Pin = A1;
// Communication information
char static Command[2] ={'\0',}; // 0 open, 1 close bunker
int static Semaphore = 2;
Thread Jin_Thread = Thread();

int ad_value=0; // sound
int curVib_value=0; // current vibration
int preVib_value = 0;// previous vibration
int gas_value=0; // gas
int flame_value=0; // flame
int iStarter = 0;
int static Lock = 0;

void Initializing();
void GetSensorValue();
void OpenMotor();
void CloseMotor();
void RecognizeThreat();
void Receive(); // for thread...

void setup() {
  iUPosition = UP_DOWN; 
  iDPosition = DOWN_DOWN; 
  iLPosition= LEFT_DOWN; 
  iRPosition=RIGHT_DOWN;
  Initializing();
}

void loop() {
    if(Jin_Thread.shouldRun()) Jin_Thread.run();
    if(Lock == 1){
    GetSensorValue();
    RecognizeThreat();
  //  delay(1000)
  }
}

void Initializing(){
  myservoD.attach(5);
  myservoU.attach(6);
  myservoL.attach(7);
  myservoR.attach(8);
  pinMode(sound_ain,INPUT);
  pinMode(vib_Pin, INPUT);
  pinMode(gas_Pin, INPUT); 
  pinMode(flame_Pin, INPUT);
  Serial.begin(9600);
  myservoU.write(UP_DOWN);
  myservoD.write(DOWN_DOWN);
  myservoL.write(LEFT_DOWN);
  myservoR.write(RIGHT_DOWN);
  BunkerState = OPEN;
  delay(5000);
  Jin_Thread.onRun(Receive);
  Jin_Thread.setInterval(5);
}

void GetSensorValue(){
  if(BunkerState == OPEN){
    if(iStarter < 10){
      ad_value = analogRead(sound_ain)-400;
     // preVib_value = curVib_value;   curVib_value = analogRead(vib_Pin)-400;
      iStarter++;
    }else{
       ad_value = analogRead(sound_ain);
    }
    preVib_value = curVib_value;   curVib_value = analogRead(vib_Pin);
    flame_value = analogRead(flame_Pin);
    gas_value = analogRead(gas_Pin);
  }
}

void OpenMotor(){
  for( ; iRPosition < RIGHT_DOWN; iRPosition+=4){
    myservoR.write(iRPosition);      //  up0, down 100
    delay(50);
  }
  for( ; iLPosition < LEFT_DOWN ; iLPosition+=4){
    myservoL.write(iLPosition);      //  up0, down 100
    delay(50);
  }
  for( ; iDPosition < DOWN_DOWN; iDPosition+=4){        //1
    myservoD.write(iDPosition);      //  up0, down 100
    delay(50);
  }
  for( ; iUPosition < UP_DOWN; iUPosition+=4){
    myservoU.write(iUPosition);      //  up0, down 100
    delay(50);
  }
  // Code
  iStarter = 0;
}

void CloseMotor(){
  for( ; iUPosition > UP_UP; iUPosition-=5){
    myservoU.write(iUPosition);      //  up0, down 100
    delay(50);
  }
  for( ; iRPosition > RIGHT_UP; iRPosition-=5){
    myservoR.write(iRPosition);      //  up0, down 100
    delay(50);
  }
  for( ; iDPosition > DOWN_UP; iDPosition-=5){
    myservoD.write(iDPosition);      //  up0, down 100
    delay(50);
  }
  for( ; iLPosition > LEFT_UP ; iLPosition-=5){
    myservoL.write(iLPosition);      //  up0, down 100
    delay(50);
  }
}
void RecognizeThreat(){
  if(BunkerState == OPEN){
    if(ad_value > 1000){ BunkerState = CLOSE;
      Serial.print("OESS"); // 사운드센서 (O-ESS-값)
      Serial.println(ad_value);
      CloseMotor();
    }
    else if( (curVib_value - preVib_value >500)&& (curVib_value >800)){ BunkerState = CLOSE;
      Serial.print("OEVV");  //진동센서
      Serial.println(curVib_value);
      CloseMotor();
    }else if(gas_value > 800){ BunkerState = CLOSE;
      Serial.print("OEGG"); //Gas 센서
      Serial.println(gas_value,DEC);
      CloseMotor();
    }else if(flame_value > 400){ BunkerState = CLOSE;
      Serial.print("OEFF");
      Serial.println(flame_value,DEC);
      CloseMotor(); 
    }
  }
}

void Receive(){
 if(Serial.available()){
      if(Command[0] == '\0'){
        Command[0] = Serial.read();
        if(Command[0] == '1' && BunkerState == CLOSE){  // if get message, OPEN MOTOR
          Command[0] = '\0'; BunkerState = OPEN;
          OpenMotor(); 
        }else if(Command[0] == '2' && BunkerState == OPEN){
          Command[0] = '\0'; BunkerState = CLOSE;
          CloseMotor(); 
        }else if(Command[0] == '3'){
          Lock = 1;
          Command[0] = '\0';
        }
      }
    }
}
