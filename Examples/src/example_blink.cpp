/*
 * example_blink.cpp
 *
 * Created: 7/1/2017 12:34:40 PM
 *  Author: roncato
 */ 

#include "mal/mal_interrupt.h"
#include "mal/mal_time.h"
#include "mal/mal_reg.h"
#include "mal/mal_reg_access.h"
#include "util/util_timer.h"
#include "actuators/actuator_pwm.h"

namespace example {

void Blink(void) {

	constexpr uint16_t kDelay = 1000;

	mal::irq::EnableAll();
	mal::time::Initialize();

	// turn off led and set as output
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, 0x05>::ClearBit();
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, 0x05>::SetBit();

	util::time::Timer timer;

	while(1) {
		mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, 0x05>::NotBit();
		timer.AwaitMillis(kDelay);
	}
}

} // namespace example