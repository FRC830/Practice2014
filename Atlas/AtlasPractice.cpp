#include "WPILib.h"
#include "Gamepad.h"
#include <cmath>

//will this get commited?
class AtlasPractice : public IterativeRobot {
	
	static const int ROLLER_MOTOR_PWM = 1;
	static const int LINE_BREAK_DIO = 9;
	static const int ARM_PWM = 2;
	static const int TOP_ARM_SWITCH_DIO = 7;
	
	static const int FRONT_LEFT_PWM = 3;
	static const int REAR_LEFT_PWM = 4;
	static const int FRONT_RIGHT_PWM = 7;
	static const int REAR_RIGHT_PWM = 8;
	
	static const int PRESSURE_SWITCH_CHANNEL = 6;
	static const int COMPRESSOR_RELAY_CHANNEL = 1;
	
	static const int WINCH_SWITCH_DIO = 4;
	static const int WINCH_PWM = 9;
	static const bool CLUTCH_IN = true;
	static const bool CLUTCH_OUT = false;
	static const int CLUTCH_SOLENOID = 2;
	
	static const float TIME_TO_MAX_SPEED = 1.0;
	
	static const int GEAR_SHIFT_FORWARD = 8;
	static const int GEAR_SHIFT_REVERSE = 1;
	static const DoubleSolenoid::Value LOW_GEAR = DoubleSolenoid::kForward;
	static const DoubleSolenoid::Value HIGH_GEAR = DoubleSolenoid::kReverse;
	
	Victor * roller_motor;
	DigitalInput * top_arm_switch;
	DigitalInput * line_break;
	Victor * arm;
	Talon * front_left;
	Talon * rear_left;
	Talon * front_right;
	Talon * rear_right;
	RobotDrive * drivetrain;
	
	DoubleSolenoid * gear_shift;
	Compressor * compressor;
	Solenoid * clutch;
	
	Victor * winch;
	DigitalInput * winch_switch;
	
	Gamepad * pilot;
	 
	DriverStationLCD * lcd;
	
	float previous_speed;
	float previous_turn;
	
	void RobotInit() {
		roller_motor = new Victor(ROLLER_MOTOR_PWM);
		
		arm = new Victor(ARM_PWM);
		top_arm_switch = new DigitalInput(TOP_ARM_SWITCH_DIO);
		line_break = new DigitalInput(LINE_BREAK_DIO);
		front_left = new Talon(FRONT_LEFT_PWM);
		rear_left = new Talon(REAR_LEFT_PWM);
		front_right = new Talon(FRONT_RIGHT_PWM);
		rear_right = new Talon(REAR_RIGHT_PWM);
		drivetrain = new RobotDrive(front_left, rear_left, front_right, rear_right);
		previous_speed = 0.0;
		previous_turn = 0.0;
		
		gear_shift = new DoubleSolenoid(GEAR_SHIFT_FORWARD, GEAR_SHIFT_REVERSE);
		compressor = new Compressor(PRESSURE_SWITCH_CHANNEL, COMPRESSOR_RELAY_CHANNEL);
		clutch = new Solenoid(CLUTCH_SOLENOID);
		
		winch = new Victor(WINCH_PWM);
		winch_switch = new DigitalInput(WINCH_SWITCH_DIO);
		
		pilot = new Gamepad(1);
		
		lcd = DriverStationLCD::GetInstance();
		
	}
	
	void DisabledPeriodic() {
		
	}
	
	void AutonomousInit() {
		compressor->Start();
	}
	
	void AutonomousPeriodic() {
		roller_motor->Set(0.5);
	}
	
	void TeleopInit() {
		compressor->Start();
	}
	
	void TeleopPeriodic() {
		//roll in
		if (pilot->GetNumberedButton(1) && line_break->Get()){
			roller_motor->Set(0.3);
		//roll out
		} else if (pilot->GetNumberedButton(2)){
			roller_motor->Set(-0.3);
		} else {
			roller_motor->Set(0.0);
		}
		
		float speed = -pilot->GetLeftY();
		float turn = -pilot->GetRightX();
		float max_delta_speed = 1.0 / (GetLoopsPerSec() * TIME_TO_MAX_SPEED);
		lcd->PrintfLine(DriverStationLCD::kUser_Line1, "d=%f", max_delta_speed);
		
		
		if (speed > previous_speed + max_delta_speed){
			speed = previous_speed + max_delta_speed;
		}
		if (speed < previous_speed - max_delta_speed){
			speed = previous_speed - max_delta_speed;
		}
		
		drivetrain->ArcadeDrive(speed, turn);

		previous_speed = speed;
		previous_turn = turn;
		
		//going down
		if(pilot->GetNumberedButton(8)){
			arm->Set(0.5);
		}
		//going up
		else if(pilot->GetNumberedButton(6) && top_arm_switch->Get()){
			arm->Set(-0.6);
		}
		else{
			arm->Set(0.0);
		}
		
		if(pilot->GetNumberedButton(5)){
			gear_shift->Set(LOW_GEAR);
		}
		else {
			gear_shift->Set(HIGH_GEAR);
		}
		
		if (pilot->GetNumberedButton(9)) {
			winch->Set(-0.7);
		}
		if (winch_switch->Get()){
			winch->Set(0.0);
		}
		
		if (pilot->GetNumberedButton(4)) {
			clutch->Set(CLUTCH_OUT);
			lcd->PrintfLine(DriverStationLCD::kUser_Line1,"firing");
		} else {
			clutch->Set(CLUTCH_IN);
			lcd->PrintfLine(DriverStationLCD::kUser_Line1, "not firing");
		}
		
		
		lcd->UpdateLCD();
	}
};

START_ROBOT_CLASS(AtlasPractice);
