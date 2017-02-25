/*
 * PID.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: Romeli
 */

#include "PID.h"
#include "Delay.h"

void PIDClass::Compute() {
	static uint64_t Lasttime = micros();
	double dt = (double) (micros() - Lasttime) / 1000000.0;
	Lasttime = micros();

	pError = PIDParam->set - PIDParam->now;			//Compute pError
	iTerm += PIDParam->ki * pError * dt;						//Compute iTerm
	iTerm = iTerm > PIDParam->out_max ? PIDParam->out_max :		//Limit of iTerm
			iTerm < PIDParam->out_min ? PIDParam->out_min : iTerm;
	dTerm = PIDParam->kd * (PIDParam->now - Last) * dt;			//Compute DTerm
	Last = PIDParam->now;									//Get last pError
	if (Mode == PIDMode_Diff) {
		PIDParam->out = PIDParam->kp * pError + iTerm - dTerm + PIDParam->out;
	} else {
		PIDParam->out = PIDParam->kp * pError + iTerm - dTerm;
	}
	PIDParam->out =
			PIDParam->out > PIDParam->out_max ? PIDParam->out_max :	//Limit of Output_Inside
			PIDParam->out < PIDParam->out_min ?
					PIDParam->out_min : PIDParam->out;
}
