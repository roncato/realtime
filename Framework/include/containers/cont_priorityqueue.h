/*
 * cont_priorityqueue.h
 *
 * Created: 7/4/2017 9:24:41 PM
 *  Author: roncato
 */ 


#ifndef CONT_PRIORITYQUEUE_H_
#define CONT_PRIORITYQUEUE_H_

#include "util/util_utilities.h"

#include "containers/containers.h"

namespace containers {

template <class K, class V, uint8_t capacity = 64U>
class PriorityQueue {
public:
	PriorityQueue(bool (*less)(K, K)) : less_{less} {}
	PriorityQueue() : PriorityQueue([](K left, K right) {return left < right;}) {}
	bool Add(K key, V value) {
		if (n_ < capacity) {
			heap_[++n_] = Entry<K, V>{key, value};
			swim(n_);
			return true;
		}
		return false;
	}
	bool Take(Entry<K, V>& entry) {
		if (n_ > 0) {
			entry = heap_[1];
			util::utilities::Swap(heap_[1], heap_[n_--]);
			sink(1);
			return true;
		}
		return false;
	}
	bool Peek(Entry<K, V>& entry) {
		if (n_ > 0) {
			entry = heap_[1];
			return true;
		}
		return false;
	}
	bool Peek(K& entry) {
		if (n_ > 0) {
			entry = heap_[1].key;
			return true;
		}
		return false;
	}
	void Clear() {
		n_ = 0;
	}
	bool IsEmpty() const {
		return n_ == 0;
	}
	class PriorityQueueIterator {
	public:
		PriorityQueueIterator(PriorityQueue* queue) : queue_{queue} {}
		bool HasNext() {
			return k <= queue_->n_;
		}
		Entry<K, V> Current() {
			return queue_->heap_[k];
		}
		void RemoveAdvance() {
			util::utilities::Swap(queue_->heap_[k], queue_->heap_[queue_->n_--]);
			if (k > 1) {
				if (queue_->less_(queue_->heap_[k/2].key, queue_->heap_[k].key)) {
					queue_->swim(k);
				} else {
					queue_->sink(k);
				}
			}
			--queue->n_;
		}
		void Advance() {
			++k;
		}
		void Rewind() {
			k = 1;
		}
	private:
		PriorityQueue* queue_;
		uint8_t k{1};
	};
private:
	uint8_t n_{};
	Entry<K, V> heap_[capacity];
	bool (*less_)(K, K);
	void swim(uint8_t k) {
		while (k > 1 && less_(heap_[k/2].key, heap_[k].key)) {
			util::utilities::Swap(heap_[k/2], heap_[k]);
			k = k/2;
		}
	}
	void sink(uint8_t k) {
		while (2*k <= n_) {
			auto j = 2*k;
			if (j < n_ && less_(heap_[j].key, heap_[j+1].key)) {
				++j;
			}
			if (!less_(heap_[k].key, heap_[j].key)) {
				break;
			}
			util::utilities::Swap(heap_[k], heap_[j]);
			k = j;
		}
	}
};

}

#endif /* CONT_PRIORITYQUEUE_H_ */
