/*
 * sensor.h
 *
 * Created: 6/11/2017 9:16:16 PM
 *  Author: roncato
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

namespace sensor {

/*
* Interrupt return type
*/
enum SensorReturn : uint8_t {
	SENSOR_RETURN_OKAY,
	SENSOR_RETURN_NOT_AFFECTED,
	SENSOR_RETURN_ERROR
};

typedef void (*SensorReadEventHandler)(void*);

template <class T>
class Sensor {
public:

	virtual T Read() const = 0;
	virtual SensorReturn Open() = 0;
	virtual SensorReturn Close() = 0;
	virtual bool IsOpen() const = 0;

	virtual void SetEventHandler(SensorReadEventHandler handler) {
		SetEventHandler(nullptr, handler);
	};
	virtual void SetEventHandler(void*, SensorReadEventHandler) {};
	virtual void RemoveEventHandler() {};
};

} // namespace sensor

#endif /* SENSOR_H_ */