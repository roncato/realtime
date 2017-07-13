/*
 * example_priorityqueue.cpp
 *
 * Created: 7/4/2017 9:37:22 PM
 *  Author: roncato
 */ 

#include "mal/mal_cpu.h"

#include "containers/cont_priorityqueue.h"
#include "containers/cont_vector.h"

namespace example {

void PriorityQueue(void) {	
	containers::PriorityQueue<uint16_t, uint16_t> pq([](const uint16_t& l, const uint16_t& r) {return r < l;});
	containers::Vector<containers::Entry<uint16_t, uint16_t>> entries;

	pq.Add(10, 100);
	pq.Add(11, 111);
	pq.Add(18, 150);
	pq.Add(20, 200);
	pq.Add(5, 50);	
	pq.Add(30, 300);

	while (!pq.IsEmpty()) {
		containers::Entry<uint16_t, uint16_t> entry{};
		pq.Take(entry);
		entries.Add(entry);
	}

	pq.Add(10, 100);
	pq.Add(11, 111);
	pq.Add(18, 150);
	entries.Clear();
	containers::PriorityQueue<uint16_t, uint16_t>::PriorityQueueIterator iterator(&pq);
	while (iterator.HasNext()) {
		if (iterator.Current().key == 11) {
			iterator.Remove();
		} else {
			iterator.Advance();
		}		
	}

	while (1) {
		mal::cpu::NOP();
	}
}

} // namespace example