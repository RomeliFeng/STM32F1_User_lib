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

struct PIDClass {
public:
	PIDClass(double p, double i, double d, double set, double *now, double *out,
			double min, double max, PIDMode mode);
	void Compute();
	void Compute(double set);
	void SetTunings(double p, double i, double d);
	void SetOutputLimits(double min, double max);
private:
	double Kp;
	double Ki;
	double Kd;

	double pError;
	double pError_Last;
	double iTerm;
	double dError;

	double Set;
	double *Now;
	double *Out;

	double OutMin;
	double OutMax;

	PIDMode Mode;
};

#endif /* PID_H_ */
