#include "WPILib.h"
#include "DashboardDataFormat.h"
#include "VelocityControl.h"
#include "PositionControl.h"
#include "Target.h"
#include "Math.h"
#include "WheelModule.h"
#include "CrabDrive.h"
#include "Manip.h"
#include "AIDistance.h"
#include "AIAngle.h"
#include "DAI.h"

#define M_PI	3.14159265358979323846
/*
 * Notes:
 * Slot 1 - Analog module
 * Slot 2 - Analog module (not used)
 * Slot 3 - 
 * Slot 4 - Left digital sidecar
 * Slot 5 - 
 * Slot 6 - Right digital sidecar
 * Slot 7 - 
 * Slot 8 - 
 */  
class Robot226 : public IterativeRobot {

	Jaguar *frontLeftPowerJag;
	Jaguar *frontRightPowerJag;
	Jaguar *rearLeftPowerJag;
	Jaguar *rearRightPowerJag;

	Jaguar *frontLeftSteerJag;
	Jaguar *frontRightSteerJag;
	Jaguar *rearLeftSteerJag;
	Jaguar *rearRightSteerJag;
	
	Victor *kickerVic;
	
	Victor *flapperVic;
	
	Victor *rollerVic;
	
	
	Victor *rotatorVic;
	
	DriverStation *m_ds; // driver station object
	UINT32 m_priorPacketNumber; // keep track of the most recent packet number from the DS
	UINT8 m_dsPacketsReceivedInCurrentSecond; // keep track of the ds packets received in the current second

	Joystick *m_rightStick; // joystick 1 (arcade stick or right tank stick)
	Joystick *m_leftStick; // joystick 2 (tank left stick)
	
	Joystick *m_manipStick;

	/*****************Sensors****************/
	AIAngle *gyro;
	ADXL345_I2C *accel;
//	Accelerometer *accel;

	AIDistance *frontLeftEncoder;
	AIDistance *frontRightEncoder;
	AIDistance *rearLeftEncoder;
	AIDistance *rearRightEncoder;
	
//	Encoder *extenderEncoder;
//	Servo *pallServo;

	AnalogChannel *frontLeftPot;
	AnalogChannel *frontRightPot;
	AnalogChannel *rearLeftPot;
	AnalogChannel *rearRightPot;
	Relay *light;
	
	/****************************************/


	/******Sensor-Assisted Implementations***/

	VelocityControl *frontLeftVC;
	VelocityControl *frontRightVC;
	VelocityControl *rearLeftVC;
	VelocityControl *rearRightVC;

	PositionControl *frontLeftPC;
	PositionControl *frontRightPC;
	PositionControl *rearLeftPC;
	PositionControl *rearRightPC;
	
//	PositionControl *extenderPC;
	
	WheelModule *frontLeftWM;
	WheelModule *frontRightWM;
	WheelModule *rearLeftWM;
	WheelModule *rearRightWM;
	
	CrabDrive *crabDrive;
	
	DaemonAI *demonAI;
	DigitalInput *kickerSwitch;
	Manip *manip;
	
//	Camera *cam;

//	AxisCamera *camera;
//	Target nullTarget;

	/****************************************/

	// Local variables to count the number of periodic loops performed
	UINT32 m_autoPeriodicLoops;
	UINT32 m_disabledPeriodicLoops;
	UINT32 m_telePeriodicLoops;

	DashboardDataFormat *dashOut;
	
