/*
 * PinChangeInterrupt.cpp
 *
 * Created: 6/10/2017 5:25:18 PM
 * Author : roncato
 */ 


#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "mal/mal_interrupt.h"
#include "mal/mal_time.h"
#include "mal/mal_reg.h"
#include "mal/mal_reg_access.h"
#include "mal/mal_pincint.h"
#include "mal/mal_cpu.h"

#include "util/util_time.h"
#include "util/util_timer.h"

#include "sensors/sensor_servo.h"

namespace
{

constexpr uint8_t kPort5Bit = 5UL;
constexpr uint8_t kPort4Bit = 4UL;

constexpr uint8_t kPin4 = 20UL;
constexpr uint8_t kPin5 = 21UL;

void ServoEventHandler4(void* context)
{
	sensor::ServoSensor* sensor = reinterpret_cast<sensor::ServoSensor*>(context);
	switch (sensor->GetState())
	{
		case sensor::ServoSensor::SS_STATE_INITIAL:
		case sensor::ServoSensor::SS_STATE_IDLE:
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort4Bit>::ClearBit();
			break;
		case sensor::ServoSensor::SS_STATE_PROBING:
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort4Bit>::SetBit();
			break;
	}
}

void ServoEventHandler5(void* context)
{
	sensor::ServoSensor* sensor = reinterpret_cast<sensor::ServoSensor*>(context);
	switch (sensor->GetState())
	{
		case sensor::ServoSensor::SS_STATE_INITIAL:
		case sensor::ServoSensor::SS_STATE_IDLE:
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort5Bit>::ClearBit();
		break;
			case sensor::ServoSensor::SS_STATE_PROBING:
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort5Bit>::SetBit();
		break;
	}
}

}

namespace example {

void SensorServo(void) {

	mal::irq::EnableAll();
	mal::time::Initialize();

	mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegD, kPort4Bit>::ClearBit();
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegD, kPort4Bit>::ClearBit();

	mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegD, kPort5Bit>::ClearBit();
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegD, kPort5Bit>::ClearBit();

	mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort4Bit>::ClearBit();
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, kPort4Bit>::SetBit();

	mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort5Bit>::ClearBit();
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, kPort5Bit>::SetBit();

	sensor::ServoSensor servo_sensor4(kPin4);
	sensor::ServoSensor servo_sensor5(kPin5);
	
	servo_sensor4.Open();
	servo_sensor5.Open();
	
	servo_sensor4.SetEventHandler(nullptr, ServoEventHandler4);
	servo_sensor5.SetEventHandler(nullptr, ServoEventHandler5);

	util::time::Timer timer;
   
    while (1) {
		mal::cpu::NOP();
    }
}

} // namespace example