/*
* cont_list.h
*
* Created: 6/15/2017 10:17:06 PM
*  Author: roncato
*/


#ifndef CONT_LIST_H_
#define CONT_LIST_H_

#include <stdlib.h>
#include <stdint.h>

#include "containers/containers.h"
#include "containers/containers.h"

#include "containers/cont_queue.h"

namespace containers {

template <class T>
class LinkedListIterator;

template <class T>
class NakedLinkedListIterator;

template <class T>
class LinkedList {
public:
	LinkedList() {
		head_.next = nullptr;
	}
	LinkedList(const LinkedList<T>& that) {
		CopyTo(that, *this);
	}
	~LinkedList();
	void Add(T elem);
	void Add(uint16_t index, T elem);
	void Remove();
	void Remove(uint16_t index);
	bool Get(uint16_t index, T& elem);
	uint16_t Size() const {
		return size_;
	}
	bool IsEmpty() const {
		return size_ <= 0;
	}
	void Enqueue(T elem);
	bool Dequeue(T& elem);
	bool Peek(T& elem);
	void Clear();
	const LinkedList<T>& operator=(const LinkedList<T>& that) {
		CopyTo(that, *this);
		return *this;
	}
	const T& operator[](uint16_t index) const {
		T value;
		Get(index, value);
		return value;
	};
	friend class LinkedListIterator<T>;
	friend class NakedLinkedListIterator<T>;
private:
	SinglyLinkedNode<T> head_;
	SinglyLinkedNode<T>* tail_{&head_};
	uint16_t size_{};
	void CopyTo(const LinkedList<T>& from, LinkedList<T>& to);
	void Remove(containers::SinglyLinkedNode<T>* prev, containers::SinglyLinkedNode<T>* node);
};

template <class T>
class LinkedListIterator {
public:
	LinkedListIterator(LinkedList<T>* list) : list_{list}, prev_{&list->head_}, next_{list->head_.next} { }
	bool HasNext() {
		return next_ != nullptr;
	}
	T Current() {
		return next_->elem;
	}
	void Advance() {
		next_ = next_->next;
	}
	void RemoveAdvance() {
		SinglyLinkedNode<T>* next = next_;
		list_->Remove(prev_, next);
		next_ = prev_->next;
	}
private:
	LinkedList<T>* list_;
	SinglyLinkedNode<T>* prev_;
	SinglyLinkedNode<T>* next_;
};

template <class T>
class NakedLinkedListIterator {
public:
	SinglyLinkedNode<T>* next;
	SinglyLinkedNode<T>* head;
	NakedLinkedListIterator(LinkedList<T>* list) : next{list->head_.next}, head{&list->head_} {}
	inline bool HasNext() {
		return next != nullptr;
	}
	inline void Advance() {
		next = next->next;
	};
	inline void Rewind() {
		next = head;
	}
};

} // namespace containers

template <class T>
containers::LinkedList<T>::~LinkedList()
{
	Clear();
}

template <class T>
void containers::LinkedList<T>::Add(T elem) {
	containers::SinglyLinkedNode<T>* new_node = reinterpret_cast<containers::SinglyLinkedNode<T>*>(malloc(sizeof(containers::SinglyLinkedNode<T>)));
	if (new_node) {
		new_node->next = nullptr;
		new_node->elem = elem;
		tail_->next = new_node;
		tail_ = new_node;
		++size_;
	}
}

template <class T>
void containers::LinkedList<T>::Add(uint16_t index, T elem) {
	if (index >= 0 && index < size_) {
		containers::SinglyLinkedNode<T>* prev = &head_;
		containers::SinglyLinkedNode<T>* node = prev->next;
		for (auto i = 0; i < index; ++i) {
			prev = node;
			node = node->next;
		}
		containers::SinglyLinkedNode<T>* new_node = reinterpret_cast<containers::SinglyLinkedNode<T>*>(malloc(sizeof(containers::SinglyLinkedNode<T>)));
		if (new_node) {
			new_node->next = node;
			new_node->elem = elem;
			prev->next = new_node;
			++size_;
		}
	}
}

template <class T>
void containers::LinkedList<T>::Remove() {
	if (size_ > 0) {
		containers::SinglyLinkedNode<T>* prev = &head_;
		containers::SinglyLinkedNode<T>* node = head_.next;
		while (node->next) {
			prev = node;
			node = node->next;
		}
		containers::SinglyLinkedNode<T>* remove_node = node;
		prev->next = nullptr;
		tail_ = prev;
		free(remove_node);
		--size_;
	}
}

template <class T>
void containers::LinkedList<T>::Remove(uint16_t index) {
	if (index >= 0 && index < size_) {
		containers::SinglyLinkedNode<T>* prev = &head_;
		containers::SinglyLinkedNode<T>* node = prev->next;
		for (uint16_t i = 0; i < index; ++i) {
			prev = node;
			node = node->next;
		}
		containers::SinglyLinkedNode<T>* remove_node = node;
		prev->next = node->next;
		free(remove_node);
		if (index == size_ - 1) {
			tail_ = prev;
		}
		--size_;
	}
}

template <class T>
void containers::LinkedList<T>::Remove(containers::SinglyLinkedNode<T>* prev, containers::SinglyLinkedNode<T>* node) {
	containers::SinglyLinkedNode<T>* remove_node = node;
	prev->next = node->next;
	free(remove_node);
	if (prev->next) {
		tail_ = prev;
	}
	--size_;
}

template <class T>
bool containers::LinkedList<T>::Get(uint16_t index, T& elem) {
	if (index == size_ - 1) {
		elem = tail_->elem;
		return true;
	}
	if (index >= 0 && index < size_) {
		containers::SinglyLinkedNode<T>* node = &head_;
		for (uint16_t i = 0; i <= index; ++i) {
			node = node->next;
		}
		elem = node->elem;
		return true;
	}
	return false;
}

template <class T>
void containers::LinkedList<T>::Enqueue(T elem) {
	return Add(elem);
}

template <class T>
bool containers::LinkedList<T>::Dequeue(T& elem) {
	bool success = Get(static_cast<uint16_t>(0), elem);
	if (success) {
		Remove(static_cast<uint16_t>(0));
	}
	return success;
}

template <class T>
bool containers::LinkedList<T>::Peek(T& elem) {
	return Get(static_cast<uint16_t>(0), elem);
}

template <class T>
void containers::LinkedList<T>::CopyTo(const containers::LinkedList<T>& from, containers::LinkedList<T>& to) {
	containers::SinglyLinkedNode<T>* from_node = from.head_.next;
	while (from_node) {
		to.Add(from_node->elem);
		from_node = from_node->next;
	}
}

template <class T>
void containers::LinkedList<T>::Clear() {
	containers::SinglyLinkedNode<T>* next = head_.next;
	while (next) {
		containers::SinglyLinkedNode<T>* free_node = next;
		next = next->next;
		free(free_node);
	}
	head_.next = nullptr;
	size_ = 0;
}

#endif /* CONT_LIST_H_ */