	float m_max_radius;
	int autonMode;

public:
	Robot226(void) {

		printf("BuiltinDefaultCode Constructor Started\n");

		/***** General init *****/
		dashOut = new DashboardDataFormat();
		
		// Initialize counters to record the number of loops completed in autonomous and teleop modes
		m_autoPeriodicLoops = 0;
		m_disabledPeriodicLoops = 0;
		m_telePeriodicLoops = 0;
		m_max_radius = 300;
		
		// Acquire the Driver Station object
		m_ds = DriverStation::GetInstance();
		m_priorPacketNumber = 0;
		m_dsPacketsReceivedInCurrentSecond = 0;
		
		/***** Drive init *****/
		frontLeftPowerJag = new Jaguar(4, 1); // TODO: Change to actual values
		frontRightPowerJag = new Jaguar(6, 1);
		rearLeftPowerJag = new Jaguar(4, 3);
		rearRightPowerJag = new Jaguar(6, 3);

		frontLeftSteerJag = new Jaguar(4, 2);
		frontRightSteerJag = new Jaguar(6, 2);
		rearLeftSteerJag = new Jaguar(4, 4);
		rearRightSteerJag = new Jaguar(6, 4);

		// Define joysticks being used at USB port #1 and USB port #2 on the Drivers Station
		m_rightStick = new Joystick(2);
		m_leftStick = new Joystick(1);

		frontLeftEncoder = new AIDistance (4, 1, 4, 2, true); //TODO: Set actual digital channels and pulse values
		
		frontRightEncoder = new AIDistance (6, 1, 6, 2, true);
		
		rearLeftEncoder = new AIDistance (4, 3, 4, 4, true);
		
		rearRightEncoder = new AIDistance (6, 3, 6, 4, true);
		
		frontLeftEncoder -> Start();
		frontRightEncoder -> Start();
		rearLeftEncoder -> Start();
		rearRightEncoder -> Start();

		frontLeftPot = new AnalogChannel(1, 3); 
		frontRightPot = new AnalogChannel(1, 4);
		rearLeftPot = new AnalogChannel(1, 5);
		rearRightPot = new AnalogChannel(1, 6);

		frontLeftVC = new VelocityControl(frontLeftPowerJag, frontLeftEncoder, 0.0105469, 0, 0, 40, 0.0472); //Different enc. 250/rot 
		frontRightVC = new VelocityControl(frontRightPowerJag, frontRightEncoder, 0.0105469, 0, 0, 40, 0.0472); //0.0472
		rearLeftVC = new VelocityControl(rearLeftPowerJag, rearLeftEncoder, 0.0105469, 0, 0, 40, 0.0472);
		rearRightVC = new VelocityControl(rearRightPowerJag, rearRightEncoder, 0.0105469, 0, 0, 40 , 0.0472);

		frontLeftPC = new PositionControl(frontLeftSteerJag, frontLeftPot, 0.02582, 0, 0, 0, 990, 697); //325	//TODO: Set values. 0 is right. p:0.0158268
		frontRightPC = new PositionControl(frontRightSteerJag, frontRightPot, -0.03370, 0, 0, 0, 990, 180); //875
		rearLeftPC = new PositionControl(rearLeftSteerJag, rearLeftPot, -0.0340, 0, 0, 0, 990, 486); //584
		rearRightPC = new PositionControl(rearRightSteerJag, rearRightPot, 0.01734, 0, 0, 0, 990, 395); //735
		
		frontLeftWM = new WheelModule(frontLeftVC, frontLeftPC, -16, 11, m_leftStick);
		frontRightWM = new WheelModule(frontRightVC, frontRightPC, 16, 11, m_leftStick);
		rearLeftWM = new WheelModule(rearLeftVC, rearLeftPC, -16, -11, m_leftStick);
		rearRightWM = new WheelModule(rearRightVC, rearRightPC, 16, -11, m_leftStick);
		
		gyro = new AIAngle(1);
		accel = new ADXL345_I2C(6);
		
		crabDrive = new CrabDrive(frontLeftWM, frontRightWM, rearLeftWM, rearRightWM, m_max_radius);
		
		demonAI = new DaemonAI(crabDrive, gyro, frontLeftEncoder, frontRightEncoder, rearLeftEncoder, rearRightEncoder, m_max_radius);
		
		autonMode = 1;
		
		/***** Manip init *****/
		
		rollerVic = new Victor(4, 7);
		
		light = new Relay(4, 1, Relay::kForwardOnly);
		
//		extenderEncoder = new Encoder(4, 5, 4, 6, true);
//		extenderEncoder -> SetDistancePerPulse(0.1); //TODO: Set actual value
//		extenderEncoder -> Start();
		
//		pallServo = new Servo(4,7);
		
		rotatorVic = new Victor(4,6);
		flapperVic = new Victor(4, 5);
		kickerVic = new Victor(6, 5);
		kickerSwitch = new DigitalInput(6, 5);
		
		m_manipStick = new Joystick(3);
		
		manip = new Manip(m_rightStick, rotatorVic, kickerVic, kickerSwitch);
	}

	/********************************** Init Routines *************************************/

	void RobotInit(void) 
	{
		printf("RobotInit() completed.\n");
	}

	void DisabledInit(void) 
	{
		//
	}

	void AutonomousInit(void) 
	{
		demonAI->EnablePID();
		demonAI->ResetEncoders();
		gyro->Reset();
	}

	void TeleopInit(void) 
	{
		demonAI->DisablePID();
	}

	/********************************** Periodic Routines *************************************/

	void DisabledPeriodic(void) 
	{
		// feed the user watchdog at every period when disabled
		GetWatchdog().Feed();

		// increment the number of disabled periodic loops completed
		m_disabledPeriodicLoops++;
		
		if(m_rightStick->GetRawButton(11))
			autonMode = 1;
		else if(m_rightStick->GetRawButton(10))
			autonMode = 2;
		else if(m_leftStick->GetRawButton(10))
			autonMode = 3;
		
	}

