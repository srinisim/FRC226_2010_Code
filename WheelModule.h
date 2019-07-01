#ifndef WHEELMODULE_H_
#define WHEELMODULE_H_


#include "WPILib.h"
#include "CustomPID.h"
#include "PositionControl.h"
#include "VelocityControl.h"
#include "Math.h"

class WheelModule
{

public:
	WheelModule(VelocityControl *power, PositionControl *steering, float offset_x, float offset_y, Joystick *leftStick);
	~WheelModule();
	
	void Set(float speed, float angle);
	void SetSpeed(float speed);
	void SetAngle(float angle);
	
	float GetSpeed(void);
	float GetTargetAngle(void);
	float GetCurrentAngle(void);
	
	float GetPowerMotorOutput(void);
	float GetSteerMotorOutput(void);
	
	float m_offset_x;
	float m_offset_y;
	
private:
	VelocityControl *m_powerMotor;
	PositionControl *m_steeringMotor;
	Joystick *m_leftStick;
	
	float m_AnalogToAnglek;
	float m_angle;
	float m_speed;
	
	float m_limit;
	
	bool m_reverse_speed;
};

#endif
