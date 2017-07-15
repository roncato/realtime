/*
 * cont_map.h
 *
 * Created: 7/15/2017 10:55:42 AM
 *  Author: roncato
 */ 


#ifndef CONT_MAP_H_
#define CONT_MAP_H_

namespace containers {

template <class K, class V>
class Map {
public:
	virtual bool Add(const K& key, const V& value) = 0;
	virtual bool Get(const K& key, Entry<K, V>& entry) = 0;
	virtual bool Remove(const K& key) = 0;
	virtual bool Contains(const K& key) = 0;
	virtual void Clear() = 0;
	virtual bool IsEmpty() = 0;
};

} // namespace containers

#endif /* CONT_MAP_H_ */