/*
 * util_random.cpp
 *
 * Created: 6/25/2017 2:56:26 PM
 *  Author: roncato
 */ 

#include <stdint.h>

#include "util/util_random.h"

namespace {

volatile uint32_t state;

}

void util::random::Initialize(uint32_t seed) {
	state = seed;
}

uint16_t util::random::NextUint32() {
	state ^= state << 13;
	state ^= state >> 17;
	state ^= state << 5;
	return state;
}