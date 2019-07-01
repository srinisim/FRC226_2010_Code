#ifndef MANIP_H
#define MANIP_H


#include "WPILib.h"
#include "DashboardDataFormat.h"

class Manip
{
public:
	Manip(Joystick *manipStick, SpeedController *rotatorMotor, SpeedController *kickerMotor, DigitalInput *kickerSwitch);
	~Manip();
	
	void Teleop(void);
	void Auton(void);
	void Kick(void);
	void Rotate(void);

	
private:
	void Kicker(void);
	SpeedController *m_kickerMotor;
	DigitalInput *m_kickerSwitch;
	enum {pre_switch_shot, post_switch_shot, idle} kickerState;
	
	Joystick *m_manipStick;
	SpeedController *m_rotatorMotor;
};

#endif
