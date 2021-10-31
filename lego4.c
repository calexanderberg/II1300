#include <stdio.h>
#include "brick.h"

#define MotorL		OUTA
#define MotorR		OUTB
#define MotorM		OUTC
#define Ultrasonic	IN1
#define Gyro		IN2

//in3	=	button
//in1	=	sensor
//in2	=	gyro

#define Motors (MotorL | MotorR | MotorM)
#define Forwards (MotorL | MotorR)

int forwards(int desired_seconds, int target_angle);
int forwards_until_distance(int cm);
int turn_right();
int turn_left();
int distance();
int get_outta_here_book();
int find_wall();
int gyroscope();
int compensate(int direction, int left_speed, int right_speed);

void main()
{
	brick_init();
	sleep_ms(800);
	tacho_reset(Motors);

	find_wall();
	forwards_until_distance(20);
	turn_right();

	int angle = gyroscope();
	forwards(8, angle);


}

int forwards(int desired_seconds, int target_angle)
{
	tacho_reset(Motors);
	int left_speed = 275;
	int right_speed = 265;
	int left = 0;
	int right = 1;
	int angle = gyroscope();
	int passed_time = 0;

	tacho_set_speed_sp(MotorL, left_speed);
	tacho_set_speed_sp(MotorR, right_speed);
	tacho_run_forever(Forwards);
	while (passed_time < (desired_seconds*5))
	{
		if (angle > (target_angle+1)) {
			compensate(left, left_speed, right_speed);
		}
		if (angle < (target_angle-1)) {
			compensate(right, left_speed, right_speed);
		}
		sleep_ms(200);
		passed_time++;
	}

	tacho_stop(Forwards);
}

int compensate(int direction, int left_speed, int right_speed)
{
	if (direction == 0)
	{
		tacho_set_speed_sp(MotorL, left_speed*0.9);
		tacho_set_speed_sp(MotorR, right_speed*1.1);
	}
	if (direction == 1)
	{
		tacho_set_speed_sp(MotorL, left_speed * 1.1);
		tacho_set_speed_sp(MotorR, right_speed * 0.9);
	}
}

int forwards_until_distance(int cm)
{
	int cm_left;
	int angle = gyroscope();
	cm_left = distance();
	while ((cm * 10) < cm_left)
	{
		forwards(2, angle);
		cm_left = distance();
	}
}

int turn_right()
{
	tacho_reset(Motors);
	int angle;
	int initial_angle = gyroscope();
	printf("turning right");
	printf("initial angle: %d\n", initial_angle);
	printf("stopping angle: %d\n", initial_angle-87);

	tacho_set_speed_sp(MotorL, 110);
	tacho_set_speed_sp(MotorR, -110);
	tacho_run_forever(Forwards);
	do
	{
		sleep_ms(20);
		angle = gyroscope();
		printf("**************current angle: %d\n", angle);
	} while (angle > (initial_angle - 88));

	tacho_stop(Forwards);
}



int distance()
{
	static int distance_array[10];
	int distance;
	int distanceA;
	int distanceB;
	distanceA = sensor_get_value(0, Ultrasonic, distance_array[1]);
	sleep_ms(20);
	distanceB = sensor_get_value(0, Ultrasonic, distance_array[1]);
	distance = (distanceA + distanceB) / 2;
	return distance;
}

int gyroscope()
{
	static int gyroscope_array[10];
	int angle;
	angle = sensor_get_value(0, Gyro, gyroscope_array[1]);
	return angle;
}

int get_outta_here_book()
{
	tacho_reset(Motors);
	tacho_set_speed_sp(MotorM, -300);
	sleep_ms(1000);
	tacho_stop(Motors);
}

int find_wall()
{
	tacho_reset(Motors);
	tacho_set_speed_sp(MotorL, 180);
	tacho_set_speed_sp(MotorR, -180);
	tacho_run_forever(Forwards);

	int measurementA;
	int measurementB;
	int lowest_measurement = 1000;
	int lowest_angle = 0;
	int angle;

	measure1:
	measurementA = distance();
	sleep_ms(500);
	measurementB = distance();

	if (measurementA > 1000)
	{
		goto measure1;
	}

	printf("\n\nmeasurement A < 1000\n");

	if (measurementA < lowest_measurement)
	{
		lowest_measurement = measurementA;
		lowest_angle = gyroscope();
		printf("lowest measurement: %d (%d)\n", lowest_measurement, lowest_angle);
	}

	if (measurementB < lowest_measurement)
	{
		lowest_measurement = measurementB;
		lowest_angle = gyroscope();
		printf("lowest measurement: %d (%d)\n", lowest_measurement, lowest_angle);
	}

	if (measurementA > measurementB)
	{
		goto measure1;
	}

	printf("measurement A: %d\n", measurementA);
	printf("measurement B: %d\n", measurementB);
	printf("moving to measure 2\n");

	tacho_stop(Forwards);
	tacho_set_speed_sp(MotorR, 100);
	tacho_set_speed_sp(MotorL, -100);
	tacho_run_forever(Forwards);
	sleep_ms(300);

	measure2:
	measurementA = distance();
	sleep_ms(300);
	measurementB = distance();

	if (measurementA < lowest_measurement)
	{
		lowest_measurement = measurementA;
		lowest_angle = gyroscope();
		printf("lowest measurement: %d (%d)\n", lowest_measurement, lowest_angle);
	}

	if (measurementB < lowest_measurement)
	{
		lowest_measurement = measurementB;
		lowest_angle = gyroscope();
		printf("lowest measurement: %d (%d)\n", lowest_measurement, lowest_angle);
	}

	if (measurementA > measurementB)
	{
		goto measure2;
	}

	sleep_ms(400);

	printf("measurement A: %d\n", measurementA);
	printf("measurement B: %d\n", measurementB);
	printf("moving to measure 3\n");

	tacho_stop(Forwards);
	tacho_set_speed_sp(MotorL, 70);
	tacho_set_speed_sp(MotorR, -70);
	tacho_run_forever(Forwards);
	printf("desired angle: %d\n", lowest_angle);

	do
	{
		sleep_ms(20);
		angle = gyroscope();
		printf("**************current angle: %d\n", angle);
	} while ((angle < (lowest_angle - 1)) || (angle > (lowest_angle + 1)));

	tacho_stop(Forwards);
	measurementA = distance();

	printf("final angle: %d\n", angle);
	printf("final distance: %d\n", measurementA);
	printf("stopping\n");

}