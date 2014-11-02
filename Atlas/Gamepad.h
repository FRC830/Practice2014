#ifndef GAMEPAD_H_
#define GAMEPAD_H_

//#include "GenericHID.h"
//#include "Base.h"
#include "Joystick.h"
#include <stdio.h>

class DriverStation;

/**
 * Handle input from Logitech Dual Action Gamepad connected to the Driver
 * Station.
 */
class Gamepad : public Joystick
{
private:
	bool buttons_pressed[12];
public:
	static const int LEFT_BUMPER = 5;
	static const int RIGHT_BUMPER = 6;
	static const int LEFT_TRIGGER = 7;
	static const int RIGHT_TRIGGER = 8;
	
	static const int F310_A = 1;
	static const int F310_B = 2;
	static const int F310_X = 3;
	static const int F310_Y = 4;
	static const int F310_LB = 5;
	static const int F310_RB = 6;
	static const int F310_L_STICK = 9;
	static const int F310_R_STICK = 10;
	
	static const int F310_LEFT_Y = 2;
	static const int F310_LEFT_X = 1;
	static const int F310_RIGHT_Y = 5;
	static const int F310_RIGHT_X = 4;
	static const int F310_TRIGGER_AXIS = 3; //left positive, right negative
	static const int F310_DPAD_X_AXIS = 6;
	static const int F310_DPAD_Y_AXIS = 2; //mode switches left y and dpad y
	
    typedef enum
    {
        kLeftXAxis, kLeftYAxis, kRightXAxis, kRightYAxis
    } AxisType;

    typedef enum
    {
        kCenter, kUp, kUpLeft, kLeft, kDownLeft, kDown, kDownRight, kRight,
        kUpRight
    } DPadDirection;

    Gamepad(UINT32 port);
    ~Gamepad();

    float GetLeftX();
    float GetLeftY();
    float GetRightX();
    float GetRightY();
    float GetAxis(AxisType axis);
    float GetRawAxis(UINT32 axis);

    bool GetNumberedButton(unsigned buttonNumber);
    bool GetNumberedButtonPressed(unsigned buttonNumber);
    bool GetNumberedButtonReleased(unsigned buttonNumber);
    bool GetLeftPush();
    bool GetRightPush();

    DPadDirection GetDPad();

protected:
    static const UINT32 kLeftXAxisNum = 1;
    static const UINT32 kLeftYAxisNum = 2;
    static const UINT32 kRightXAxisNum = 3;
    static const UINT32 kRightYAxisNum = 4;
    static const UINT32 kDPadXAxisNum = 5;
    static const UINT32 kDPadYAxisNum = 6;

    static const unsigned kLeftAnalogStickButton = 11;
    static const unsigned kRightAnalogStickButton = 12;

    DriverStation *ap_ds;
    UINT32 a_port;
};

#endif 
