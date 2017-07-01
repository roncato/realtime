/*
 * util_timer.h
 *
 * Created: 6/12/2017 8:38:29 AM
 *  Author: roncato
 */ 


#ifndef UTIL_TIMER_H_
#define UTIL_TIMER_H_

#include "mal/mal_time.h"

namespace util {
namespace time {

class Timer {
public:	
	Timer() : start_time_{mal::time::Micros()} 
	{}

	void Reset() {
		start_time_ = mal::time::Micros();
	}

	mal::time::time_t Elapsed() {
		return mal::time::Micros() - start_time_;
	}

	/*
		* General function to wait for time to elapse
	*/
	void AwaitMillis(mal::time::time_t duration);

	/*
		* Await microseconds to elapse, use this function for values less than 16383
	*/
	void AwaitMicros(mal::time::time_t micros);

private:
	mal::time::time_t start_time_;	
};

} // namespace timer
} // namespace util

#endif /* UTIL_TIMER_H_ */