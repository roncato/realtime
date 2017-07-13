/*
 * Bst.h
 *
 * Created: 7/8/2017 2:35:32 PM
 *  Author: roncato
 */ 


#ifndef BST_H_
#define BST_H_

#include <stdlib.h>

#include "containers.h"
#include "containers/cont_linkedlist.h"
#include "containers/cont_stack.h"

namespace {

template <class K, class V>
class TraversalNode {
public:
	TraversalNode(containers::BinaryTreeNode<K, V>* node) : node{node} {}
	containers::BinaryTreeNode<K, V>* node;
	bool visited_left{};
	bool visited_right{};
};

}

namespace containers {

template <class K, class V>
class Bst {
public:
	~Bst() {
		Clear();
	}
	bool Add(const K& key, const V& value);
	bool Get(const K& key, Entry<K, V>& entry);
	bool Remove(const K& key);
	bool Contains(const K& key);
	bool Ceiling(const K& key, Entry<K, V>& entry);
	bool Floor(const K& key, Entry<K, V>& entry);
	void Clear();
	bool IsEmpty() {
		return size_ == 0;
	}
	bool Min(Entry<K, V>& entry);
	bool Max(Entry<K, V>& entry);
	bool RemoveMin(Entry<K, V>& entry);
	bool RemoveMax(Entry<K, V>& entry);
	bool Traverse(LinkedList<Entry<K, V>>& entries); 
private:
	uint16_t size_{};
	BinaryTreeNode<K, V>* root_{nullptr};
	BinaryTreeNode<K, V>* NewNode(const K& key, const V& value);
	BinaryTreeNode<K, V>* RemoveMin(BinaryTreeNode<K, V>* root);
	bool Min(containers::BinaryTreeNode<K, V>* root, containers::BinaryTreeNode<K, V>** min);
	bool Traverse(LinkedList<BinaryTreeNode<K, V>*>& nodes);
};

template <class K, class V>
containers::BinaryTreeNode<K, V>* containers::Bst<K, V>::NewNode(const K& key, const V& value) {
	auto const node = reinterpret_cast<containers::BinaryTreeNode<K, V>*>(malloc(sizeof(containers::BinaryTreeNode<K, V>)));
	if (node) {
		node->key = key;
		node->value = value;
		++size_;
	}
	return node;
}

template <class K, class V>
bool containers::Bst<K, V>::Add(const K& key, const V& value) {
	if (!root_) {
		root_ = NewNode(key, value);
		return root_;
	} else {
		auto root = root_;
		auto node = root_;
		while (1) {
			if (!node) {
				node = NewNode(key, value);
				if (key < root->key) {
					root->left = node;
				} else {
					root->right = node;
				}
				return node;
			}
			if (key < node->key) {
				root = node;
				node = node->left;
			} else if (key > node->key) {
				root = node;
				node = node->right;
			} else if (key == node->key) {
				node->value = value;
				return node;
			}
		}
	}
}

template <class K, class V>
bool containers::Bst<K, V>::Get(const K& key, containers::Entry<K, V>& entry) {
	auto node = root_;
	while (1) {
		if (!node) {
			return false;
		}
		if (key < node->key) {
			node = node->left;
		} else if (key > node->key) {
			node = node->right;
		} else if (key == node->key) {
			entry = Entry<K, V>{node->key, node->value};
			return true;
		}
	}
}

template <class K, class V>
bool containers::Bst<K, V>::Contains(const K& key) {
	auto node = root_;
	while (1) {
		if (!node) {
			return false;
		}
		if (key < node->key) {
			node = node->left;
		} else if (key > node->key) {
			node = node->right;
		} else if (key == node->key) {
			return true;
		}
	}
}

template <class K, class V>
void containers::Bst<K, V>::Clear() {
	containers::LinkedList<containers::BinaryTreeNode<K, V>*> list;
	if (Traverse(list)) {
		containers::LinkedListIterator<BinaryTreeNode<K, V>*> iterator(&list);
		while (iterator.HasNext()) {
			free(iterator.Current());
			iterator.Advance();
		}
		size_ = 0;
		root_ = nullptr;
	}
}

template <class K, class V>
bool containers::Bst<K, V>::Min(containers::Entry<K, V>& entry) {
	if (size_ > 0) {
		containers::BinaryTreeNode<K, V>* node;
		if (Min(root_, &node)) {
			entry = containers::Entry<K, V>{node->key, node->value};
			return true;
		}
	}
	return false;
}

template <class K, class V>
bool containers::Bst<K, V>::Min(containers::BinaryTreeNode<K, V>* root, containers::BinaryTreeNode<K, V>** min) {
	if (size_ == 0) {
		return false;
	}
	auto node = root;
	while (node->left) {
		node = node->left;
	}
	*min = node;
	return true;
}

template <class K, class V>
bool containers::Bst<K, V>::Max(Entry<K, V>& entry) {
	if (size_ == 0) {
		return false;
	}
	auto node = root_;
	while (node->right) {
		node = node->right;
	}
	entry = containers::Entry<K, V>{node->key, node->value};
	return true;
}

template <class K, class V>
bool containers::Bst<K, V>::RemoveMin(containers::Entry<K, V>& entry) {
	if (size_ == 0) {
		return false;
	}
	auto root = root_;
	auto node = root_;
	if (!root->left) {
		root_ = root->right;
	} else {
		while (node->left) {
			root = node;
			node = node->left;
		}
		root->left = node->right;
	}
	entry = Entry<K, V> {node->key, node->value};
	free(node);
	--size_;
	return true;
}

template <class K, class V>
bool containers::Bst<K, V>::RemoveMax(containers::Entry<K, V>& entry) {
	if (size_ == 0) {
		return false;
	}
	auto root = root_;
	auto node = root_;
	if (!root->right) {
		root_ = root->left;
	} else {
		while (node->right) {
			root = node;
			node = node->right;
		}
		root->right = node->left;
	}	
	entry = Entry<K, V> {node->key, node->value};
	free(node);
	--size_;
	return true;
}

template <class K, class V>
containers::BinaryTreeNode<K, V>* containers::Bst<K, V>::RemoveMin(containers::BinaryTreeNode<K, V>* root) {
	if (!root->left) {
		return root->right;
	} else {
		root->left = RemoveMin(root->left);
		return root;
	}
}

template <class K, class V>
bool containers::Bst<K, V>::Remove(const K& key) {
	if (size_ == 0) {
		return false;
	}
	auto root = root_;
	auto node = root_;
	while (1) {
		if (!node) {
			return false;
		}
		if (key < node->key) {
			root = node;
			node = node->left;
		} else if (key > node->key) {
			root = node;
			node = node->right;
		} else if (key == node->key) {
			if (node->right == nullptr) {
				if (root->right == node) {
					root->right = node->left;
				} else if (root->left == node) {
					root->left = node->left;
				} else { // root
					root_ = node->left;
				}
			} else if (node->left == nullptr) {
				if (root->right == node) {
					root->right = node->right;
				} else if (root->left == node)  {
					root->left = node->right;
				} else {
					root_ = node->right;
				}
			} else {
				containers::BinaryTreeNode<K, V>* t = node;
				if (Min(t->right, &node)) {
					node->right = RemoveMin(t->right);
					node->left = t->left;
					if (root->right == t) {
						root->right = node;
					} else if (root->left == t) {
						root->left = node;
					} else {
						root_ = node;
					}
					node = t;
				} else {
					return false;
				}
			}
			free(node);
			--size_;
			return true;
		}
	}
}

template <class K, class V>
bool containers::Bst<K, V>::Traverse(containers::LinkedList<containers::BinaryTreeNode<K, V>*>& nodes) {
	if (!root_) {
		return false;
	}
	TraversalNode<K, V> current(root_);
	containers::Stack<TraversalNode<K, V>> stack;
	stack.Push(current);
	while (!stack.IsEmpty()) {
		if (stack.Pop(current)) {
			if (!current.visited_left &&
				current.node->left) {
				current.visited_left = true;
				stack.Push(current);
				stack.Push(TraversalNode<K, V>(current.node->left));
				continue;
			}
			if (!current.visited_right) {
				nodes.Add(current.node);
				if (current.node->right) {
					current.visited_right = true;
					stack.Push(current);
					stack.Push(TraversalNode<K, V>(current.node->right));
					continue;
				}
			}
		}
	}
	return nodes.Size() == size_;
}

template <class K, class V>
bool containers::Bst<K, V>::Traverse(containers::LinkedList<containers::Entry<K, V>>& entries) {
	containers::LinkedList<containers::BinaryTreeNode<K, V>*> list;
	if (Traverse(list)) {
		containers::BinaryTreeNode<K, V>* node;
		while (!list.IsEmpty() && list.Dequeue(node)) {
			entries.Add(containers::Entry<K, V>{node->key, node->value});
		}
		return entries.Size() == size_;
	}
	return false;
}

} // namespace containers

#endif /* BST_H_ */