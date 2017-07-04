/*
 * example_vector.cpp
 *
 * Created: 7/3/2017 5:05:20 PM
 *  Author: roncato
 */ 

 #include "mal/mal_cpu.h"

#include "containers/cont_vector.h"

namespace example {

void Vector(void) {
	containers::Vector<volatile uint8_t> vector;
	vector.Add(1);
	vector.Add(2);
	vector.Add(3);
	volatile uint8_t el0;
	volatile uint8_t el1;
	volatile uint8_t el2;

	vector.Get(0, el0);
	vector.Get(1, el1);
	vector.Get(2, el2);

	vector.Remove();
	vector.Get(0, el0);
	vector.Get(1, el1);
	vector.Add(3);
	vector.Remove(1);
	vector.Get(0, el0);
	vector.Get(1, el1);
	containers::Vector<volatile uint8_t> vector2 = vector;
	containers::Vector<volatile uint8_t> vector3;
	vector3 = vector;

	while (1) {
		mal::cpu::NOP();
	}
}

} // namespace example