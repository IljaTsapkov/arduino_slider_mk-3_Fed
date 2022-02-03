#include <NewPing.h>
// подключение мотора
#include <Stepper.h>
// квадрат
#include <LedControl.h>
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
// Hardware SPI
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
#define MAX_DEVICES 1
#define DIN 10
#define SC 9
#define CLK 8

// Сстрелки
byte arrowR[8] = {0x18, 0x30, 0x60, 0xFF, 0xFF, 0x60, 0x30, 0x18};
byte arrowL[8] = {0x18, 0x0C, 0x06, 0xFF, 0xFF, 0x06, 0x0C, 0x18};

// что-то с квадратом
LedControl lc = LedControl(DIN, CLK, SC, 0);

// пульт
#define RECV_PIN 47

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, SC, MAX_DEVICES);

// кейпад
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

IRrecv irrecv(RECV_PIN);
decode_results results;

#define TRIGGER_PIN_L 43
#define ECHO_PIN_L 45
#define MAX_DISTANCE_L 200

#define TRIGGER_PIN_R 39
#define ECHO_PIN_R 41
#define MAX_DISTANCE_R 200

// настройка объекта NewPing: контакты и максимальная дистанция
NewPing sonarR(TRIGGER_PIN_R, ECHO_PIN_R, MAX_DISTANCE_R);
NewPing sonarL(TRIGGER_PIN_L, ECHO_PIN_L, MAX_DISTANCE_L);

// шаговый мотор
Stepper myStepper(200, 4, 5, 6, 7);

String state = "task3";

void setup()
{
}

void setup_3()
{
  // старт и очистка квадрата
  lc.shutdown(0, false);  //The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0, 15); // Set the brightness to maximum value
  lc.clearDisplay(0);     // and clear the display

  lcd.init();
  //lcd.backlight(); // Включаем подсветку дисплея
  myStepper.setSpeed(60);
  Serial.begin(9600); // Открываем последовательную связь на скорости 9600

  myDisplay.begin();
  myDisplay.setIntensity(0);
  myDisplay.displayClear();

  irrecv.enableIRIn();  // enable the receiver
  irrecv.blink13(true); // enable blinking of the built-in LED when an IR signal is received

  lcd.clear();
  print_lcd("Task 3            ", 1);
  print_lcd("Ping-Pong         ", 2);
}

int get_distance(char lr)
{
  unsigned int uS = sonarR.ping_cm();
  if (lr == 'r')
    return uS = sonarR.ping_cm();
  else if (lr == 'l')
    return uS = sonarL.ping_cm();
  return 0;
}

// чтобы квадрат выводил символы
void printByte(byte character[])
{
  int i = 0;
  for (i = 0; i < 8; i++)
    lc.setRow(0, i, character[i]);
}

// пульT
String pult()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  if (irrecv.decode(&results)) // Returns 0 if no data ready, 1 if data ready.
  {
    int x = results.value; // Results of decoding are stored in result.value
    Serial.println(" ");
    Serial.print("Code: ");
    Serial.println(results.value); //prints the value a a button press
    Serial.println(" ");
    irrecv.resume(); // Restart the ISR state machine and Receive the next value
  }

  switch (results.value)
  {
  case 0xFF22DD:
    return "|<<";
  case 0xFF02FD:
    return ">>|";
  case 0xFF9867:
    return "100+";
  case 0xFFB04F:
    return "200+";
  case 0xFF6897:
    return "0";
  case 0xFF906F:
    return "STOPPED";
  case 0xFF30CF:
    return "1";
  case 0xFF18E7:
    return "2";
  case 0xFF7A85:
    return "3";
  case 0xFF10EF:
    return "4";
  case 0xFF38C7:
    return "5";
  case 0xFF5AA5:
    return "6";
  case 0xFF42BD:
    return "7";
  case 0xFF4AB5:
    return "8";
  case 0xFF52AD:
    return "9";
  case 0xFFC23D:
    return "|>||";
  default:
    return "";
  }
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

