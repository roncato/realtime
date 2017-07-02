/*
 * example_sensor_analog.cpp
 *
 * Created: 6/28/2017 8:23:19 PM
 *  Author: roncato
 */ 

#include "mal/mal_interrupt.h"
#include "mal/mal_time.h"
#include "mal/mal_cpu.h"
#include "mal/mal_reg.h"

#include "sensors/sensor_volts.h"
#include "sensors/sensor_interpolated.h"
#include "sensors/sensor_function.h"

namespace example {

void SensorAnalog(void) {

	constexpr uint8_t adc_pin = 5U;
	constexpr float volt_ref = 5.0;

	float interp_table[2][2] = {{0.0,0.0},{5.0,1.0}}; // percent

	sensor::InterpolatedSensor<2U> interpolate_sensor(adc_pin, volt_ref, interp_table);
	interpolate_sensor.Open();
	float value;
	interpolate_sensor.Read(value);
	interpolate_sensor.Close();

	sensor::VoltsSensor volts_sensor(adc_pin, volt_ref);
	volts_sensor.Open();
	float volts;
	volts_sensor.Read(volts);
	volts_sensor.Close();

	sensor::FunctionSensor func_sensor(adc_pin, volt_ref, [](const float& v, float& quantity) {
		quantity = v;
		return true;
	});
	func_sensor.Open();
	func_sensor.Read(volts);
	func_sensor.Close();

	while(1) {
		mal::cpu::NOP();
	}
}

} // namespace example