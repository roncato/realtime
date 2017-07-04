/*
* exmaple_list.cpp
*
* Created: 6/15/2017 10:19:46 PM
*  Author: roncato
*/ 

#include <stdlib.h>

#include "mal/mal_cpu.h"

#include "containers/cont_linkedlist.h"

namespace example {

void LinkedList(void) {
	containers::LinkedList<volatile uint8_t> list;
	list.Add(1);
	list.Add(2);
	list.Add(3);
	volatile uint8_t el0;
	volatile uint8_t el1;
	volatile uint8_t el2;

	list.Get(0, el0);
	list.Get(1, el1);
	list.Get(2, el2);

	list.Remove();
	list.Get(0, el0);
	list.Get(1, el1);
	list.Add(3);
	list.Remove(1);
	list.Get(0, el0);
	list.Get(1, el1);
	containers::LinkedList<volatile uint8_t> list2 = list;	
	containers::LinkedList<volatile uint8_t> list3;
	list3 = list;

	while(1) {
		mal::cpu::NOP();
	}
}

} // namespace example
