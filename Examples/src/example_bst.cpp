/*
 * example_bst.cpp
 *
 * Created: 7/12/2017 7:14:57 PM
 *  Author: roncato
 */ 

#include "mal/mal_cpu.h"
#include "mal/mal_interrupt.h"
#include "mal/mal_time.h"

#include "containers/cont_bst.h"
#include "containers/cont_linkedlist.h"

volatile mal::time::time_t diff;

namespace example {

void Bst(void) {

	mal::irq::EnableAll();
	mal::time::Initialize();
	containers::Bst<char, int16_t> bst;

	bst.Add('S', 6);
	bst.Add('X', 7);
	bst.Add('E', 2);
	bst.Add('R', 5);
	bst.Add('A', 0);
	bst.Add('C', 1);
	bst.Add('R', 5);
	bst.Add('H', 3);
	bst.Add('M', 4);

	containers::Entry<char, int16_t> entry{};
	bst.Get('S', entry);
	bst.Get('M', entry);
	bst.Get('C', entry);
	bst.Get('A', entry);
	bst.Get('R', entry);
	bst.Get('H', entry);

	bst.Floor('P', entry);
	bst.Ceiling('P', entry);

	containers::LinkedList<containers::Entry<char, int16_t>> list;
	auto micros1 = mal::time::Micros();
	bst.TraverseRecursive(list);
	diff = mal::time::Micros() - micros1;

	containers::LinkedListIterator<containers::Entry<char, int16_t>> iterator(&list);
	while (iterator.HasNext()) {
		entry = iterator.Current();
		iterator.Advance();
	}

	bst.Remove('E');
	bst.Remove('S');
	bst.Remove('A');
	bst.Remove('R');
	
	while (1) {
		mal::cpu::NOP();
	}

}

} // namespace containers