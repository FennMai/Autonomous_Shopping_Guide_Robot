#include <iostream>
#include <mutex>

#include "pigpio.h"
#include "MPU6050.h"

MPU6050* MPU6050::globalInstance = nullptr;
// std::atomic<bool> mpu6050_newdata(false);

MPU6050 :: MPU6050() : MPU6050(200, CALIBRATION_COUNT) {};

MPU6050 :: MPU6050(int customSampleRate, int calibrationTimes) :
	ax(0.0f),
	ay(0.0f),
	az(0.0f),
	gx(0.0f),
	gy(0.0f),
	gz(0.0f),
	mx(0.0f),
	my(0.0f),
	mz(0.0f), 
	iicMPU6050(MPU6050_ADDRESS),
	iicGY271(GY_271_ADDRESS),
	a_offset{0.0f, 0.0f, 0.0f}, 
	g_offset{0.0f, 0.0f, 0.0f},
	m_offset{0.0f, 0.0f, 0.0f} {

	offset_count = 0;
	needToExit = false;
	calibrationCount = calibrationTimes;
	globalInstance = this;

	MPU6050RawData[14] = {};
	GY271RawData[6] = {};

	calibrate_ready.store(false);
	mpu6050_newdata.store(false);

	// 1000/sample_rate_int if DLPF is enabled.
    sampleRate = (1000 / customSampleRate) - 1; 
    std::cout << "Sample rate has been set to " << customSampleRate << std::endl;

    // Wake MPU6050 up.
	char dataString[2] = {0x6B, 0};
	if(iicMPU6050.write(dataString, 2) != 0) {
		std::cerr << "Can't wake MPU6050 up." << std::endl;
	}
	
    // Real sample rate = 1000 / (1+ sampleRate ).
	char setSampleRate[2] = { 0x19, sampleRate }; 
	if(iicMPU6050.write(setSampleRate, 2) != 0) {
		std::cerr << "Can't set sample rate of MPU6050." << std::endl;
	} 
	
	// Enable interrupt.
	char enableInt[2] = { 0x38, 0x01 };
	if(iicMPU6050.write(enableInt, 2) != 0) {
		std::cerr << "Can't enable interrupt of MPU6050." << std::endl;
	}

    // Set DLPF.
	char setDLPF[2] = {0x1A, 0x06};
	if(iicMPU6050.write(setDLPF, 2) != 0) {
		std::cerr << "Can't set DLPF of MPU6050." << std::endl;
	}

    // set full scale range of accelerometer to +-2g
	setRangeOfAcce(needToExit);
	if (needToExit) { return; }
	else { std::cout << "Scale range of accelerometer: +-2g." << std::endl; }

	// set full scale range of Gyroscope to +- 2000 °/s
	setRangeOfGyro(needToExit);
	if (needToExit) { return; }
	else { std::cout << "Scale range of gyroscope: +- 2000°/s." << std::endl; }
}

bool MPU6050 :: checkNewData() {

	return (mpu6050_newdata.load());

}

void MPU6050 :: MPU6050ReadData(char* data) {

	char MPU6050StartAddr = 0x3B;

	if(iicMPU6050.read(data, 14, MPU6050StartAddr) != 0) {
		std::cerr << "Can't read data of MPU6050." << std::endl;
	}
}

void MPU6050 :: GY271ReadData(char* data) {

	char GY271StartAddr = 0x00;

	if(iicGY271.read(data, 6, GY271StartAddr) != 0) {
		std::cerr << "Can't read data of GY271." << std::endl;
	}
}

void MPU6050 :: setRangeOfAcce(bool needToExit) {

    char dataAcce[2] = {0x1C, 0 << 3 };

	iicMPU6050.write(dataAcce, 2);

}

void MPU6050 :: setRangeOfGyro(bool needToExit) {

    char dataGyro[2] = {0x1B, 0x18};

	iicMPU6050.write(dataGyro, 2);

}

