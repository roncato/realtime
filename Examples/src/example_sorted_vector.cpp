/*
 * example_sorted_vector.cpp
 *
 * Created: 7/8/2017 12:12:14 PM
 *  Author: roncato
 */
  
 #include "mal/mal_cpu.h"

 #include "containers/cont_sortedvector.h"
 #include "containers/cont_vector.h"

 namespace example {

void SortedVector(void) {
	containers::SortedVector<uint16_t, uint16_t> map;
	containers::Vector<containers::Entry<uint16_t, uint16_t>> entries;

	map.Add(10, 100);
	map.Add(20, 200);
	map.Add(15, 150);
	map.Add(5, 50);
	map.Add(10, 111);
	map.Add(30, 300);

	map.Contains(30);

	while (!map.IsEmpty()) {
		containers::Entry<uint16_t, uint16_t> entry{};
		map.Min(entry);
		entries.Add(entry);
		map.RemoveMin();
	}

	while (1) {
		mal::cpu::NOP();
	}
}

 } // namespace example