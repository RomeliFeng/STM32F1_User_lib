/*
 * PID.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: Romeli
 */

#include "PID.h"

PIDClass::PIDClass(double p, double i, double d, double *set, double *now,
		double *out, double min, double max, PIDMode mode)
{
	Kp = p;
	Ki = i;
	Kd = d;
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

void PIDClass::Compute()
{
	pError = *Set - *Now;							//Compute pError
	iTerm += Ki * pError;								//Compute iTerm
	iTerm = iTerm > OutMax ? OutMax :		//Limit of iTerm
			iTerm < OutMin ? OutMin : iTerm;
	DTerm = Kd * (*Now - Last);						//Compute DTerm
	Last = *Now;									//Get last pError
	if (Mode == PIDMode_Diff)
	{
		*Out = Kp * pError + iTerm - DTerm + *Out;
	}
	else
	{
		*Out = Kp * pError + iTerm - DTerm;
	}
	*Out = *Out > OutMax ? OutMax :						//Limit of Output_Inside
			*Out < OutMin ? OutMin : *Out;
}

void PIDClass::SetTunings(double p, double i, double d)
{
	Kp = p;
	Ki = i;
	Kd = d;
}

void PIDClass::SetOutputLimits(double min, double max)
{
	OutMin = min;
	OutMax = max;
}

void PIDClass::Clear()
{
	*Out = 0;
	iTerm = 0;
	Last = 0;
}
