/*
 * sensor_volts.cpp
 *
 * Created: 6/28/2017 11:55:57 PM
 *  Author: roncato
 */ 

#include "sensors/sensor_volts.h"

namespace {

uint8_t instances{};

}

bool sensor::VoltsSensor::Read(float& quantity) {
	mal::reg::Or(mal::reg::kADMuxReg, adc_pin_);
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kADControlStatusReg, mal::reg::kADCIntFlagBit>::ClearBit();
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kADControlStatusReg, mal::reg::kADCStartBit>::SetBit(); // starts conversion
	while (!mal::reg::Access<uint8_t, uint8_t, mal::reg::kADControlStatusReg, mal::reg::kADCIntFlagBit>::GetBit());
	uint16_t sample = mal::reg::GetVal<uint8_t, uint16_t>(mal::reg::kADCDataRegL);
	quantity = volt_ref_ * sample / kADCResolution;
	return true;
}

sensor::SensorReturn sensor::VoltsSensor::Open() {
	if (!is_open) {
		mal::reg::Access<uint8_t, uint8_t, mal::reg::kADControlStatusReg, mal::reg::kADCEnableBit>::SetBit();
		mal::reg::SetBit(mal::reg::kDInputDisableReg, adc_pin_);
		++instances;
		is_open = true;
		return SENSOR_RETURN_OKAY;
	}	
	return SENSOR_RETURN_NOT_AFFECTED;
};

sensor::SensorReturn sensor::VoltsSensor::Close() {
	if (is_open) {
		if (--instances <= 0) {
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kADControlStatusReg, mal::reg::kADCEnableBit>::ClearBit();
		}
		mal::reg::ClearBit(mal::reg::kDInputDisableReg, adc_pin_);
		is_open = false;
		return SENSOR_RETURN_OKAY;
	}	
	return SENSOR_RETURN_NOT_AFFECTED;
}
