/*
 * cont_ringarray.h
 *
 * Created: 6/25/2017 4:25:22 PM
 *  Author: roncato
 */ 


#ifndef CONT_RINGBUFFER_H_
#define CONT_RINGBUFFER_H_

#include <stdint.h>

namespace containers {

template <typename T, uint16_t capacity>
class RingBuffer {
public:
	void Place(const T& value) {
		if (position_ >= capacity) {
			position_ = 0;
		}
		array_[position_++] = value;
		if (size_ < capacity) {
			++size_;
		}
	}
	void Replace(uint16_t position, const T& value) {
		position = position % capacity;
		if (position >= size_) {
			return;
		}
		array_[position] = value;
	}
	T Get(uint16_t index) const {
		index += position_;
		index %= capacity;
		return array_[index];
	}
	void CopyTo(T* array, uint16_t offset, uint16_t len) const {
		for (auto i = static_cast<uint16_t>(0U), k = position_; i < len; ++i, ++k) {
			if (k >= capacity) {
				k = 0;
			}
			array[i + offset] = array_[k];
		}
	}
	uint16_t Size() const {
		return size_;
	}
	T operator[](uint16_t index) const {
		return Get(index);
	};
private:
	uint16_t position_{0};
	uint16_t size_{};
	T array_[capacity];
};

}

#endif /* CONT_RINGARRAY_H_ */