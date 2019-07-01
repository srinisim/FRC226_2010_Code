#include "WPILib.h"
#include "CustomPID.h"
#include "Timer.h"
#include <math.h>

CustomPID::CustomPID(float kp, float ki, float kd)
{
	m_P = kp;
	m_I = ki;
	m_D = kd;
	
	m_timer = new Timer();
	
	m_current = 0;
	m_destination = 0;
	
	m_upperOutput = 1;
	m_lowerOutput = -1;
	
	m_upperInput = 0;
	m_lowerInput = 0;
	
	m_continuous = false;

	m_prevError = 0;
	m_totalError = 0;
	m_tolerance = .05;
}

void CustomPID::Set_Constants(float kp, float ki, float kd)
{
	m_P = kp;
	m_I = ki;
	m_D = kd;
}

void CustomPID::Set_Continuous()
{
	m_continuous = true;
}

void CustomPID::Set_NonContinuous(float lowerlimit, float upperlimit)
{
	m_continuous = false;
	m_lowerInput = lowerlimit;
	m_upperInput = upperlimit;
	
}

void CustomPID::Set_Output_Range(float minimum, float maximum)
{
	m_lowerOutput = minimum;
	m_upperOutput = maximum;
}

/*
void Set_CriticalPoints(float start, float current, float destination)
{
	m_destination = destination;
	m_current = current;
	m_start = start;
}
*/

float CustomPID::Calculate(float start, float current, float destination)
{
	m_destination = destination;
	m_current = current;
	m_start = start;
	
	m_error = m_destination - m_current;
	
	if (m_continuous)
	{
		if (fabs(m_error) > ((m_upperInput - m_lowerInput)/2) )
		{
			if (m_error > 0)
				m_error = m_error - m_upperInput + m_lowerInput;
			else
				m_error = m_error + m_upperInput - m_lowerInput;
		}
	}

	if (((m_totalError + m_error) * m_I < m_upperOutput) &&
			((m_totalError + m_error) * m_I > m_lowerOutput))
		m_totalError += m_error;

			
	m_result = m_P * m_error + m_I * m_totalError + m_D * (m_error - m_prevError);
	m_prevError = m_error;
	
	if (m_result > m_upperOutput)
		m_result = m_upperOutput;
	else if (m_result < m_lowerOutput)
		m_result = m_lowerOutput;
	
	return m_result;
}

void CustomPID::Set_Tolerance(float input)
{
	m_tolerance = input;
}

bool CustomPID::OnTarget() 
{
	bool temp = false;
	
	if(m_upperInput>0 && m_lowerInput>0) 		// <--- Untested: Use with pot.
		temp = (fabs(m_error)<m_tolerance / 100 * (m_upperInput - m_lowerInput));
	else										// <--- Tested: Successful.
	{
		if( (fabs(m_destination-m_start))*m_tolerance  > fabs(m_error) )
			temp = true;
		else
			temp = false;
	}
	
	return temp;
}

void CustomPID::Set_OnTargetTime(float seconds)
{
	m_onTargetMaxTime = seconds;
}

void CustomPID::ResetTimer()
{
	m_timer->Reset();
}

bool CustomPID::OnTarget_OverTime(bool onTarget)
{	
	if(onTarget)
	{
		m_timer->Start();
		//m_onTargetCurrentCount++;
	}
	else
	{
		m_timer->Stop();
		m_timer->Reset();
		//m_onTargetCurrentCount = 0;
	}
		
	
	if(m_timer->Get() >= m_onTargetMaxTime)
	{ 
		return true;
	}
	else
		return false;
}		



float CustomPID::Get_Error()
{
	return (m_destination-m_current);
}

void CustomPID::Reset_Total_Error()
{
	m_totalError = 0;
}

