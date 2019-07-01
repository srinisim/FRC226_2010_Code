#include "AIDistance.h"

AIDistance::AIDistance(UINT32 aSlot, UINT32 aChannel, UINT32 bSlot, UINT32 _bChannel, bool reverseDirection, EncodingType encodingType) 
: Encoder(aSlot, aChannel, bSlot, _bChannel, reverseDirection, encodingType)
{
	m_strafe_angle = 90;
	
	m_pid = new PIDController(0.04703, 0, 0.0068, this, this); //0.04703, 0, 0.0068,
	m_pid -> SetContinuous(false);
	m_pid -> SetOutputRange(-.35, .35);
	m_pid -> SetTolerance(0.02);
	m_pid -> Enable();
}

AIDistance::~AIDistance()
{
	//
}

double AIDistance::PIDGet()
{
	return -GetDistance();
}

void AIDistance::PIDWrite(float input)
{
	m_distance_error = input;
}

void AIDistance::SetDistance(float distance)
{
	m_pid->SetSetpoint(distance);
}

float AIDistance::GetDistanceError(void)
{
	return m_distance_error;
}

