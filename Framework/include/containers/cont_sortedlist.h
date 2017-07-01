/*
 * cont_sortedlist.h
 *
 * Created: 6/17/2017 2:07:35 PM
 *  Author: roncato
 */ 


#ifndef CONT_SORTEDLIST_H_
#define CONT_SORTEDLIST_H_

#include "containers/containers.h"

#include "containers/cont_linkedlist.h"

namespace containers {

template <class K, class V>
class SortedList {
public:
	SortedList() {
		head_.next = nullptr;
	}
	SortedList(const SortedList<K, V>& that) {
		CopyTo(that, *this);
	}
	~SortedList();
	void Add(K key, V value);
	bool Take(Entry<K, V>& entry);
	bool Peek(Entry<K, V>& entry);
	bool Peek(K& entry);
	SortedList<K, V>& operator=(const SortedList<K, V>& that) {
		CopyTo(that, *this);
	}
	bool IsEmpty() const {
		return size_ <= 0;
	}
private:
	SinglyLinkedEntryNode<K, V> head_{};
	uint16_t size_{};
	void CopyTo(const SortedList<K, V>& from, SortedList<K, V>& to);
};

} // namespace containers

template <class K, class V>
containers::SortedList<K, V>::~SortedList() {
	containers::SinglyLinkedEntryNode<K, V>* node = head_.next;
	while (node) {
		containers::SinglyLinkedEntryNode<K, V>* free_node = node;
		node = node->next;
		free(free_node);
	}
}

template <class K, class V>
void containers::SortedList<K, V>::Add(K key, V value) {
	containers::SinglyLinkedEntryNode<K, V>* prev = &head_;
	containers::SinglyLinkedEntryNode<K, V>* node = prev->next;
	while (node && key > node->key) {
		prev = node;
		node = node->next;
	}
	containers::SinglyLinkedEntryNode<K, V>* new_node = reinterpret_cast<containers::SinglyLinkedEntryNode<K, V>*>(malloc(sizeof(containers::SinglyLinkedEntryNode<K, V>)));
	if (new_node) {
		new_node->next = node;
		new_node->key = key;
		new_node->value = value;
		prev->next = new_node;
		++size_;
	}
}

template <class K, class V>
bool containers::SortedList<K, V>::Take(Entry<K, V>& entry) {
	if (size_ > 0) {
		containers::SinglyLinkedEntryNode<K, V>* free_node = head_.next;
		entry.key = free_node->key;
		entry.value = free_node->value;
		head_.next = free_node->next;
		free(free_node);
		--size_;
		return true;
	}
	return false;
}

template <class K, class V>
bool containers::SortedList<K, V>::Peek(containers::Entry<K, V>& entry) {
	if (size_ > 0) {
		entry.key = head_.next->key;
		entry.value = head_.next->value;
		return true;
	}
	return false;
}

template <class K, class V>
bool containers::SortedList<K, V>::Peek(K& key) {
	if (size_ > 0) {
		key = head_.next->key;
		return true;
	}
	return false;
}

template <class K, class V>
void containers::SortedList<K, V>::CopyTo(const containers::SortedList<K, V>& from, containers::SortedList<K, V>& to) {
	containers::SinglyLinkedEntryNode<K, V>* from_node = from.head_.next;
	while (from_node) {
		//to.Add(from_node->elem_);
		from_node = from_node->next;
	}
}

#endif /* CONT_SORTEDLIST_H_ */