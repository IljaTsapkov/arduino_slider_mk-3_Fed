// lcd дисплей
#include <LiquidCrystal.h>
// мембранныя клавиатура
#include <Keypad.h>
// модуль управления шаговым мотором
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
// пульт
#include <IRremote.h>
// Hardware SPI maxon:
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
#define MAX_DEVICES 1
#define CS_PIN 9
#define DATA_PIN 10
#define CLK_PIN 8

// bruh pul5t
#define RECEIVER_PIN 47 // define the IR receiver pin

// Create a new instance of the MD_Parola class with hardware SPI connection:
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// lcd connection
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Keypad
const byte ROWS = 4; // Количество рядов
const byte COLS = 4; // Количество строк
char keys[ROWS][COLS] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {23, 25, 27, 29}; // Выводы, подключение к строкам
byte colPins[COLS] = {31, 33, 35, 37}; // Выводы, подключение к столбцам

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

IRrecv receiver(RECEIVER_PIN); // create a receiver object of the IRrecv class
decode_results results;        // create a results object of the decode_results class
unsigned long key_value = 0;   // variable to store the pressed key value

#include <NewPing.h>

#define TRIGGER_PIN_L 43
#define ECHO_PIN_L 45
#define MAX_DISTANCE_L 200

#define TRIGGER_PIN_R 43
#define ECHO_PIN_R 45
#define MAX_DISTANCE_R 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
// настройка объекта NewPing: контакты и максимальная дистанция

// левые датчики
// const int trigPinL = 43;
// const int echoPinL = 45;
// правые датчики
// const int trigPinR = 39;
// const int echoPinR = 41;

#define RECEIVER_PIN 47 // define the IR receiver pin

void setup()
{
  String state = "task3";
  if (state == "task3")
    setup_3();
}

void setup_3()
{
  Serial.begin(9600); // Открываем последовательную связь на скорости 9600

  // lcd code
  lcd.begin(16, 2);
  lcd.print("Task 3");
  lcd.setCursor(0, 1);
  lcd.print("Ping-pong");

  myDisplay.begin();
  myDisplay.setIntensity(0);
  myDisplay.displayClear();

  receiver.enableIRIn();  // enable the receiver
  receiver.blink13(true); // enable blinking of the built-in LED when an IR signal is received

  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);

  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
}

int getDistance()
{
  delay(50);
  unsigned int uS = sonar.ping_cm();
  Serial.print(uS);
  Serial.println("cm"); //  "сантиметров"
  return uS;
}

MoveToCenter()
{
  do
  {
    int distanceLeft = getDistance();
    Serial.println(distanceL);
    int distanceRight = getDistance();
    Serial.println(distanceR);
  } while (edge_to_edge_move());
}

void loop()
{
  // put your main code here, to run repeatedly:
  double speed double position
      string direction
}