// void MPU6050 :: getData(float a[3], float g[3], float m[3]) {

// 	mpu6050_newdata.store(false);

// 	a[0] = ((ax.load() - a_offset[0]) / 16384.0f)*9.818f;
// 	a[1] = ((ay.load() - a_offset[1]) / 16384.0f)*9.818f;
// 	a[2] = ((az.load() - a_offset[2]) / 16384.0f)*9.818f;
// 	a[2] = a[2] + 9.818f;

// 	// a[0] = (ax.load() / 16384.0f)*9.818f;
// 	// a[1] = (ay.load() / 16384.0f)*9.818f;
// 	// a[2] = (az.load() / 16384.0f)*9.818f;

// 	g[0] = (gx.load() - g_offset[0]) / (16.384f) * DEG_TO_RAD;
// 	g[1] = (gy.load() - g_offset[1]) / (16.384f) * DEG_TO_RAD;
// 	g[2] = (gz.load() - g_offset[2]) / (16.384f) * DEG_TO_RAD;

// 	m[0] = (mx.load() - m_offset[0]) * 0.000122f;
// 	m[1] = (my.load() - m_offset[1]) * 0.000122f;
// 	m[2] = (mz.load() - m_offset[2]) * 0.000122f;	

// 	// std::cout << g[0] << "|" << g[1] << "|" << g[2] << std::endl;
// }

void MPU6050::setCallback(CallbackFunction callback) {

    callback_ = callback;

}

void MPU6050 :: getData() {

	float a[3] = {};
	float g[3] = {};
	float m[3] = {};

	mpu6050_newdata.store(false);

	a[0] = ((ax.load() - a_offset[0]) / 16384.0f)*9.818f;
	a[1] = ((ay.load() - a_offset[1]) / 16384.0f)*9.818f;
	a[2] = ((az.load() - a_offset[2]) / 16384.0f)*9.818f;
	a[2] = a[2] + 9.818f;

	// a[0] = (ax.load() / 16384.0f)*9.818f;
	// a[1] = (ay.load() / 16384.0f)*9.818f;
	// a[2] = (az.load() / 16384.0f)*9.818f;

	g[0] = (gx.load() - g_offset[0]) / (16.384f) * DEG_TO_RAD;
	g[1] = (gy.load() - g_offset[1]) / (16.384f) * DEG_TO_RAD;
	g[2] = (gz.load() - g_offset[2]) / (16.384f) * DEG_TO_RAD;

	m[0] = (mx.load() - m_offset[0]) * 0.000122f;
	m[1] = (my.load() - m_offset[1]) * 0.000122f;
	m[2] = (mz.load() - m_offset[2]) * 0.000122f;	

	if(callback_) {

		callback_(a, g, m);

	}

	// std::cout << g[0] << "|" << g[1] << "|" << g[2] << std::endl;
}

void MPU6050 :: calibrateData() {

	g_offset[0] += gx.load();
	g_offset[1] += gy.load();
	g_offset[2] += gz.load();

	a_offset[0] += ax.load();
	a_offset[1] += ay.load();
	a_offset[2] += az.load();

	m_offset[0] += mx.load();
	m_offset[1] += my.load();
	m_offset[2] += mz.load();

	std::cout << "Calibrating! " << (calibrationCount - offset_count) << " times left." << std::endl;

	offset_count++;

	if (offset_count > calibrationCount) {
		g_offset[0] = g_offset[0] / offset_count;
		g_offset[1] = g_offset[1] / offset_count;
		g_offset[2] = g_offset[2] / offset_count;

		a_offset[0] = a_offset[0] / offset_count;
		a_offset[1] = a_offset[1] / offset_count;
		a_offset[2] = a_offset[2] / offset_count;

		m_offset[0] = m_offset[0] / offset_count;
		m_offset[1] = m_offset[1] / offset_count;
		m_offset[2] = m_offset[2] / offset_count;

		offset_count = 0;

		calibrate_ready.store(true);
	}
}

