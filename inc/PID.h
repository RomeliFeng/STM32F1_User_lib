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
	float Input;
	float Output;
	float SetPoint;
} PIDParam_Typedef;

typedef enum _PIDDir_Typedef {
	PIDDir_Postive, PIDDir_Negtive
} PIDDir_Typedef;

class PIDClass {
public:
	PIDClass(float kp, float ki, float kd, float interval, PIDDir_Typedef dir,
			PIDParam_Typedef *pidParam, PIDMode_Typedef mode);
	void SetTunings(float kp, float ki, float kd);
	void SetInterval(float interval);
	void SetLimits(float min, float max);
	void SetDir(PIDDir_Typedef dir);
	void Compute();
	void Clear();

private:
	PIDParam_Typedef *PIDParam;
	PIDMode_Typedef Mode;
	PIDDir_Typedef Dir;
	float Kp;
	float Ki;
	float Kd;
	float Last;
	float pError;
	float iTerm;
	float dTerm;
	float Min;
	float Max;
	float Interval;
};

#endif /* PID_H_ */
