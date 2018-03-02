#ifndef MDD10A_h
#define MDD10A_h

#include <Arduino.h>

class MDD10A{

public:

	MDD10A();

	void setMotorSpeed(int controller, int speed);
	void init();

	int dir = 0;


	//PinOuts
	//mc0 = motor pod 1
	const int _mc0_dir0 = 42;  //Right Front
	const int _mc0_pwm0 = 10;
	const int _mc0_dir1 = 43;  //Lift?
	const int _mc0_pwm1 = 11;
	//mc1 = motor pod 2
	const int _mc1_dir0 = 28;  //Right rear
	const int _mc1_pwm0 = 2;
	const int _mc1_dir1 = 26;  //Lift
	const int _mc1_pwm1 = 3;
	//mc2 = motor pod 3
	const int _mc2_dir0 = 32;  //Left Front
	const int _mc2_pwm0 = 8;
	const int _mc2_dir1 = 30;  //Left Rear
	const int _mc2_pwm1 = 9;
	//mc3 = motor pod 4
	const int _mc3_dir0 = 27;
	const int _mc3_pwm0 = 5;
	const int _mc3_dir1 = 29;
	const int _mc3_pwm1 = 6;





};

#endif
