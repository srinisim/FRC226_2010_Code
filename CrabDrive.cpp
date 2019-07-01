#include "CrabDrive.h"
#include "math.h"


#define M_PI	3.14159265358979323846

CrabDrive::CrabDrive(WheelModule *FrontLeft, WheelModule *FrontRight, WheelModule *RearLeft, WheelModule *RearRight, float max_radius)
{
	m_frontLeft = FrontLeft;
	m_frontRight = FrontRight;
	m_rearLeft = RearLeft;
	m_rearRight = RearRight;
	
	m_RotTankAngle = tan(m_frontLeft->m_offset_y / m_frontLeft->m_offset_x);
	
	m_max_radius = max_radius;
	m_stickDeadband = 0.08;
	m_limit = 0.95;
}

CrabDrive::~CrabDrive()
{
	//
}

void CrabDrive::Drive(Joystick *leftStick, Joystick *rightStick, float power_scalar)
{	
	
	if(rightStick->GetRawButton(3))
	{
		TankDriveWide(leftStick, rightStick);
	}
	else if(leftStick->GetRawButton(3))
	{
		TankDriveLong(leftStick, rightStick);
	}
	else if(GetJoystickRadius(leftStick) < m_stickDeadband && rightStick->GetX() >= -m_stickDeadband && rightStick->GetX() <= m_stickDeadband )
	{
		StrafeBot(0, M_PI/2, power_scalar);
	}
	else if(rightStick->GetX() >= -m_stickDeadband && rightStick->GetX() <= m_stickDeadband && GetJoystickRadius(leftStick) >= m_stickDeadband)
	{
		StrafeBot(GetJoystickRadius(leftStick), GetJoystickAngle(leftStick), power_scalar);
	}
	else
	{
		float radius = rightStick->GetX();

		if(radius > 0)
		{
			radius = 1 - radius;
			radius *= radius;
		}
		else
		{
			radius = (1 - fabs(radius))*-1;
			radius *= -radius;
		}

		radius *= m_max_radius;
		AckBot(leftStick->GetX(), leftStick->GetY(), radius, rightStick, power_scalar);

	}
}

void CrabDrive::StrafeBot(float strafe_power, float strafe_angle, float power_scalar)
{
	m_strafe_power = strafe_power;
	m_strafe_angle = (2*M_PI-strafe_angle);
	
	m_strafe_power = m_strafe_power > 1 ?  1 : m_strafe_power;
	
	strafe_power *= power_scalar;
	
	m_frontLeft->Set(strafe_power, strafe_angle*(180/M_PI));
	m_frontRight->Set(strafe_power, strafe_angle*(180/M_PI));
	m_rearLeft->Set(strafe_power, strafe_angle*(180/M_PI));
	m_rearRight->Set(strafe_power, strafe_angle*(180/M_PI));
}

void CrabDrive::AckBot(float x, float y, float radius, Joystick *rightStick, float power_scalar)
{
	if(radius == 0 && rightStick->GetX() < 0)
		radius = -0.001;
	
	float power = sqrt(x*x + y*y);

	power = power > m_limit ?  m_limit : power;
	
	float theta = GetJoystickAngle(x, y);
	
	power *= power_scalar;
	
	if(power_scalar < 0)
	{
		radius *= -1;
		if(radius == 0 && rightStick->GetX() > 0)
			radius = -0.001;
	}
	
	AckBot(power, theta, radius);
}

