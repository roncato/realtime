/*
 * util_lock.cpp
 *
 * Created: 6/12/2017 1:40:22 PM
 *  Author: roncato
 */ 

#include "util/util_lock.h"

util::lock::InterruptLock::InterruptLock() : status_reg_{mal::reg::Access<uint8_t, uint8_t, mal::reg::kStatusReg>::GetVal()} {
	mal::irq::DisableAll();
}

util::lock::InterruptLock::~InterruptLock() {
	mal::reg::SetVal(mal::reg::kStatusReg, status_reg_);
}