/*
 * example_await.cpp
 *
 * Created: 6/10/2017 5:25:18 PM
 * Author : roncato
 */ 

#include <avr/io.h>
#include <string.h>

#include "mal/mal_interrupt.h"
#include "mal/mal_time.h"
#include "util/util_time.h"
#include "util/util_timer.h"
#include "mal/mal_reg.h"
#include "mal/mal_reg_access.h"

namespace example {

void Timer(void) {
	mal::irq::EnableAll();
	mal::time::Initialize();

	// Make pins as output
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kPortRegB, 0x05UL>::ClearBit();
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kDataDirectionRegB, 0x05UL>::SetBit();

	util::time::Timer timer;

    /* Replace with your application code */
    while (1) {
		mal::reg::Access<uint32_t, uint32_t, mal::reg::kPortRegB, 0x05UL>::NotBit();
		const auto micros1 = mal::time::Micros();
		timer.AwaitMicros(10);
		const auto micros2 = mal::time::Micros();
		volatile auto diff = micros2 - micros1;
		if (diff > 0) {
			diff = 0;
		}
    }
}

} // namespace example
