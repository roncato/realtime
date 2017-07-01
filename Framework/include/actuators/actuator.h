/*
 * actuator.h
 *
 * Created: 6/20/2017 9:34:26 PM
 *  Author: roncato
 */ 


#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include "actuators/actuator.h"

namespace actuator {

/*
* Interrupt return type
*/
enum ActuatorReturn : uint8_t {
	ACTUATOR_RETURN_OKAY,
	ACTUATOR_RETURN_NOT_AFFECTED,
	ACTUATOR_RETURN_ERROR
};

template <class T>
class Actuator {
public:
	virtual bool Write(T) const = 0;
	virtual ActuatorReturn Open() = 0;
	virtual ActuatorReturn Close() = 0;
	virtual bool IsOpen() const = 0;
};

}

#endif /* ACTUATOR_H_ */