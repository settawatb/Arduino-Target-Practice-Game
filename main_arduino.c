//  Arduino Target Practice Game
//  6351000457 Settawat Buddhakanchana
//  ARDUINO SOURCE CODE / C,C++

#include <Servo.h>     //--> Library Servo Motor
#include <MD_Parola.h> //-->) Library ของจอ MAX7219 LED MATRIX DOT
#include <MD_MAX72xx.h>//-->}                  ''
#include <SPI.h>       //-->)                  ''                
#include <secTimer.h> //--> Library จับเวลา

//=============== กำหนดค่าต่างๆ

//===== นิยามพอร์ตของ LED Matrix DOT (Constant)
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN 13
#define DATA_PIN 11
#define CS_PIN 10
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

//===== กำหนดชื่อของ Servo Motor
Servo myservo0, myservo1, myservo2, myservo3;

//===== กำหนดชื่อของตัวแปรต่างๆ

int time_sec = 60;
int count_sec = 1;
int point = 0;
int ldr0 = A0, ldr1 = A1, ldr2 = A2, ldr3 = A3;
int stat_servo0 = 1;
int stat_servo1 = 1;
int stat_servo2 = 1;
int stat_servo3 = 1;
int roundz = 1;
int randomnum = 1;
int intervals_H = 55, intervals_L = 52;
int intervalt_H = 60, intervalt_L = 56;
int star_stat = 0;

//--> timer จับเวลา
secTimer Timer;
//--> กำหนดค่าเวลา timer_val = unsigned long
unsigned long timer_val = Timer.readTimer();

void starter() //--> Function Intro ของเกม
{
  P.setTextAlignment(PA_CENTER); //--> จัดหน้า LED Matrix Dot ให้อยู่กึ่งกลาง
  P.print("welcome"); //--> แสดงผล LED Matrix Dot "welcome"
  delay(1000);
  P.print("to");
  delay(1000);
  P.print("arduino");
  delay(1000);
  P.print("target");
  delay(1000);
  P.print("practice");
  delay(1000);
  P.print("game");
  delay(2600);

  P.setTextAlignment(PA_CENTER);
  P.print("READY!");
  delay(3800);
  P.print("3");
  delay(1000);
  P.print("2");
  delay(1000);
  P.print("1");
  delay(1000);
  P.print("START");
  delay(1000);
}

void Timeoutchk()  //--> Function เช็คเวลา 60s และแสดงผล Outtro
{
  if (Timer.readTimer() == 60)
  {
    myservo0.write(0);
    delay(200);
    myservo1.write(0);
    delay(200);
    myservo2.write(0);
    delay(200);
    myservo3.write(180);
    delay(200);
    stat_servo0, stat_servo1, stat_servo2, stat_servo3 = 0;
    P.setTextAlignment(PA_CENTER);
    P.print("TIME");
    delay(1000);
    P.print("OUT!");
    delay(3000);
    P.print("YOUR");
    delay(1000);
    P.print("SCORE");
    delay(200);
    Serial.print(point); // send score to serial (talk to esp8266)
    delay(1000);
    P.print("=");
    delay(1000);
    P.print(point);
    delay(2000);
    P.print("");
    delay(750);
    P.print(point);
    delay(2000);
    P.print("");
    delay(750);
    P.print(point);
    delay(2000);
    P.print("");
    delay(750);
    for (int i = 1; i <= 40; i++) //--> ดาวกะพริบ
    {
      print_ttscore();
      delay(1000);
    }
    void (*resetFunc)(void) = 0; // declare reset fuction at address 0
    resetFunc();                 // reset
  }
}

void time_sec_count() //--> function นับวินาที
{
  if (timer_val == count_sec)
  {
    time_sec -= 1;
    count_sec += 1;
  }
}

void randomservo() //--> function ให้ ServoMotor ตั้งขึ้นแบบสุ่มหลังจากที่ยิงครบเข้าเป้าทั้งหมดแล้ว
{
  randomnum = random(1, 4);
  if (randomnum == 1)
  {
    myservo0.write(90); 
    delay(200);
    myservo1.write(90);
    delay(200);
    myservo2.write(90);
    delay(200);
    myservo3.write(90);
  }
  else if (randomnum == 2)
  {
    myservo1.write(90);
    delay(200);
    myservo2.write(90);
    delay(200);
    myservo3.write(90);
    delay(200);
    myservo0.write(90);
  }
  else if (randomnum == 3)
  {
    myservo2.write(90);
    delay(200);
    myservo3.write(90);
    delay(200);
    myservo0.write(90);
    delay(200);
    myservo1.write(90);
  }
  else if (randomnum == 4)
  {
    myservo3.write(90);
    delay(200);
    myservo0.write(90);
    delay(200);
    myservo1.write(90);
    delay(200);
    myservo2.write(90);
  }
  //======= สถานะ Servo Motor = 1
  stat_servo0 = 1;
  stat_servo1 = 1;
  stat_servo2 = 1;
  stat_servo3 = 1;
  
}

