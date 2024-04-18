/*
  Copyright (c) 2024 Shenzhen Milk-V Technology Co., Ltd
  Author: William James <willian@milkv.io>
          Carbon <carbon@milkv.io>
          Zhang Yuntian <zhangyuntian@milkv.io>

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef __WIRINGX_sg200x_H
#define __WIRINGX_sg200x_H

#include "../soc.h"
#include "../../wiringx.h"

extern struct soc_t *sg200x;

void sg200xInit(void);

#endif
