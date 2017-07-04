/*
 * mal_reg.h
 *
 * Created: 6/10/2017 8:31:51 PM
 *  Author: roncato
 */ 


#ifndef MAL_REG_H_
#define MAL_REG_H_

#include <stdint.h>

namespace mal {
namespace reg {

typedef uint8_t reg_value_t;

constexpr uint8_t kPinPortNumbers = 3;

constexpr uint8_t kSfrOffset = 0x20U;

constexpr uint8_t kRegBitWidth = sizeof(reg_value_t) * 8UL;

// Bit-position values.
constexpr uint8_t kBitVal0  = 1U;
constexpr uint8_t kBitVal1  = 1U << 1U;
constexpr uint8_t kBitVal2  = 1U << 2U;
constexpr uint8_t kBitVal3  = 1U << 3U;
constexpr uint8_t kBitVal4  = 1U << 4U;
constexpr uint8_t kBitVal5  = 1U << 5U;
constexpr uint8_t kBitVal6  = 1U << 6U;
constexpr uint8_t kBitVal7  = 1U << 7U;

// CPU Reg
constexpr uint8_t kStatusReg = 0x3FU + kSfrOffset;

// Port Regs
constexpr uint8_t kInputPinsRegB		= 0x03U + kSfrOffset;
constexpr uint8_t kDataDirectionRegB	= 0x04U + kSfrOffset;
constexpr uint8_t kPortRegB				= 0x05U + kSfrOffset;
constexpr uint8_t kInputPinsRegC		= 0x06U + kSfrOffset;
constexpr uint8_t kDataDirectionRegC	= 0x07U + kSfrOffset;
constexpr uint8_t kPortRegC				= 0x08U + kSfrOffset;
constexpr uint8_t kInputPinsRegD		= 0x09U + kSfrOffset;
constexpr uint8_t kDataDirectionRegD	= 0x0AU + kSfrOffset;
constexpr uint8_t kPortRegD				= 0x0BU + kSfrOffset;

// Timer 0 Regs
constexpr uint8_t kTimerIntFlagReg0			= 0x15U + kSfrOffset;
constexpr uint8_t kTimerControlReg0A		= 0x24U + kSfrOffset;
constexpr uint8_t kTimerControlReg0B		= 0x25U + kSfrOffset;
constexpr uint8_t kTimerCounterReg0			= 0x26U + kSfrOffset;
constexpr uint8_t kTimerCompareReg0A		= 0x27U + kSfrOffset;
constexpr uint8_t kTimerCompareReg0B		= 0x28U + kSfrOffset;
constexpr uint8_t kTimerInterruptMskReg0	= 0x6EU;

// Timer 1 Regs
constexpr uint8_t kTimerIntFlagReg1			= 0x16U + kSfrOffset;
constexpr uint8_t kTimerControlReg1A		= 0x60U + kSfrOffset;
constexpr uint8_t kTimerControlReg1B		= 0x61U + kSfrOffset;
constexpr uint8_t kTimerControlReg1C		= 0x62U + kSfrOffset;
constexpr uint8_t kTimerCounterReg1L		= 0x64U + kSfrOffset;
constexpr uint8_t kTimerCounterReg1H		= 0x65U + kSfrOffset;
constexpr uint8_t kTimerCompareReg1LA		= 0x68U + kSfrOffset;
constexpr uint8_t kTimerCompareReg1HA		= 0x69U + kSfrOffset;
constexpr uint8_t kTimerCompareReg1LB		= 0x6AU + kSfrOffset;
constexpr uint8_t kTimerCompareReg1HB		= 0x6BU + kSfrOffset;
constexpr uint8_t kTimerInterruptMskReg1	= 0x6FU;

// Timer 2 Regs
constexpr uint8_t kTimerIntFlagReg2			= 0x17U + kSfrOffset;
constexpr uint8_t kTimerControlReg2A		= 0xB0U;
constexpr uint8_t kTimerControlReg2B		= 0xB1U;
constexpr uint8_t kTimerCounterReg2			= 0xB2U;
constexpr uint8_t kTimerCompareReg2A			= 0xB3U;
constexpr uint8_t kTimerCompareReg2B		= 0xB4U;
constexpr uint8_t kTimerInterruptMskReg2	= 0x70U;

// SPI(TM) Regs.
constexpr uint8_t kSpiControlReg0	= 0x2CU + kSfrOffset;
constexpr uint8_t kSpiStatusReg0	= 0x2DU + kSfrOffset;
constexpr uint8_t kSpiDataReg0		= 0x2EU + kSfrOffset;

// Pin Change Regs.
constexpr uint8_t KPinChangeCntlReg = 0x48U + kSfrOffset;
constexpr uint8_t KPinChangeFlagReg = 0x1BU + kSfrOffset;

constexpr uint8_t KPinChangeMskReg0 = 0x4BU + kSfrOffset;
constexpr uint8_t KPinChangeMskReg1 = 0x4CU + kSfrOffset;
constexpr uint8_t KPinChangeMskReg2 = 0x4DU + kSfrOffset;
		
// External Interrupt Regs.
constexpr uint8_t kExtIntCntlRegA	= 0x49U + kSfrOffset;
constexpr uint8_t KExtIntMskReg		= 0x1DU + kSfrOffset;
constexpr uint8_t KExtIntFlagReg	= 0x1CU + kSfrOffset;

// ADC MUX
constexpr uint8_t kADControlStatusReg	= 0x7A;
constexpr uint8_t kADMuxReg				= 0x7C;
constexpr uint8_t kDInputDisableReg		= 0x7E;
constexpr uint8_t kADCDataRegL			= 0x78;
constexpr uint8_t kADCDataRegH			= 0x79;

constexpr uint8_t kADC0					= 0x00;
constexpr uint8_t kADC1					= 0x01;
constexpr uint8_t kADC2					= 0x02;
constexpr uint8_t kADC3					= 0x03;
constexpr uint8_t kADC4					= 0x04;
constexpr uint8_t kADC5					= 0x05;
constexpr uint8_t kADC6					= 0x06;
constexpr uint8_t kADC7					= 0x07;

constexpr uint8_t kADCIntFlagBit		= 0x04;
constexpr uint8_t kADCStartBit			= 0x06;
constexpr uint8_t kADCEnableBit			= 0x07;

// USART
constexpr uint8_t kUsart0DataReg			= 0xC6;
constexpr uint8_t kUsart0ControlStatusRegA	= 0xC0;
constexpr uint8_t kUsart0ControlStatusRegB	= 0xC1;
constexpr uint8_t kUsart0ControlStatusRegC	= 0xC2;
constexpr uint8_t kUsart0BaudL				= 0xC4;
constexpr uint8_t kUsart0BaudH				= 0xC5;
constexpr uint8_t kUsart0TxEn0Val			= kBitVal3;
constexpr uint8_t kUsart0RxEn0Val			= kBitVal4;
constexpr uint8_t kUsart0DataEmptyIntEnVal	= kBitVal5;
constexpr uint8_t kUsart0TxCompleteIntEnVal	= kBitVal6;	
constexpr uint8_t kUsart0RxCompleteIntEnVal	= kBitVal7;
constexpr uint8_t kUsart0TxCompleteBit		= 0x06;
constexpr uint8_t kUsart0DataEmptyIntEnBit	= 0x05;
constexpr uint8_t kUsart0ParityStartBit		= 0x04;
constexpr uint8_t kUsart0DataSizeStartBit	= 0x01;
constexpr uint8_t kUsart02xBit				= 0x01;
constexpr uint8_t kUsart0StopbitsStartBit	= 0x03;
constexpr uint8_t kUsartRxErrorMask			= 0x1C;

} // namespace reg
} // namespace mal


#endif /* MAL_REG_H_ */