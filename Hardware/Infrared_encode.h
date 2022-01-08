#ifndef __INFRARED_H
#define __INFRARED_H

#include "stm32f10x.h"                  // Device header

void InfraredEncode_init(void);
uint16_t InfraredEncode_getCnt(void);

#endif // __INFRARED_H
