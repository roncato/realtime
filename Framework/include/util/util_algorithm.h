/*
 * util_algorithm.h
 *
 * Created: 6/25/2017 12:43:02 PM
 *  Author: roncato
 */ 


#ifndef UTIL_ALGORITHM_H_
#define UTIL_ALGORITHM_H_

#include <stdint.h>

#include "util/util_utilities.h"

#include "containers/cont_stack.h"

namespace {

typedef struct {
	uint16_t lo;
	uint16_t hi;
} Boundaries;

template <typename T>
uint8_t GetFurthest(const float avg, const T& v1, const T& v2) {
	auto v1_dist = util::utilities::Abs(avg - v1);
	auto v2_dist = util::utilities::Abs(avg - v2);
	if (v1_dist > v2_dist) {
		return 0;
	} else {
		return 1;
	}
}

template <typename T>
void Merge(T* array, T* aux, uint16_t lo, uint16_t mid, uint16_t hi, bool (*less)(T,T)) {
	for (uint16_t k = lo; k <= hi; ++k) {
		aux[k] = array[k];
	}

	auto i = lo, j = mid + 1;
	for (auto k = lo; k <= hi; ++k) {
		if (i > mid) {
			array[k] = aux[j++];
		} else if (j > hi) {
			array[k] = aux[i++];
		} else if (less(aux[i], aux[j])) {
			array[k] = aux[i++];
		} else {
			array[k] = aux[j++];
		}
	}
}

template <typename T>
uint16_t Partition(T* array, uint16_t lo, uint16_t hi, bool (*less)(T,T) = [](T l, T r){return l < r;}) {
	auto pivot = array[lo];
	auto i = lo, j = hi + 1;
	while (1) {
		while (less(array[++i], pivot)) {
			if (i >= hi) {
				break;
			}
		}
		while (less(pivot, array[--j])) {
			if (j <= lo) {
				break;
			}
		}
		if (i >= j) {
			break;
		}
		util::utilities::Swap(array[i], array[j]);
	}
	util::utilities::Swap(array[lo], array[j]);
	return j;
}

template <typename T>
inline void sink(T* array, uint16_t k, uint16_t n, bool (*less)(T,T)) {
	while (2*k <= n) {
		auto j = 2*k;
		if (j < n && less(array[j], array[j+1])) {
			++j;
		}
		if (!less(array[k], array[j])) {
			break;
		}
		util::utilities::Swap(array[k], array[j]);
		k = j;
	}
}

}

namespace util {
namespace algorithm {

constexpr uint16_t kSortingThreshold = 15U;

template <typename T>
void Sort(T* array, uint16_t lo, uint16_t hi, bool (*less)(T,T) = [](T l, T r){return l < r;}) {
	auto size = hi - lo + 1;
	uint16_t h = static_cast<uint16_t>(1U);
	while (h < size/3U) {
		h = 3*h + 1;
	}
	while (h >= 1) {
		for (auto i = lo + h; i < size; ++i) {
			for (auto j = i; j >= lo + h && less(array[j], array[j-h]); j -= h) {
				util::utilities::Swap(array[j], array[j-h]);
			}
		}
		h /= 3U;
	}
}

template <typename T>
void MergeSort(T* array, T* aux, uint16_t lo, uint16_t hi, bool (*less)(T,T) = [](T l, T r){return l < r;}) {
	for (uint16_t sz = 1U; sz <= hi; sz += sz+sz) {
		for (uint16_t lk = 0; lk <= hi - sz; lk += sz+sz) {
			auto const mid = lk + sz - 1;
			auto const hk = util::utilities::Min(lk + sz + sz, hi);
			Merge(array, aux, lk, mid, hk, less);
		}
	}
}

template <typename T>
void QuickSort(T* array, uint16_t lo, uint16_t hi, bool (*less)(T,T) = [](T l, T r){return l < r;}) {
	containers::Stack<Boundaries> stack;
	stack.Push(Boundaries{lo, hi});
	Boundaries b;
	while (!stack.IsEmpty()) {
		if (stack.Pop(b) && b.lo < b.hi) {
			const auto j = Partition(array, b.lo, b.hi, less);
			stack.Push(Boundaries{b.lo, j});
			stack.Push(Boundaries{j+1, b.hi});
		}
	}
}

template <typename T>
void InsertionSort(T* array, uint16_t lo, uint16_t hi, bool (*less)(T,T) = [](T l, T r){return l < r;}) {
	for (auto i = lo + 1U; i <= hi; ++i) {
		for (auto j = i; j > 0 && less(array[j], array[j-1]); --j) {
			util::utilities::Swap(array[j], array[j-1]);
		}
	}
}

template <typename T>
void HeapSort(T* array, uint16_t n, bool (*less)(T,T) = [](T l, T r){return l < r;}) {
	for (auto k = n/2; k >= 1; --k) {
		sink(array, k-1, n-1, less);
	}
	while (n > 1) {
		util::utilities::Swap(array[0], array[--n]);
		sink(array, 0, n-1, less);
	}
}

template <typename T>
bool IsSorted(T* array, uint16_t size) {
	for (auto i = static_cast<uint16_t>(1U); i < size; ++i) {
		if (array[i - 1] > array[i]) {
			return false;
		}
	}
	return true;
}

template <typename T>
void EliminateOutliers(T* array, uint16_t& lo, uint16_t& hi) {
	auto size = hi - lo + 1;
	util::algorithm::Sort(array, lo, hi);
	while (1) {
		auto q1_index = (size >> 2U);
		auto q3_index = (size * 3U) >> 2U;
		auto iqr = util::utilities::Abs(1.5f * (array[q3_index] - array[q1_index]));
		auto avg = util::utilities::Avg(array, lo, hi);
		auto rank = GetFurthest(avg, array[lo], array[hi]);
		if (rank == 0) {
			if (util::utilities::Abs(avg - array[lo]) <= iqr) {
				break;
			}
			++lo;
		} else {
			if (util::utilities::Abs(avg - array[hi]) <= iqr) {
				break;
			}
			--hi;
		}
		size = hi - lo + 1;
	}
}

template <typename T>
bool FindRange(T* array, const T& v, uint16_t& lo, uint16_t& hi) {
	while (1) {
		if (lo > hi) {
			return false;
		}
		auto mid = lo + (hi - lo)/2;
		auto mid_v = array[mid];
		if (v == mid_v) {
			return true;
		} else if ((hi - lo) == 1U) {
			return v >= array[lo] && v <= array[hi];
		} else if (v > mid_v) {
			lo = mid;
		} else if (v < mid_v) {
			hi = mid;
		} 
	}
}

}
}

#endif /* UTIL_ALGORITHM_H_ */