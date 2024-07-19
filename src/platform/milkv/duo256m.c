/*
  Copyright (c) 2024 Shenzhen Milk-V Technology Co., Ltd
  Author: William James <willian@milkv.io>
          Carbon <carbon@milkv.io>
          Zhang Yuntian <zhangyuntian@milkv.io>
          wcl <wangchenlong@milkv.io>

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "duo256m.h"

struct platform_t *milkv_duo256m = NULL;

static int map[] = {
	/*	XGPIOA[28]	XGPIOA[29]	XGPIOA[19]	XGPIOA[18]	*/
		28,		29,		19,		18,

	/*	PWR_GPIO[19]	PWR_GPIO[20]	PWR_GPIO[23]	PWR_GPIO[22]	*/
		115,		116,		119,		118,

	/*	PWR_GPIO[21]	PWR_GPIO[18]	XGPIOC[14]	XGPIOC[15]	*/
		117,		114,		78,		79,

	/*	XGPIOA[16]	XGPIOA[17]	XGPIOA[14]	XGPIOA[15]	*/
		16,		17,		14,		15,

	/*	XGPIOA[23]	XGPIOA[24]	XGPIOA[22]	XGPIOA[25]	*/
		23,		24,		22,		25,

	/*	XGPIOA[27]	XGPIOA[26]	PWR_GPIO[4]	N/A		*/
		27,		26,		100,		-1,

	/*	N/A		PWR_GPIO[2]	XGPIOB[3]	XGPIOB[6]	*/
		-1,		98,		35,		38
};

static int milkv_duo256mValidGPIO(int pin) {
	return milkvValidGPIO(pin, map, _sizeof(map));
}

static int milkv_duo256mSetup(void) {
	return milkvSetup(milkv_duo256m, map, _sizeof(map));
}

void milkv_duo256mInit(void) {
	platform_register(&milkv_duo256m, "milkv_duo256m");

	milkv_duo256m->soc = soc_get("Sophgo", "SG200X");
	milkv_duo256m->soc->setMap(map, _sizeof(map));

	milkvInit(milkv_duo256m);

	milkv_duo256m->setup = &milkv_duo256mSetup;
	milkv_duo256m->validGPIO = &milkv_duo256mValidGPIO;
		
	milkv_duo256m->pwmSetPeriod = milkv_duo256m->soc->socSetPWMPeriod;
	milkv_duo256m->pwmSetDuty = milkv_duo256m->soc->socSetPWMDuty;
	milkv_duo256m->pwmSetPolarity = milkv_duo256m->soc->socSetPWMPolarity;
	milkv_duo256m->pwmEnable = milkv_duo256m->soc->socEnablePWM;
}
