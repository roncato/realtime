/*
 * example_movingavg.cpp
 *
 * Created: 6/25/2017 6:09:23 PM
 *  Author: roncato
 */ 

#include "mal/mal_interrupt.h"
#include "mal/mal_time.h"
#include "util/util_random.h"
#include "mal/mal_cpu.h"

#include "filters/filter_movingavg.h"

namespace example {

void MovingAverage(void) {
	
	constexpr uint16_t capacity = 20;
	constexpr uint16_t nominal = 1000;
	constexpr uint16_t variance = 20;

	mal::irq::EnableAll();
	mal::time::Initialize();
	util::random::Initialize(mal::time::Micros());

	uint16_t values[capacity];

	for (auto i = static_cast<uint16_t>(0UL); i < capacity; ++i) {
		values[i] = nominal + util::random::NextUint32() % variance;
	}

	// Outlier removal comparison largest value
	filter::MovingAverage<uint16_t, capacity> filter1(true);
	for (auto i = static_cast<uint16_t>(0UL); i < capacity - 1; ++i) {
		filter1.Write(values[i]);
	}
	filter1.Write(nominal + 200);
	uint16_t avg1;
	filter1.Read(avg1);

	filter::MovingAverage<uint16_t, capacity> filter2;
	for (auto i = static_cast<uint16_t>(0UL); i < capacity - 1; ++i) {
		filter2.Write(values[i]);
	}
	filter2.Write(nominal + 200);
	uint16_t avg2;
	filter2.Read(avg2);

	// Outlier removal comparison smallest value
	for (auto i = static_cast<uint16_t>(0UL); i < capacity - 1; ++i) {
		filter1.Write(values[i]);
	}
	filter1.Write(nominal - 200);
	filter1.Read(avg1);

	for (auto i = static_cast<uint16_t>(0UL); i < capacity - 1; ++i) {
		filter2.Write(values[i]);
	}
	filter2.Write(nominal - 200);
	filter2.Read(avg2);

	while(1) {
		mal::cpu::NOP();
	}
}

} // namespace example