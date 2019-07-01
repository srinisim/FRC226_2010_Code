#include "VelocityControl.h"

VelocityControl::VelocityControl(SpeedController *motor, Encoder *encoder, float kp, float ki, float kd, float max_speed, float scale_factor)
{
	m_motor = motor;
	m_max_speed = max_speed;
	m_encoder = encoder;
	m_p = kp;
	m_i = ki;
	m_d = kd;
	
	m_speed = 0;
	m_scale_factor = scale_factor;
	
	m_pid = new PIDController(kp, ki, kd, this, this);
	m_pid -> SetContinuous(false);
	m_pid -> SetInputRange(-m_max_speed, m_max_speed);
	m_pid -> SetOutputRange(-1, 1);
	m_pid -> SetTolerance(0.02);
	m_pid -> Disable();
	
	m_encoder->SetDistancePerPulse(m_scale_factor);
	
	m_running = false;
}

VelocityControl::~VelocityControl()
{
	//
}

float VelocityControl::Get(void)
{
	return m_motor->Get();
}

float VelocityControl::GetMotorOutput(void)
{
	return m_motor->Get();
}

void VelocityControl::Set(float speed)
{
//	speed*=4;
	if(!m_running)
		m_motor->Set(speed);
	else
	{
		m_pid->SetSetpoint(speed * m_max_speed);
	}
	
}

void VelocityControl::SetPID(float p, float i, float d)
{
	m_pid->SetPID(p, i, d);
}

float VelocityControl::GetTargetSpeed()
{
	return m_pid->GetSetpoint();
}

float VelocityControl::GetCurrentSpeed()
{
	return (m_encoder->GetRate() * m_scale_factor);
}

void VelocityControl::Enable()
{
	m_running = true;
	m_speed = m_motor->Get();
	m_pid->Enable();
}

void VelocityControl::Disable()
{
	if(m_running)
		m_pid->Disable();
	m_running = false;
	m_speed = 0;
}

void VelocityControl::PIDWrite(float input)
{
	m_speed += input;
	if(m_speed > 1)
		m_speed = 1;
	if(m_speed < -1)
		m_speed = -1;
	m_motor->Set(m_speed);
}

double VelocityControl::PIDGet(void)
{
	return -GetCurrentSpeed();
}
