#include "WPILib.h"
#include "Manip.h"


Manip::Manip(Joystick *manipStick, SpeedController *rotatorMotor, SpeedController *kickerMotor, DigitalInput *kickerSwitch)
{
	m_kickerMotor = kickerMotor;
	m_kickerSwitch = kickerSwitch;
	
	m_manipStick = manipStick;
	m_rotatorMotor = rotatorMotor;
	
	kickerState = pre_switch_shot;

}

Manip::~Manip()
{
	//
}

void Manip::Auton()
{
	// TODO: do anything during auton?
	// camera, shoot etc
	Kicker();
}

void Manip::Teleop()
{
	//Kicker();
	Rotate();
}

void Manip::Kick(void)
{
	kickerState = pre_switch_shot;
}

void Manip::Kicker(void)
{
	switch(kickerState)
	{
		case pre_switch_shot:
			m_kickerMotor->Set(-1);
			if(!m_kickerSwitch->Get())
			{
				kickerState = post_switch_shot;
			}
			break;
		case post_switch_shot:
			m_kickerMotor->Set(-1);
			if(m_kickerSwitch->Get())
			{
				kickerState = idle;
			}
			break;
		case idle:
			m_kickerMotor->Set(0);
			break;
		default:
			kickerState = idle;
	}	
}

void Manip::Rotate(void)
{
	if(m_manipStick->GetRawButton(11))
		m_rotatorMotor->Set(1);
	else if(m_manipStick->GetRawButton(10))
		m_rotatorMotor->Set(-1);
	else
		m_rotatorMotor->Set(0);
	
	//m_rotatorMotor->Set(m_manipStick->GetY());
}
