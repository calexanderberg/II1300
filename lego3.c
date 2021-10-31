#include <stdio.h>
#include "brick.h"

#define MotorL		OUTA
#define MotorR		OUTB
#define MotorM		OUTC
#define Ultrasonic	IN1

//in3	=	button
//in1	=	sensor
//in2	=	gyro

#define Motors (MotorL | MotorR | MotorM)
#define Forwards (MotorL | MotorR)

int forwards(int seconds);
int forwards_until_distance(int cm);
int turn_right();
int turn_left();
int distance();
int get_outta_here_book();

void main()
{
	brick_init();
	sleep_ms(1000);
	tacho_reset(Motors);

	forwards_until_distance(50);
	turn_right();
	forwards(4);
	turn_left();
	get_outta_here_book();
}

int forwards(int seconds)
{
	tacho_reset(Motors);
	tacho_set_speed_sp(MotorL, 300);
	tacho_set_speed_sp(MotorR, 290);
	tacho_run_forever(Forwards);
	sleep_ms(seconds * 1000);
	tacho_stop(Forwards);
	return 0;
}

int forwards_until_distance(int cm)
{
	int cm_left;
	cm_left = distance();
	while ((cm * 10) < cm_left)
	{
		forwards(2);
		cm_left = distance();
	}
	return 0;
}

int turn_right()
{
	tacho_reset(Motors);
	sleep_ms(100);
	tacho_set_speed_sp(MotorL, 250);
	tacho_run_forever(MotorL);
	sleep_ms(2500);
	tacho_stop(MotorL);
	return 0;
}

int turn_left()
{
	tacho_reset(Motors);
	sleep_ms(100);
	tacho_set_speed_sp(MotorR, 240);
	tacho_run_forever(MotorR);
	sleep_ms(1400);
	tacho_stop(MotorR);
	return 0;
}

int distance()
{
	static int distance_array[10];
	int distance;
	distance = sensor_get_value(0, Ultrasonic, distance_array[1]);
	return distance;
}

int get_outta_here_book()
{
	tacho_reset(Motors);
	tacho_set_speed_sp(MotorM, -300);
	sleep_ms(1000);
	tacho_stop(Motors);
}