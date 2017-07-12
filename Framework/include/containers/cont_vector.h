/*
 * Vector.h
 *
 * Created: 7/3/2017 4:56:45 PM
 *  Author: roncato
 */ 

#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>
#include <stdint.h>

#include "containers/containers.h"

namespace containers {

constexpr uint16_t kVectorDefaultCapacity = 10U;

template <class T>
class Vector {
public:
	Vector() : Vector(containers::kVectorDefaultCapacity) {}
	Vector(uint16_t capacity) : size_{}, capacity_{capacity}, store_{nullptr} {}
	Vector(const Vector<T>& that) {
		CopyTo(that, *this);
	}
	~Vector();
	void Add(const T& elem);
	void Add(uint16_t index, const T& elem);
	void Remove();
	void Remove(uint16_t index);
	bool Get(uint16_t index, T& elem);
	uint16_t Size() const {
		return size_;
	}
	bool IsEmpty() const {
		return size_ == 0;
	}
	void Clear();
	void CopyTo(const Vector<T>& from, Vector<T>& to);
	const Vector<T>& operator=(Vector<T>& that) {
		CopyTo(that, *this);
		return *this;
	}
private:
	uint16_t size_;
	uint16_t capacity_;
	T* store_;
	bool EnsureCapacity();
};

} // namespace containers

template <class T>
containers::Vector<T>::~Vector() {
	Clear();
}

template <class T>
bool containers::Vector<T>::EnsureCapacity() {
	if (!store_) {
		store_ = reinterpret_cast<T*>(malloc(capacity_*sizeof(T)));
	} else if (size_ >= capacity_) {
		capacity_ *= 1.5;
		auto const new_store = reinterpret_cast<T*>(malloc(capacity_*sizeof(T)));
		if (new_store) {
			for (uint16_t i = 0; i < size_; ++i) {
				new_store[i] = store_[i];
			}
			free((void*)store_);
			store_ = new_store;
		}
		return new_store;
	}
	return store_;
}

template <class T>
void containers::Vector<T>::Add(const T& elem) {
	if (EnsureCapacity()) {
		store_[size_++] = elem;	
	}
}

template <class T>
void containers::Vector<T>::Add(uint16_t index, const T& elem) {
	if (index < size_ && EnsureCapacity()) {
		if (size_ > 0) {
			for (auto i = size_; i > index; --i) {
				store_[i] = store_[i-1];
			}
		}
		store_[index] = elem;
		size_++;		
	}
}

template <class T>
void containers::Vector<T>::Remove() {
	if (size_ > 0) {
		--size_;
	}
}

template <class T>
void containers::Vector<T>::Remove(uint16_t index) {
	if (size_ > 0) {
		const auto hi = size_ - 1;
		for (auto i = index; i < hi; ++i) {
			store_[i] = store_[i+1];
		}
		--size_;
	}	
}

template <class T>
bool containers::Vector<T>::Get(uint16_t index, T& elem) {
	if (index < size_) {
		elem = store_[index];
		return true;
	}
	return false;
}

template <class T>
void containers::Vector<T>::Clear() {
	free((void*)store_);
	capacity_ = kVectorDefaultCapacity;
	store_ = nullptr;
	size_ = 0;
}

template <class T>
void containers::Vector<T>::CopyTo(const Vector<T>& from, Vector<T>& to) {
	to.store_ = reinterpret_cast<T*>(malloc(from.capacity_*sizeof(T)));
	if (to.store_) {
		to.capacity_ = from.capacity_;
		to.size_ = from.size_;
		for (uint16_t i = 0; i < from.size_; ++i) {
			to.store_[i] = from.store_[i];
		}
	}	
}

#endif /* VECTOR_H_ */