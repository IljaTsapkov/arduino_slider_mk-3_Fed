 #include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей
void setup()
{
  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея
  lcd.print("iarduino.ru");
  lcd.setCursor(8, 1);
  lcd.print("LCD 1602");
}
void loop()
{
  // Устанавливаем курсор на вторую строку и нулевой символ.
  lcd.setCursor(0, 1);
  // Выводим на экран количество секунд с момента запуска ардуины
  lcd.print(millis()/1000);
}