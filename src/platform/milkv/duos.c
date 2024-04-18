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
#include "duos.h"

struct platform_t *milkv_duos = NULL;

static int map[] = {
	/* XGPIOA[29]      3v3         NULL    XGPIOB[20]     NULL    */
	       29,         -1,         -1,         52,         -1,       
	/* XGPIOB[21]      GND     XGPIOB[18]  XGPIOA[16] */
	       53,         -1,         50,         16,         
	/* XGPIOB[17]  XGPIOA[17]  XGPIOB[11]  XGPIOB[19] */
         49,         17,         43,         51, 
	/* XGPIOB[12]      GND     XGPIOB[22]  XGPIOA[20] */
	       44,         -1,         54,         20,       
	/*     3V3     XGPIOA[19]  XGPIOB[13]      GND    */
	       -1,         19,         45,         -1,               
	/* XGPIOB[14]  XGPIOA[18]  XGPIOB[15]  XGPIOB[16] */
	       46,         18,         47,         48, 
	/*     GND     XGPIOA[28]  XGPIOC[13]  XGPIOC[15] */
	       -1,         28,         77,         79, 
	/* XGPIOC[12]  XGPIOC[14]     GND          GND    */
	       76,         78,         -1,         -1,        
	/* XGPIOC[17]  XGPIOC[21]  XGPIOC[16]  XGPIOC[20] */
	       81,         85,         80,         84, 	
	/*    GND          GND     XGPIOC[19]  PWR_GPIO[0]*/
	      -1,          -1,         83,         96, 
	/* XGPIOC[18]  PWR_GPIO[1]    3V3      PWR_GPIO[2]*/
	       82,         97,         -1,         98,         
  /* XGPIOC[23]  XGPIOB[3]   XGPIOC[22]  XGPIOB[2]  */
	       87,         35,         86,         34,        
	/* XGPIOC[25]   XGPIOB[1]   XGPIOC[24]    NULL    */
	       89,         33,         88,         -1,
};

#define _sizeof(arr) (sizeof(arr) / sizeof(arr[0]))

static int milkv_duosValidGPIO(int pin) {
	if(pin >= 0 && pin < _sizeof(map)) {
		if(map[pin] == -1) {
			return -1;
		}
		return 0;
	} else {
		return -1;
	}
}

static int milkv_duosSetup(void) {
	milkv_duos->soc->setup();
	milkv_duos->soc->setMap(map, _sizeof(map));
	milkv_duos->soc->setIRQ(map, _sizeof(map));
	return 0;
}

void milkv_duosInit(void) {
	platform_register(&milkv_duos, "milkv_duos");

	milkv_duos->soc = soc_get("Sophgo", "sg200x");
	milkv_duos->soc->setMap(map, _sizeof(map));

	milkv_duos->digitalRead = milkv_duos->soc->digitalRead;
	milkv_duos->digitalWrite = milkv_duos->soc->digitalWrite;
	milkv_duos->pinMode = milkv_duos->soc->pinMode;
	milkv_duos->setup = &milkv_duosSetup;

	milkv_duos->isr = milkv_duos->soc->isr;
	milkv_duos->waitForInterrupt = milkv_duos->soc->waitForInterrupt;

	milkv_duos->selectableFd = milkv_duos->soc->selectableFd;
	milkv_duos->gc = milkv_duos->soc->gc;

	milkv_duos->validGPIO = &milkv_duosValidGPIO;
}
