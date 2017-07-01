/*
 * actuator_servo.h
 *
 * Created: 6/20/2017 9:36:54 PM
 *  Author: roncato
 */ 


#ifndef ACTUATOR_SERVO_H_
#define ACTUATOR_SERVO_H_

#include <stdint.h>

#include "actuators/actuator.h"

namespace actuator {

constexpr uint16_t kServoMinPulseWidth = 1000;
constexpr uint16_t kServoMaxPulseWidth = 2000;
constexpr uint16_t kServoDefaultPulseWidth = 1000;
constexpr uint16_t kServoRefreshInterval = 20000;

class ServoActuator {
public:
	ServoActuator(uint8_t port_address, uint8_t pin_number) : port_address_{port_address}, pin_number_{pin_number}, pulse_width_{kServoDefaultPulseWidth} 
	{}
	bool Write(uint16_t pulse_width);
	ActuatorReturn Open();
	ActuatorReturn Close();
	bool IsOpen() const {
		return is_open_;
	}
	uint16_t PulseWidth() const {
		return pulse_width_;
	}
private:
	const uint8_t port_address_;
	const uint8_t pin_number_;
	uint8_t servo_number_;
	uint16_t pulse_width_;
	bool is_open_{false};
};

}


#endif /* ACTUATOR_SERVO_H_ */