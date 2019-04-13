#ifndef __IO_H
#define __IO_H

#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "can_database.h"
#include "tim.h"

#define Legs PBout(11)
#define Rise PBout(3)
#define Gerege PAout(15)
#define WideLeg PBout(10)

#define LED PCout(0)

#define SWITCH_ON  1
#define SWITCH_OFF 0
void IO_Config(void);
#endif
