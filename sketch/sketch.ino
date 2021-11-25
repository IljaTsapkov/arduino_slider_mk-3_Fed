// lcd дисплей
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
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

#define TRIGGER_PIN_R 39
#define ECHO_PIN_R 41
#define MAX_DISTANCE_R 200

NewPing sonarR(TRIGGER_PIN_R, ECHO_PIN_R, MAX_DISTANCE_R);
NewPing sonarL(TRIGGER_PIN_L, ECHO_PIN_L, MAX_DISTANCE_L);
// настройка объекта NewPing: контакты и максимальная дистанция

#define RECEIVER_PIN 47 // define the IR receiver pin

// подключение мотора
#include <Stepper.h>

// Number of steps per output rotation
const int stepsPerRevolution = 3000;

// Create Instance of Stepper library
Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7);

void setup()
{
  String state = "task3";
  if (state == "task3")
    setup_3(60, 2);
}

void setup_3(int speed, byte napravlenije)
{

  lcd.init();
  //lcd.backlight(); // Включаем подсветку дисплея
  myStepper.setSpeed(speed);
  Serial.begin(9600); // Открываем последовательную связь на скорости 9600

  myDisplay.begin();
  myDisplay.setIntensity(0);
  myDisplay.displayClear();

  receiver.enableIRIn();  // enable the receiver
  receiver.blink13(true); // enable blinking of the built-in LED when an IR signal is received
}

int get_distance(char lr)
{
  delay(50);
  unsigned int uS = sonarR.ping_cm();
  if (lr == 'r')
  {
    return uS = sonarR.ping_cm();
  }
  else if (lr == 'l')
  {
    return uS = sonarL.ping_cm();
  }

  // unsigned int uS = sonarR.ping_cm();
  // Serial.print(uS);
  // Serial.println("cm"); //  "сантиметров"
  return 0;
}

void print_distance()
{
  int dl = get_distance('l');
  Serial.print("Left distance\t");
  Serial.println(dl);
  int dr = get_distance('r');
  Serial.print("Right distance\t");
  Serial.println(dr);
}

void motor_control(byte lr)
{
  if (lr == 1) // to left
  {
    Serial.println("clockwise");
    myStepper.step(stepsPerRevolution);
  }
  else if (lr == 2) //to right
  {
    Serial.println("counterclockwise");
    myStepper.step(-stepsPerRevolution);
  }
}

void print_lcd(String str, byte row)
{
  // lcd code
  if (row == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print(str);
  }
  else if (row == 2)
  {
    lcd.setCursor(0, 1);
    lcd.print(str);
  }
}
int counter = 0;

void opredelenie_napravlenija_dvizenija_motora()
{
  if (sonarL.ping_cm() <= 10) //  если левый датчик видит препятсвиее, то мы крутим вправо
  {
    counter++;
    while (counter >= 4)
    {
      motor_control(2);
      if (sonarR.ping_cm() <= 10)
      {
        counter = 0;
        return;
      }
    }
  }
  if (sonarR.ping_cm() <= 10) //  аналогично
  {
    counter++;
    while (counter >= 4)
    {
      motor_control(1);
      if (sonarL.ping_cm() <= 10)
      {
        counter = 0;
        return;
      }
    }
  }
}

// void red_alert_3()
// {
//
// }
// void test_na_muhu()
// {
//   if (sonarR.ping_cm() >= 10)
//   {
//   }
// }

void loop()
{
  print_distance();
  // red_alert_3();
  opredelenie_napravlenija_dvizenija_motora();

  print_lcd("Task 3            ", 1);
  print_lcd("Ping-Pong         ", 2);
}