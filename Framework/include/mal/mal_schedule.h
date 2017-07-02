/*
 * mal_schedule.h
 *
 * Created: 6/17/2017 6:20:27 PM
 *  Author: roncato
 */ 


#ifndef MAL_SCHEDULE_H_
#define MAL_SCHEDULE_H_

#include "mal/mal_time.h"

namespace mal {
namespace schedule {

/*
* Task handle
*/
typedef uint16_t handle_t;

/*
* Scheduled task handler type
*/
typedef void (*scheduled_task_handler_t)(void*);

/*
* Null handle
*/
constexpr handle_t kNullHandle = 0U;

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
* Register interrupt request service
*/
bool ScheduleTask(void* context, const mal::time::time_t delay, const mal::schedule::scheduled_task_handler_t handler, bool is_in_interrupt_handler);

/*
* Schedule recurring task
*/
handle_t ScheduleTaskAtFixedRate(void* context, const mal::time::time_t delay, const mal::time::time_t initial_delay, const mal::schedule::scheduled_task_handler_t handler);

/*
* Dequeue task from schedule. Complexity O(n) use with care. Acquires interrupt lock
*/
bool UnscheduleTask(handle_t);

} // namespace schedule
} // namespace mal


#endif /* MAL_SCHEDULE_H_ */