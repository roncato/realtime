/*
 * mal_interrupt.h
 *
 * Created: 6/10/2017 7:35:12 PM
 *  Author: roncato
 */ 

#ifndef MAL_INTERRUPT_H_
#define MAL_INTERRUPT_H_

#include <stdint.h>

namespace mal {
namespace irq {

typedef void config_type;

/*
 * Interrupt request trigger type
 */
enum IrqTrigger : uint8_t {
	IRQ_TRIGGER_NONE	= 0,
	IRQ_TRIGGER_RISING	= (1 << 1),
	IRQ_TRIGGER_FALLING	= (1 << 2),
	IRQ_TRIGGER_CHANGED	= (1 << 3),
	IRQ_TRIGGER_HIGH	= (1 << 4),
	IRQ_TRIGGER_LOW		= (1 << 5),
};

/*
 * Interrupt return type
 */
enum IrqReturn : uint8_t {
	IRQ_REGISTERED,
	IRQ_UNREGISTERED,
	IRQ_REPLACED,
	IRQ_ERROR
};

inline void EnableAll() {
	asm volatile ("sei	\n");
}

inline void DisableAll() {
	asm volatile ("cli	\n");
}

} // namespace irq
} // namespace mal

#endif /* MAL_INTERRUPT_H_ */