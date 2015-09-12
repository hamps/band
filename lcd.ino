#include <Wire.h>
#include <ST7032.h>

ST7032 lcd;

const int buttonPin = 2;
int number = 2;
int kind = 6;

void setup() {
  Serial.begin(9600);
  
  //LCD setting
  lcd.begin(8, 2);
  lcd.setContrast(30);
  
  pinMode(buttonPin, INPUT);

  setDice();
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    lcd.clear();
    
    setDice();
    
    lcd.setCursor(0, 1);
    lcd.print("=" + String(dice()));
    
    delay(200);
  }
}
void setDice() {
  String d = String(number) + "d" + String(kind);
  lcd.setCursor(0, 0);
  lcd.print(d);
}

int dice() {
  int result = 0;
  for (int i = 1; i <= number; i++) {
    result += random(1, kind + 1);
  }
  Serial.println("roll!");
  return result;
}
