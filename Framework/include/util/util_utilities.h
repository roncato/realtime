/*
 * util_utilities.h
 *
 * Created: 6/25/2017 3:00:10 PM
 *  Author: roncato
 */ 


#ifndef UTIL_UTILITIES_H_
#define UTIL_UTILITIES_H_

namespace util {
namespace utilities {

/*
 * Calculates the minimum between two values
 */
template <typename T>
T Min(T l, T r) {
	return l < r ? l : r;
}

/*
 * Calculates the maximum between two values
 */
template <typename T>
T Max(T l, T r) {
	return l > r ? l : r;
}

/*
 * Swaps to elements
 */
template <typename T>
void Swap(T& l, T& r) {
	T tmp = l;
	l = r;
	r = tmp;
}

/*
 * Calculates the average of segment in the array. Does not check for division for 0
 */
template <typename T>
float Avg(T* array, uint16_t lo, uint16_t hi) {
	T sum{};
	auto k = lo;
	for (; k <= hi; ++k) {
		sum += array[k];
	}
	return (sum / k);
}

template <typename T>
float Abs(const T& value) {
	return value < 0 ? -value : value;
}

}

}



#endif /* UTIL_UTILITIES_H_ */