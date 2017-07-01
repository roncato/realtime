/*
 * mal_cpu.h
 *
 * Created: 6/11/2017 9:04:15 PM
 *  Author: roncato
 */ 


#ifndef MAL_CPU_H_
#define MAL_CPU_H_

#include <stdint.h>

#include <avr/pgmspace.h>

namespace mal {
namespace cpu {

inline void NOP() {
	asm volatile("nop	\n");
}

inline uint8_t ReadProgramMemory(volatile uint8_t* program_memory_ptr) {
	return pgm_read_byte(reinterpret_cast<uint16_t>(program_memory_ptr));
}

} // namespace cpu
} // namespace mal


#endif /* MAL_CPU_H_ */