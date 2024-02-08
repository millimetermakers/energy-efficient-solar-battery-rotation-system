#include <Servo.h> 
 

/////////////////////////////////////////////////////////////// 
////////////////////////    PINS   ////////////////////////////
/////////////////////////////////////////////////////////////// 

 
#define L_PHOTO_S_PIN 		A0
#define R_PHOTO_S_PIN 		A1 
#define SOLAR_PANEL_PIN 	A3 

#define SERVO_PIN 			9 
 
 
 
 
/////////////////////////////////////////////////////////////// 
///////////////////////    SETTINGS   /////////////////////////
/////////////////////////////////////////////////////////////// 




#define UART_SPEED 115200

#define LOOP_CYCLE_DELAY 20 



#define VCC			5.0
#define ADC_MAX 	1023

#define SOLAR_LOW_VOLTAGE 4.0 


#define LIGHT_DIF_THRESHOLD  48 
 

#define SERVO_STEP_ANGLE 	2 
#define SERVO_STEP_TIME_MS	5

#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180


#define LEFT 	1
#define RIGHT 	0





/////////////////////////////////////////////////////////////// 
////////////////////////    SERVO   ///////////////////////////
/////////////////////////////////////////////////////////////// 


Servo servo; 

void servo_init(){
	servo.attach(SERVO_PIN);
}
 
int servo_get_pos(){
	return servo.read();
}


bool is_servo_rightEnd(){
	return servo.read() == SERVO_MIN_ANGLE;
}


bool is_servo_leftEnd(){
	return servo.read() == SERVO_MAX_ANGLE;
}


void turn_right_360(){ 
  for(int i=SERVO_MAX_ANGLE; i > SERVO_MIN_ANGLE - 1; i -= SERVO_STEP_ANGLE){ 
    servo.write(i); 
    delay(SERVO_STEP_TIME_MS); 
  } 
} 
 
 
void turn_left_360(){ 
  for(int i=SERVO_MIN_ANGLE; i < SERVO_MAX_ANGLE - 1; i += SERVO_STEP_ANGLE){ 
    servo.write(i); 
    delay(SERVO_STEP_TIME_MS); 
  } 
} 
 
 
void turn_right_oneStep(){ 
  servo.write(servo.read() + SERVO_STEP_ANGLE); 
} 
 
 
void turn_left_oneStep(){ 
  servo.write(servo.read() - SERVO_STEP_ANGLE); 
} 






/////////////////////////////////////////////////////////////// 
///////////////////    PHOTO SENSORS   ////////////////////////
/////////////////////////////////////////////////////////////// 


void r_photoS_init(){
	pinMode(R_PHOTO_S_PIN, INPUT);
}


void l_photoS_init(){
	pinMode(L_PHOTO_S_PIN, INPUT);
}

int r_photoS_getRaw(){
	return analogRead(R_PHOTO_S_PIN);
}

int l_photoS_getRaw(){
	return analogRead(L_PHOTO_S_PIN);
}
 

bool get_turning_dir(int light_dir){
	return light_dir > 0 ? RIGHT : LEFT;
}	





/////////////////////////////////////////////////////////////// 
////////////////////    SOLAR PANEL   /////////////////////////
/////////////////////////////////////////////////////////////// 


void solarPanel_init(){
	pinMode(SOLAR_PANEL_PIN, INPUT);
}


int solarPanel_getRaw(){
	return analogRead(L_PHOTO_S_PIN);
}


float solarPanel_getVoltage(){
	return solarPanel_getRaw() * VCC / ADC_MAX;
}








/////////////////////////////////////////////////////////////// 
///////////////////    SETUP AND LOOP   ///////////////////////
/////////////////////////////////////////////////////////////// 


  
void setup() { 
	Serial.begin(UART_SPEED);
	
	l_photoS_init();
	r_photoS_init();
	solarPanel_init();
	servo_init();
} 
 
 

 
 
void loop() 
{ 
	float solar_v = solarPanel_getVoltage();

	int l_light = l_photoS_getRaw(); 
	int r_light = r_photoS_getRaw();   
	int light_dif = l_light - r_light; 
	
	bool is_solarEfficiency_low = solar_v <= SOLAR_LOW_VOLTAGE;
	bool is_angleError_big = abs(light_dif) >= LIGHT_DIF_THRESHOLD;
   
	if (is_solarEfficiency_low and is_angleError_big)
	{
		bool dir = get_turning_dir(light_dif);
		if (dir == RIGHT)
		{ 
			if (is_servo_rightEnd())
			{ 
			  turn_left_360(); 
			} 
			turn_right_oneStep();   
		} 
		else if (dir == LEFT)
		{
			if (is_servo_leftEnd())
			{ 
			  turn_right_360(); 
			} 
			turn_left_oneStep();
		} 
	}  
	
	delay(LOOP_CYCLE_DELAY); 
}
