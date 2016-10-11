/*
 * PID.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: Romeli
 */

#include "PID.h"
#include "Delay.h"

PIDClass::PIDClass(double p, double i, double d, double *set, double *now,
		double *out, double min, double max, PIDPostion postion, PIDMode mode) {
	Postion = postion;
	if (Postion == PIDPostionPos) {
		Kp = p;
		Ki = i;
		Kd = d;
	} else {
		Kp = -p;
		Ki = -i;
		Kd = -d;
	}
	Set = set;
	Now = now;
	Out = out;
	OutMin = min;
	OutMax = max;

	Mode = mode;

	DTerm = 0;
	pError = 0;
	Last = 0;
	iTerm = 0;
}

void PIDClass::Compute() {
	static uint64_t Lasttime = micros();
	double dt = (double) (micros() - Lasttime) / 1000000.0;
	Lasttime = micros();
	pError = *Set - *Now;							//Compute pError
	iTerm += Ki * pError * dt;								//Compute iTerm
	iTerm = iTerm > OutMax ? OutMax :		//Limit of iTerm
			iTerm < OutMin ? OutMin : iTerm;
	DTerm = Kd * (*Now - Last) * dt;						//Compute DTerm
	Last = *Now;									//Get last pError
	if (Mode == PIDMode_Diff) {
		*Out = Kp * pError + iTerm - DTerm + *Out;
	} else {
		*Out = Kp * pError + iTerm - DTerm;
	}
	*Out = *Out > OutMax ? OutMax :						//Limit of Output_Inside
			*Out < OutMin ? OutMin : *Out;
}

void PIDClass::SetTuningP(double p) {
	if (Postion == PIDPostionPos) {
		Kp = p;
	} else {
		Kp = -p;
	}
}

void PIDClass::SetTuningI(double i) {
	if (Postion == PIDPostionPos) {
		Ki = i;
	} else {
		Ki = -i;
	}
}

void PIDClass::SetTuningD(double d) {
	if (Postion == PIDPostionPos) {
		Kd = d;
	} else {
		Kd = -d;
	}
}

void PIDClass::SetTunings(double p, double i, double d) {
	if (Postion == PIDPostionPos) {
		Kp = p;
		Ki = i;
		Kd = d;
	} else {
		Kp = -p;
		Ki = -i;
		Kd = -d;
	}
}

void PIDClass::SetOutputLimits(double min, double max) {
	OutMin = min;
	OutMax = max;
}

void PIDClass::Clear() {
	iTerm = 0;
	Last = 0;
}
