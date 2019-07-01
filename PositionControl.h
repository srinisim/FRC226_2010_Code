#ifndef POSITIONCONTROL_H_
#define POSITIONCONTROL_H_

#include "WPILib.h"
#include "CustomPID.h"

class PositionControl : public PIDSource, public PIDOutput
{
	
public:
	PositionControl(SpeedController *motor, AnalogChannel *pot, float kp, float ki, float kd, int desired_min_position, int desired_max_position, int start_position);
	virtual ~PositionControl();
	
	void Set(int position);
	float Get(void);
	
	void PIDWrite(float input);
	double PIDGet(void);
	
	void SetPID(float p, float i, float d);
	float GetTargetPosition();
	float GetCurrentPosition();
	float GetRelativePosition();
	float GetMotorOutput();
	
	void SetCenter(int center);
	int GetCenter();
	
	void Enable();
	void Disable();
	
	PIDController *m_pid;
private:
	SpeedController *m_motor;
	AnalogChannel *m_pot;


	float m_position;
	bool m_running; 

	float m_p;
	float m_i;
	float m_d;
	int m_min_pos;
	int m_max_pos;
	int m_zero_position;
	
	bool m_position_is_set;
	
	int m_position_start;
	int m_position_current;
	int m_position_destination;
};

#endif