	void AutonomousPeriodic(void) 
	{
		// 52 in bump to first dot
		// 36 in dot to dot
		
		// feed the user watchdog at every period when in autonomous
		GetWatchdog().Feed();
		
		m_autoPeriodicLoops++;
		
		static int task = 1;
		
		static int counter = 0;
		static bool kicked = false;
		static bool kick = false;
		static bool roller = true;
		
		if((m_autoPeriodicLoops % 200) < 100)
		{
			light->Set(Relay::kOff);
		}
		else
			light->Set(Relay::kOn);
		
		if(m_autoPeriodicLoops % 10)
			light->Set(Relay::kOn);
		else
			light->Set(Relay::kOff);
		
		if(autonMode == 1)
		{
			if(roller) 
				rollerVic->Set(0.8);
			else 
				rollerVic->Set(0);
			
			switch(task)
			{
			case 1:
				demonAI->StrafeDaemon(150, (M_PI / 2), true);
				if(frontRightEncoder->GetDistance() <= -21)
				{
					kick = true;
					if(kicked)
					{
						kick = false;
						task++;
					}
						
	//				demonAI->Hold();
				}
				break;
			case 2:
				if(frontRightEncoder->GetDistance() <= -55)
				{
					kick = true;
					if(kicked)
					{
						kick = false;
						task++;
					}
	//				demonAI->Hold();
				}
				break;
				
			case 3:
				if(frontRightEncoder->GetDistance() <= -93)
				{
					kick = true;
					if(kicked)
					{
						kick = false;
						task++;
					}
	//				demonAI->Hold();
				}
				break;
			case 4:
				roller = false;
				task++;
				break;
				
			default:
				kick = false;
				//crabDrive->StrafeBot(0, (M_PI / 2));
				demonAI->Hold();
				
			}
			
			if(kick && !kicked)
			{
				kickerVic->Set(-1);
				counter++;
				if(counter > 25 && kickerSwitch->Get())
				{
					kicked = true;
					counter = 0;
				}
			}
			else
			{
				if(!kickerSwitch->Get())
					kickerVic->Set(-1);
				else
				{
					kickerVic->Set(0);
					kicked = false;
				}
			}
		}
		else if(autonMode == 2)
		{
			//50 68 84..... in, center, out, antiGuerella 
			demonAI->StrafeDaemon(75, 0, true);
		}
		
		else if (autonMode == 3)
		{
			char string[] = "1";
			static int length = strlen(string);
			static int letter = 0;
			static int morse_counter = 0;
			static const int DOT = 10;
			static const int DASH = 20;
			static const int DELAY = 5;
			static int mode;
			
			morse_counter++;
			if(letter <= length)
			{
				switch(string [letter])
				{
					case 0:
						if(morse_counter > (DELAY + DOT))
						{
							letter++;
							counter = 0;
							mode = 11;
						}
						else if(morse_counter > DOT)
						{
							light->Set(Relay::kOff);
							mode = 12;
						}
						else
						{
							light->Set(Relay::kOn);
							mode = 13;
						}
						morse_counter++;
						break;
					case 1:
						if(morse_counter > (DELAY + DASH))
						{
							letter++;
							morse_counter = 0;
							mode = 21;
						}
						else if(morse_counter > DASH)
						{
							light->Set(Relay::kOff);
							mode = 22;
						}
						else
						{
							light->Set(Relay::kOn);
							mode = 23;
						}
						morse_counter++;
						break;
					case 2:
						if(morse_counter > DASH)
						{
							mode = 31;
							letter++;
						}
						morse_counter++;
						break;
				}
			}
			printf("%i, %i\n", morse_counter, mode);
		}
		
/*		char string[] = "ab";
		static int letter = 0;
		static int sub = 1;
		static int morse_counter = 0;
		static const int DOT = 100;
		static const int DASH = 200;
		static const int DELAY = 50;
		
		switch(string [letter])
		{
		case 'a':
			switch(sub)
			{
			case 1:
				if(morse_counter > (DELAY + DOT))
				{
					sub++;
					counter = 0;
				}
				else if(morse_counter > DOT)
					light->Set(Relay::kOff);
				else
					light->Set(Relay::kOn);
				morse_counter++;
				break;
			case 2:
				if(morse_counter > (DELAY + DASH))
				{
					sub++;
					morse_counter = 0;
				}
				else if(morse_counter > DASH)
					light->Set(Relay::kOff);
				else
					light->Set(Relay::kOn);
				morse_counter++;
				break;
			case 3:
				if(morse_counter > DASH)
				{
					sub++;
				}
				morse_counter++;
			}
		}*/
	}

