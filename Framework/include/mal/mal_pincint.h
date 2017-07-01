/*
 * pin_change.h
 *
 * Created: 6/10/2017 5:51:14 PM
 *  Author: roncato
 */ 

#ifndef MAL_PINCINT_H_
#define MAL_PINCINT_H_

#include <stdint.h>

#include "mal/mal_interrupt.h"
#include "mal/mal_reg.h"

namespace mal {
namespace irq {

/*
 * Request interrupt number types
 */
typedef uint8_t pcint_number_t;
typedef void (*PCIntRequestHandler)(void*);

constexpr mal::reg::reg_value_t LOW		= static_cast<mal::reg::reg_value_t>(0UL);
constexpr mal::reg::reg_value_t HIGH	= static_cast<mal::reg::reg_value_t>(1UL);

constexpr uint8_t kMaxHandlersNumber = 24;

const uint8_t pinc_to_pin_map[mal::reg::kPinPortNumbers] { mal::reg::kInputPinsRegB, mal::reg::kInputPinsRegC, mal::reg::kInputPinsRegD };

/*
 * Register interrupt request service
 */
IrqReturn RegisterIrq(void* context, const pcint_number_t pcint_number, const PCIntRequestHandler pcint_request_handler, const mal::irq::IrqTrigger trigger);

/*
 * Unregister interrupt request service
 */
IrqReturn UnregisterIrq(const pcint_number_t, const mal::irq::IrqTrigger trigger);

}
}

#endif /* MAL_PINCINT_H_ */