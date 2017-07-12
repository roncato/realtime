/*
 * Bst.h
 *
 * Created: 7/8/2017 2:35:32 PM
 *  Author: roncato
 */ 


#ifndef BST_H_
#define BST_H_

namespace containers {

template <class K, class V>
class Bst {
public:
	bool Add(K key, V value);
	bool Rank(const K& key, uint16_t& rank);
	bool Get(const K& key, Entry<K, V>& entry);
	bool Remove(const K& key);
	bool RemoveAt(uint16_t rank);
	bool Contains(const K& key);
	bool Ceiling(const K& key, Entry<K, V>& entry);
	bool Floor(const K& key, Entry<K, V>& entry);
	void Clear();
	bool IsEmpty();
	bool Min(Entry<K, V>& entry);
	bool Min(Entry<K, V>& entry);
	bool RemoveMin();
	bool RemoveMax();
};

} // namespace containers

#endif /* BST_H_ */