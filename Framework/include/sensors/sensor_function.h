/*
 * sensor_function.h
 *
 * Created: 6/28/2017 11:26:58 PM
 *  Author: roncato
 */ 

#include "mal/mal_reg_access.h"
#include "mal/mal_reg.h"
#include "util/util_algorithm.h"

#include "sensors/sensor.h"
#include "sensors/sensor_volts.h"

#ifndef SENSOR_FUNCTION_H_
#define SENSOR_FUNCTION_H_

namespace sensor {

typedef bool (*MapFunction)(const float&, float&);

class FunctionSensor : public VoltsSensor {
public:
	FunctionSensor(uint8_t adc_pin, float volt_ref, sensor::MapFunction function) :
		VoltsSensor(adc_pin, volt_ref), function_{function} {}

	bool Read(float& quantity) {
		float volts;
		if (VoltsSensor::Read(volts)) {
			return function_ && function_(volts, quantity);
		}
		return false;
	}

private:
	sensor::MapFunction function_{nullptr};
};

} // namespace sensor


#endif /* SENSOR_FUNCTION_H_ */