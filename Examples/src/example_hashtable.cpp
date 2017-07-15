/*
 * example_hashtable.cpp
 *
 * Created: 7/15/2017 11:13:15 AM
 *  Author: roncato
 */ 

#include "mal/mal_cpu.h"
#include "mal/mal_interrupt.h"
#include "mal/mal_time.h"

#include "containers/cont_hashtable.h"
#include "containers/cont_linkedlist.h"

namespace example {

void HashTable(void) {

	mal::irq::EnableAll();
	mal::time::Initialize();
	containers::HashTable<char, int16_t> ht;

	ht.Add('S', 6);
	ht.Add('X', 7);
	ht.Add('E', 2);
	ht.Add('R', 5);
	ht.Add('A', 0);
	ht.Add('C', 1);
	ht.Add('R', 50);
	ht.Add('H', 3);
	ht.Add('M', 4);

	containers::Entry<char, int16_t> entry{};
	ht.Get('S', entry);
	ht.Get('M', entry);
	ht.Get('C', entry);
	ht.Get('A', entry);
	ht.Get('R', entry);
	ht.Get('H', entry);

	containers::HashTable<char, int16_t>::HashTableIterator iterator(&ht);
	while (iterator.HasNext()) {
		entry = iterator.Current();
		iterator.Advance();
	}

	ht.Remove('E');
	ht.Remove('S');
	ht.Remove('A');
	ht.Remove('R');
	
	while (1) {
		mal::cpu::NOP();
	}

}

}