void printscore() //--> function แสดงผล SCORE ผ่าน LED Matrix
{
  char PT[12];
  snprintf(PT, 12, "# %d p.", point);
  P.print(PT);
}

void print_ttscore() //--> function แสดงผล TOTAL SCORE ผ่าน LED Matrix
{
  if (star_stat == 0)
  {
    printscore();
    star_stat = 1;
  }
  else if (star_stat == 1)
  {
    char PT2[12];
    snprintf(PT2, 12, "$ %d p.", point);
    P.setTextAlignment(PA_CENTER);
    P.print(PT2);
    star_stat = 0;
  }
}

void printtime() //--> function แสดงผล เวลา ผ่าน LED Matrix
{
  char TMM[12];
  snprintf(TMM, 12, "@ %d", time_sec);
  P.setTextAlignment(PA_CENTER);
  P.print(TMM);
}

void setup() //============================================== VOID SETUP
{ 
  //=========== กำหนด PIN และ Port การเชื่อมต่อ / Default ของอุปกรณืต่างๆในตอนเริ่ม
  Serial.begin(9600);
  pinMode(ldr0, INPUT);
  pinMode(ldr1, INPUT);
  pinMode(ldr2, INPUT);
  pinMode(ldr3, INPUT);

  myservo0.attach(2);
  myservo1.attach(3);
  myservo2.attach(4);
  myservo3.attach(5);
  delay(200);

  myservo0.write(0);
  delay(200);
  myservo0.write(90);
  delay(200);

  myservo1.write(0);
  delay(200);
  myservo1.write(90);
  delay(200);

  myservo2.write(0);
  delay(200);
  myservo2.write(90);
  delay(200);

  myservo3.write(180);
  delay(200);
  myservo3.write(90);
  delay(200);
  stat_servo0, stat_servo1, stat_servo2, stat_servo3 = 1;

  P.begin();
}

void loop() //============================================== VOID LOOP
{
  if (roundz == 1) //--> ถ้าเกิดเป็นการรันรอบแรกให้แสดงผล Intro ก่อน
  {
    starter();
    roundz = 2;
  }
  else if (roundz == 2) //--> ถ้าเกิดเป็นรอบที่ 2 ให้เริ่มทำการจับเวลา
  {
    Timer.startTimer(); //--> เริ่มจับเวลา
    roundz = 3;
  }

  timer_val = Timer.readTimer(); 

  if (time_sec <= intervals_H && time_sec >= intervals_L)
  {
    printscore();
  }
  else
  {
    printtime();
  }

  if (time_sec == intervals_L - 1)
  {
    intervals_H -= 10;
    intervals_L -= 10;
  }
  if (time_sec == intervalt_L - 1)
  {
    intervalt_H -= 10;
    intervalt_L -= 10;
  }

  Timeoutchk(); //--> เช็คเวลาหมด
  time_sec_count(); //--> นับเวลาหน่วยวินาที

//--> รับค่าแสงจาก LDR มาเก็บไว้ในตัวแปร (Analog)
  int val_ldr0 = analogRead(ldr0);
  int val_ldr1 = analogRead(ldr1);
  int val_ldr2 = analogRead(ldr2);
  int val_ldr3 = analogRead(ldr3);

  if (val_ldr0 >= 1000) //--> ค่าที่รับได้มีค่ามากกว่า 1000 (ยิงแสงเลเซอร์ไปที่เป้า)
  {
    myservo0.write(0); //--> หุบเป้าลง
    delay(100);
    stat_servo0 = 0; //--> สถานะ Servo Motor = 0
    point += 1; //--> คะแนน +1
  }
  if (val_ldr1 >= 1000)
  {
    myservo1.write(0);
    delay(100);
    stat_servo1 = 0;
    point += 1;
  }
  if (val_ldr2 >= 1000)
  {
    myservo2.write(0);
    delay(100);
    stat_servo2 = 0;
    point += 1;
  }
  if (val_ldr3 >= 1000)
  {
    myservo3.write(180);
    delay(100);
    stat_servo3 = 0;
    point += 1;
  } //========== ถ้าหากว่าสถานะทุกเป้า = 0 (เป้าหุบทั้งหมด) ให้ทำการยกเป้าขึ้นมาใหม่แบบสุ่ม
  if ((stat_servo0) == 0 && (stat_servo1) == 0 && (stat_servo2) == 0 && (stat_servo3) == 0)
  {
    randomservo();
  }
}