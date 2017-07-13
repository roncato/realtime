/*
 * containers.h
 *
 * Created: 6/17/2017 4:31:03 PM
 *  Author: roncato
 */ 


#ifndef CONTAINERS_H_
#define CONTAINERS_H_

namespace containers {

template <class T>
struct SinglyLinkedNode {
	T elem;
	SinglyLinkedNode<T>* next;
};

template <class K, class V>
struct Entry {
	K key;
	V value;
};

template <class K, class V>
struct SinglyLinkedEntryNode {
	K key;
	V value;
	SinglyLinkedEntryNode<K, V>* next;
};


template <class K, class V>
struct BinaryTreeNode {
	K key;
	V value;
	BinaryTreeNode<K, V>* left{nullptr};
	BinaryTreeNode<K, V>* right{nullptr};
};

} // namespace containers

#endif /* CONTAINERS_H_ */