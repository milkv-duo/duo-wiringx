/*
  Copyright (c) 2024 Shenzhen Milk-V Technology Co., Ltd
  Author: William James <willian@milkv.io>
          Carbon <carbon@milkv.io>
          Zhang Yuntian <zhangyuntian@milkv.io>

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "../../soc/soc.h"
#include "../../wiringx.h"
#include "../platform.h"
#include "duo.h"
#include "duo256m.h"

struct platform_t *milkv_duo256m = NULL;

static int map[] = {
	/*	XGPIOA[28]	XGPIOA[29]	XGPIOA[19]	XGPIOA[18]	*/
			28,			29,			19,			18,

	/*	PWR_GPIO[19]	PWR_GPIO[20]	PWR_GPIO[23]	PWR_GPIO[22]	*/
			115,			116,			119,			118,

	/*	PWR_GPIO[21]	PWR_GPIO[18]	XGPIOC[14]	XGPIOC[15]	*/
			117,			114,			78,			79,

	/*	XGPIOA[16]	XGPIOA[17]	XGPIOA[14]	XGPIOA[15]	*/
			16,			17,			14,			15,

	/*	XGPIOA[23]	XGPIOA[24]	XGPIOA[22]	XGPIOA[25]	*/
			23,			24,			22,			25,

	/*	XGPIOA[27]	XGPIOA[26]	PWR_GPIO[4]	*/
			27,			26,			100,			-1,

	/*		   PWR_GPIO[2]	XGPIOB[3]	XGPIOB[6]	*/
			-1,			98,			35,			38
};

#define _sizeof(arr) (sizeof(arr) / sizeof(arr[0]))

static int milkv_duo256mValidGPIO(int pin) {
	if(pin >= 0 && pin < _sizeof(map)) {
		if(map[pin] == -1) {
			return -1;
		}
		return 0;
	} else {
		return -1;
	}
}

static int milkv_duo256mSetup(void) {
	milkv_duo256m->soc->setup();
	milkv_duo256m->soc->setMap(map, _sizeof(map));
	milkv_duo256m->soc->setIRQ(map, _sizeof(map));
	return 0;
}

void milkv_duo256mInit(void) {
	platform_register(&milkv_duo256m, "milkv_duo256m");

	milkv_duo256m->soc = soc_get("Sophgo", "sg200x");
	milkv_duo256m->soc->setMap(map, _sizeof(map));

	milkv_duo256m->digitalRead = milkv_duo256m->soc->digitalRead;
	milkv_duo256m->digitalWrite = milkv_duo256m->soc->digitalWrite;
	milkv_duo256m->pinMode = milkv_duo256m->soc->pinMode;
	milkv_duo256m->setup = &milkv_duo256mSetup;

	milkv_duo256m->isr = milkv_duo256m->soc->isr;
	milkv_duo256m->waitForInterrupt = milkv_duo256m->soc->waitForInterrupt;

	milkv_duo256m->selectableFd = milkv_duo256m->soc->selectableFd;
	milkv_duo256m->gc = milkv_duo256m->soc->gc;

	milkv_duo256m->validGPIO = &milkv_duo256mValidGPIO;
}
