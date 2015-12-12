#include <AquesTalk.h>
#include <Wire.h>    // I2Cライブラリ 必須！

AquesTalk atp;       // 音声合成のインスタンス

int fsrPin = 0;      // the FSR and 10K pulldown are connected to a0
int fsrReading = 0;  // the analog reading from the FSR resistor divider

int fsrBufferUp = 0;
int fsrBufferDw = 0;
int count = 0;
int timer = 0;
int onFlag = false;


void setup(void) {
  Serial.begin(9600);
  Serial.println("-- setup() --");
  
  if( !atp.IsActive() ){
    Serial.println("No response.");
  }
  else {
    Serial.println("Active");
    atp.SetSpeed(100);
  }
}

void loop(void) {
  fsrReading = analogRead(fsrPin);
  
  if (fsrReading > fsrBufferUp && !onFlag) {
    if(timer == 2) {
      count = setCount(count);
      fsrBufferUp = fsrReading;
      fsrBufferDw = 0;
    }
  }
  if (fsrReading < fsrBufferUp && !onFlag) {
    if(timer == 2) {
      if (fsrReading > fsrBufferDw && fsrBufferDw != 0) {
        count = setCount(count);
        fsrBufferUp = fsrReading;
        fsrBufferDw = 0;
      }
      else if (fsrReading > 0) {
        fsrBufferDw = fsrReading;
        callCheer(count);
      }
    }
  }
  
  if (fsrReading == 0) {
    onFlag = false;
    timer = 0;
  }
  else {
    if (timer == 2)  onFlag = true;
    timer++;
  }
  
  Serial.print(onFlag);
  Serial.print("\t");
  Serial.print(fsrReading);
  Serial.print("\t");
  Serial.print(fsrBufferUp);
  Serial.print("\t");
  Serial.print(fsrBufferDw);
  Serial.print("\t");
  Serial.println(count);
  
//  count = setCount(count);
//  callCheer(count);
  delay(1000);
}


//カウントされたら、読み上げる。
int setCount(int cnt) {
  String str;
  cnt++;
  
  str = "<NUMK VAL=" + String(cnt) + "COUNTER=ha'i MODE=GcH>mede'_su.";
  atp.SyntheS(str);

  while(atp.IsBusy());
  
  return cnt;
}


//コースターに置いたら応援する。
void callCheer(int cnt) {
  String str;
  
  if (cnt == 1) {
    str = "ma'damada/-'";
  }
  else if (cnt == 2) {
    str = "mo'tto-";
  }
  else {
    str = "o_tsukaresamade'_shita.";
  }
  atp.SyntheS(str);

  while(atp.IsBusy());
}
