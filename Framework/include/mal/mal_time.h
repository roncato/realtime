/*
 * mal_time.h
 *
 * Created: 6/12/2017 6:21:52 AM
 *  Author: roncato
 */ 


#ifndef MAL_TIME_H_
#define MAL_TIME_H_

#include <stdint.h>
#include "mal/mal_cpu.h"

#define CLOCK_CYCLES_PER_MICROSECOND() (F_CPU / 1000000UL)
#define CLOCK_CYCLES_TO_MICROSECOND(v) ((v) / CLOCK_CYCLES_PER_MICROSECOND())
#define MICRO_SECONDS_PER_CLOCK_CYCLE(v) ((v) * CLOCK_CYCLES_PER_MICROSECOND())

namespace mal {
namespace time {

enum TimerNumber : uint8_t {
	TIMER_0 = 0,
	TIMER_1,
	TIMER_2
};

constexpr TimerNumber kSystemTickTimer = TIMER_0;

typedef uint32_t time_t;

/*
 * Initialize PIN CHANGE INTERRUPT
 */
void Initialize();

/*
 * Uninitialize PIN CHANGE INTERRUPT
 */
void Uninitialize();

/*
* Whether it is initialized
*/
inline bool IsInitialized();

/*
 * Gets system micros
 */
time_t Micros();

/*
 * Gets system micros to be used inside interrupts. It gets unpredictable after 256 ticks
 */
time_t MicrosInt();

/*
 * Gets system millis
 */
time_t Millis();

} // namespace time
} // namespace mal

#endif /* MAL_TIME_H_ */