#ifndef __ROTATION_ENCODE_H
#define __ROTATION_ENCODE_H

#include "stm32f10x.h"                  // Device header

void RotationEncode_init(void);
int16_t RotationEncode_getChangeCnt(void);

#endif // __ROTATION_ENCODE_H
