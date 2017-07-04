/*
 * comm_serial.cpp
 *
 * Created: 7/3/2017 8:58:53 PM
 *  Author: roncato
 */ 

#include <avr/interrupt.h>

#include "mal/mal_interrupt.h"
#include "mal/mal_cpu.h"
#include "mal/mal_reg.h"
#include "mal/mal_reg_access.h"

#include "util/util_lock.h"

#include "communication/comm_serial.h"
#include "containers/cont_buffer.h"

namespace {

constexpr uint8_t kSerialBufferSize = 64U;

uint8_t instances{};

containers::Buffer<uint8_t, kSerialBufferSize> rx_buffer;
containers::Buffer<uint8_t, kSerialBufferSize> tx_buffer;

}

ISR(USART_RX_vect) {
	const auto error_status = mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0ControlStatusRegA>::GetVal() & mal::reg::kUsartRxErrorMask;
	if (error_status == 0) {
		rx_buffer.Write(mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0DataReg>::GetVal());
	}	
}

ISR(USART_UDRE_vect) {
	uint8_t data;
	if (tx_buffer.Read(data)) {
		mal::reg::SetVal(mal::reg::kUsart0DataReg, data);
	} else {
		mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0ControlStatusRegB, mal::reg::kUsart0DataEmptyIntEnBit>::ClearBit();
	}
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0ControlStatusRegA, mal::reg::kUsart0TxCompleteBit>::ClearBit();
	if (tx_buffer.Available() == 0) {
		mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0ControlStatusRegB, mal::reg::kUsart0DataEmptyIntEnBit>::ClearBit();
	}
}

void comm::Serial::Open() {
	if (!is_open_ && instances == 0) {
		INTERRUPT_LOCK()
			mal::reg::SetVal(mal::reg::kUsart0BaudH, baud_ >> 8);
			mal::reg::SetVal(mal::reg::kUsart0BaudL, baud_ & 0xFF);
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0ControlStatusRegB, 
			mal::reg::kUsart0RxEn0Val | mal::reg::kUsart0TxEn0Val | mal::reg::kUsart0RxCompleteIntEnVal>::SetVal();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0ControlStatusRegA, mal::reg::kUsart02xBit>::SetBit();
			mal::reg::SetVal(mal::reg::kUsart0ControlStatusRegC, (parity_ << mal::reg::kUsart0ParityStartBit) | (stop_bits_ << mal::reg::kUsart0StopbitsStartBit) | data_bits_ << mal::reg::kUsart0DataSizeStartBit);
		INTERRUPT_UNLOCK()
		++instances;
		is_open_ = true;
	}		
}

void comm::Serial::Close() {
	if (is_open_) {
		INTERRUPT_LOCK()
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0ControlStatusRegB, 0x00>::SetVal();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0ControlStatusRegC, 0x00>::SetVal();
			tx_buffer.Clear();
			rx_buffer.Clear();
		INTERRUPT_UNLOCK()
		--instances;
		is_open_ = false;
	}
}

uint8_t comm::Serial::Write(uint8_t data) {
	uint8_t written = 0U;
	if (is_open_) {
		if (mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0ControlStatusRegA, mal::reg::kUsart0DataEmptyIntEnBit>::GetBit()) {
			mal::reg::SetVal(mal::reg::kUsart0DataReg, data);
			++written;
		} else {
			written = tx_buffer.Write(data);
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0ControlStatusRegB, mal::reg::kUsart0DataEmptyIntEnBit>::SetBit();
		}
	}
	return written;
}

uint8_t comm::Serial::Write(uint8_t* buffer, uint8_t len) {
	uint8_t written = 0U;
	if (is_open_) {
		if (mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0ControlStatusRegA, mal::reg::kUsart0DataEmptyIntEnBit>::GetBit()) {
			mal::reg::SetVal(mal::reg::kUsart0DataReg, *buffer);
			++written;
		}
		if (len > 1U) {
			written += tx_buffer.Write(buffer+1U, len-1U);
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kUsart0ControlStatusRegB, mal::reg::kUsart0DataEmptyIntEnBit>::SetBit();
		}
	}
	return written;
}

uint8_t comm::Serial::Read(uint8_t& data) {
	return rx_buffer.Read(data);
}

uint8_t comm::Serial::Read(uint8_t* buffer, uint8_t len) {
	return rx_buffer.Read(buffer, len);
}

uint8_t comm::Serial::Available() {
	return rx_buffer.Available();
}