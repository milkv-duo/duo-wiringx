/*
  Copyright (c) 2024 Shenzhen Milk-V Technology Co., Ltd
  Author: William James <willian@milkv.io>
          Carbon <carbon@milkv.io>
          Zhang Yuntian <zhangyuntian@milkv.io>

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef __WIRINGX_SOPHGO_COMMON_H
#define __WIRINGX_SOPHGO_COMMON_H

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../../wiringx.h"
#include "../soc.h"
#include "../../i2c-dev.h"

// register data currently unavailable.
// Assuming each bank contains 32 GPIO lines, we fill them with placeholder data, in case they become available in the future.
#define GPIO_UNAVAILABLE(x) {(x), 0, 0, {0, 0}, {0, 0}, {0, 0}, FUNCTION_UNKNOWN, 0, 0}

const static uintptr_t gpio_register_physical_address[MAX_REG_AREA] = {0x03020000, 0x03021000, 0x03022000, 0x05021000};
#define GPIO_SWPORTA_DR		0x000
#define GPIO_SWPORTA_DDR	0x004
#define GPIO_EXT_PORTA		0x050

static uintptr_t pinmux_register_virtual_address = NULL;

#define PINMUX_BASE		0x03001000	// pinmux group 1

#define CLEAR_BITS(addr, bit, size) \
	(*addr = *addr & ~(~(-1 << size) << bit) | (~(-1 << size) << bit << REGISTER_WRITE_MASK))
#define GET_BITS(addr, bit, size) \
	((*addr & ~(-1 << size) << bit) >> (bit - size))

#endif
