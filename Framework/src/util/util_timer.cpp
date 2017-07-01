/*
 * util_timer.cpp
 *
 * Created: 6/12/2017 8:38:59 AM
 *  Author: roncato
 */ 

#include "util/util_timer.h"
#include "mal/mal_time.h"
#include "mal/mal_cpu.h"

void util::time::Timer::AwaitMillis(mal::time::time_t millis) {
	auto start_time = mal::time::Micros();
	while (millis > 0) {
		while (millis > 0 && static_cast<int32_t>(mal::time::Micros() - start_time) >= 1000) {
			millis--;
			start_time += 1000;
		}
	}
}

void util::time::Timer::AwaitMicros(mal::time::time_t micros) {  // CALL overhead 12 cycles

#if F_CPU >= 16000000L
	
	// for the 16 MHz clock

	if (micros <= 1) { //  = 5 cycles
		return; 
	}

	// Multiply by 4 as each loop has 4 cycles at 1/16 us or 0.25 us
 	micros <<= 2; // 8 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 29 + 4 = 33 / 4 ~ 8
	micros -= 8; // 4 cycles,

#elif F_CPU >= 8000000L
	
	// for the 16 MHz clock

	if (micros <= 2) {
		return; //  3 cycles, (4 when true)
	}

	// Multiply by 2 as each loop has 4 cycles at 1/8 us or 0.5 us
	micros <<= 1;

	// account for the time taken in the preceeding commands.
	// we just burned 19 (21) cycles above, remove 5, (5*4=20)
	// us is at least 8 so we can substract 5
	micros -= 4; // = 2 cycles,

#endif

	// busy wait
	asm volatile (
		"1: subi %A0,1"	"\n" // 1 cycles
		"sbci %B0,0"	"\n" // 1 cycles
		"brcc 1b" : : "r" (micros) // 2 cycles
	);

	// return 4 cycles
}