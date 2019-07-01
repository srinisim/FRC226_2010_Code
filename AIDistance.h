#ifndef AIDISTANCE_H_
#define AIDISTANCE_H_

#include "WPILib.h"

class AIDistance : public Encoder, public PIDSource, public PIDOutput
{
public:	
	AIDistance(UINT32 aSlot, UINT32 aChannel, UINT32 bSlot, UINT32 _bChannel, bool reverseDirection=false, EncodingType encodingType = k4X);
	~AIDistance();
	
	void PIDWrite(float input);
	double PIDGet(void);
	
	void SetDistance(float distance);
	float GetDistanceError(void);
	
	PIDController *m_pid;
	
	
private:

	float m_distance_error;
	float m_strafe_angle;

};

#endif
