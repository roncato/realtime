/*
 * const_hashtable.h
 *
 * Created: 7/15/2017 10:52:26 AM
 *  Author: roncato
 */ 


#ifndef CONT_HASHTABLE_H_
#define CONT_HASHTABLE_H_

#include "containers/containers.h"
#include "containers/cont_linkedlist.h"

namespace containers {

template <class K, class V, uint8_t capacity = 61>
class HashTable {
public:
	bool Add(const K& key, const V& value) {
		LinkedList<Entry<K, V>>* list = &store_[Hash(key)];
		const auto size = list->Size();
		if (list->AddOrReplace(key, Entry<K, V>{key, value}, KeyHit)) {
			if (list->Size() > size) {
				++size_;
			}
			return true;
		}
		return false;
	}
	bool Get(const K& key, containers::Entry<K, V>& entry) {
		return store_[Hash(key)].Get(key, entry, KeyHit);
	}
	bool Remove(const K& key) {
		if (store_[Hash(key)].Remove(key, KeyHit)) {
			--size_;
			return true;
		}
		return false;
	}
	bool Contains(const K& key) {
		return store_[Hash(key)].Contains(key, KeyHit);
	}
	void Clear() {
		for (uint8_t i = 0; i < capacity; ++i) {
			store_[i].Clear();
		}
		size_ = 0;
	}
	bool IsEmpty() {
		return size_ == 0;
	}
	uint8_t Size() {
		return size_;
	}
	class HashTableIterator {
	public:
		HashTableIterator(HashTable<K, V>* table) : table_index_{}, list_index_{}, table_{table} {
			Rewind();
		}
		Entry<K, V> Current() {
			Entry<K, V> entry;
			table_->store_[table_index_].Get(list_index_, entry);
			return entry;
		}
		bool HasNext() {
			return table_index_ < capacity && list_index_ < table_->store_[table_index_].Size();
		}
		void Advance() {
			++list_index_;
			while (list_index_ >= table_->store_[table_index_].Size() &&
				table_index_ < capacity) {
				++table_index_;
				list_index_= 0;
			}
		}
		void Rewind() {
			list_index_ = 0;
			for (table_index_ = 0; table_->store_[table_index_].Size() == 0 && table_index_ < capacity; ++table_index_);
		}
	private:
		uint8_t table_index_;
		uint8_t list_index_;
		HashTable<K, V>* table_;
	};
private:
	static bool KeyHit(const K& k, const Entry<K, V>& elem) {
		return k == elem.key;
	}
	uint8_t Hash(const K& key) {
		return static_cast<uint8_t>(key % capacity);
	}
	uint8_t size_{};
	LinkedList<Entry<K, V>> store_[capacity];
};

} // namespace containers


#endif /* CONT_HASHTABLE_H_ */