void interruptHandler(int GPIO, int level, unsigned int tick) {

	// std::cout << "Int trigger!" << std::endl;
	
	MPU6050::globalInstance -> MPU6050ReadData(&MPU6050::globalInstance -> MPU6050RawData[0]);
	MPU6050::globalInstance -> GY271ReadData(&MPU6050::globalInstance -> GY271RawData[0]);

	// ax = (float)((MPU6050RawData[0] << 8) | MPU6050RawData[1]);
	// ay = (float)((MPU6050RawData[2] << 8) | MPU6050RawData[3]);
	// az = (float)((MPU6050RawData[4] << 8) | MPU6050RawData[5]);
	// gx = (float)((MPU6050RawData[8] << 8) | MPU6050RawData[9]);
	// gy = (float)((MPU6050RawData[10] << 8) | MPU6050RawData[11]);
	// gz = (float)((MPU6050RawData[12] << 8) | MPU6050RawData[13]);
	// mx = (float)((GY271RawData[0] << 8) | GY271RawData[1]);
	// my = (float)((GY271RawData[2] << 8) | GY271RawData[3]);
	// mz = (float)((GY271RawData[4] << 8) | GY271RawData[5]);

	short tempax, tempay, tempaz, tempgx, tempgy, tempgz, tempmx, tempmy, tempmz;

	tempax = ((MPU6050::globalInstance -> MPU6050RawData[0] << 8) | MPU6050::globalInstance -> MPU6050RawData[1]);
	tempay = ((MPU6050::globalInstance -> MPU6050RawData[2] << 8) | MPU6050::globalInstance -> MPU6050RawData[3]);
	tempaz = ((MPU6050::globalInstance -> MPU6050RawData[4] << 8) | MPU6050::globalInstance -> MPU6050RawData[5]);
	tempgx = ((MPU6050::globalInstance -> MPU6050RawData[8] << 8) | MPU6050::globalInstance -> MPU6050RawData[9]);
	tempgy = ((MPU6050::globalInstance -> MPU6050RawData[10] << 8) | MPU6050::globalInstance -> MPU6050RawData[11]);
	tempgz = ((MPU6050::globalInstance -> MPU6050RawData[12] << 8) | MPU6050::globalInstance -> MPU6050RawData[13]);
	tempmx = ((MPU6050::globalInstance -> GY271RawData[0] << 8) | MPU6050::globalInstance -> GY271RawData[1]);
	tempmy = ((MPU6050::globalInstance -> GY271RawData[2] << 8) | MPU6050::globalInstance -> GY271RawData[3]);
	tempmz = ((MPU6050::globalInstance -> GY271RawData[4] << 8) | MPU6050::globalInstance -> GY271RawData[5]);
	MPU6050::globalInstance -> ax.store(tempax);
	MPU6050::globalInstance -> ay.store(tempay);
	MPU6050::globalInstance -> az.store(tempaz);
	MPU6050::globalInstance -> gx.store(tempgx);
	MPU6050::globalInstance -> gy.store(tempgy);
    MPU6050::globalInstance -> gz.store(tempgz);
    MPU6050::globalInstance -> mx.store(tempmx);
    MPU6050::globalInstance -> my.store(tempmy);
    MPU6050::globalInstance -> mz.store(tempmz);

	//std::cout << MPU6050::globalInstance -> gx.load() << "|" << MPU6050::globalInstance -> gy.load() << std::endl;


	if(!MPU6050::globalInstance -> calibrate_ready.load()) {
		MPU6050::globalInstance -> calibrateData();
	}

	else
		MPU6050::globalInstance -> mpu6050_newdata.store(true);
}

// Trigger interrupt to read data from MPU6050.
void initializeMPUISR() {
    
    gpioSetMode(INT_PIN, PI_INPUT);

	if (gpioSetISRFunc(INT_PIN, RISING_EDGE, 0, interruptHandler) != 0) {
		std::cout << "mpu6050 int failed." << std::endl;
		return;
	}
}