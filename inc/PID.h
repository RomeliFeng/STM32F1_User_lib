/*
 * PID.h
 *
 *  Created on: Aug 10, 2016
 *      Author: Romeli
 */

#ifndef PID_H_
#define PID_H_

typedef enum {
	PIDMode_Diff, PIDMode_Post  //Post Œª÷√–Õ
} PIDMode;
typedef enum {
	PIDPostionPos, PIDPostionNeg
} PIDPostion;

struct PIDClass {
public:
	PIDClass(double p, double i, double d, double *set, double *now,
			double *out, double min, double max, PIDPostion postion,
			PIDMode mode);
	void Compute();
	void SetTunings(double p, double i, double d);
	void SetOutputLimits(double min, double max);
	void Clear();

private:
	double Kp;
	double Ki;
	double Kd;

	double pError;
	double Last;
	double iTerm;
	double DTerm;

	double *Set;
	double *Now;
	double *Out;

	double OutMin;
	double OutMax;

	PIDPostion Postion;
	PIDMode Mode;

};

#endif /* PID_H_ */