	void TeleopPeriodic(void) 
	{

		// feed the user watchdog at every period when in autonomous
		GetWatchdog().Feed();

		// increment the number of teleop periodic loops completed
		m_telePeriodicLoops++;

		m_dsPacketsReceivedInCurrentSecond++; // increment DS packets received
		
		static bool auto_kick_mode = true;
			
		/*
		float autonEncPID_P = ((m_leftStick->GetZ()+1) * 0.07);
		float autonEncPID_I = ((m_rightStick->GetZ()+1) * 0.0007);
		static float autonEncPID_D = 0.0;
		if(m_rightStick->GetRawButton(10))
			autonEncPID_D -= 0.0001;
		if(m_rightStick->GetRawButton(11))
			autonEncPID_D += 0.0001;
		if(m_rightStick->GetRawButton(8) && m_rightStick->GetRawButton(10))
			autonEncPID_D = 0;
		static bool autonMode = false;
		static int secondaryAutonMode = 0;
		gyro->m_pid->SetPID(autonEncPID_P, autonEncPID_I, autonEncPID_D);
		*/
		if(m_rightStick->GetRawButton(2))
			rollerVic->Set(1);
		else if(m_rightStick->GetRawButton(4))
			rollerVic->Set(-1);
		else
			rollerVic->Set(0);


		if(m_rightStick->GetRawButton(6) && m_rightStick->GetRawButton(7))
		{
			auto_kick_mode = false;
		}
		
		if(m_leftStick->GetRawButton(10) && m_leftStick->GetRawButton(11))
		{
			auto_kick_mode = true;
		}
		
		if(auto_kick_mode)
		{
			if(!m_rightStick->GetTrigger())
			{
				if(!kickerSwitch->Get())
					kickerVic->Set(-1);
				else
					kickerVic->Set(0);
			}
			else
				kickerVic->Set(-1);
		}
		else
		{
			if(m_rightStick->GetTrigger())
				kickerVic->Set(-1);
			else
				kickerVic->Set(0);
		}
		
		
		
		if(m_leftStick->GetTrigger())
			flapperVic->Set(1);
		else
			flapperVic->Set(0);
		
		if(kickerSwitch->Get())
			light->Set(Relay::kOn);
		else
			light->Set(Relay::kOff);
		
		manip->Teleop();
		
		if(m_leftStick->GetRawButton(4))
		{
			crabDrive->Drive(m_leftStick, m_rightStick, -1);
		}
		else
			crabDrive->Drive(m_leftStick, m_rightStick, 1);

		
		static int fr, fl, rr, rl;
//		CalibrateDrive();
//		CalibratePID();
/*		dashOut->customFloat[0] = frontLeftEncoder->GetDistance();
		dashOut->customFloat[1] = frontRightEncoder->GetDistance();
		dashOut->customFloat[2] = rearLeftEncoder->GetDistance();
		dashOut->customFloat[3] = rearRightEncoder->GetDistance();
		dashOut->customFloat[4] = (frontLeftEncoder->GetDistance() + frontRightEncoder->GetDistance() + 
								   rearLeftEncoder->GetDistance() + rearRightEncoder->GetDistance() ) / 4;
*/

/*		printf("Front left: %f\n", frontLeftEncoder->GetDistance());
		printf("Front right: %f\n", frontRightEncoder->GetDistance());
		printf("Rear left: %f\n", rearLeftEncoder->GetDistance());
		printf("Rear right: %f\n", rearRightEncoder->GetDistance());
		printf("Average: %f\n", (frontLeftEncoder->GetDistance() + frontRightEncoder->GetDistance() +
								   rearLeftEncoder->GetDistance() + rearRightEncoder->GetDistance() ) / 4);
	*/	
//		dashOut->sendIOPortData();
	} // TeleopPeriodic(void)


	/********************************** Continuous Routines *************************************/

	void DisabledContinuous(void) 
	{
		Wait(0);
	}

	void AutonomousContinuous(void) 
	{
		Wait(0);
	}

	void TeleopContinuous(void) 
	{
		Wait(0);
	}
	
