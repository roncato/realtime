/*
 * util_lock.h
 *
 * Created: 6/12/2017 1:35:55 PM
 *  Author: roncato
 */ 


#ifndef UTIL_LOCK_H_
#define UTIL_LOCK_H_

#include "mal/mal_interrupt.h"
#include "mal/mal_reg.h"
#include "mal/mal_reg_access.h"

#define INTERRUPT_LOCK() { util::lock::InterruptLock lock;
#define UNLOCK() }

namespace util {
namespace lock {

class InterruptLock {
	public:
	InterruptLock();
	~InterruptLock();
	private:
	uint8_t status_reg_;
};

} // namespace lock
} // namespace util

#endif /* UTIL_LOCK_H_ */