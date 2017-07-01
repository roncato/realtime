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
* Scheduled task handler type
*/
typedef void (*scheduled_task_handler_t)(void*);

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
bool ScheduleTaskAtFixedRate(void* context, const mal::time::time_t delay, const mal::time::time_t initial_delay, const mal::schedule::scheduled_task_handler_t handler);

} // namespace schedule
} // namespace mal


#endif /* MAL_SCHEDULE_H_ */