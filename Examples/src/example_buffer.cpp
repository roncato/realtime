/*
 * example_buffer.cpp
 *
 * Created: 7/3/2017 7:07:33 PM
 *  Author: roncato
 */ 

#include "mal/mal_cpu.h"

#include "containers/const_buffer.h"
#include "util/util_random.h"

namespace example {

void Buffer(void) {

	constexpr uint8_t capacity = 64U;	
	
	util::random::Initialize(1U);

	containers::Buffer<uint8_t, capacity> buffer;
	uint8_t array[capacity];
	uint8_t written{};
	uint8_t read{};

	for (uint8_t i = 0; i < 32U; ++i) {
		written += buffer.Write(util::random::NextUint8());
	}

	buffer.Read(array, written / 2);
	buffer.Write(array, 2 * written);

	for (uint8_t i = 0; i < 2 * written; ++i) {
		read += buffer.Read(array[i]);
	}

	while (1) {
		mal::cpu::NOP();
	}
		
}

} // namespace example