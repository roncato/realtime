/*
 * example_comm_serial.cpp
 *
 * Created: 7/4/2017 3:13:59 AM
 *  Author: roncato
 */ 

#include <string.h>

#include "mal/mal_interrupt.h"
#include "mal/mal_time.h"
#include "mal/mal_cpu.h"
#include "communication/comm_serial.h"
#include <math.h>

namespace example {

void Serial(void) {

	mal::irq::EnableAll();
	mal::time::Initialize();

	comm::Serial serial(comm::SERIAL_BAUD_115_2k);
	serial.Open();

	const char* line = "READY\n\r";
	serial.Write((uint8_t*)line, strlen(line));

	uint8_t buffer[255];

	while (1) {
		//if (serial.Available()) {
			//uint8_t data;
			//serial.Read(data);
			//serial.Write(data);
		//}
		if (serial.Available()) {
			const auto read = serial.Read(buffer, sizeof(buffer));
			serial.Write(buffer, read);
		}
	}
}

}