void CrabDrive::AckBot(float power, float theta, float radius)
{
    float frontLeftRadius = GetWheelRadius(m_frontLeft, theta, radius);
	float frontRightRadius = GetWheelRadius(m_frontRight, theta, radius);
	float rearLeftRadius = GetWheelRadius(m_rearLeft, theta, radius);
	float rearRightRadius = GetWheelRadius(m_rearRight, theta, radius);
	
	float frontLeftAngle = GetWheelAngle(m_frontLeft, theta, radius);
	float frontRightAngle = GetWheelAngle(m_frontRight, theta, radius);
	float rearLeftAngle = GetWheelAngle(m_rearLeft, theta, radius);
	float rearRightAngle = GetWheelAngle(m_rearRight, theta, radius);
	
	float largestRadius = fabs(GetLargestRadius(frontLeftRadius, frontRightRadius, rearLeftRadius, rearRightRadius));
	
	float frontLeftPower = (power*frontLeftRadius)/largestRadius;
	float frontRightPower = (power*frontRightRadius)/largestRadius;
	float rearLeftPower = (power*rearLeftRadius)/largestRadius;
	float rearRightPower = (power*rearRightRadius)/largestRadius;
	
    if(radius < 0)
    {
		frontLeftAngle -= M_PI;
		frontRightAngle -= M_PI;
		rearLeftAngle -= M_PI;
		rearRightAngle -= M_PI;
    }      

	m_frontLeft->Set(frontLeftPower, frontLeftAngle*(180/M_PI));
	m_frontRight->Set(frontRightPower, frontRightAngle*(180/M_PI));
	m_rearLeft->Set(rearLeftPower, rearLeftAngle*(180/M_PI));
	m_rearRight->Set(rearRightPower, rearRightAngle*(180/M_PI));
	//m_frontLeft->Set(-frontLeftPower, fmod(frontLeftAngle*(180/M_PI)+180,360));
	//m_frontRight->Set(-frontRightPower, fmod(frontRightAngle*(180/M_PI)+180,360));
	//m_rearLeft->Set(-rearLeftPower, fmod(rearLeftAngle*(180/M_PI)+180,360));
	//m_rearRight->Set(-rearRightPower, fmod(rearRightAngle*(180/M_PI)+180,360));

}

float CrabDrive::GetWheelAngle(WheelModule *module, float theta, float radius)
{
	float cos_value = -cos(theta) * radius - (module->m_offset_y);
	float sin_value = sin(theta) * radius - (module->m_offset_x);
	
	if(cos_value == 0 && sin_value == 0)
		return theta;
	
	float tan_value =  M_PI/2 + atan(cos_value/sin_value); //This is INTENTIONALLY reversed (cos/sin)
	
	if(sin_value < 0)
	{
		tan_value += M_PI;
	}
		
	return tan_value;
}

float CrabDrive::GetWheelRadius(WheelModule *module, float theta, float radius)
{
    float radius_angle;
    
    radius_angle = theta - M_PI/2;  
        
    float radius_x = radius*cos(radius_angle);
    float radius_y = radius*sin(radius_angle);
    
    float wheelRadius_x = radius_x - module->m_offset_x;;
    float wheelRadius_y = radius_y - module->m_offset_y;
    
    return sqrt(wheelRadius_x*wheelRadius_x + wheelRadius_y*wheelRadius_y);
}


float CrabDrive::GetJoystickRadius(Joystick *stick)
{
	return GetJoystickRadius(stick->GetX(), stick->GetY());
}

float CrabDrive::GetJoystickRadius(float x, float y)
{
	return sqrt(x*x + y*y)*sqrt(x*x + y*y);
}

float CrabDrive::GetJoystickAngle(Joystick *stick)
{
	return GetJoystickAngle(stick->GetX(), stick->GetY());
}

float CrabDrive::GetJoystickAngle(float x, float y)
{
	float hyp = sqrt(x*x + y*y); 
	
	float angle = acos(x/hyp);
	
	if(y < 0)
	{
		angle = (2*M_PI)-angle;
	}
	
	return (2*M_PI-angle);
}

void CrabDrive::TankDriveWide(Joystick *leftStick, Joystick *rightStick)
{
	TankDriveWide(leftStick->GetY(), rightStick->GetY());
}

void CrabDrive::TankDriveWide(float leftDrivePower, float rightDrivePower)
{
	m_frontLeft->Set(-leftDrivePower, 90);
	m_rearLeft->Set(-leftDrivePower, 90);
	
	m_frontRight->Set(-rightDrivePower, 90);
	m_rearRight->Set(-rightDrivePower, 90);
}

void CrabDrive::TankDriveLong(Joystick *leftStick, Joystick *rightStick)
{
	TankDriveLong(leftStick->GetY(), rightStick->GetY());
}

