/*
 * sensor_volts.h
 *
 * Created: 6/28/2017 11:13:49 PM
 *  Author: roncato
 */ 


#ifndef SENSOR_VOLTS_H_
#define SENSOR_VOLTS_H_

#include "mal/mal_reg_access.h"
#include "mal/mal_reg.h"
#include "util/util_algorithm.h"

#include "sensors/sensor.h"

namespace sensor {

constexpr uint16_t kADCResolution = (1U << 10U) - 1;

class VoltsSensor {
public:
	VoltsSensor(uint8_t adc_pin, float volt_ref) : adc_pin_{adc_pin}, volt_ref_{volt_ref} {}
	virtual bool Read(float& quantity);
	SensorReturn Open();
	SensorReturn Close();
	bool IsOpen() const {
		return is_open;
	}
private:
	const uint8_t adc_pin_;
	const float volt_ref_;
	bool is_open{};
};


}


#endif /* SENSOR_VOLTS_H_ */