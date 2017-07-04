/*
 * sensor_servo.cpp
 *
 * Created: 6/11/2017 9:24:47 PM
 *  Author: roncato
 */ 

#include <stdint.h>

#include "mal/mal_pincint.h"
#include "mal/mal_time.h"
#include "mal/mal_reg.h"
#include "mal/mal_reg_access.h"

#include "util/util_time.h"
#include "util/util_lock.h"

#include "sensors/sensor_servo.h"

sensor::SensorReturn sensor::ServoSensor::Open() {
	if (!is_open_) {
		state_ = SS_STATE_INITIAL;
		mal::irq::RegisterIrq(reinterpret_cast<void*>(this), pcint_number_, sensor::ServoSensor::Rising, mal::irq::IRQ_TRIGGER_RISING);
		mal::irq::RegisterIrq(reinterpret_cast<void*>(this), pcint_number_, sensor::ServoSensor::Falling, mal::irq::IRQ_TRIGGER_FALLING);
		is_open_ = true;
						
		return SENSOR_RETURN_OKAY;		
	}
	return SENSOR_RETURN_NOT_AFFECTED;
}

sensor::SensorReturn sensor::ServoSensor::Close() {
	if (is_open_) {
		mal::irq::UnregisterIrq(pcint_number_, mal::irq::IRQ_TRIGGER_RISING);
		mal::irq::UnregisterIrq(pcint_number_, mal::irq::IRQ_TRIGGER_FALLING);
		is_open_ = false;
		return SENSOR_RETURN_OKAY;		
	}
	return SENSOR_RETURN_NOT_AFFECTED;	
}

uint16_t sensor::ServoSensor::Read() const {
	return value_;
}

void sensor::ServoSensor::SetEventHandler(void* context, SensorReadEventHandler handler) {
	INTERRUPT_LOCK()
		event_handler_ = SensorEventRecord{context, handler};
	INTERRUPT_UNLOCK()
}

void sensor::ServoSensor::RemoveEventHandler() {
	INTERRUPT_LOCK()
		event_handler_ = SensorEventRecord{nullptr, nullptr};
	INTERRUPT_UNLOCK()
}

void sensor::ServoSensor::Rising(void* ptr) {
	const auto sensor = reinterpret_cast<ServoSensor*>(ptr);
	sensor->start_time_ = mal::time::MicrosInt();
}

void sensor::ServoSensor::Falling(void* ptr) {
	const auto sensor = reinterpret_cast<ServoSensor*>(ptr);
	const auto end_time = mal::time::MicrosInt();
	auto value = end_time - sensor->start_time_;
	if (value >= sensor::kServoMinPulseWidth && value <= sensor::kServoMaxPulseWidth) {
		sensor->value_ = static_cast<uint16_t>(value);
		if (sensor->event_handler_.handler) {
			sensor->event_handler_.handler(ptr);
		}
	}
}