#ifndef DAI_H_
#define DAI_H_

#include "WPILib.h"
#include "AIAngle.h"
#include "AIDistance.h"
#include "CrabDrive.h"

class DaemonAI
{
public:
	DaemonAI(CrabDrive *drive, AIAngle *aiangle, AIDistance *frontLeftAID, AIDistance *frontRightAID, AIDistance *rearLeftAID, AIDistance *rearRightAID, float max_radius);
	virtual ~DaemonAI();
	
	void StrafeDaemon(float distance, float wheel_angle, bool maintain_heading = true);
	
	void ResetEncoders();
	void Hold();
	void DisablePID();
	void EnablePID();
	
	void KickBall();
	
	void TrackTarget();
	
	// Yes, we are having too much fun with this
	typedef enum {M16, M4, Tommy, M60, SPAS12, M40} WeaponsLocker; 
	void AntiGuerillaWarfare(WeaponsLocker weapon);
	
private:	
	Gyro *m_gyro;
	CrabDrive *m_drive;

	AIDistance *m_frontLeftAID;
	AIDistance *m_frontRightAID;
	AIDistance *m_rearLeftAID;
	AIDistance *m_rearRightAID;
	
	AIAngle *m_AIAngle;
	
	float m_max_radius;
	
	float m_AIAngleDeadband;
};

#endif
