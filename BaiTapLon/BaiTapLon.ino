#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#define LED1 5
#define LED2 6
#define button1 7
#define button2 8
#define button3 9
#define relay 10
#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
int i1, i2, i3, batdau = 0;
float nhietdo = 80, tempC;
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  Serial.begin(9600);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);


  //chao hoi
  lcd.print("HELLO CO HOC II");
  delay(1000);


  //hien thi setup
  lcd.clear();
  lcd.print("SETUP NHIET DO");
  lcd.setCursor(0, 1);
  lcd.print("T=");
}

void loop()
{
  //hienthi();
  getTemperature(); //Ham do nhiet do

  //setup nhiet do
  if (batdau == 0)
  {
    lcd.setCursor(3, 1);
    lcd.print(nhietdo, 1);
    lcd.setCursor(8, 1);
    lcd.print("|");
    lcd.setCursor(10, 1);
    lcd.print(tempC, 1);
    lcd.setCursor(15, 1);
    lcd.print("|");
    buttonstage(); //Trạng thái của các button
    setupT();  // setup nhiệt độ bằng button
  };
  /* neu batdau khac 0 thi khong the setup duoc nhiet do nua */


  //Thuc thi chuong trinh khi an start
  // dun nuoc
  if (batdau != 0 & nhietdo - tempC > 0.5)
  {
    LEDstastus();
    Start(); // Bắt đầu thực thi đun nước
    relayControl();
  };

  // Nhiet do nuoc dat chuan
  if (batdau != 0 & nhietdo - tempC >= -0.5 & nhietdo - tempC <= 0.5)
  {
    LEDstastus();
    relayControl();
    lcd.clear();
    lcd.print("Nuoc dat chuan");
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.setCursor(9, 1);
    lcd.print("'C");
    lcd.setCursor(3, 1);
    lcd.print(tempC);
  };

  //Nhiet do nuoc qua cao
  if (batdau != 0 & tempC - nhietdo > 0.5)
  {
    LEDstastus();
    relayControl();
    lcd.clear();
    lcd.print("T nuoc qua cao");
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.setCursor(3, 1);
    lcd.print(tempC);
    lcd.setCursor(9, 1);
    lcd.print("'C");
  };
}

void buttonstage()
{
  i1 = digitalRead(button1);
  i2 = digitalRead(button2);
  i3 = digitalRead(button3);
  if (i3 == 0)
    batdau++;
}

void setupT()
{
  if (i1 == 0)
    nhietdo = nhietdo + 1;
  if (i2 == 0)
    nhietdo = nhietdo - 1;
}

void Start()
{
  lcd.setCursor(0, 0);
  lcd.print("Dang dun nuoc ");
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.setCursor(2, 1);
  lcd.print(tempC);
  lcd.setCursor(8, 1);
  lcd.print("|");
  lcd.setCursor(9, 1);
  lcd.print(" ");
  lcd.setCursor(10, 1);
  lcd.print(nhietdo, 1);
  lcd.setCursor(14, 1);
  lcd.print("'C");
}

void getTemperature()   //ham do nhiet do
{
  DS18B20.requestTemperatures();
  tempC = DS18B20.getTempCByIndex(0);
}

void relayControl()
{
  if (nhietdo - tempC <= 0.5)
    digitalWrite(relay, LOW);
  else
  {
    digitalWrite(relay, HIGH);
  }
}

void LEDstastus()
{
  //Dang dun nuoc
  if (batdau != 0 & nhietdo - tempC > 0.5)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  };

  //Nhiet do nuoc dat chuan
  if (batdau != 0 & nhietdo - tempC >= -0.5 & nhietdo - tempC <= 0.5)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    delay(400);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    delay(400);
  };

  // Nhiet do qua cao
  if (batdau != 0 & tempC - nhietdo > 0.5)
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    delay(50);
    digitalWrite(LED2, LOW);
    delay(50);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    delay(50);
    digitalWrite(LED2, LOW);
    delay(50);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    delay(50);
    digitalWrite(LED2, LOW);
    digitalWrite(LED1, HIGH);
    delay(50);
  };
}
