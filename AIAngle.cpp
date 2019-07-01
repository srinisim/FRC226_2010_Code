#include "AIAngle.h"

AIAngle::AIAngle(int gyro_channel): Gyro(gyro_channel)
{
	m_angle_error = 0;
	
	m_pid = new PIDController(0.0666, 0, 0, this, this);
	m_pid -> SetContinuous(false);
	m_pid -> SetOutputRange(-1, 1);
	m_pid -> SetTolerance(0.02);
	m_pid -> Enable();
}

AIAngle::~AIAngle()
{
	//
}

void AIAngle::PIDWrite(float input)
{
	m_angle_error = input;
}

void AIAngle::SetAngle(float angle)
{
	m_pid->SetSetpoint(angle);
}

float AIAngle::GetAngleError(void)
{
	return m_angle_error;
}
