///*
 //* example_servo_actuator.cpp
 //*
 //* Created: 6/20/2017 11:34:04 PM
 //*  Author: roncato
 //*/ 
//
//#include "mal/mal_time.h"
//#include "mal/mal_interrupt.h"
//#include "mal/mal_cpu.h"
//#include "mal/mal_reg.h"
//#include "mal/mal_reg_access.h"
//#include "actuators/actuator_servo.h"
//
//namespace {
//
//constexpr uint8_t kPort13Bit = 0x05UL;
//constexpr uint8_t kPort12Bit = 0x04UL;
//
//}
//
//int main(void)
//{
	//mal::irq::EnableAll();
	//mal::time::Initialize();
//
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort12Bit>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, kPort12Bit>::SetBit();
//
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort13Bit>::ClearBit();
	//mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, kPort13Bit>::SetBit();
//
	//actuator::ServoActuator servo12(mal::reg::kPortRegB, kPort12Bit);
	//actuator::ServoActuator servo13(mal::reg::kPortRegB, kPort13Bit);
	//
	//servo12.Open();
	//servo12.Write(1500);
//
	//servo13.Open();
	//servo13.Write(1500);
//
	//while (1) 
	//{
		//mal::cpu::NOP();
	//}
//}