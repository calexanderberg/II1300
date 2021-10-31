#include <stdio.h>
#include <unistd.h>
#include "brick.h"

#define MotorL		OUTA
#define MotorR		OUTB
#define MotorM		OUTC
#define Ultrasonic	IN1

//in3	=	button
//in1	=	sensor
//in2	=	gyro

#define Motors	(MotorL | MotorR | MotorM)
#define Forwards (MotorL | MotorR)

void main()
{
	brick_init();
	sleep_ms(1000);
	printf("ya\n");
	tacho_reset(Motors);
	//int speed = tacho_get_speed(MotorL,float);
	float sensor sensor_get_value0(Ultrasonic, float);
	printf("/d", sensor);
	

	testRun();
}

int testRun()
{
	sleep_ms(1000);
	tacho_set_speed_sp(Forwards, 500);
	printf("forwards\n");
	tacho_run_forever(Forwards);
	sleep_ms(1000);
	printf("forwards stannar\n");
	tacho_stop(Forwards);
	return 0;
}
