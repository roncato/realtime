///*
 //* example_actuator_pwm.cpp
 //*
 //* Created: 7/1/2017 9:01:46 AM
 //*  Author: roncato
 //*/ 
//
//#include "mal/mal_interrupt.h"
//#include "mal/mal_time.h"
//#include "util/util_time.h"
//#include "util/util_timer.h"
//#include "mal/mal_reg.h"
//#include "mal/mal_reg_access.h"
//#include "mal/mal_cpu.h"
//#include "actuators/actuator_pwm.h"
//#include "util/util_random.h"
//
//int main(void)
//{
//
	//mal::irq::EnableAll();
	//mal::time::Initialize();
	//util::random::Initialize(1U);
//
	//// 3
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegD, 0x03>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegD, 0x03>::SetBit();
	//
	//// 5
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegD, 0x05>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegD, 0x05>::SetBit();
//
	////6
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegD, 0x06>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegD, 0x06>::SetBit();
//
	//// 9
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, 0x01>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, 0x01>::SetBit();
//
	//// 10
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, 0x02>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, 0x02>::SetBit();
//
	//// 11
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, 0x03>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, 0x03>::SetBit();
//
	////13
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, 0x05>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, 0x05>::SetBit();
//
	//actuator::PwmActuator<actuator::PWM_OUTPUT_A, mal::time::TIMER_0, actuator::PWM_FAST> actuator0A;
	//actuator::PwmActuator<actuator::PWM_OUTPUT_B, mal::time::TIMER_0, actuator::PWM_FAST> actuator0B;
//
	//actuator::PwmActuator<actuator::PWM_OUTPUT_A, mal::time::TIMER_1, actuator::PWM_FAST> actuator1A;
	//actuator::PwmActuator<actuator::PWM_OUTPUT_B, mal::time::TIMER_1, actuator::PWM_FAST> actuator1B;
//
	//actuator::PwmActuator<actuator::PWM_OUTPUT_A, mal::time::TIMER_2, actuator::PWM_NORMAL> actuator2A;
	//actuator::PwmActuator<actuator::PWM_OUTPUT_B, mal::time::TIMER_2, actuator::PWM_NORMAL> actuator2B;
//
	//// 6
	//actuator0A.Open();
	//actuator0A.Write(127);
//
	//// 5
	//actuator0B.Open();
	//actuator0B.Write(127);
//
	//// 9
	//actuator1A.Open();
	//actuator1A.Write(127);
//
	//// 10
	//actuator1B.Open();
	//actuator1B.Write(127);
//
	//// 11
	//actuator2A.Open();
	//actuator2A.Write(127);
//
	//// 3
	//actuator2B.Open();
	//actuator2B.Write(127);
//
	//util::time::Timer timer;
//
	//while(1)
	//{
		//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, 0x05>::NotBit();
		//actuator0A.Write(util::random::NextUint32() % 255);
		//actuator0B.Write(util::random::NextUint32() % 255);
		//actuator1A.Write(util::random::NextUint32() % 255);
		//actuator1B.Write(util::random::NextUint32() % 255);
		//actuator2A.Write(util::random::NextUint32() % 255);
		//actuator2B.Write(util::random::NextUint32() % 255);
		//timer.AwaitMillis(1000);
	//}
//
//}