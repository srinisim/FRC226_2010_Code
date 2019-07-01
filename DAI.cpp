#include "DAI.h"
#include "AIDistance.h"
#include "AIAngle.h"

DaemonAI::DaemonAI(CrabDrive *drive, AIAngle *aiangle, AIDistance *frontLeftAID, AIDistance *frontRightAID, AIDistance *rearLeftAID, AIDistance *rearRightAID, float max_radius)
{	
	m_drive = drive;
	
	m_AIAngle = aiangle;
	m_frontLeftAID = frontLeftAID;
	m_frontRightAID = frontRightAID;
	m_rearLeftAID = rearLeftAID;
	m_rearRightAID = rearRightAID;
	
	m_AIAngleDeadband = 0.0001;
	m_max_radius = max_radius;
}

DaemonAI::~DaemonAI()
{
	//
}

void DaemonAI::StrafeDaemon(float distance, float strafe_angle, bool maintain_heading)
{
	if(maintain_heading)
	{
		m_AIAngle->SetAngle(0);
	}
	
	m_frontLeftAID->SetDistance(distance);
	m_frontRightAID->SetDistance(distance);
	m_rearLeftAID->SetDistance(distance);
	m_rearRightAID->SetDistance(distance);
	
	float averageDist = (m_frontLeftAID->GetDistanceError() + m_frontRightAID->GetDistanceError() 
						+ m_rearLeftAID->GetDistanceError() + m_rearRightAID->GetDistanceError())/4;
	
	if(m_AIAngle->GetAngleError() == 0)
	{
		m_drive->StrafeBot(averageDist, strafe_angle, 1);

	}
	else
	{
		if(m_AIAngle->GetAngleError() > 0)
			m_drive->AckBot(averageDist, strafe_angle, m_max_radius - m_AIAngle->GetAngleError());
		else
			m_drive->AckBot(averageDist, strafe_angle, -m_max_radius + m_AIAngle->GetAngleError());
	}
}

void DaemonAI::ResetEncoders()
{
	m_frontLeftAID->Reset();
	m_frontRightAID->Reset();
	m_rearLeftAID->Reset();
	m_rearRightAID->Reset();
}

void DaemonAI::DisablePID()
{
	m_frontLeftAID->m_pid->Disable();
	m_frontRightAID->m_pid->Disable();
	m_rearLeftAID->m_pid->Disable();
	m_rearRightAID->m_pid->Disable();
}

void DaemonAI::EnablePID()
{
	m_frontLeftAID->m_pid->Enable();
	m_frontRightAID->m_pid->Enable();
	m_rearLeftAID->m_pid->Enable();
	m_rearRightAID->m_pid->Enable();
}

void DaemonAI::Hold()
{
	ResetEncoders();
	StrafeDaemon(0, 1.57, true);
}

void DaemonAI::AntiGuerillaWarfare(WeaponsLocker weapon)
{
	StrafeDaemon(75, 0, true);
}
