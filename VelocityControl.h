#ifndef VelocityControl_h_
#define VelocityControl_h_

#include "WPILib.h"
#include "CustomPID.h"

class VelocityControl : public PIDSource, public PIDOutput
{
	
public:
	VelocityControl(SpeedController *motor, Encoder *encoder, float kp, float ki, float kd, float max_speed, float scale_factor);
	virtual ~VelocityControl();
	
	void Set(float speed);
	float Get(void);
	
	void PIDWrite(float input);
	double PIDGet(void);
	
	float GetRelativeVelocity();
	
	void SetPID(float p, float i, float d);
	float GetTargetSpeed();
	float GetCurrentSpeed();
	float GetMotorOutput();
	
	void Enable();
	void Disable();
	
	PIDController *m_pid;
	float m_max_speed;
	float m_speed;
	
private:
	SpeedController *m_motor;
	Encoder *m_encoder;
	

	bool m_running; 
	float m_scale_factor;
	float m_p;
	float m_i;
	float m_d;
};

#endif
