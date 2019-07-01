#ifndef CRABDRIVE_H_
#define CRABDRIVE_H_

#include "WPILib.h"
#include "CustomPID.h"
#include "WheelModule.h"
#include "math.h"

class CrabDrive
{
public:
	CrabDrive(WheelModule *FrontLeft, WheelModule *FrontRight, WheelModule *RearLeft, WheelModule *RearRight, float max_radius);
	~CrabDrive();
	
	void Drive(Joystick *leftStick, Joystick *rightStick, float power_scalar);
	
	void StrafeBot(float strafe_power, float strafe_angle, float power_scalar);
	void AckBot(float power, float theta, float radius);
	void AckBot(float x, float y, float radius, Joystick *rightStick, float power_scalar);
	
	void TankDriveWide(Joystick *leftStick, Joystick *rightStick);
	void TankDriveWide(float leftDrivePower, float RightDrivePower);
	
	void TankDriveLong(Joystick *leftStick, Joystick *rightStick);
	void TankDriveLong(float RearDrivePower, float FrontDrivePower);
	
//	void RotationTankDrive(Joystick *leftStick, Joystick *rightStick);
	
	// Small functions
	float GetJoystickRadius(Joystick *stick);
	float GetJoystickRadius(float x, float y);
	float GetJoystickAngle(Joystick *stick);
	float GetJoystickAngle(float x, float y);
	void SwapArgs(float &a, float &b);
	
	
	float GetWheelAngle(WheelModule *module, float theta, float radius);
	float GetWheelRadius(WheelModule *module, float theta, float radius);
	
	//Debugging functions
	float GetStrafePower(void);
	float GetStrafeAngle(void);
	
	float GetFLSpeed();
	float GetFLAngle();
	
	float GetFRSpeed();
	float GetFRAngle();
	
	float GetRLSpeed();
	float GetRLAngle();
	
	float GetRRSpeed();
	float GetRRAngle();
	
private:
	WheelModule *m_frontLeft;
	WheelModule *m_frontRight;
	WheelModule *m_rearLeft;
	WheelModule *m_rearRight;

	float GetLargestRadius(float fl_radius, float fr_radius, float rl_radius, float rr_radius);
	float GetLargestRadius(float r1, float r2);
	
	float m_RotTankAngle;
	
	float m_max_radius;
	float m_stickDeadband;
	
	float m_strafe_power;
	float m_strafe_angle;
	float m_limit;
	
};


#endif 
