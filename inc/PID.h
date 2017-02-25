/*
 * PID.h
 *
 *  Created on: Aug 10, 2016
 *      Author: Romeli
 */

#ifndef PID_H_
#define PID_H_

typedef enum _PIDMode_Typedef {
	PIDMode_Diff, PIDMode_Post  //Post λ����
} PIDMode_Typedef;

typedef struct _PIDParam_Typedef {
	double kp;
	double ki;
	double kd;
	double set;
	double now;
	double out;
	double out_min;
	double out_max;
} PIDParam_Typedef;

class PIDClass {
public:
	PIDClass(PIDParam_Typedef *pidParam, PIDMode_Typedef mode) {
		PIDParam = pidParam;
		Mode = mode;
	}
	void SetPIDParam(PIDParam_Typedef *pidParam) {
		PIDParam = pidParam;
	}
	void Compute();
	void Clear(){
		iTerm = 0;
	}

private:
	PIDParam_Typedef *PIDParam;
	PIDMode_Typedef Mode = PIDMode_Diff;
	double Last = 0;
	double pError = 0;
	double iTerm = 0;
	double dTerm = 0;
};

#endif /* PID_H_ */
