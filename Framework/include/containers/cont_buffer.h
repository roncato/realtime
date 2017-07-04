/*
 * const_buffer.h
 *
 * Created: 7/3/2017 6:45:08 PM
 *  Author: roncato
 */ 


#ifndef CONST_BUFFER_H_
#define CONST_BUFFER_H_

#include <stdint.h>

namespace containers {

template <class T, uint8_t capacity>
class Buffer {
public:
	uint8_t Write(T data) {
		if (available <= capacity) {
			store_[write_index++] = data;
			++available;
			if (write_index >= capacity) {
				write_index = 0;
			}
			return static_cast<uint8_t>(1U);
		}
		return static_cast<uint8_t>(0);
	}
	uint8_t Write(T* data, uint8_t len) {
		uint8_t index = 0;
		while (index < len && available < capacity) {
			store_[write_index++] = data[index++];
			++available;
			if (write_index >= capacity) {
				write_index = 0;
			}
		}
		return index;
	}
	uint8_t Read(T& data) {
		if (available > 0) {
			data = store_[read_index++];
			--available;
			if (read_index >= capacity) {
				read_index = 0;
			}
			return static_cast<uint8_t>(1U);
		}
		return static_cast<uint8_t>(0);
	}
	uint8_t Read(T* data, uint8_t len) {
		uint8_t index = 0;
		while (index < len && available > 0) {
			data[index++] = store_[read_index++];
			--available;
			if (read_index >= capacity) {
				read_index = 0;
			}
		}
		return index;
	}
	uint8_t CopyTo(Buffer<T, capacity>& to) {
		return to.Write(store_, available);
	}
	inline const uint8_t Capacity() const {
		return capacity;
	}
	inline uint8_t Available() {
		return available;
	}
private:
	uint8_t write_index{};
	uint8_t read_index{};
	uint8_t available{};
	T store_[capacity];
};

} // namespace containers

#endif /* CONST_BUFFER_H_ */