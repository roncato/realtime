/*
 * util_time.h
 *
 * Created: 6/12/2017 8:55:05 AM
 *  Author: roncato
 */ 


#ifndef UTIL_TIME_H_
#define UTIL_TIME_H_

#include "mal/mal_time.h"

namespace util {
namespace time {

template <typename time_value_t>
inline mal::time::time_t FromMicroSeconds(const time_value_t& value) {
	return static_cast<mal::time::time_t>(value);
}
template <typename time_value_t>
inline mal::time::time_t FromMilliSeconds(const time_value_t& value) {
	return static_cast<mal::time::time_t>(1000UL) * FromMicroSeconds(value);
}
template <typename time_value_t>
inline mal::time::time_t FromSeconds(const time_value_t& value) {
	return static_cast<mal::time::time_t>(1000UL) * FromMilliSeconds(value);
}
template <typename time_value_t>
inline mal::time::time_t FromMinutes(const time_value_t& value) {
	return static_cast<mal::time::time_t>(60UL) * FromSeconds(value);
}
template <typename time_value_t>
inline mal::time::time_t FromHours(const time_value_t& value) {
	return static_cast<mal::time::time_t>(60UL) * FromMinutes(value);
}
template <typename time_value_t>
inline mal::time::time_t FromDays(const time_value_t& value) {
	return static_cast<mal::time::time_t>(24UL) * FromHours(value);
}
template <typename time_value_t>
inline mal::time::time_t FromWeeks(const time_value_t& value) {
	return static_cast<mal::time::time_t>(7UL) * FromDays(value);
}

} // namespace time
} // namespace util


#endif /* UTIL_TIME_H_ */