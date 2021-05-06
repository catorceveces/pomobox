#include <LiquidCrystal.h>                  // incluir libreria LCD

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // inicializar LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int playPin = 7;    // pin de play
int stopPin = 8;    // pin de stop
int greenPin = 9;   // pin de led
int redPin = 10;    // pin de led

int pomoState = LOW;      // check instance
int currentPlay;          // estado de play
int currentStop;          // estado de stop
int previousPlay = HIGH;

int minutes;
int seconds;
int periods;
char timeline[16];
char trackperiods[16];

boolean onBreak = false;

void setup() {

  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(playPin, INPUT);
  pinMode(stopPin, INPUT);

  lcd.begin(16, 2);       // set up the lcd number of columns and rows
  lcd.setCursor(4,0);
  lcd.print("POMO-BOX");  // 8 char
  lcd.setCursor(5,1);
  lcd.print("v. 1.0");    //  6 char
  delay(5000);            // pausa de cinco segundos hasta entrar al menu principal

  menu();

}

void menu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> pomodoro cycle");
  lcd.setCursor(0, 1);
  lcd.print("to exit, press X");

  minutes = 0;          // asigna valores a las variables acá por si el usuario cancela la sesión luego
  seconds = 0;          // y se reinicia el programa volviendo a la función menu()
  periods = 0;
  onBreak = false;
}

void loop() {

  currentPlay = digitalRead(playPin);

  if (currentPlay == HIGH && previousPlay == HIGH) {

    if (pomoState == LOW) {

      pomoState = HIGH;
      currentStop = digitalRead(stopPin);

      while (digitalRead(stopPin) == currentStop) {                         // inicia ciclo de pomodoros, que debe estructurarse enteramente con sentencias if / else
                                                                            // ya que, de otro modo, no podría leer (adentro de un loop) el botón de stop
        if (onBreak == false) {

          // cuenta un pomodoro y termina con periods++ y onBreak = true

              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("IN FOCUS - ");
              lcd.setCursor(11, 0);
              sprintf(timeline, "%0.2d:%0.2d", minutes, seconds);
              lcd.print(timeline);

              lcd.setCursor(2, 1);
              sprintf(trackperiods, "%0.2d pomodoros", periods);
              lcd.print(trackperiods);

              digitalWrite(greenPin, LOW);
              digitalWrite(redPin, HIGH);

              delay(1000);
              seconds++;

              if (seconds > 59) {
                seconds = 0;
                minutes++;
              }

              else if (minutes > 25) {
                periods ++;
                minutes = 0;
                seconds = 0;
                onBreak = true;
              }
        }

        else if (onBreak == true && periods % 4 !=0) {

          // cuenta un descanso breve y termina con onBreak = false

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("ON BREAK - ");
          lcd.setCursor(11, 0);
          sprintf(timeline, "%0.2d:%0.2d", minutes, seconds);
          lcd.print(timeline);

          lcd.setCursor(2, 1);
          sprintf(trackperiods, "%0.2d pomodoros", periods);
          lcd.print(trackperiods);

          digitalWrite(redPin, LOW);
          digitalWrite(greenPin, HIGH);

          delay(1000);
          seconds++;

          if (seconds > 59) {
            seconds = 0;
            minutes++;
          }

          else if (minutes > 5) {
            minutes = 0;
            seconds = 0;
            onBreak = false;
          } continue;
        }

        else if (onBreak == true && periods % 4 == 0) {

          // cuenta un descanso largo y termina con onBreak = false

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("ON BREAK - ");
          lcd.setCursor(11, 0);
          sprintf(timeline, "%0.2d:%0.2d", minutes, seconds);
          lcd.print(timeline);

          lcd.setCursor(2, 1);
          sprintf(trackperiods, "%0.2d pomodoros", periods);
          lcd.print(trackperiods);

          digitalWrite(redPin, LOW);
          digitalWrite(greenPin, HIGH);

          delay(1000);
          seconds++;

          if (seconds > 59) {
            seconds = 0;
            minutes++;
          }

          else if (minutes > 20) {
            minutes = 0;
            seconds = 0;
            onBreak = false;
          }

        }

      }

    if (digitalRead(stopPin) != currentStop) {                  // lectura del botón de stop

        pomoState = LOW;

        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, LOW);
        delay(300);
        digitalWrite(greenPin, HIGH);
        delay(300);
        digitalWrite(greenPin, LOW);
        digitalWrite(redPin, HIGH);
        delay(300);
        digitalWrite(redPin, LOW);
        delay(300);


        lcd.clear();
        lcd.setCursor(2, 0);
        sprintf(trackperiods, "%0.2d pomodoros", periods);
        lcd.print(trackperiods);
        lcd.setCursor(3, 1);
        lcd.print("good work!");

        delay(5000);

        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("catorceveces");
        lcd.setCursor(3, 1);
        lcd.print(".github.io");

        delay(10000);

        menu();                                                 // vuelve al menú inicial y reinicia los valores de ciclo a 0
      }

    }

  }

  previousPlay = currentPlay;

}
