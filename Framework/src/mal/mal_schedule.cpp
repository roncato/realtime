/*
 * mal_schedule.cpp
 *
 * Created: 6/17/2017 6:21:49 PM
 *  Author: roncato
 */ 

#include <avr/interrupt.h>
 
#include "mal/mal_time.h"
#include "mal/mal_reg.h"
#include "mal/mal_reg_access.h"
#include "mal/mal_interrupt.h"
#include "mal/mal_schedule.h"
#include "util/util_lock.h"

#include "containers/cont_sortedlist.h"

namespace {

/*
 * Scheduled task structure
 */
typedef struct {
	void* context;
	mal::time::time_t queue_time;
	mal::time::time_t due_time;
	mal::time::time_t delay;
	mal::schedule::scheduled_task_handler_t handler;
	bool recurring;
	mal::schedule::handle_t handle;
} ScheduledTask;

volatile bool is_initialized{false};

volatile mal::schedule::handle_t next_handle{};

constexpr mal::time::time_t kMicroSecondsPerTimerTick = CLOCK_CYCLES_TO_MICROSECOND(64);
constexpr mal::time::time_t kMaxDelayMicros = kMicroSecondsPerTimerTick * 256;
constexpr mal::time::time_t kMicroSecondsPerTimerOverflow = CLOCK_CYCLES_TO_MICROSECOND(kMicroSecondsPerTimerTick * 256);
constexpr uint8_t kTrimTicks = 0x01U;

containers::SortedList<mal::time::time_t, ScheduledTask> queue;

volatile mal::schedule::scheduled_task_handler_t global_handler;

volatile mal::time::time_t interrupt_time;

inline void ResetCompare(uint8_t next_cnt) {
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerInterruptMskReg2, 0x00U>::ClearBit();
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerInterruptMskReg2, 0x01U>::SetBit();
	mal::reg::SetVal(mal::reg::kTimerCompareReg2A, next_cnt);
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerCounterReg2, 0x00U>::SetVal();
}

inline void ResetOverFlow() {
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerInterruptMskReg2, 0x01U>::ClearBit();
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerInterruptMskReg2, 0x00U>::SetBit();
	mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerCounterReg2, 0x00U>::SetVal();
}

/*
 * Resets time schedule must be called with an interrupt lock held
 */
void ResetTimer() {
	const auto now = mal::time::MicrosInt();
	containers::Entry<mal::time::time_t, ScheduledTask> entry;
	if (queue.Peek(entry)) {
		if (entry.value.due_time > now) {
			// Get the max between current and queue time diff, when tasks are too fast and happen between
			// a system tick overflow, the overflow interrupt will be posponed until this is done as a result
			// we may get an inaccurate timestamp
			auto diff = entry.value.due_time - now;
			const auto diffq = entry.value.due_time - entry.value.queue_time;
			diff = diff < diffq ? diff : diffq;
			if (diff > kMaxDelayMicros) {
				ResetOverFlow();				
			} else {
				const auto next_cnt = static_cast<uint8_t>(diff / kMicroSecondsPerTimerTick + kTrimTicks);
				ResetCompare(next_cnt);
			}
		} else { // overdue
			ResetCompare(kTrimTicks);
		}		
	}
}

inline void RescheduleTask(const containers::Entry<mal::time::time_t, ScheduledTask>& entry) {
	queue.Add(entry.value.due_time, entry.value);
}

}

ISR(TIMER2_OVF_vect) {
	if (is_initialized) {
		if (!queue.IsEmpty()) {
			ResetTimer();
		}		
	}
}

ISR(TIMER2_COMPA_vect) {
	if (is_initialized) {
		containers::Entry<mal::time::time_t, ScheduledTask> entry;
		if (queue.Take(entry)) {
			entry.value.handler(entry.value.context);
			if (entry.value.recurring) {
				entry.value.queue_time = mal::time::MicrosInt();
				entry.value.due_time = entry.value.queue_time + entry.value.delay;
				RescheduleTask(entry);
			}
		}
		ResetTimer();
	}
}

void mal::schedule::Initialize() {

	if (!is_initialized) {

		INTERRUPT_LOCK()
		
			// clear timer2 compare flag
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerIntFlagReg2, 0x00U>::ClearBit();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerIntFlagReg2, 0x01U>::ClearBit();

			// enable timer2 compare A interrupt enabled
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerInterruptMskReg2, 0x01U>::SetBit();

			// initialize timer counter to max value and reset
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerCompareReg2A, 0xFFU>::SetVal();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerCounterReg2, 0x00U>::SetVal();

			// Enables timer
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg2B, 0x00U>::ClearBit();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg2B, 0x01U>::ClearBit();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg2B, 0x02U>::SetBit();

		INTERRUPT_UNLOCK()

		is_initialized = true;
	}
}

void mal::schedule::Uninitialize() {
	
	if (is_initialized) {

		INTERRUPT_LOCK()

			// Clear queue
			queue.Clear();

			// disable timer1 compare flag
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerInterruptMskReg2, 0x01U>::ClearBit();

			// clear interrupt flag
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerIntFlagReg2, 0x00U>::ClearBit();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerIntFlagReg2, 0x01U>::ClearBit();

			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg2B, 0x00U>::ClearBit();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg2B, 0x01U>::ClearBit();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg2B, 0x02U>::ClearBit();

		INTERRUPT_UNLOCK()
	}
}

inline bool mal::schedule::IsInitialized() {
	return is_initialized;
}

bool mal::schedule::ScheduleTask(void* context, 
								const mal::time::time_t delay, 
								const mal::schedule::scheduled_task_handler_t handler,
								bool is_in_interrupt_handler) {
							
	if (!is_initialized) {
		return false;
	}
	
	if (is_in_interrupt_handler) {
		const auto queue_time = mal::time::MicrosInt();
		const auto due_time = queue_time + delay;
		ScheduledTask task {context, queue_time, due_time, delay, handler, false, kNullHandle};
		queue.Add(due_time, task);
	} else {

		INTERRUPT_LOCK()
			const auto queue_time = mal::time::Micros();
			const auto due_time = queue_time + delay;
			ScheduledTask task {context, queue_time, due_time, delay, handler, false, kNullHandle};
			queue.Add(due_time, task);
			ResetTimer();

		INTERRUPT_UNLOCK()
	}
	return true;
}

mal::schedule::handle_t mal::schedule::ScheduleTaskAtFixedRate(void* context, 
								const mal::time::time_t initial_delay,
								const mal::time::time_t delay, 
								const mal::schedule::scheduled_task_handler_t handler) {
							
	if (!is_initialized) {
		return mal::schedule::kNullHandle;
	}
	
	const auto queue_time = mal::time::Micros();
	const auto due_time = queue_time + initial_delay;

	INTERRUPT_LOCK()

		++next_handle;
		ScheduledTask task {context, queue_time, due_time, delay, handler, true, next_handle};
		queue.Add(due_time, task);
		ResetTimer();

	INTERRUPT_UNLOCK()

	return next_handle;
}

bool mal::schedule::UnscheduleTask(mal::schedule::handle_t handle) {
	bool changed{};
	containers::SortedListIterator<mal::time::time_t, ScheduledTask> iterator(&queue);
	INTERRUPT_LOCK()
		while (iterator.HasNext()) {
			if (iterator.Next().value.handle == handle) {
				iterator.RemoveAdvance();
				changed = true;
				break;
			}
			iterator.Advance();
		}
	INTERRUPT_UNLOCK()
	return changed;
}