void CrabDrive::TankDriveLong(float rearDrivePower, float frontDrivePower)
{
	m_frontLeft->Set(frontDrivePower, 0);
	m_frontRight->Set(frontDrivePower, 0);
	
	m_rearLeft->Set(-rearDrivePower, 180);
	m_rearRight->Set(-rearDrivePower, 180);
}

/*
void CrabDrive::RotationTankDrive(Joystick *leftStick, Joystick *rightStick)
{
	float frontLeftAngle = m_frontLeft->GetCurrentAngle();
	float frontRightAngle = m_frontRight->GetCurrentAngle();
	float rearLeftAngle = m_rearLeft->GetCurrentAngle();
	float rearRightAngle = m_rearRight->GetCurrentAngle();
	
	float frontLeftVirtualAngle = frontLeftAngle + m_RotTankAngle;
	float frontRightVirtualAngle = frontRightAngle + (360 - m_RotTankAngle);
	float rearLeftVirtualAngle = rearLeftAngle + (360 - m_RotTankAngle);
	float rearRightVirtualAngle = rearRightAngle + m_RotTankAngle;
	
	float raw_power = GetJoystickRadius(leftStick);
	float difference = -rightStick->GetX();
	
	float frontLeftOut = raw_power - (difference*cos(frontLeftVirtualAngle));
	float frontRightOut = raw_power + (difference*cos(frontRightVirtualAngle));
	float rearLeftOut = raw_power - (difference*cos(rearLeftVirtualAngle));
	float rearRightOut = raw_power + (difference*cos(rearRightVirtualAngle));
	
	//Reverse power if angles cross over virtual line
	if(frontLeftAngle < (360 - m_RotTankAngle) && frontLeftAngle > (180 - m_RotTankAngle))
		frontLeftOut *= -1;
	
	if(frontRightAngle < (180 + m_RotTankAngle) && frontRightAngle > m_RotTankAngle)
		frontRightOut *= -1;
	
	if(rearLeftAngle < (180 + m_RotTankAngle) && rearLeftAngle > m_RotTankAngle)
		rearLeftOut *= -1;
	
	if(rearRightAngle < (360 - m_RotTankAngle) && rearRightAngle > (180 - m_RotTankAngle))
		rearRightOut *= -1;
		
	m_frontLeft->Set(frontLeftOut, GetJoystickAngle(leftStick));
	m_frontRight->Set(frontRightOut, GetJoystickAngle(leftStick));
	m_rearLeft->Set(rearLeftOut, GetJoystickAngle(leftStick));
	m_rearRight->Set(rearRightOut, GetJoystickAngle(leftStick));
	
}
*/

float CrabDrive::GetLargestRadius(float fl_radius, float fr_radius, float rl_radius, float rr_radius)
{
	return GetLargestRadius(GetLargestRadius(fl_radius, fr_radius), GetLargestRadius(rl_radius, rr_radius));
}

float CrabDrive::GetLargestRadius(float r1, float r2)
{
	return fabs(r1) > fabs(r2) ? r1 : r2;
}

void CrabDrive::SwapArgs(float &a, float &b)
{
	float temp_a = a;
	
	a = b;
	b = temp_a;
}

float CrabDrive::GetStrafePower()
{
	return m_strafe_power;
}

float CrabDrive::GetStrafeAngle()
{
	return m_strafe_angle;
}

float CrabDrive::GetFLSpeed()
{
	return m_frontLeft->GetSpeed();
}

float CrabDrive::GetFLAngle()
{
	return m_frontLeft->GetCurrentAngle();
}

float CrabDrive::GetFRSpeed()
{
	return m_frontRight->GetSpeed();
}

float CrabDrive::GetFRAngle()
{
	return m_frontRight->GetCurrentAngle();
}

float CrabDrive::GetRLSpeed()
{
	return m_rearLeft->GetSpeed();
}

float CrabDrive::GetRLAngle()
{
	return m_rearLeft->GetCurrentAngle();
}

float CrabDrive::GetRRSpeed()
{
	return m_rearRight->GetSpeed();
}

float CrabDrive::GetRRAngle()
{
	return m_rearRight->GetCurrentAngle();
}
