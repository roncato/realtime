/*
 * comm_uart.h
 *
 * Created: 7/3/2017 8:52:41 PM
 *  Author: roncato
 */ 


#ifndef COMM_SERIAL_H_
#define COMM_SERIAL_H_

#include <stdint.h>
#include <math.h>

#include "communication/comm_adapter.h"

namespace comm {

enum SerialBaud : uint16_t {
	SERIAL_BAUD_2400	= static_cast<uint16_t>(round(F_CPU / 19200.0 - 1)),
	SERIAL_BAUD_4800	= static_cast<uint16_t>(round(F_CPU / 38400.0  - 1)),
	SERIAL_BAUD_9600	= static_cast<uint16_t>(round(F_CPU / 76800.0  - 1)),
	SERIAL_BAUD_14_4k	= static_cast<uint16_t>(round(F_CPU / 115200.0  - 1)),
	SERIAL_BAUD_19_2k	= static_cast<uint16_t>(round(F_CPU / 153600.0  - 1)),
	SERIAL_BAUD_28_8k	= static_cast<uint16_t>(round(F_CPU / 230400.0  - 1)),
	SERIAL_BAUD_38_4k	= static_cast<uint16_t>(round(F_CPU / 307200.0  - 1)),
	SERIAL_BAUD_57_6k	= static_cast<uint16_t>(round(F_CPU / 921600.0  - 1)),
	SERIAL_BAUD_76_8k	= static_cast<uint16_t>(round(F_CPU / 460800.0  - 1)),
	SERIAL_BAUD_115_2k	= static_cast<uint16_t>(round(F_CPU / 921600.0  - 1)),
	SERIAL_BAUD_230_4k	= static_cast<uint16_t>(round(F_CPU / 1843200.0  - 1)),
	SERIAL_BAUD_250_0k	= static_cast<uint16_t>(round(F_CPU / 2000000.0  - 1)),
	SERIAL_BAUD_500_0k	= static_cast<uint16_t>(round(F_CPU / 4000000.0  - 1)),
	SERIAL_BAUD_1000_0k	= static_cast<uint16_t>(round(F_CPU / 8000000.0  - 1)),
};

enum SerialDataBits {
	SERIAL_DATA_5	= 0x00,
	SERIAL_DATA_6	= 0x01,
	SERIAL_DATA_7	= 0x02,
	SERIAL_DATA_8	= 0x03
};

enum SerialParity {
	SERIAL_PARITY_NONE	= 0x00,
	SERIAL_PARITY_EVEN	= 0x02,
	SERIAL_PARITY_ODD	= 0x03,
};

enum SerialStopBits {
	SERIAL_STOP_1	= 0x00,
	SERIAL_STOP_2	= 0x01
};

class Serial {
public:
	Serial(SerialBaud baud, SerialDataBits data_bits, SerialParity parity, SerialStopBits stop_bits) : 
		baud_{baud}, data_bits_{data_bits}, parity_{parity}, stop_bits_{stop_bits}, is_open_{} {}
	Serial(SerialBaud baud) :
		Serial(baud, SERIAL_DATA_8, SERIAL_PARITY_NONE, SERIAL_STOP_1) {}
	Serial() : Serial(SERIAL_BAUD_9600) {}
	void Open();
	void Close();
	uint8_t Write(uint8_t data);
	uint8_t Write(uint8_t* buffer, uint8_t len);
	uint8_t Read(uint8_t& data);
	uint8_t Read(uint8_t* buffer, uint8_t len);
	bool IsOpen() const {
		return is_open_;
	};
	uint8_t Available();
private:
	SerialBaud baud_;
	SerialDataBits data_bits_;
	SerialParity parity_;
	SerialStopBits stop_bits_;
	bool is_open_;
};

}


#endif /* COMM_SERIAL_H_ */