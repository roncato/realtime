/*
 * actuator_servo.cpp
 *
 * Created: 6/20/2017 9:36:42 PM
 *  Author: roncato
 */ 

#include <string.h>
#include <avr/interrupt.h>

#include "mal/mal_time.h"
#include "mal/mal_reg_access.h"
#include "util/util_lock.h"
#include "actuators/actuator_servo.h"

namespace {

typedef struct {
	uint16_t pulse_ticks;
	bool is_open;
	uint8_t port_address;
	uint8_t pin_number;
} ServoRecord;

enum ServoRefreshState {
	REFRESHING,
	IDLE
};

constexpr float kMicroSecondsPerTimerTick = CLOCK_CYCLES_TO_MICROSECOND(8.0f);
constexpr mal::time::time_t kMaxDelayMicros = kMicroSecondsPerTimerTick * 65535;
constexpr mal::time::time_t kMicroSecondsPerTimerOverflow = CLOCK_CYCLES_TO_MICROSECOND(kMicroSecondsPerTimerTick * 65535);
constexpr uint8_t kTrimTicks = 0x04U;
constexpr uint8_t kMaxServos = 12;
constexpr uint16_t kRefreshIntervalTicks = (actuator::kServoRefreshInterval / kMicroSecondsPerTimerTick);

inline uint16_t GetTimerTicks();
inline void SetTimerTicks(uint16_t ticks);
inline void SetTimerCompare(uint16_t ticks);

volatile bool is_initialized{};

volatile int8_t servo_index{};

volatile uint8_t servos_number{};
uint8_t open_servos{};

volatile ServoRecord servos[kMaxServos];

volatile ServoRefreshState refresh_state;

inline uint16_t MicrosToTicks(uint16_t us) {
	return us / kMicroSecondsPerTimerTick;
}

inline uint16_t TicksToMicros(uint16_t ticks) {
	return ticks * kMicroSecondsPerTimerTick;
}

void Initialize() {
	if (!is_initialized) {
		
		INTERRUPT_LOCK()
		
			refresh_state = IDLE;
			servo_index = static_cast<int8_t>(-1);
			for (auto i = 0; i < kMaxServos; ++i) {
				servos[i].is_open = false;
				servos[i].pulse_ticks = MicrosToTicks(actuator::kServoDefaultPulseWidth);
			}

			// clear timer1 compare flag
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerIntFlagReg1, 0x01U>::ClearBit();

			// enable timer1 compare A interrupt enabled
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerInterruptMskReg1, 0x01U>::SetBit();

			// initialize timer counter to max value and reset
			SetTimerCompare(kRefreshIntervalTicks);
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerCounterReg1H, 0x00U>::SetVal();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerCounterReg1L, 0x00U>::SetVal();

			// Enable timer
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg1B, 0x00U>::ClearBit();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg1B, 0x01U>::SetBit();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg1B, 0x02U>::ClearBit();

		UNLOCK()

		is_initialized = true;
	}
}

void Uninitialize() {
	if (is_initialized) {
		
		INTERRUPT_LOCK()

			// disable timer
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg1B, 0x00U>::ClearBit();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg1B, 0x01U>::ClearBit();
			mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg1B, 0x02U>::ClearBit();

		UNLOCK()

		is_initialized = false;
	}
}

inline uint16_t GetTimerTicks() {
	uint16_t timer_ticks = mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerCounterReg1L>::GetVal();
	timer_ticks |= mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerCounterReg1H>::GetVal() << 8;
	return timer_ticks;
}

inline void SetTimerTicks(uint16_t ticks) {
	mal::reg::SetVal(mal::reg::kTimerCounterReg1H, static_cast<uint8_t>((ticks >> 8) & 0xFFU));
	mal::reg::SetVal(mal::reg::kTimerCounterReg1L, static_cast<uint8_t>(ticks & 0xFFU));
}

inline void SetTimerCompare(uint16_t ticks) {
	mal::reg::SetVal(mal::reg::kTimerCompareReg1HA, static_cast<uint8_t>((ticks >> 8) & 0xFFU));
	mal::reg::SetVal(mal::reg::kTimerCompareReg1LA, static_cast<uint8_t>(ticks & 0xFFU));
}

ISR(TIMER1_COMPA_vect) {
	switch(refresh_state) {
		case REFRESHING:
			if (servos[servo_index].is_open) {
				mal::reg::ClearBit(servos[servo_index].port_address, servos[servo_index].pin_number);
			}
			++servo_index;
			if (servo_index < servos_number) {
				if (servos[servo_index].is_open) {
					mal::reg::SetBit(servos[servo_index].port_address, servos[servo_index].pin_number);
					SetTimerCompare(GetTimerTicks() + servos[servo_index].pulse_ticks);	
				}
			} else {
				const auto timer_ticks = GetTimerTicks();
				if (timer_ticks > kRefreshIntervalTicks) { // overdue
					refresh_state = IDLE;
					SetTimerCompare(kTrimTicks);
				} else {
					refresh_state = IDLE;
					SetTimerCompare(kRefreshIntervalTicks + kTrimTicks);					
				}				
			}
			break;
		case IDLE:
			SetTimerTicks(0);
			servo_index = 0;
			if (servos[servo_index].is_open) {
				mal::reg::SetBit(servos[servo_index].port_address, servos[servo_index].pin_number);
				SetTimerCompare(GetTimerTicks() + servos[servo_index].pulse_ticks);	
			}
			refresh_state = REFRESHING;
			break;
	}
}

}

bool actuator::ServoActuator::Write(uint16_t pulse_width) {
	if (pulse_width >= kServoMinPulseWidth && pulse_width <= kServoMaxPulseWidth) {
		pulse_width_ = pulse_width;
		uint16_t pulse_ticks = MicrosToTicks(pulse_width_);
		INTERRUPT_LOCK()
			servos[servo_number_].pulse_ticks = pulse_ticks;
		UNLOCK()
		return true;
	}
	return false;
};

actuator::ActuatorReturn actuator::ServoActuator::Open() {
	if (servos_number > kMaxServos) {
		return ACTUATOR_RETURN_ERROR;
	}
	if (!is_initialized) {
		Initialize();
	}
	if (!is_open_) {
		is_open_ = true;
		servo_number_ = servos_number++;
		++open_servos;
		INTERRUPT_LOCK()
			servos[servo_number_].port_address = port_address_;
			servos[servo_number_].pin_number = pin_number_;
			servos[servo_number_].is_open = true;
		UNLOCK()
		return ACTUATOR_RETURN_OKAY;
	}
	return ACTUATOR_RETURN_NOT_AFFECTED;
 }

actuator::ActuatorReturn actuator::ServoActuator::Close() {
	if (is_open_) {
		is_open_ = false;
		INTERRUPT_LOCK()
			servos[servo_number_].is_open = false;
		UNLOCK()
		--open_servos;
		if (open_servos <= 0) {
			Uninitialize();	
		}
		return ACTUATOR_RETURN_OKAY;
	}
	return ACTUATOR_RETURN_NOT_AFFECTED;
}