/*
 * example_sortedlist.cpp
 *
 * Created: 6/17/2017 4:24:46 PM
 *  Author: roncato
 */ 

#include "mal/mal_cpu.h"

#include "containers/cont_linkedlist.h"
#include "containers/cont_sortedlist.h"

namespace example {

void SortedList(void) {
	containers::SortedList<uint16_t, uint16_t> sortedlist;
	containers::LinkedList<containers::Entry<uint16_t, uint16_t>> entries;

	sortedlist.Add(10, 100);
	sortedlist.Add(20, 200);
	sortedlist.Add(15, 150);
	sortedlist.Add(5, 50);
	sortedlist.Add(10, 111);
	sortedlist.Add(30, 300);

	while (!sortedlist.IsEmpty()) {
		containers::Entry<uint16_t, uint16_t> entry{};
		sortedlist.Take(entry);
		entries.Add(entry);
	}

	while (1) {
		mal::cpu::NOP();
	}
}

} // namespace example