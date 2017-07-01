/*
 * mal_pincint2.cpp
 *
 * Created: 6/26/2017 8:30:33 PM
 *  Author: roncato
 */ 

#include <stdint.h>
#include <string.h>
#include <avr/interrupt.h>

#include "mal/mal_pincint.h"
#include "mal/mal_reg_access.h"
#include "mal/mal_reg.h"
#include "mal/mal_cpu.h"
#include "util/util_lock.h"

#include "containers/cont_linkedlist.h"

namespace {

/*
 * Handler register task structure
 */
typedef struct {
	mal::irq::IrqTrigger trigger;
	void* context;
	mal::irq::PCIntRequestHandler handler;
	uint8_t pin_address;
	uint8_t bit_number;
	uint8_t last_state;
} IrqRecord;

containers::LinkedList<IrqRecord> pinc_irq_0;
containers::LinkedList<IrqRecord> pinc_irq_1;
containers::LinkedList<IrqRecord> pinc_irq_2;

inline bool ShouldTrigger(IrqRecord* record) {
	auto should_trigger{false};
	const auto state = mal::reg::GetBit(record->pin_address, record->bit_number) ? static_cast<uint8_t>(1UL) : static_cast<uint8_t>(0UL);
	if (state != record->last_state) {
		if (record->trigger == mal::irq::IRQ_TRIGGER_RISING) {
			if (record->last_state == mal::irq::LOW) {
				should_trigger = true;
			}
		} else if (record->trigger == mal::irq::IRQ_TRIGGER_FALLING) {
			if (record->last_state == mal::irq::HIGH) {
				should_trigger = true;
			}
		} else if (record->trigger == mal::irq::IRQ_TRIGGER_CHANGED) {
			should_trigger = true;
		}
		record->last_state = state;
	}
	return should_trigger;
}

inline void HandleInterrupt(containers::LinkedList<IrqRecord>* list) {
	containers::NakedLinkedListIterator<IrqRecord> iterator(list);
	while (iterator.HasNext()) {
		if (ShouldTrigger(&iterator.next->elem)) {
			iterator.next->elem.handler(iterator.next->elem.context);
		}
		iterator.Advance();
	}
}

volatile uint16_t group_regs_cnt[3]{0,0,0};

}

ISR(PCINT0_vect) {
	HandleInterrupt(&pinc_irq_0);
}

ISR(PCINT1_vect) {
	HandleInterrupt(&pinc_irq_1);
}

ISR(PCINT2_vect) {
	HandleInterrupt(&pinc_irq_2);
}

mal::irq::IrqReturn mal::irq::RegisterIrq(	void* context,
											const pcint_number_t pcint_number,
											const PCIntRequestHandler pcint_request_handler,
											const mal::irq::IrqTrigger trigger) {
	if (pcint_number >= mal::irq::kMaxHandlersNumber) {
		return IRQ_ERROR;
	}

	// Compute record location
	const auto offset = pcint_number / mal::reg::kRegBitWidth;
	const auto bit_number = static_cast<uint8_t>(pcint_number % mal::reg::kRegBitWidth);

	// Assign initial state
	const auto pin_address = mal::irq::pinc_to_pin_map[offset];

	// Initial state
	const auto state = mal::reg::GetBit(pin_address, bit_number) ? static_cast<uint8_t>(1UL) : static_cast<uint8_t>(0UL);

	// Irq record
	IrqRecord record = IrqRecord{trigger, context, pcint_request_handler, pin_address, bit_number, state};

	switch (offset) {
		case 0:
			pinc_irq_0.Add(record);
		break;
		case 1:
			pinc_irq_1.Add(record);
		break;
		case 2:
			pinc_irq_2.Add(record);
		break;
	}

	// Increment register
	INTERRUPT_LOCK()
		group_regs_cnt[offset]++;
	UNLOCK()

	// Enable interrupt for pin change interrupt group
	mal::reg::SetBit(mal::reg::KPinChangeCntlReg, offset);
	mal::reg::SetBit(mal::reg::KPinChangeMskReg0 + offset, bit_number);

	return IRQ_REGISTERED;
}

mal::irq::IrqReturn mal::irq::UnregisterIrq(const pcint_number_t pcint_number, const mal::irq::IrqTrigger trigger) {
	if (pcint_number >= kMaxHandlersNumber) {
		return IRQ_ERROR;
	}

	// Record locator
	const auto offset = pcint_number / mal::reg::kRegBitWidth;
	const auto pin_address = mal::irq::pinc_to_pin_map[offset];
	const auto bit_number = pcint_number % mal::reg::kRegBitWidth;

	containers::LinkedList<IrqRecord>* list;

	switch (offset) {
		default:
			list = &pinc_irq_0;
		break;
		case 1:
			list = &pinc_irq_1;
		break;
		case 2:
			list = &pinc_irq_2;
		break;
	}

	// Disables bit
	mal::reg::ClearBit(mal::reg::KPinChangeMskReg0 + offset, bit_number);

	// Decrements count
	group_regs_cnt[offset]--;
	
	if (group_regs_cnt[offset] <= 0) {
		mal::reg::ClearBit(mal::reg::KPinChangeMskReg0 + offset, bit_number);
	}

	INTERRUPT_LOCK()
		containers::LinkedListIterator<IrqRecord> iterator(list);
		while (iterator.HasNext()) {
			if (iterator.Next().pin_address == pin_address &&
				iterator.Next().bit_number == bit_number &&
				iterator.Next().trigger == trigger) {
				iterator.RemoveAdvance();
			} else {
				iterator.Advance();
			}			
		}
	UNLOCK()

	return IRQ_UNREGISTERED;
}