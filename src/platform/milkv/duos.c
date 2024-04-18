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

#include "duos.h"

struct platform_t *milkv_duos = NULL;

static int map[] = {
	/*	XGPIOA[29]	3V3		VSYS		XGPIOB[20]	*/
		29,		-1,		-1,		52,

	/*	VSYS		XGPIOB[21]	GND		XGPIOB[18]	*/
		-1,		53,		-1,		50,

	/*	XGPIOA[16]	XGPIOB[17]	XGPIOA[17]	XGPIOB[11]	*/
		16,		49,		17,		43,

	/*	XGPIOB[19]	XGPIOB[12]	GND		XGPIOB[22]	*/
		51,		44,		-1,		54,

	/*	XGPIOA[20]	N/A		XGPIOA[19]	XGPIOB[13]	*/
		20,		-1,		19,		45,

	/*	GND		XGPIOB[14]	XGPIOA[18]	XGPIOB[15]	*/
		-1,		46,		18,		47,

	/*	XGPIOB[16]	GND		XGPIOA[28]	XGPIOC[13]	*/
		48,		-1,		28,		77,

	/*	XGPIOC[15]	XGPIOC[12]	XGPIOC[14]	GND		*/
		79,		76,		78,		-1,

	/*	GND		XGPIOC[17]	XGPIOC[21]	XGPIOC[16]	*/
		-1,		81,		85,		80,

	/*	XGPIOC[20]	GND		GND		XGPIOC[19]	*/
		84,		-1,		-1,		83,

	/*	PWR_GPIO[0]	XGPIOC[18]	PWR_GPIO[1]	3V3		*/
		96,		82,		97,		-1,

	/*	PWR_GPIO[2]	AUDIO_IN_L	XGPIOB[3]	AUDIO_IN_R	*/
		98,		-1,		35,		-1,

	/*	XGPIOB[2]	AUDIO_OUT_L	XGPIOB[1]	AUDIO_OUT_R	*/
		34,		-1,		33,		-1,
};

static int milkv_duosValidGPIO(int pin) {
	return milkvValidGPIO(pin, map, _sizeof(map));
}

static int milkv_duosSetup(void) {
	return milkvSetup(milkv_duos, map, _sizeof(map));
}

void milkv_duosInit(void) {
	platform_register(&milkv_duos, "milkv_duos");

	milkv_duos->soc = soc_get("Sophgo", "SG200X");
	milkv_duos->soc->setMap(map, _sizeof(map));

	milkvInit(milkv_duos);

	milkv_duos->setup = &milkv_duosSetup;
	milkv_duos->validGPIO = &milkv_duosValidGPIO;
}
