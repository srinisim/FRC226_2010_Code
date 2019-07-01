#include "PositionControl.h"

PositionControl::PositionControl(SpeedController *motor, AnalogChannel *pot, float kp, float ki, float kd, int desired_min_position, int desired_max_position, int zero_position)
{
	m_motor = motor;
	m_min_pos = desired_min_position;
	m_max_pos = desired_max_position;
	m_zero_position = zero_position;
	m_pot = pot;
	m_p = kp;
	m_i = ki;
	m_d = kd;
	
	m_position = 512;
	
/*	m_pid = new CustomPID(m_p, m_i, m_d);
	m_pid -> Set_Continuous();
	m_pid -> Set_Output_Range(-1, 1);
	m_pid -> Set_Tolerance(0.02);
	m_pid -> Set_OnTargetTime(1); */
	
	m_pid = new PIDController(m_p, m_i, m_d, this, this);
	m_pid -> SetContinuous(true);
	m_pid -> SetInputRange(m_min_pos, m_max_pos);
	m_pid -> SetOutputRange(-1, 1);
	m_pid -> SetTolerance(0.05);
	m_pid -> Enable();
	
	m_running = true;
	
	m_position_start = m_position_current = m_position_destination = 0;
	m_position_is_set = false;
	
//	Set(m_zero_position);
}

PositionControl::~PositionControl()
{
	//
}

float PositionControl::Get(void)
{
	return m_pot->GetValue();

}

float PositionControl::GetRelativePosition(void)
{
	return ((m_pot->GetValue() - m_zero_position)+m_max_pos)%m_max_pos;
}

float PositionControl::GetMotorOutput(void)
{
	return m_motor->Get();
}

void PositionControl::Set(int position)
{
	if(!m_running)
		m_motor->Set(position/m_max_pos);
	else
	{
		m_pid->SetSetpoint(((position + m_zero_position)+m_max_pos)%m_max_pos);
	}
}

void PositionControl::SetPID(float p, float i, float d)
{
	m_pid->SetPID(p, i, d);
}

void PositionControl::SetCenter(int center)
{
	m_zero_position = center;
}

int PositionControl::GetCenter(void)
{
	return m_zero_position;
}

float PositionControl::GetTargetPosition()
{
	return m_pid->GetSetpoint();
}

float PositionControl::GetCurrentPosition()
{
	return PIDGet();
}

void PositionControl::Enable()
{
	m_running = true;
	m_pid->Enable();
}

void PositionControl::Disable()
{
	if(m_running)
		m_pid->Disable();
	m_running = false;
}

void PositionControl::PIDWrite(float input)
{
	m_motor->Set(input);
}

double PositionControl::PIDGet(void)
{
	return m_pot->GetValue();
}

