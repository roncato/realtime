/*
 * actuator_pwm.h
 *
 * Created: 6/29/2017 8:57:13 PM
 *  Author: roncato
 */ 


#ifndef ACTUATOR_PWM_H_
#define ACTUATOR_PWM_H_

#include "actuators/actuator.h"
#include "mal/mal_time.h"
#include "mal/mal_reg.h"
#include "mal/mal_reg_access.h"

namespace actuator {

enum PwmOutputNumber : uint8_t {
	PWM_OUTPUT_A = 0,
	PWM_OUTPUT_B
};

enum PwmMode : uint8_t {
	PWM_NORMAL = 0,
	PWM_FAST
};

template <PwmOutputNumber output_number, uint8_t timer_number, PwmMode mode>
class PwmActuator {
public:
	PwmActuator() {
		switch (timer_number) {
			default:
				control_reg_address_ = mal::reg::kTimerControlReg0A;
				switch (output_number) {
					default:
						compare_reg_address_ = mal::reg::kTimerCompareReg0A;
						compare_reg_bit_val_ = mal::reg::kBitVal7;
					break;
					case PWM_OUTPUT_B:
						compare_reg_address_ = mal::reg::kTimerCompareReg0B;
						compare_reg_bit_val_ = mal::reg::kBitVal5;
					break;
				}
				if (timer_number != mal::time::kSystemTickTimer) {
					mal::reg::Or(mal::reg::kTimerControlReg0B, static_cast<uint8_t>(0x03));
				}
			break;
			case mal::time::TIMER_1:
				control_reg_address_ = mal::reg::kTimerControlReg1A;
				switch (output_number) {
					default:
						compare_reg_address_ = mal::reg::kTimerCompareReg1LA;
						compare_reg_bit_val_ = mal::reg::kBitVal7;
					break;
					case PWM_OUTPUT_B:
						compare_reg_address_ = mal::reg::kTimerCompareReg1LB;
						compare_reg_bit_val_ = mal::reg::kBitVal5;
					break;
				}
				if (timer_number != mal::time::kSystemTickTimer) {
					mal::reg::Or(mal::reg::kTimerControlReg1B, static_cast<uint8_t>(0x03));
				}
			break;
			case mal::time::TIMER_2:
				control_reg_address_ = mal::reg::kTimerControlReg2A;
				switch (output_number) {
					default:
						compare_reg_address_ = mal::reg::kTimerCompareReg2A;
						compare_reg_bit_val_ = mal::reg::kBitVal7;
					break;
					case PWM_OUTPUT_B:
						compare_reg_address_ = mal::reg::kTimerCompareReg2B;
						compare_reg_bit_val_ = mal::reg::kBitVal5;
					break;
				}
				if (timer_number != mal::time::kSystemTickTimer) {
					mal::reg::Or(mal::reg::kTimerControlReg2B, static_cast<uint8_t>(0x04));
				}
			break;
		}
	}
	ActuatorReturn Open() {
		if (!is_open) {
			switch (mode) {
				case PWM_NORMAL: {
					uint8_t reg_val = compare_reg_bit_val_ | mal::reg::kBitVal0;
					mal::reg::Or(control_reg_address_, static_cast<uint8_t>(reg_val));
				}					
				break;
				case PWM_FAST: {
					uint8_t reg_val;
					if (timer_number == mal::time::TIMER_1) {
						reg_val = compare_reg_bit_val_ | mal::reg::kBitVal0;
						mal::reg::Access<uint8_t, uint8_t, mal::reg::kTimerControlReg1B, mal::reg::kBitVal3>::Or();
					} else {
						reg_val = compare_reg_bit_val_ | mal::reg::kBitVal1 | mal::reg::kBitVal0;
					}					
					mal::reg::Or(control_reg_address_, static_cast<uint8_t>(reg_val));
				}					
				break;
			}
			is_open = true;
		}
		return ACTUATOR_RETURN_OKAY;
	}
	ActuatorReturn Close() {
		if (is_open) {
			mal::reg::And(control_reg_address_, ~compare_reg_bit_val_);
			is_open = false;
		}
		return ACTUATOR_RETURN_OKAY;
	}
	inline bool Write(uint8_t value) {
		mal::reg::SetVal(compare_reg_address_, value);
		return true;
	}
	inline bool IsOpen() const {
		return is_open;
	}
private:
	bool is_open{};
	uint8_t control_reg_address_;
	uint8_t compare_reg_address_;
	uint8_t compare_reg_bit_val_;
};

}


#endif /* ACTUATOR_PWM_H_ */