/*
 * example_queue.cpp
 *
 * Created: 6/17/2017 2:02:21 PM
 *  Author: roncato
 */ 

#include <stdlib.h>

#include "mal/mal_cpu.h"

#include "containers/cont_linkedlist.h"

namespace example {

void Queue(void) {
	containers::LinkedList<volatile uint8_t> list;
	list.Enqueue(1);
	list.Enqueue(2);
	list.Enqueue(3);
	volatile uint8_t el0;
	volatile uint8_t el1;
	volatile uint8_t el2;

	list.Dequeue(el0);
	list.Dequeue(el1);
	list.Dequeue(el2);

	while(1) {
		mal::cpu::NOP();
	}
}

} // namespace example