#ifndef CUSTOMPID_H_
#define CUSTOMPID_H_

#include "Timer.h"

class CustomPID 
{
public:
	
	CustomPID(float kp, float ki, float kd);
	~CustomPID();
	
	void Set_Constants(float kp, float ki, float kd);
	
	void Set_Continuous();
	void Set_NonContinuous(float lowerlimit, float upperlimit);
	void Set_Output_Range(float minimum, float maximum);
	void Set_Tolerance(float input);
	
	float Get_Error();
	void Reset_Total_Error();
	
	float Calculate(float start, float current, float destination); //Start isn't necessary
																	//Used only for OnTarget range
																	//Set to 0 if used for speed applications
	bool OnTarget();		
	bool OnTarget_OverTime(bool onTarget); 		//Call only once. Do not use replacement variables.
	void Set_OnTargetTime(float seconds);
//	void Set_CriticalPoints(float start, float current, float destination);

	void ResetTimer();
	
	float m_result;
	float m_setPoint;
	float m_error;
	
	float m_start; 			//Used for OnTarget()
	float m_current;
	float m_destination;
	
	//Extra variables used to monitor data if necessary
	float m_extraFloat_variable_1; 
	float m_extraFloat_variable_2;
	float m_extraFloat_variable_3;
	float m_extraFloat_variable_4;
	float m_extraFloat_variable_5; 
	
	int m_extraInt_variable_1;
	int m_extraInt_variable_2;
	int m_extraInt_variable_3;
	int m_extraInt_variable_4;
	int m_extraInt_variable_5;
		 
	
private:
	
	Timer *m_timer;
	
	float m_P;
	float m_I;
	float m_D;

	float m_upperInput;
	float m_lowerInput;
	float m_upperOutput;
	float m_lowerOutput;
	
	bool m_continuous;

	float m_onTargetMaxTime;

	float m_prevError;
	float m_totalError;
	
	float m_tolerance;

};

#endif 
