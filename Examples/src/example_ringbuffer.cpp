/*
 * example_ringbuffer.cpp
 *
 * Created: 6/25/2017 4:32:49 PM
 *  Author: roncato
 */ 

#include "containers/cont_ringbuffer.h"
#include "mal/mal_cpu.h"

namespace example {

void RingBuffer(void) {
	
	constexpr uint16_t capacity = 100;
	containers::RingBuffer<uint16_t, capacity> buffer;

	for (auto i = static_cast<uint16_t>(0UL); i < static_cast<uint16_t>(1.8 * capacity); ++i) {
		buffer.Place(i);
	}

	uint16_t copy[capacity];
	buffer.CopyTo(copy, 0, buffer.Size());

	while(1) {
		mal::cpu::NOP();
	}
}

} // namespace example