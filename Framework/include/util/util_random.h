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

uint16_t NextUint32();

}

}


#endif /* UTIL_RANDOM_H_ */