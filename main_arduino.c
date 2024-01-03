//  Arduino Target Practice Game
//  Settawat Buddhakanchana
//  ARDUINO SOURCE CODE / C,C++

#include <Servo.h>     //--> Library Servo Motor
#include <MD_Parola.h> //-->) Library ของจอ MAX7219 LED MATRIX DOT
#include <MD_MAX72xx.h>//-->}                  ''
#include <SPI.h>       //-->)                  ''                
#include <secTimer.h> //--> Library จับเวลา

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN 13
#define DATA_PIN 11
#define CS_PIN 10
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

Servo servos[4];
int ldrs[4] = {A0, A1, A2, A3};
int stat_servos[4] = {1, 1, 1, 1};
int time_sec = 60, count_sec = 1, point = 0;
int intervals_H = 55, intervals_L = 52, intervalt_H = 60, intervalt_L = 56;
int star_stat = 0;

secTimer Timer;
unsigned long timer_val = Timer.readTimer();

void starter() {
  P.setTextAlignment(PA_CENTER);
  const char *messages[] = {"welcome", "to", "arduino", "target", "practice", "game"};
  for (const char *msg : messages) {
    P.print(msg);
    delay(1000);
  }
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

void Timeoutchk() {
  if (Timer.readTimer() == 60) {
    for (Servo &servo : servos) {
      servo.write(0);
      delay(200);
    }
    for (int i = 0; i < 4; i++) {
      stat_servos[i] = 0;
    }
    P.setTextAlignment(PA_CENTER);
    const char *outroMessages[] = {"TIME", "OUT!", "YOUR", "SCORE", ""};
    for (const char *msg : outroMessages) {
      P.print(msg);
      delay(1000);
    }
    Serial.print(point);
    delay(1000);
    P.print("=");
    delay(1000);
    P.print(point);
    delay(2000);
    for (int i = 0; i < 3; i++) {
      P.print("");
      delay(750);
      P.print(point);
      delay(2000);
    }
    for (int i = 1; i <= 40; i++) {
      print_ttscore();
      delay(1000);
    }
    void (*resetFunc)(void) = 0;
    resetFunc();
  }
}

void time_sec_count() {
  if (timer_val == count_sec) {
    time_sec -= 1;
    count_sec += 1;
  }
}

void randomservo() {
  int randomnum = random(1, 4);
  for (int i = 0; i < 4; i++) {
    int index = (randomnum + i) % 4;
    servos[index].write(90);
    delay(200);
  }
  for (int i = 0; i < 4; i++) {
    stat_servos[i] = 1;
  }
}

void printscore() {
  char PT[12];
  snprintf(PT, 12, "# %d p.", point);
  P.print(PT);
}

void print_ttscore() {
  if (star_stat == 0) {
    printscore();
    star_stat = 1;
  } else if (star_stat == 1) {
    char PT2[12];
    snprintf(PT2, 12, "$ %d p.", point);
    P.setTextAlignment(PA_CENTER);
    P.print(PT2);
    star_stat = 0;
  }
}

void printtime() {
  char TMM[12];
  snprintf(TMM, 12, "@ %d", time_sec);
  P.setTextAlignment(PA_CENTER);
  P.print(TMM);
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(ldrs[i], INPUT);
    servos[i].attach(2 + i);
    delay(200);
    servos[i].write(0);
    delay(200);
    servos[i].write(90);
    delay(200);
  }
  servos[3].write(180);
  delay(200);
  servos[3].write(90);
  delay(200);
  for (int i = 0; i < 4; i++) {
    stat_servos[i] = 1;
  }
  P.begin();
}

void loop() {
  if (roundz == 1) {
    starter();
    roundz = 2;
  } else if (roundz == 2) {
    Timer.startTimer();
    roundz = 3;
  }

  timer_val = Timer.readTimer();

  if (time_sec <= intervals_H && time_sec >= intervals_L) {
    printscore();
  } else {
    printtime();
  }

  if (time_sec == intervals_L - 1) {
    intervals_H -= 10;
    intervals_L -= 10;
  }
  if (time_sec == intervalt_L - 1) {
    intervalt_H -= 10;
    intervalt_L -= 10;
  }

  Timeoutchk();
  time_sec_count();

  int val_ldrs[4];
  for (int i = 0; i < 4; i++) {
    val_ldrs[i] = analogRead(ldrs[i]);
  }

  for (int i = 0; i < 4; i++) {
    if (val_ldrs[i] >= 1000) {
      servos[i].write(0);
      delay(100);
      stat_servos[i] = 0;
      point += 1;
    }
  }

  if (stat_servos[0] == 0 && stat_servos[1] == 0 && stat_servos[2] == 0 && stat_servos[3] == 0) {
    randomservo();
  }
}