	void CalibrateDrive(void)
	{
		//Calibrate to chains facing front of robot
		
		static int fl_values = 0;
		static int fl_counts = 0;
		
		static int fr_values = 0;
		static int fr_counts = 0;
		
		static int rl_values = 0;
		static int rl_counts = 0;
		
		static int rr_values = 0;
		static int rr_counts = 0;
		
		frontLeftPC->Disable();
		frontRightPC->Disable();
		rearLeftPC->Disable();
		rearRightPC->Disable();
		
		if(m_leftStick->GetRawButton(6))
		{
			frontLeftSteerJag->Set(m_rightStick->GetX()*0.4);
			//frontLeftVC->Set(m_leftStick->GetY()*0.4);
		}
		else if(m_leftStick->GetRawButton(11))
		{
			frontRightSteerJag->Set(m_rightStick->GetX()*0.4);
			//frontRightVC->Set(m_leftStick->GetY()*0.4);
		}
		else if(m_leftStick->GetRawButton(7))
		{
			rearLeftSteerJag->Set(m_rightStick->GetX()*0.4);
			//rearLeftVC->Set(m_leftStick->GetY()*0.4);
		}
		else if(m_leftStick->GetRawButton(10))
		{
			rearRightSteerJag->Set(m_rightStick->GetX()*0.4);
			//rearRightVC->Set(m_leftStick->GetY()*0.4);
		}
		else
		{
			frontLeftSteerJag->Set(0);
			frontRightSteerJag->Set(0);
			rearLeftSteerJag->Set(0);
			rearRightSteerJag->Set(0);
		}
		
		if(m_rightStick->GetRawButton(6))
		{
			fl_values += frontLeftPot->GetValue();
			fl_counts ++;
		}
		else if(m_rightStick->GetRawButton(11))
		{
			fr_values += frontRightPot->GetValue();
			fr_counts ++;
		}
		else if(m_rightStick->GetRawButton(7))
		{
			rl_values += rearLeftPot->GetValue();
			rl_counts ++;
		}
		else if(m_rightStick->GetRawButton(10))
		{
			rr_values += rearRightPot->GetValue();
			rr_counts ++;
		}
		else
		{
			fl_values = 0;
			fl_counts = 0;
			fr_values = 0;
			fr_counts = 0;
			rl_values = 0;
			rl_counts = 0;
			rr_values = 0;
			rr_counts = 0;
		}
		
		dashOut->customInt[0] = frontLeftPot->GetValue();
		dashOut->customInt[1] = frontRightPot->GetValue();
		dashOut->customInt[2] = rearLeftPot->GetValue();
		dashOut->customInt[3] = rearRightPot->GetValue();
		
		dashOut->customInt[4] = fl_values / fl_counts;
		dashOut->customInt[5] = fr_values / fr_counts;
		dashOut->customInt[6] = rl_values / rl_counts;
		dashOut->customInt[7] = rr_values / rr_counts;
		
		printf("Front Left value: %i\tAvg: %i\n", frontLeftPot->GetValue(), fl_values / fl_counts);
		printf("Front Right value: %i\tAvg: %i\n", frontRightPot->GetValue(), fr_values / fr_counts);
		printf("Rear Left value: %i\tAvg: %i\n", rearLeftPot->GetValue(), rl_values / rl_counts);
		printf("Rear Right value: %i\tAvg: %i\n", rearRightPot->GetValue(), rr_values / rr_counts);
		
		while(m_rightStick->GetRawButton(3))
		{
			// pause
		}
	}
	
	void CalibratePID(void)
	{
		static float pfl, pfr, prl, prr = 0;
		
		if(m_leftStick->GetRawButton(6))
	 	{
			frontLeftWM->SetAngle(m_leftStick->GetX() * 360);
			pfl = (m_leftStick->GetZ()+1) * 0.02;
			frontLeftPC->m_pid->SetPID(pfl, 0, 0);
		}
		if(m_leftStick->GetRawButton(11))
		{
			frontRightWM->SetAngle(m_leftStick->GetX() * 360);
			pfr = (m_leftStick->GetZ()+1) * 0.02;
			frontRightPC->m_pid->SetPID(-pfl, 0, 0);
		}
		if(m_leftStick->GetRawButton(7))
		{
			rearLeftWM->SetAngle(m_leftStick->GetX() * 360);
			prl = (m_leftStick->GetZ()+1) * 0.02;
			rearLeftPC->m_pid->SetPID(-pfl, 0, 0);
		}
		if(m_leftStick->GetRawButton(10))
		{
			rearRightWM->SetAngle(m_leftStick->GetX() * 360);
			prr = (m_leftStick->GetZ()+1) * 0.02;
			rearRightPC->m_pid->SetPID(pfl, 0, 0);
		}		
		
		dashOut->customFloat[0] = pfl;
		dashOut->customFloat[1] = pfr;
		dashOut->customFloat[2] = prl;
		dashOut->customFloat[3] = prr;
	}
};

START_ROBOT_CLASS(Robot226);
