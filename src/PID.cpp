/*
 * PID.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: Romeli
 */

#include "PID.h"
#include "Delay.h"

PIDClass::PIDClass(float kp, float ki, float kd, float interval,
		PIDDir_Typedef dir, PIDParam_Typedef* pidParam, PIDMode_Typedef mode) {
	PIDParam = pidParam;
	Interval = interval;
	Dir = dir;
	SetTunings(kp, ki, kd);
	Mode = mode;
}

void PIDClass::SetTunings(float kp, float ki, float kd) {
	if (kp < 0 || ki < 0 || kd < 0) {
		return;
	}
	Kp = kp;
	Ki = ki * Interval;
	Kd = kd / Interval;

	if (Dir == PIDDir_Negtive) {
		Kp = (0 - Kp);
		Ki = (0 - Ki);
		Kd = (0 - Kd);
	}
}

void PIDClass::SetInterval(float interval) {
	if (interval < 0) {
		return;
	}
	float ratio = interval / Interval;
	Ki *= ratio;
	Kd /= ratio;
	Interval = interval;
}

void PIDClass::SetLimits(float min, float max) {
	if (max < min) {
		return;
	}
	Min = min;
	Max = max;
	if (iTerm > Max) {
		iTerm = Max;
	} else if (iTerm < Min) {
		iTerm = Min;
	}
	if (PIDParam->Output > Max) {
		PIDParam->Output = Max;
	} else if (PIDParam->Output < Min) {
		PIDParam->Output = Min;
	}
}

void PIDClass::SetDir(PIDDir_Typedef dir) {
	if (Dir != dir) {
		Kp = 0 - Kp;
		Ki = 0 - Ki;
		Kd = 0 - Kd;
		Dir = dir;
	}
}

void PIDClass::Compute() {
	pError = PIDParam->SetPoint - PIDParam->Input;			//Compute pError

	iTerm += Ki * pError;						//Compute iTerm
	if (iTerm > Max) {	//Limit iTerm
		iTerm = Max;
	} else if (iTerm < Min) {
		iTerm = Min;
	}

	dTerm = Kd * (PIDParam->Input - Last);			//Compute DTerm
	if (Mode == PIDMode_Diff) {
		PIDParam->Output = Kp * pError + iTerm - dTerm + PIDParam->Output;
	} else {
		PIDParam->Output = Kp * pError + iTerm - dTerm;
	}

	if (PIDParam->Output > Max) { //Limit Output
		PIDParam->Output = Max;
	} else if (PIDParam->Output < Min) {
		PIDParam->Output = Min;
	}

	Last = PIDParam->Input;									//Get last pError
}

void PIDClass::Clear() {
	PIDParam->Output = 0;
	iTerm = 0;
}
