#ifndef AIANGLE_H_
#define AIANGLE_H_

#include "WPILib.h"


class AIAngle : public Gyro, public PIDOutput
{
public:
	AIAngle(int gyro_channel);
	virtual ~AIAngle();
	
	void PIDWrite(float input);
	
	void SetAngle(float angle);
	float GetAngleError(void);
	
	PIDController *m_pid;
	
private:
	Gyro *m_gyro;
	
	float m_angle_error;
};


#endif
