/*
 * mal_time.cpp
 *
 * Created: 6/12/2017 6:24:09 AM
 *  Author: roncato
 */ 

#include <avr/interrupt.h>

#include "mal/mal_time.h"
#include "mal/mal_reg.h"
#include "mal/mal_reg_access.h"
#include "mal/mal_interrupt.h"

namespace {

volatile bool is_initialized{false};

constexpr mal::time::time_t kTimerPrescaler = 64U;
constexpr mal::time::time_t kMicroSecondsPerTimer0Tick = CLOCK_CYCLES_TO_MICROSECOND(kTimerPrescaler);
constexpr mal::time::time_t kMicroSecondsPerTimer0Overflow = kMicroSecondsPerTimer0Tick * 256;
constexpr uint16_t kMillisInc = kMicroSecondsPerTimer0Overflow / 1000;
constexpr uint16_t kFractInc = (kMicroSecondsPerTimer0Overflow % 1000) >> 3U;
constexpr uint32_t kFractMax = (1000 >> 3);

volatile mal::time::time_t timer0_millis{};
volatile uint8_t timer0_fract{};
volatile uint32_t timer0_overflow_count{};

}

ISR(TIMER0_OVF_vect) {

	register auto m = timer0_millis;
	register auto f = timer0_fract;

	m += kMillisInc;
	f += kFractInc;

	if (f >= kFractMax) {
		f -= kFractMax;
		m += 1;
	}

	timer0_fract = f;
	timer0_millis = m;
	++timer0_overflow_count;
}

void mal::time::Initialize() {

	if (!is_initialized) {
		
		// clear timer0 overflow flag
		mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerIntFlagReg0, 0x01U>::ClearBit();

		// prescaler of 64
		mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg0B, 0x01U>::SetBit();
		mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg0B, 0x00U>::SetBit();

		// enable timer0 overflow interrupt enabled
		mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerInterruptMskReg0, 0x00U>::SetBit();

		is_initialized = true;
	}
}

void mal::time::Uninitialize() {
	if (is_initialized) {
		mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerInterruptMskReg0, 0x00U>::ClearBit();
		mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerIntFlagReg0, 0x01U>::ClearBit();

		is_initialized = false;
	}
}

inline bool mal::time::IsInitialized() {
	return is_initialized;
}

mal::time::time_t mal::time::Micros() {
	const auto status_reg = mal::reg::Access<uint8_t, uint8_t, mal::reg::kStatusReg>::GetVal();
	mal::irq::DisableAll();
	register const auto m = timer0_overflow_count;
	register const auto t = mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerCounterReg0>::GetVal();
	mal::reg::SetVal(mal::reg::kStatusReg, status_reg);	
	return ((m << 8) + t) * (kTimerPrescaler / CLOCK_CYCLES_PER_MICROSECOND());
}

mal::time::time_t mal::time::MicrosInt() {
	register const auto m = timer0_overflow_count;
	register const auto t = mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerCounterReg0>::GetVal();
	return ((m << 8) + t) * (kTimerPrescaler / CLOCK_CYCLES_PER_MICROSECOND());
}

mal::time::time_t mal::time::Millis() {
	mal::time::time_t m;
	const auto status_reg = mal::reg::GetVal<uint8_t, uint8_t>(mal::reg::kStatusReg);
	mal::irq::DisableAll();
	m = timer0_millis;
	mal::reg::SetVal(mal::reg::kStatusReg, status_reg);
	return m;
}