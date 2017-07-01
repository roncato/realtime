///*
 //* PinChangeInterrupt.cpp
 //*
 //* Created: 6/10/2017 5:25:18 PM
 //* Author : roncato
 //*/ 
//
//#include <avr/io.h>
//#include <string.h>
//
//#include "mal/mal_interrupt.h"
//#include "mal/mal_time.h"
//#include "util/util_time.h"
//#include "util/util_timer.h"
//#include "mal/mal_reg.h"
//#include "mal/mal_reg_access.h"
//#include "mal/mal_pincint.h"
//#include "mal/mal_cpu.h"
//#include "mal/mal_schedule.h"
//
//#include "sensors/sensor_servo.h"
//#include "actuators/actuator_servo.h"
//
//namespace
//{
//
//constexpr uint16_t kFreshRate = static_cast<uint16_t>(100000U);
//
//constexpr uint8_t kPort4Bit = 4UL;
//constexpr uint8_t kPort5Bit = 5UL;
//constexpr uint8_t kPort6Bit = 6UL;
//constexpr uint8_t kPort7Bit = 7UL;
//
//constexpr uint8_t kPin4 = 20UL;
//constexpr uint8_t kPin5 = 21UL;
//
//sensor::ServoSensor servo_sensor4(kPin4);
//sensor::ServoSensor servo_sensor5(kPin5);
//
//actuator::ServoActuator servo_actuator4(mal::reg::kPortRegB, kPort4Bit);
//actuator::ServoActuator servo_actuator5(mal::reg::kPortRegB, kPort5Bit);
//
//}
//
//int main(void)
//{
//
	//mal::irq::EnableAll();
	//mal::time::Initialize();
	//mal::schedule::Initialize();
//
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegD, kPort4Bit>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegD, kPort4Bit>::ClearBit();
//
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegD, kPort5Bit>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegD, kPort5Bit>::ClearBit();
//
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegD, kPort6Bit>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegD, kPort6Bit>::SetBit();
//
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegD, kPort6Bit>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegD, kPort6Bit>::SetBit();
//
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort4Bit>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, kPort4Bit>::SetBit();
//
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort5Bit>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, kPort5Bit>::SetBit();
//
	//util::time::Timer timer;
	//
	//servo_sensor4.Open();
	//servo_sensor5.Open();
//
	//servo_actuator4.Open();
	//servo_actuator5.Open();
//
	//mal::schedule::ScheduleTaskAtFixedRate(&servo_sensor5, kFreshRate, kFreshRate, [](void* context) {
		//sensor::ServoSensor* sensor = reinterpret_cast<sensor::ServoSensor*>(context);
		//servo_actuator5.Write(sensor->Read());
	//});
	//mal::schedule::ScheduleTaskAtFixedRate(&servo_sensor4, kFreshRate, kFreshRate, [](void* context) {
		//sensor::ServoSensor* sensor = reinterpret_cast<sensor::ServoSensor*>(context);
		//servo_actuator4.Write(sensor->Read());
	//});
	//mal::schedule::ScheduleTaskAtFixedRate(nullptr, kFreshRate, kFreshRate >> 1, [](void* context) {
		//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegD, kPort6Bit>::NotBit();
	//});
//
    //while (1) 
    //{
		////mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegD, kPort4Bit>::NotBit();
		////timer.AwaitMicros(2000);
		////mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegD, kPort4Bit>::NotBit();
		////timer.AwaitMicros(10000);
    //}
//}