#include <Servo.h>

//PINS
#define LIGHT_RIGHT_PIN A1
#define LIGHT_LEFT_PIN A0
#define SERVO_PIN 9
#define SOLAR_PANEL_PIN A3

//CONST
#define MAX_DIF_LIGH 15
#define STEP_OF_SERVO 2
#define MIN_CURRENT 5.5
#define DIF_LIGH 48

Servo servo;


//настройка пинов
void setup()
{
  pinMode(LIGHT_RIGHT_PIN, INPUT);
  pinMode(LIGHT_LEFT_PIN, INPUT);
  pinMode(SOLAR_PANEL_PIN, INPUT);

  Serial.begin(9600);
  servo.attach(SERVO_PIN);
}


void counter_clockwise(){
  for(int i=180; i > 1; i--){
    servo.write(i);
    delay(5);
  }
}


void clockwise_rotation(){
  for(int i=0; i < 179; i++){
    servo.write(i);
    delay(5);
  }
}


void servo_write_rotation(){
  servo.write(srvo.read() + STEP_OF_SERVO);
}


void servo_write_counter(){
  servo.write(servo.read() - STEP_OF_SERVO);
}


//int left_light_check(){
  //return analogRead(LIGHT_LEFT_PIN);
//}


void loop()
{
  int light1 = analogRead(LIGHT_LEFT_PIN);
  int light2 = analogRead(LIGHT_RIGHT_PIN);
  float raw_solar_tok = analogRead(SOLAR_PANEL_PIN);
  float solar_tok = raw_solar_tok / 204.6;

  
  int difrence = abs(light1 - light2);
  
  if(solar_tok <= MIN_CURRENT){
    if(difrence > DIF_LIGH){
      if(light1 > light2){
        if(servo.read() == 0){
          clockwise_rotation();
        }
        servo_write_counter();  
      }
      else{
        if(servo.read() == 180){
          counter_clockwise();
        }
        servo_write_rotation();
      }
    }
  }
  delay(20);
}