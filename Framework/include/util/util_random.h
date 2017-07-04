/*
 * util_random.h
 *
 * Created: 6/25/2017 2:53:07 PM
 *  Author: roncato
 */ 


#ifndef UTIL_RANDOM_H_
#define UTIL_RANDOM_H_

#include <stdint.h>

namespace util {
namespace random {

void Initialize(uint32_t seed);

uint8_t NextUint8();
uint16_t NextUint16();
uint32_t NextUint32();
float NextFloat();

}

}


#endif /* UTIL_RANDOM_H_ */