/*
 * sensor_analog.h
 *
 * Created: 6/28/2017 8:01:41 PM
 *  Author: roncato
 */ 


#ifndef SENSOR_ANALOG_H_
#define SENSOR_ANALOG_H_

#include "mal/mal_reg_access.h"
#include "mal/mal_reg.h"
#include "util/util_algorithm.h"

#include "sensors/sensor.h"
#include "sensors/sensor_volts.h"

namespace sensor {

template <uint16_t interp_table_size>
class InterpolatedSensor : public VoltsSensor {
public:
	InterpolatedSensor(uint8_t adc_pin, float volt_ref, float interp_table[interp_table_size][2]) :
		VoltsSensor(adc_pin, volt_ref) {
			for (uint16_t i = 0; i < interp_table_size; ++i) {
				x_[i] = interp_table[i][0];
				y_[i] = interp_table[i][1];
			}			
		}

	bool Read(float& quantity) {
		float volts;
		if (VoltsSensor::Read(volts)) {
			uint16_t lo = static_cast<uint16_t>(0);
			uint16_t hi = interp_table_size-1;
			if (util::algorithm::FindRange<float>(x_, volts, lo, hi)) {
				const auto f = (volts - x_[lo]) / (x_[hi] - x_[lo]);
				quantity = (1.0-f)*(y_[lo]) + f*y_[hi];
				return true;
			}
		}		
		return false;
	}

private:
	float x_[interp_table_size];
	float y_[interp_table_size];
};

} // namespace sensor

#endif /* SENSOR_ANALOG_H_ */