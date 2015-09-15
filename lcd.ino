//ダイスロールをします。テストプログラムです。

#include <Wire.h>

//http://ore-kb.net/archives/195のライブラリを使用
#include <ST7032.h>

ST7032 lcd;

const int buttonPin = 2;
const int vrPin = A0;
int number = 2;
int kind = 6;
int vrValue = 0;
int pressTime = 0;
boolean setting = false;
int setMode = 0;

void setup() {
  Serial.begin(9600);
  
  //LCD setting
  lcd.begin(8, 2);
  lcd.setContrast(30);
  
  pinMode(buttonPin, INPUT);

  randomSeed(analogRead(A5));

  setDice();

  vrValue = analogRead(vrPin);
}

void loop() {
  //可変抵抗の変化分だけ数字を変える
  if(abs(vrValue - analogRead(vrPin)) > 1 && setting) {
    setValue();
  }
  
  if(digitalRead(buttonPin) == LOW && !setting) {
    //ボタンが押されたらサイコロを振る
    lcd.clear();
    setDice();
    lcd.setCursor(0, 1);
    lcd.print("=" + String(roll()));
    
    //押された時間を記録
    if(pressTime == 0) {
      pressTime = millis();
    }
    //設定の種類の変更
    if(setting){
      setMode++;
    }
  }
  
  else {
    //ボタンが離されたときに長押しを判定
    if(pressTime != 0) {
      if(millis() - pressTime > 1000) {
        //設定モードを開始
        setting = true;
        setMode++;
      }
      pressTime = 0;
    }
  }

  //設定している項目を点滅する
  if(setting) {
    if(millis() % 500 == 0) {
      switch(setMode) {
        String spaces = "   ";
        case 1:
          lcd.setCursor(0, 0);
          lcd.print(spaces.remove(String(number).length()));
          break;
        case 2:
          lcd.setCursor(0, String(number).length() + 1);
          lcd.print(spaces.remove(String(kind).length()));
          break;
      }
    }
    if(millis() % 500 == 250) {
      setDice();
    }
  }
  
  //設定モード解除
  if(setMode >= 3) {
    setMode = 0;
    setting = false;
  }
}
void setDice() {
  String d = String(number) + "d" + String(kind);
  lcd.setCursor(0, 0);
  lcd.print(d);
}

int roll() {
  int result = 0;
  for(int i = 1; i <= number; i++) {
    result += random(1, kind + 1);
  }
  Serial.println("roll!");
  return result;
}

void setValue() {
  switch(setMode) {
    case 1:
      if(number + (analogRead(vrPin) - vrValue) > 0) {
        number += analogRead(vrPin) - vrValue;
      }
      break;
    case 2:
      if(kind + (analogRead(vrPin) - vrValue) > 0) {
        kind += analogRead(vrPin) - vrValue;
      }
      break;
  }
  lcd.clear();
  setDice();
  vrValue = analogRead(vrPin);
  Serial.println(vrValue);
}
