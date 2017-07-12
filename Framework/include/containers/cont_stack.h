/*
 * cont_stack.h
 *
 * Created: 6/17/2017 11:44:24 AM
 *  Author: roncato
 */ 


#ifndef CONT_STACK_H_
#define CONT_STACK_H_

#include <stdlib.h>

#include "containers/containers.h"

namespace containers {

template <class T>
class Stack {
public:
	void Push(const T& elem) {
		if (!first_) {
			first_ = reinterpret_cast<SinglyLinkedNode<T>*>(malloc(sizeof(SinglyLinkedNode<T>)));
			first_->elem = elem;
			first_->next = nullptr;
		} else {
			SinglyLinkedNode<T>* node = reinterpret_cast<SinglyLinkedNode<T>*>(malloc(sizeof(SinglyLinkedNode<T>)));
			node->elem = elem;
			node->next = first_;
			first_ = node;
		}
		++size_;
	}
	bool Pop(T& elem) {
		if (size_ > 0) {
			elem = first_->elem;
			SinglyLinkedNode<T>* free_node = first_;
			first_ = first_->next;
			free(free_node);
			--size_;
			return true;
		}
		return false;
	}
	bool Peek(T& elem) {
		if (size_ > 0) {
			elem = first_->elem;
		}
		return false;
	}
	bool IsEmpty() const {
		return size_ <= 0;
	}
private:
	uint16_t size_{};
	SinglyLinkedNode<T>* first_{nullptr};
};

} // namespace containers


#endif /* CONT_STACK_H_ */