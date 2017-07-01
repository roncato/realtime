///*
 //* example_algorithms.cpp
 //*
 //* Created: 6/25/2017 2:51:59 PM
 //*  Author: roncato
 //*/ 
//
//#include "mal/mal_interrupt.h"
//#include "mal/mal_time.h"
//#include "mal/mal_cpu.h"
//
//#include "util/util_random.h"
//#include "util/util_algorithm.h"
//
//int main(void)
//{
	//mal::irq::EnableAll();
	//mal::time::Initialize();
	//util::random::Initialize(mal::time::Micros() + 1000);
//
	//constexpr uint16_t size = 100;
	//uint16_t lo = 0;
	//uint16_t hi = size-1;
	//uint32_t array[size];
//
	//// sort
	//for (auto i = 0; i < size; ++i) {
		//array[i] = util::random::NextUint32();
	//}
	//auto start = mal::time::Micros();
	//util::algorithm::Sort(array, lo, hi);
	//volatile auto diff = mal::time::Micros() - start;
	//volatile auto success = util::algorithm::IsSorted(array, size);
//
	//lo = 0;
	//hi = size-1;
	//auto needle = array[size/2] + 1;
	//start = mal::time::Micros();
	//success = util::algorithm::FindRange(array, needle, lo, hi);
	//diff = mal::time::Micros() - start;
//
	//while(1)
	//{
		//mal::cpu::NOP();
	//}
//
//}