void motor_control(char lr, int speed)
{
  if (lr == 'l') // to left
  {
    while (true)
    {
      if (get_distance('l') <= 15 || pult() == "|>||")
        return;
      //Serial.println("to_left");
      myStepper.step(speed);
    }
  }
  else if (lr == 'r') //to right
  {
    while (true)
    {
      if (get_distance('r') <= 15 || pult() == "|>||")
        return;
      //Serial.println("to_left");
      myStepper.step(-speed);
    }
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

// в этой функции мы определяем стены,
// делаем тест на муху (ТЕСТ НЕ ПРОЙДЕН, ТЕСТ НА СТЕНУ ЕГО ПОЛНОСТЬЮ ЗАМЕНЯЕТ),
// проверяем нажатие клавишь пульта
// и потом отдаём приказ мотору / дисплею
void dvizenije_zizn(char lr, int speed)
{
  lcd.clear();
  print_lcd("Task 3 PingPong", 1);
  print_lcd("To stop: |>||", 2);
  delay(1000);
  while (true)
  {
    if (pult() == "|>||")
    {
      lcd.clear();
      return;
    }
    lr = stena(lr);
    lcc(lr);
    motor_control(lr, speed);
  }
}

// экран 8x8 стрелочки
void lcc(char lr)
{
  if (lr == 'r')
    printByte(arrowR);
  else if (lr == 'l')
    printByte(arrowL);
}

// возвращает куда крутить мотор
char stena(char kuda)
{
  if (get_distance('r') <= 15)
    kuda = 'l';
  else if (get_distance('l') <= 15)
  {
    kuda = 'r';
    // Serial.println("Стена вернула " + String(kuda));
  }
  return kuda;
}

// возвращает скорость с пульта
String inp_speed()
{
  String speed = "";
  while (true)
  {
    print_lcd("Input speed:", 1);
    print_lcd(speed + "%   ", 2);

    // delay(500);
    String p = pult();
    if (p == "100+")
      return speed;
    else if (p == "200+")
      speed = "   ";
    else if (p == "0")
      speed = "100";
    else if (p == "1")
      speed = "10";
    else if (p == "2")
      speed = "20";
    else if (p == "3")
      speed = "30";
    else if (p == "4")
      speed = "40";
    else if (p == "5")
      speed = "50";
    else if (p == "6")
      speed = "60";
    else if (p == "7")
      speed = "70";
    else if (p == "8")
      speed = "80";
    else if (p == "9")
      speed = "90";
  }

  if (speed.toInt() > 200)
    speed = "200";
  else if (speed.toInt() < 1)
    speed = "1";

  return speed;
}

void task_3()
{

  // // чисто пульт коды делает
  // if (irrecv.decode(&results))
  //  {
  //    Serial.println(results.value, HEX); //вывод результатов в монитор порта
  //    irrecv.resume(); // прием следующего значения
  //  }
  // ввод скорости
  lcd.backlight();
  lcd.clear();
  print_lcd("Input speed", 1);
  print_lcd("max 200", 2);
  delay(3000);
  lcd.clear();
  print_lcd("100+ ok", 1);
  print_lcd("200+ clear", 2);
  delay(3000);
  lcd.clear();
  int speed_end = inp_speed().toInt();
  delay(3000);
  lcd.clear();
  print_lcd("Press:", 1);
  print_lcd("|<< or >>|", 2);

  speed_end = speed_end * 2;
  while (true)
  {
    // выбор направления и запуск мотора
    if (pult() == "|<<")
      dvizenije_zizn('l', speed_end);
    else if (pult() == ">>|")
      dvizenije_zizn('r', speed_end);
    else if (pult() == "|>||")
      return;
    Serial.println("Работает вайл в лупе");
    Serial.println("pult = " + pult());
  }

  // проверка_на_пульт();
  lcd.print(pult());
  delay(1000);
  lcd.clear();
}

void loop()
{
  if (state == "task3")
  {
    setup_3();
    task_3();
  }
}
