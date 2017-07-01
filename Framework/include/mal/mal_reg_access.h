/*
 * mal_reg_access.h
 *
 * Created: 6/10/2017 8:04:56 PM
 *  Author: roncato
 */ 


#ifndef MAL_REG_ACCESS_H_
#define MAL_REG_ACCESS_H_

namespace mal {
namespace reg {

template<typename register_address_t, typename register_value_t>
inline void SetVal(register_address_t address, register_value_t value) {
	*reinterpret_cast<volatile register_value_t*>(address) = value;
}

template<typename register_address_t, typename register_value_t>
inline register_value_t GetVal(register_address_t address) {
	return *reinterpret_cast<volatile register_value_t*>(address);
}

template<typename register_address_t, typename register_value_t>
inline void And(register_address_t address, register_value_t value) {
	*reinterpret_cast<volatile register_value_t*>(address) &= value;
}

template<typename register_address_t, typename register_value_t>
inline void Or(register_address_t address, register_value_t value) {
	*reinterpret_cast<volatile register_value_t*>(address) |= value;
}

template<typename register_address_t, typename register_value_t>
inline void AndNot(register_address_t address, register_value_t value) {
	*reinterpret_cast<volatile register_value_t*>(address) &= register_value_t(~value);
}

template<typename register_address_t, typename register_value_t>
inline void SetBit(register_address_t address, register_value_t value) {
	*reinterpret_cast<volatile register_value_t*>(address) |= static_cast<register_value_t>(1UL << value);
}

template<typename register_address_t, typename register_value_t>
inline void ClearBit(register_address_t address, register_value_t value) {
	*reinterpret_cast<volatile register_value_t*>(address) &= static_cast<register_value_t>(~static_cast<register_value_t>(1UL << value));
}

template<typename register_address_t, typename register_value_t>
inline void NotBit(register_address_t address, register_value_t value) {
	*reinterpret_cast<volatile register_value_t*>(address) ^= static_cast<register_value_t>(1UL << value);
}

template<typename register_address_t, typename register_value_t>
inline bool GetBit(register_address_t address, register_value_t value) {
	return (static_cast<volatile register_value_t>(mal::reg::GetVal<register_address_t, register_value_t>(address) & static_cast<register_value_t>(1UL << value)) != static_cast<register_value_t>(0U));
}

template<typename register_address_t,
		typename register_value_t,
		register_address_t address,
		register_value_t value = static_cast<register_value_t>(0)>
struct Access {
	inline static void SetVal() {
		mal::reg::SetVal(address, value);
	}
	inline static register_value_t GetVal() {
		return mal::reg::GetVal<register_address_t, register_value_t>(address);
	}
	inline static void And() {
		mal::reg::And(address, value);
	}
	inline static void Or() {
		mal::reg::Or(address, value);
	}
	inline static void AndNot() {
		mal::reg::AndNot(address, value);
	}
	inline static void SetBit() {	
		mal::reg::SetBit(address, value);
	}
	inline static void ClearBit() {
		mal::reg::ClearBit(address, value);
	}
	inline static void NotBit() {
		mal::reg::NotBit(address, value);
	}
	inline static bool GetBit() {
		return mal::reg::GetBit(address, value);
	}
};
		
} // namespace reg
} // namespace mal


#endif /* MAL_REG_ACCESS_H_ */