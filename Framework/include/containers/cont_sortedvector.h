/*
 * cont_sortedvector.h
 *
 * Created: 7/8/2017 10:05:35 AM
 *  Author: roncato
 */ 


#ifndef CONT_SORTEDVECTOR_H_
#define CONT_SORTEDVECTOR_H_

#include <stdlib.h>

#include <containers/containers.h>

namespace containers {

constexpr uint16_t kDefaultVectorCapacity = 10U;

template <class K, class V>
class SortedVector {
public:
	SortedVector(uint16_t capacity) : size_{0}, capacity_{capacity}, store_{nullptr} {}
	SortedVector() : SortedVector(kDefaultVectorCapacity) {}
	SortedVector(const SortedVector<K, V>& that) {
		CopyTo(that, *this);
	}
	~SortedVector() {
		Clear();
	}
	bool Add(const K& key, const V& value);
	void Clear() {
		capacity_ = 0;
		size_ = 0;
		free((void*)store_);
		store_ = nullptr;
	}
	bool IsEmpty() const {
		return size_ <= 0;
	}
	bool Rank(const K& key, uint16_t& rank);
	bool Get(const K& key, Entry<K, V>& entry);
	bool Select(uint16_t rank, Entry<K, V>& entry);
	bool Remove(const K& key);
	bool RemoveAt(uint16_t rank);
	bool Contains(const K& key) {
		uint16_t rank;
		return Rank(key, rank);
	}
	bool Ceiling(const K& key, Entry<K, V>& entry);
	bool Floor(const K& key, Entry<K, V>& entry);
	bool Min(Entry<K, V>& entry) {
		if (size_ > 0) {
			entry = store_[0];
			return true;
		}
		return false;
	}
	bool Max(Entry<K, V>& entry) {
		if (size_ > 0) {
			entry = store_[size_-1];
			return true;
		}
		return false;
	}
	bool RemoveMin(Entry<K, V>& entry) {
		if (size_ > 0) {
			entry = Entry<K, V> {store_[0].key, store_[0].value};
		}
		return RemoveAt(0);
	}
	bool RemoveMax(Entry<K, V>& entry) {
		const auto hi = size_-1;
		if (size_ > 0) {
			entry = Entry<K, V> {store_[hi].key, store_[hi].value};
		}
		return RemoveAt(hi);
	}
	void CopyTo(const SortedVector<K, V>& from, SortedVector<K, V>& to);
	const SortedVector<K, V>& operator=(SortedVector<K, V>& that) {
		CopyTo(that, *this);
		return *this;
	}
private:
	uint16_t size_;
	uint16_t capacity_;
	Entry<K, V>* store_;
	bool EnsureCapacity();
};

template <class K, class V>
bool containers::SortedVector<K, V>::Add(const K& key, const V& value) {
	uint16_t rank;
	if (Rank(key, rank)) {
		store_[rank].value = value;
		return true;
	} else if (EnsureCapacity()) {
		for (auto i = size_; i > rank; --i) {
			store_[i] = store_[i-1];
		}
		store_[rank] = Entry<K, V>{key,value};
		size_++;
		return true;
	}
	return false;
}

template <class K, class V>
bool containers::SortedVector<K, V>::Rank(const K& key, uint16_t& rank) {
	uint16_t lo = 0;
	if (size_ > 0) {
		uint16_t hi = size_ - 1;
		while (lo <= hi) {
			rank = lo + (hi - lo) / 2;
			if (key > store_[rank].key) {
				lo = rank + 1;
			} else if (key < store_[rank].key) {
				if (rank == 0) {
					hi = 0;
					break;
				} else {
					hi = rank - 1;
				}				
			} else {
				return true;
			}
		}
	}	
	rank = lo;
	return false;
}

template <class K, class V>
bool containers::SortedVector<K, V>::Get(const K& key, containers::Entry<K, V>& entry) {
	if (size_ > 0) {
		uint16_t rank;
		if (Rank(key, rank)) {
			entry = store_[rank];
			return true;
		}		
	}
	return false;
}

template <class K, class V>
bool containers::SortedVector<K, V>::Select(uint16_t rank, containers::Entry<K, V>& entry) {
	if (rank >= 0 && rank < size_) {
		entry = store_[rank];
		return true;
	}
	return false;
}

template <class K, class V>
bool containers::SortedVector<K, V>::Ceiling(const K& key, containers::Entry<K, V>& entry) {
	uint16_t rank;
	if (Rank(key, rank)) {
		entry = store_[rank];
		return true;
	} else if (size_ > 0 && rank < size_ - 1) {
		entry = store_[rank + 1];
		return true;
	}
	return false;
}

template <class K, class V>
bool containers::SortedVector<K, V>::Floor(const K& key, containers::Entry<K, V>& entry) {
	uint16_t rank;
	if (Rank(key, rank)) {
		entry = store_[rank];
		return true;
	} else if (rank > 0) {
		entry = store_[rank - 1];
		return true;
	}
	return false;
}

template <class K, class V>
bool containers::SortedVector<K, V>::Remove(const K& key) {
	uint16_t rank;
	if (Rank(key, rank)) {
		return Remove(rank);
	}
	return false;
}

template <class K, class V>
bool containers::SortedVector<K, V>::RemoveAt(uint16_t rank) {
	if (size_ > 0) {
		const auto hi = size_ - 1;
		for (auto i = rank; i < hi; ++i) {
			store_[i] = store_[i+1];
		}
		--size_;
		return true;	
	}
	return false;
}

template <class K, class V>
bool containers::SortedVector<K, V>::EnsureCapacity() {
	if (!store_) {
		store_ = reinterpret_cast<containers::Entry<K, V>*>(malloc(capacity_*sizeof(containers::Entry<K, V>)));
	} else if (size_ >= capacity_) {
		capacity_ *= 1.5;
		auto const new_store = reinterpret_cast<containers::Entry<K, V>*>(malloc(capacity_*sizeof(containers::Entry<K, V>)));
		if (new_store) {
			for (uint16_t i = 0; i < size_; ++i) {
				new_store[i] = store_[i];
			}
			free((void*)store_);
			store_ = new_store;
		} else {
			return new_store;
		}
	}
	return store_;
}

template <class K, class V>
void containers::SortedVector<K, V>::CopyTo(const SortedVector<K, V>& from, SortedVector<K, V>& to) {
	to.store_ = reinterpret_cast<containers::Entry<K, V>*>(malloc(from.capacity_*sizeof(containers::Entry<K, V>)));
	if (to.store_) {
		to.capacity_ = from.capacity_;
		to.size_ = from.size_;
		for (uint16_t i = 0; i < from.size_; ++i) {
			to.store_[i] = from.store_[i];
		}
	}
}

} // namespace containers

#endif /* CONT_SORTEDVECTOR_H_ */