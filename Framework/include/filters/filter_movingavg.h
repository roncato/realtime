/*
 * filters_movingavg.h
 *
 * Created: 6/25/2017 4:21:46 PM
 *  Author: roncato
 */ 


#ifndef FILTERS_MOVINGAVG_H_
#define FILTERS_MOVINGAVG_H_

#include <stdint.h>

#include "util/util_utilities.h"
#include "util/util_algorithm.h"
#include "containers/cont_ringbuffer.h"

namespace filter {

template <typename T, uint16_t capacity>
class MovingAverage {
public:
	MovingAverage() {}
	MovingAverage(bool eliminate_outliers) : eliminate_outliers_{eliminate_outliers} {}
	void Write(T sample) {
		buffer_.Place(sample);
	}
	bool Read(T& value) {
		if (buffer_.Size() > 0) {
			auto size = buffer_.Size();
			buffer_.CopyTo(aux_, 0, size);
			uint16_t lo = 0;
			uint16_t hi = size - 1;
			if (eliminate_outliers_ && size > 2U) {						
				util::algorithm::EliminateOutliers(aux_, lo, hi);
				value = static_cast<T>(util::utilities::Avg(aux_, lo, hi));		
			} else {
				value = buffer_[0];
			}				
			return true;		
		}
		return false;		
	}
private:
	bool eliminate_outliers_{false};
	T aux_[capacity];
	containers::RingBuffer<T, capacity> buffer_;
};

}

#endif /* FILTERS_MOVINGAVG_H_ */