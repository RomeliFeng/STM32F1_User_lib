/*
 * MPU.cpp
 *
 *  Created on: Aug 8, 2016
 *      Author: Romeli
 */

#include "MPU.h"
#include "I2C.h"

#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)#define	ACCEL_XOUT		0x3B#define	ACCEL_YOUT		0x3D#define	ACCEL_ZOUT		0x3F#define	TEMP_OUT		0x41#define	GYRO_XOUT		0x43#define	GYRO_YOUT		0x45#define	GYRO_ZOUT		0x47
#define MPU_Add 0xd0
#define MPU_S_Add 0x0c<<1

MPUClass MPU;
_MPUData MPUData;

void MPUClass::Init() {
//	I2C.Send(MPU_Add, SMPLRT_DIV, 0x07); // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
//	I2C.Send(MPU_Add, CONFIG, 0x06); // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
//	I2C.Send(MPU_Add, GYRO_CONFIG, 0x18); // Set Gyro Full Scale Range to ±250deg/s
//	I2C.Send(MPU_Add, ACCEL_CONFIG, 0x01); // Set Accelerometer Full Scale Range to ±2g
//	I2C.Send(MPU_Add, PWR_MGMT_1, 0x00);			//reset

	I2C.Send(MPU_Add, SMPLRT_DIV, 7); // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
	I2C.Send(MPU_Add, CONFIG, 0x00); // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
	I2C.Send(MPU_Add, GYRO_CONFIG, 0x00); // Set Gyro Full Scale Range to ±250deg/s
	I2C.Send(MPU_Add, ACCEL_CONFIG, 0x00); // Set Accelerometer Full Scale Range to ±2g
	I2C.Send(MPU_Add, PWR_MGMT_1, 0); // Clear the 'sleep' bit to start the sensor.

	//change Address to Compass

//	I2C.Send(MPU_S_Add, 0x0A, 0x00); //PowerDownMode
//	I2C.Send(MPU_S_Add, 0x0A, 0x0F); //SelfTest
//	I2C.Send(MPU_S_Add, 0x0A, 0x00); //PowerDownMode

	//change Address to MPU

	I2C.Send(MPU_Add, 0x24, 0x40); //Wait for Data at Slave0
	I2C.Send(MPU_Add, 0x25, 0x8C); //Set i2c address at slave0 at 0x0C
	I2C.Send(MPU_Add, 0x26, 0x02); //Set where reading at slave 0 starts
	I2C.Send(MPU_Add, 0x27, 0x88); //set offset at start reading and enable
	I2C.Send(MPU_Add, 0x28, 0x0C); //set i2c address at slv1 at 0x0C
	I2C.Send(MPU_Add, 0x29, 0x0A); //Set where reading at slave 1 starts
	I2C.Send(MPU_Add, 0x2A, 0x81); //Enable at set length to 1
	I2C.Send(MPU_Add, 0x64, 0x01); //overvride register
	I2C.Send(MPU_Add, 0x67, 0x03); //set delay rate
	I2C.Send(MPU_Add, 0x01, 0x80);

	I2C.Send(MPU_Add, 0x34, 0x04); //set i2c slv4 delay
	I2C.Send(MPU_Add, 0x64, 0x00); //override register
	I2C.Send(MPU_Add, 0x6A, 0x00); //clear usr setting
	I2C.Send(MPU_Add, 0x64, 0x01); //override register
	I2C.Send(MPU_Add, 0x6A, 0x20); //enable master i2c mode
	I2C.Send(MPU_Add, 0x34, 0x13); //disable slv4
}

void MPUClass::RefreshData() {
	uint8_t Buffer[14];
	I2C.Receive(MPU_Add, ACCEL_XOUT, Buffer, 14);
	MPUData.ACC.X.Byte[1] = Buffer[0];
	MPUData.ACC.X.Byte[0] = Buffer[1];

	MPUData.ACC.Y.Byte[1] = Buffer[2];
	MPUData.ACC.Y.Byte[0] = Buffer[3];

	MPUData.ACC.Z.Byte[1] = Buffer[4];
	MPUData.ACC.Z.Byte[0] = Buffer[5];

	MPUData.GYRO.X.Byte[1] = Buffer[8];
	MPUData.GYRO.X.Byte[0] = Buffer[9];

	MPUData.GYRO.Y.Byte[1] = Buffer[10];
	MPUData.GYRO.Y.Byte[0] = Buffer[11];

	MPUData.GYRO.Z.Byte[1] = Buffer[12];
	MPUData.GYRO.Z.Byte[0] = Buffer[13];
}

