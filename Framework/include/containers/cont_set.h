/*
 * const_set.h
 *
 * Created: 7/8/2017 12:59:12 PM
 *  Author: roncato
 */ 


#ifndef CONT_SET_H_
#define CONT_SET_H_

namespace containers {

template <class T>
class Set {
public:
	virtual bool Add(const T& elem) = 0;
	virtual bool Remove(const T& elem) = 0;
	virtual bool Contains(const T& elem) = 0;
	virtual void Clear() = 0;
	virtual bool IsEmpty() = 0;
};

} // namespace containers


#endif /* CONT_SET_H_ */