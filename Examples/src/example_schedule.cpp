/*
 * example_schedule.cpp
 *
 * Created: 6/17/2017 7:12:43 PM
 *  Author: roncato
 */ 

#include "mal/mal_interrupt.h"
#include "util/util_time.h"
#include "util/util_timer.h"
#include "mal/mal_time.h"
#include "mal/mal_reg.h"
#include "mal/mal_reg_access.h"
#include "mal/mal_cpu.h"
#include "mal/mal_schedule.h"

namespace {

constexpr mal::time::time_t kServoRefreshPeriod = 20000;
constexpr mal::time::time_t kServoValue = 1500;
constexpr mal::time::time_t kLedDelay = 1000000;
constexpr uint8_t kPort13Bit = 0x05UL;
constexpr uint8_t kPort12Bit = 0x04UL;

void Task1(void* context) {
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort13Bit>::NotBit();
}

void Task2(void* context) {
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort12Bit>::NotBit();
	mal::schedule::ScheduleTask(nullptr, kLedDelay, Task2, true);
}

void Task3(void* context) {
	mal::cpu::NOP();
}

void Task4(void* context) {
	mal::cpu::NOP();
}

void Task5(void* context) {
	mal::schedule::ScheduleTask(nullptr, kLedDelay, Task5, true);
}

void Task6(void* context) {
	mal::schedule::ScheduleTask(nullptr, kLedDelay, Task6, true);
}

}

namespace example {

void Schedule(void) {
	mal::irq::EnableAll();
	mal::time::Initialize();
	mal::schedule::Initialize();

	mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort13Bit>::ClearBit();
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, kPort13Bit>::SetBit();

	mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, kPort12Bit>::ClearBit();
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, kPort12Bit>::SetBit();

	auto task1_handle = mal::schedule::ScheduleTaskAtFixedRate(nullptr, kLedDelay, kLedDelay, Task1);
	mal::schedule::ScheduleTask(nullptr, kLedDelay, Task2, false);
	mal::schedule::ScheduleTaskAtFixedRate(nullptr, kLedDelay, kLedDelay, Task3);
	mal::schedule::ScheduleTaskAtFixedRate(nullptr, kLedDelay, kLedDelay, Task4);
	mal::schedule::ScheduleTask(nullptr, kLedDelay, Task5, false);
	mal::schedule::ScheduleTask(nullptr, kLedDelay, Task6, false);

	if (task1_handle) {
		util::time::Timer timer;
		timer.AwaitMillis(5000);
		mal::schedule::UnscheduleTask(task1_handle);
		task1_handle = mal::schedule::kNullHandle;
	}

	while (1) {
		mal::cpu::NOP();		
	}
}

} // namespace example