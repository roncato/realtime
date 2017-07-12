/*
 * cont_map.h
 *
 * Created: 7/8/2017 11:52:12 AM
 *  Author: roncato
 */ 


#ifndef CONT_MAP_H_
#define CONT_MAP_H_

namespace containers {

template <class K, class V>
class Map {
public:
	virtual bool Add(K key, V value) = 0;
	virtual bool Rank(const K& key, uint16_t& rank) = 0;
	virtual bool Get(const K& key, Entry<K, V>& entry) = 0;
	virtual bool Remove(const K& key) = 0;
	virtual  RemoveAt(uint16_t rank) = 0;
	virtual bool Contains(const K& key) = 0;
	virtual bool Ceiling(const K& key, Entry<K, V>& entry) = 0;
	virtual bool Floor(const K& key, Entry<K, V>& entry) = 0;
	virtual void Clear() = 0;
	virtual bool IsEmpty() = 0;
	virtual bool Min(Entry<K, V>& entry) = 0;
	virtual bool Min(Entry<K, V>& entry) = 0;
	virtual bool RemoveMin() = 0;
	virtual bool RemoveMax() = 0;
};

} // namespace containers

#endif /* CONT_MAP_H_ */