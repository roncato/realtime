/*
 * comm_adapter.h
 *
 * Created: 7/3/2017 8:54:21 PM
 *  Author: roncato
 */ 


#ifndef COMM_ADAPTER_H_
#define COMM_ADAPTER_H_

#include <stdint.h>

namespace comm {

class Adapter {
public:
	virtual void Open() = 0;
	virtual void Close() = 0;
	virtual uint8_t Write(uint8_t data) = 0;
	virtual uint8_t Write(uint8_t* buffer, uint8_t len) = 0;
	virtual uint8_t Read(uint8_t& data) = 0;
	virtual uint8_t Read(uint8_t* buffer, uint8_t len) = 0;
	virtual bool IsOpen() const = 0;
	virtual uint8_t Available() const = 0;
};

} // namespace comm


#endif /* COMM_ADAPTER_H_ */