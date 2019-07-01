
#include "WheelModule.h"
#include "CustomPID.h"

#define M_PI	3.14159265358979323846

WheelModule::WheelModule(VelocityControl *power, PositionControl *steering, float offset_x, float offset_y, Joystick *leftStick)
{
	m_powerMotor = power;
	m_steeringMotor = steering;
	m_AnalogToAnglek = 2.75;
	m_angle = m_speed = 0;
	m_limit = 0.95;
	m_reverse_speed = false;
	m_leftStick = leftStick;
	
	m_offset_x = offset_x;
	m_offset_y = offset_y;
}

WheelModule::~WheelModule()
{
	//
}

void WheelModule::Set(float speed, float angle)
{
	SetAngle(angle);
	SetSpeed(speed);
}

void WheelModule::SetSpeed(float speed)
{
	m_speed = speed;
	
	if(m_speed > m_limit)
		m_speed = m_limit;
	else if(m_speed < -m_limit) 
		m_speed = -m_limit;
	
	if(m_reverse_speed)
		m_speed *= -1;
	
	m_powerMotor->Set(m_speed);	
}

void WheelModule::SetAngle(float angle)
{
	m_angle = (int)(360-angle);
	
	if(m_leftStick->GetRawButton(2))
	{
		if(fabs(GetTargetAngle() - GetCurrentAngle()) > 120)
		{
			m_angle = fmod((m_angle+180),360);
			m_reverse_speed = true;
		}
		else
		{
			m_reverse_speed = false;
		}
			
	}	
	else
		m_reverse_speed = false;
	
	m_steeringMotor->Set(m_angle*m_AnalogToAnglek);
}

float WheelModule::GetTargetAngle()
{
	return (360-m_angle);
}

float WheelModule::GetSpeed()
{
	return m_speed;
}

float WheelModule::GetCurrentAngle()
{
	return (360 - (m_steeringMotor->GetRelativePosition() / m_AnalogToAnglek));
}

float WheelModule::GetPowerMotorOutput(void)
{
	return m_powerMotor->Get();
}

float WheelModule::GetSteerMotorOutput(void)
{
	return m_steeringMotor->Get();
}
