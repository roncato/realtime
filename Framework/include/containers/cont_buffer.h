/*
 * const_buffer.h
 *
 * Created: 7/3/2017 6:45:08 PM
 *  Author: roncato
 */ 


#ifndef CONT_BUFFER_H_
#define CONT_BUFFER_H_

#include <stdint.h>

namespace containers {

template <class T, uint8_t capacity>
class Buffer {
public:
	uint8_t Write(const T& data) {
		const auto next_index = (write_index_ + 1U) % capacity;
		if (next_index != read_index_) {
			store_[write_index_++] = data;
			if (write_index_ >= capacity) {
				write_index_ = 0;
			}
			return static_cast<uint8_t>(1U);
		}
		return static_cast<uint8_t>(0);
	}
	uint8_t Write(T* data, uint8_t len) {
		auto next_index = (write_index_ + 1) % capacity;
		uint8_t index = 0;
		while (index < len && next_index != read_index_) {
			store_[write_index_++] = data[index++];
			if (write_index_ >= capacity) {
				write_index_ = 0;
			} 
			next_index = write_index_ + 1U;
		}
		return index;
	}
	uint8_t Read(T& data) {
		if (write_index_ != read_index_) {
			data = store_[read_index_++];
			if (read_index_ >= capacity) {
				read_index_ = 0;
			}
			return static_cast<uint8_t>(1U);
		}
		return static_cast<uint8_t>(0);
	}
	uint8_t Read(T* data, uint8_t len) {
		uint8_t index = 0;
		while (index < len && write_index_ != read_index_) {
			data[index++] = store_[read_index_++];
			if (read_index_ >= capacity) {
				read_index_ = 0;
			}
		}
		return index;
	}
	uint8_t CopyTo(Buffer<T, capacity>& to) {
		return to.Write(store_, Available());
	}
	void Clear() {
		write_index_ = 0;
		read_index_ = 0;
	}
	inline const uint8_t Capacity() const {
		return capacity;
	}
	inline uint8_t Available() {
		return read_index_ > write_index_ ? write_index_ + capacity - read_index_ : write_index_ - read_index_;
	}
private:
	volatile uint8_t write_index_{};
	volatile uint8_t read_index_{};
	T store_[capacity];
};

} // namespace containers

#endif /* CONT_BUFFER_H_ */