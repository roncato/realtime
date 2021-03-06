/*
 * const_queue.h
 *
 * Created: 6/17/2017 4:43:37 AM
 *  Author: roncato
 */ 


#ifndef CONT_QUEUE_H_
#define CONT_QUEUE_H_

namespace containers {

template <class T>
class Queue {
public:
	virtual void Enqueue(const T& elem) = 0;
	virtual bool Dequeue(T& elem) = 0;
	virtual bool IsEmpty() const = 0;
	virtual void Clear() = 0;
};

} // namespace containers

#endif /* CONT_QUEUE_H_ */