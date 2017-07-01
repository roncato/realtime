/*
 * sensor_servo.h
 *
 * Created: 6/11/2017 9:20:07 PM
 *  Author: roncato
 */ 


#ifndef SENSOR_SERVO_H_
#define SENSOR_SERVO_H_

#include <stdint.h>

#include "mal/mal_time.h"
#include "sensor.h"

namespace sensor {

constexpr uint16_t kServoMinPulseWidth = 1000;
constexpr uint16_t kServoMaxPulseWidth = 2000;
	
class ServoSensor {
public:

	enum ServoSensorState {
		SS_STATE_INITIAL,
		SS_STATE_IDLE,
		SS_STATE_PROBING
	};

	ServoSensor(uint8_t pcint_number) : pcint_number_{pcint_number}, event_handler_{} {}
	~ServoSensor() {
		Close();
	}
	uint16_t Read() const;
	SensorReturn Open();
	SensorReturn Close();
	bool IsOpen() const {
		return is_open_;
	}
	void SetEventHandler(void*, SensorReadEventHandler);
	void RemoveEventHandler();
	ServoSensorState GetState() const {
		return state_;
	}
private:
	typedef struct {
		void* context;
		SensorReadEventHandler handler;
	} SensorEventRecord;

	const uint8_t pcint_number_;

	mal::time::time_t start_time_;
	bool is_open_{false};

	volatile uint16_t value_;
	volatile ServoSensorState state_;
	SensorEventRecord event_handler_;

	static void Rising(void* ptr);
	static void Falling(void* ptr);
};

} // namespace sensor


#endif /* SENSOR_SERVO_H_ */