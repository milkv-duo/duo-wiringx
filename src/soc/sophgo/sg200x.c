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

#include "sg200x.h"

#define SG200X_GPIO_GROUP_COUNT 4

struct soc_t *sg200x = NULL;

static struct layout_t {
	char *name;
	int gpio_group;
	int num;

	struct {
		unsigned long offset;
		unsigned long value;
	} pinmux;

	struct {
		unsigned long offset;
		unsigned long bit;
	} direction;

	struct {
		unsigned long offset;
		unsigned long bit;
	} data;

	int support;
	enum pinmode_t mode;
	int fd;
} layout[] = {
	{"XGPIOA_0", 0, 480, {0x00, 0x3}, {GPIO_SWPORTA_DDR, 0}, {GPIO_SWPORTA_DR, 0}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_1", 0, 481, {0x04, 0x3}, {GPIO_SWPORTA_DDR, 1}, {GPIO_SWPORTA_DR, 1}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_2", 0, 482, {0x08, 0x3}, {GPIO_SWPORTA_DDR, 2}, {GPIO_SWPORTA_DR, 2}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_3", 0, 483, {0x0c, 0x3}, {GPIO_SWPORTA_DDR, 3}, {GPIO_SWPORTA_DR, 3}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_4", 0, 484, {0x10, 0x3}, {GPIO_SWPORTA_DDR, 4}, {GPIO_SWPORTA_DR, 4}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_5", 0, 485, {0x14, 0x3}, {GPIO_SWPORTA_DDR, 5}, {GPIO_SWPORTA_DR, 5}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_6", 0, 486, {0x18, 0x3}, {GPIO_SWPORTA_DDR, 6}, {GPIO_SWPORTA_DR, 6}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_7", 0, 487, {0x1c, 0x3}, {GPIO_SWPORTA_DDR, 7}, {GPIO_SWPORTA_DR, 7}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_8", 0, 488, {0x20, 0x3}, {GPIO_SWPORTA_DDR, 8}, {GPIO_SWPORTA_DR, 8}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_9", 0, 489, {0x24, 0x3}, {GPIO_SWPORTA_DDR, 9}, {GPIO_SWPORTA_DR, 9}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_10", 0, 490, {0x28, 0x3}, {GPIO_SWPORTA_DDR, 10}, {GPIO_SWPORTA_DR, 10}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_11", 0, 491, {0x2c, 0x3}, {GPIO_SWPORTA_DDR, 11}, {GPIO_SWPORTA_DR, 11}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_12", 0, 492, {0x30, 0x3}, {GPIO_SWPORTA_DDR, 12}, {GPIO_SWPORTA_DR, 12}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_13", 0, 493, {0x34, 0x3}, {GPIO_SWPORTA_DDR, 13}, {GPIO_SWPORTA_DR, 13}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_14", 0, 494, {0x38, 0x3}, {GPIO_SWPORTA_DDR, 14}, {GPIO_SWPORTA_DR, 14}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_15", 0, 495, {0x3c, 0x3}, {GPIO_SWPORTA_DDR, 15}, {GPIO_SWPORTA_DR, 15}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_16", 0, 496, {0x40, 0x3}, {GPIO_SWPORTA_DDR, 16}, {GPIO_SWPORTA_DR, 16}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_17", 0, 497, {0x44, 0x3}, {GPIO_SWPORTA_DDR, 17}, {GPIO_SWPORTA_DR, 17}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_18", 0, 498, {0x68, 0x3}, {GPIO_SWPORTA_DDR, 18}, {GPIO_SWPORTA_DR, 18}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_19", 0, 499, {0x64, 0x3}, {GPIO_SWPORTA_DDR, 19}, {GPIO_SWPORTA_DR, 19}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_20", 0, 500, {0x6c, 0x3}, {GPIO_SWPORTA_DDR, 20}, {GPIO_SWPORTA_DR, 20}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_21", 0, 501, {0x48, 0x3}, {GPIO_SWPORTA_DDR, 21}, {GPIO_SWPORTA_DR, 21}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_22", 0, 502, {0x50, 0x3}, {GPIO_SWPORTA_DDR, 22}, {GPIO_SWPORTA_DR, 22}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_23", 0, 503, {0x5c, 0x3}, {GPIO_SWPORTA_DDR, 23}, {GPIO_SWPORTA_DR, 23}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_24", 0, 504, {0x60, 0x3}, {GPIO_SWPORTA_DDR, 24}, {GPIO_SWPORTA_DR, 24}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_25", 0, 505, {0x54, 0x3}, {GPIO_SWPORTA_DDR, 25}, {GPIO_SWPORTA_DR, 25}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_26", 0, 506, {0x4c, 0x3}, {GPIO_SWPORTA_DDR, 26}, {GPIO_SWPORTA_DR, 26}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_27", 0, 507, {0x58, 0x3}, {GPIO_SWPORTA_DDR, 27}, {GPIO_SWPORTA_DR, 27}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_28", 0, 508, {0x70, 0x3}, {GPIO_SWPORTA_DDR, 28}, {GPIO_SWPORTA_DR, 28}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_29", 0, 509, {0x74, 0x3}, {GPIO_SWPORTA_DDR, 29}, {GPIO_SWPORTA_DR, 29}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOA_30", 0, 510, {0x78, 0x3}, {GPIO_SWPORTA_DDR, 30}, {GPIO_SWPORTA_DR, 30}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	GPIO_UNAVAILABLE("XGPIOA_31"),
	{"XGPIOB_0", 1, 448, {0xec, 0x3}, {GPIO_SWPORTA_DDR, 0}, {GPIO_SWPORTA_DR, 0}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_1", 1, 449, {0xf0, 0x3}, {GPIO_SWPORTA_DDR, 1}, {GPIO_SWPORTA_DR, 1}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_2", 1, 450, {0xf4, 0x3}, {GPIO_SWPORTA_DDR, 2}, {GPIO_SWPORTA_DR, 2}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_3", 1, 451, {0xf8, 0x3}, {GPIO_SWPORTA_DDR, 3}, {GPIO_SWPORTA_DR, 3}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_4", 1, 452, {0xfc, 0x3}, {GPIO_SWPORTA_DDR, 4}, {GPIO_SWPORTA_DR, 4}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_5", 1, 453, {0x100, 0x3}, {GPIO_SWPORTA_DDR, 5}, {GPIO_SWPORTA_DR, 5}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_6", 1, 454, {0x108, 0x3}, {GPIO_SWPORTA_DDR, 6}, {GPIO_SWPORTA_DR, 6}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_7", 1, 455, {0x118, 0x3}, {GPIO_SWPORTA_DDR, 7}, {GPIO_SWPORTA_DR, 7}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_8", 1, 456, {0x114, 0x3}, {GPIO_SWPORTA_DDR, 8}, {GPIO_SWPORTA_DR, 8}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_9", 1, 457, {0x120, 0x3}, {GPIO_SWPORTA_DDR, 9}, {GPIO_SWPORTA_DR, 9}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_10", 1, 458, {0x11c, 0x3}, {GPIO_SWPORTA_DDR, 10}, {GPIO_SWPORTA_DR, 10}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_11", 1, 459, {0x134, 0x3}, {GPIO_SWPORTA_DDR, 11}, {GPIO_SWPORTA_DR, 11}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_12", 1, 460, {0x138, 0x3}, {GPIO_SWPORTA_DDR, 12}, {GPIO_SWPORTA_DR, 12}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_13", 1, 461, {0x13c, 0x3}, {GPIO_SWPORTA_DDR, 13}, {GPIO_SWPORTA_DR, 13}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_14", 1, 462, {0x140, 0x3}, {GPIO_SWPORTA_DDR, 14}, {GPIO_SWPORTA_DR, 14}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_15", 1, 463, {0x144, 0x3}, {GPIO_SWPORTA_DDR, 15}, {GPIO_SWPORTA_DR, 15}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_16", 1, 464, {0x148, 0x3}, {GPIO_SWPORTA_DDR, 16}, {GPIO_SWPORTA_DR, 16}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_17", 1, 465, {0x14c, 0x3}, {GPIO_SWPORTA_DDR, 17}, {GPIO_SWPORTA_DR, 17}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_18", 1, 466, {0x150, 0x3}, {GPIO_SWPORTA_DDR, 18}, {GPIO_SWPORTA_DR, 18}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_19", 1, 467, {0x154, 0x3}, {GPIO_SWPORTA_DDR, 19}, {GPIO_SWPORTA_DR, 19}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_20", 1, 468, {0x158, 0x3}, {GPIO_SWPORTA_DDR, 20}, {GPIO_SWPORTA_DR, 20}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_21", 1, 469, {0x15c, 0x3}, {GPIO_SWPORTA_DDR, 21}, {GPIO_SWPORTA_DR, 21}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_22", 1, 470, {0x160, 0x3}, {GPIO_SWPORTA_DDR, 22}, {GPIO_SWPORTA_DR, 22}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_23", 1, 471, {0x1cc, 0x3}, {GPIO_SWPORTA_DDR, 23}, {GPIO_SWPORTA_DR, 23}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_24", 1, 472, {0x128, 0x3}, {GPIO_SWPORTA_DDR, 24}, {GPIO_SWPORTA_DR, 24}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_25", 1, 473, {0x124, 0x3}, {GPIO_SWPORTA_DDR, 25}, {GPIO_SWPORTA_DR, 25}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_26", 1, 474, {0x130, 0x3}, {GPIO_SWPORTA_DDR, 26}, {GPIO_SWPORTA_DR, 26}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOB_27", 1, 475, {0x12c, 0x3}, {GPIO_SWPORTA_DDR, 27}, {GPIO_SWPORTA_DR, 27}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	GPIO_UNAVAILABLE("XGPIOB_28"),
	GPIO_UNAVAILABLE("XGPIOB_29"),
	GPIO_UNAVAILABLE("XGPIOB_30"),
	GPIO_UNAVAILABLE("XGPIOB_31"),
	{"XGPIOC_0", 2, 416, {0x164, 0x3}, {GPIO_SWPORTA_DDR, 0}, {GPIO_SWPORTA_DR, 0}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_1", 2, 417, {0x168, 0x3}, {GPIO_SWPORTA_DDR, 1}, {GPIO_SWPORTA_DR, 1}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_2", 2, 418, {0x16c, 0x3}, {GPIO_SWPORTA_DDR, 2}, {GPIO_SWPORTA_DR, 2}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_3", 2, 419, {0x170, 0x3}, {GPIO_SWPORTA_DDR, 3}, {GPIO_SWPORTA_DR, 3}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_4", 2, 420, {0x174, 0x3}, {GPIO_SWPORTA_DDR, 4}, {GPIO_SWPORTA_DR, 4}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_5", 2, 421, {0x178, 0x3}, {GPIO_SWPORTA_DDR, 5}, {GPIO_SWPORTA_DR, 5}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_6", 2, 422, {0x17c, 0x3}, {GPIO_SWPORTA_DDR, 6}, {GPIO_SWPORTA_DR, 6}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_7", 2, 423, {0x180, 0x3}, {GPIO_SWPORTA_DDR, 7}, {GPIO_SWPORTA_DR, 7}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_8", 2, 424, {0x184, 0x3}, {GPIO_SWPORTA_DDR, 8}, {GPIO_SWPORTA_DR, 8}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_9", 2, 425, {0x188, 0x3}, {GPIO_SWPORTA_DDR, 9}, {GPIO_SWPORTA_DR, 9}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_10", 2, 426, {0x18c, 0x3}, {GPIO_SWPORTA_DDR, 10}, {GPIO_SWPORTA_DR, 10}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_11", 2, 427, {0x190, 0x3}, {GPIO_SWPORTA_DDR, 11}, {GPIO_SWPORTA_DR, 11}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_12", 2, 428, {0x1b4, 0x3}, {GPIO_SWPORTA_DDR, 12}, {GPIO_SWPORTA_DR, 12}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_13", 2, 429, {0x1b8, 0x3}, {GPIO_SWPORTA_DDR, 13}, {GPIO_SWPORTA_DR, 13}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_14", 2, 430, {0x1ac, 0x3}, {GPIO_SWPORTA_DDR, 14}, {GPIO_SWPORTA_DR, 14}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_15", 2, 431, {0x1b0, 0x3}, {GPIO_SWPORTA_DDR, 15}, {GPIO_SWPORTA_DR, 15}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_16", 2, 432, {0x1a4, 0x3}, {GPIO_SWPORTA_DDR, 16}, {GPIO_SWPORTA_DR, 16}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_17", 2, 433, {0x1a8, 0x3}, {GPIO_SWPORTA_DDR, 17}, {GPIO_SWPORTA_DR, 17}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_18", 2, 434, {0x194, 0x3}, {GPIO_SWPORTA_DDR, 18}, {GPIO_SWPORTA_DR, 18}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_19", 2, 435, {0x198, 0x3}, {GPIO_SWPORTA_DDR, 19}, {GPIO_SWPORTA_DR, 19}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_20", 2, 436, {0x19c, 0x3}, {GPIO_SWPORTA_DDR, 20}, {GPIO_SWPORTA_DR, 20}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_21", 2, 437, {0x1a0, 0x3}, {GPIO_SWPORTA_DDR, 21}, {GPIO_SWPORTA_DR, 21}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_22", 2, 438, {0x1c0, 0x3}, {GPIO_SWPORTA_DDR, 22}, {GPIO_SWPORTA_DR, 22}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_23", 2, 439, {0x1bc, 0x3}, {GPIO_SWPORTA_DDR, 23}, {GPIO_SWPORTA_DR, 23}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_24", 2, 440, {0x1c8, 0x3}, {GPIO_SWPORTA_DDR, 24}, {GPIO_SWPORTA_DR, 24}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"XGPIOC_25", 2, 441, {0x1c4, 0x3}, {GPIO_SWPORTA_DDR, 25}, {GPIO_SWPORTA_DR, 25}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	GPIO_UNAVAILABLE("XGPIOC_26"),
	GPIO_UNAVAILABLE("XGPIOC_27"),
	GPIO_UNAVAILABLE("XGPIOC_28"),
	GPIO_UNAVAILABLE("XGPIOC_29"),
	GPIO_UNAVAILABLE("XGPIOC_30"),
	GPIO_UNAVAILABLE("XGPIOC_31"),
	{"PWR_GPIO_0", 3, 352, {0xa4, 0x0}, {GPIO_SWPORTA_DDR, 0}, {GPIO_SWPORTA_DR, 0}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_1", 3, 353, {0xa8, 0x0}, {GPIO_SWPORTA_DDR, 1}, {GPIO_SWPORTA_DR, 1}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_2", 3, 354, {0xac, 0x0}, {GPIO_SWPORTA_DDR, 2}, {GPIO_SWPORTA_DR, 2}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_3", 3, 355, {0x84, 0x3}, {GPIO_SWPORTA_DDR, 3}, {GPIO_SWPORTA_DR, 3}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_4", 3, 356, {0x88, 0x3}, {GPIO_SWPORTA_DDR, 4}, {GPIO_SWPORTA_DR, 4}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_5", 3, 357, {0x8c, 0x3}, {GPIO_SWPORTA_DDR, 5}, {GPIO_SWPORTA_DR, 5}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_6", 3, 358, {0x90, 0x3}, {GPIO_SWPORTA_DDR, 6}, {GPIO_SWPORTA_DR, 6}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_7", 3, 359, {0x94, 0x3}, {GPIO_SWPORTA_DDR, 7}, {GPIO_SWPORTA_DR, 7}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_8", 3, 360, {0x98, 0x3}, {GPIO_SWPORTA_DDR, 8}, {GPIO_SWPORTA_DR, 8}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_9", 3, 361, {0x9c, 0x3}, {GPIO_SWPORTA_DDR, 9}, {GPIO_SWPORTA_DR, 9}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_10", 3, 362, {0xb0, 0x3}, {GPIO_SWPORTA_DDR, 10}, {GPIO_SWPORTA_DR, 10}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_11", 3, 363, {0xb4, 0x3}, {GPIO_SWPORTA_DDR, 11}, {GPIO_SWPORTA_DR, 11}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_12", 3, 364, {0xb8, 0x3}, {GPIO_SWPORTA_DDR, 12}, {GPIO_SWPORTA_DR, 12}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_13", 3, 365, {0xbc, 0x3}, {GPIO_SWPORTA_DDR, 13}, {GPIO_SWPORTA_DR, 13}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_14", 3, 366, {0xc0, 0x3}, {GPIO_SWPORTA_DDR, 14}, {GPIO_SWPORTA_DR, 14}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_15", 3, 367, {0xc4, 0x3}, {GPIO_SWPORTA_DDR, 15}, {GPIO_SWPORTA_DR, 15}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_16", 3, 368, {0xc8, 0x3}, {GPIO_SWPORTA_DDR, 16}, {GPIO_SWPORTA_DR, 16}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_17", 3, 369, {0xcc, 0x3}, {GPIO_SWPORTA_DDR, 17}, {GPIO_SWPORTA_DR, 17}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_18", 3, 370, {0xd0, 0x3}, {GPIO_SWPORTA_DDR, 18}, {GPIO_SWPORTA_DR, 18}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_19", 3, 371, {0xd4, 0x3}, {GPIO_SWPORTA_DDR, 19}, {GPIO_SWPORTA_DR, 19}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_20", 3, 372, {0xd8, 0x3}, {GPIO_SWPORTA_DDR, 20}, {GPIO_SWPORTA_DR, 20}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_21", 3, 373, {0xdc, 0x3}, {GPIO_SWPORTA_DDR, 21}, {GPIO_SWPORTA_DR, 21}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_22", 3, 374, {0xe0, 0x3}, {GPIO_SWPORTA_DDR, 22}, {GPIO_SWPORTA_DR, 22}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_23", 3, 375, {0xe4, 0x3}, {GPIO_SWPORTA_DDR, 23}, {GPIO_SWPORTA_DR, 23}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	{"PWR_GPIO_24", 3, 376, {0x1d0, 0x3}, {GPIO_SWPORTA_DDR, 24}, {GPIO_SWPORTA_DR, 24}, FUNCTION_DIGITAL, PINMODE_NOT_SET, 0},
	GPIO_UNAVAILABLE("PWR_GPIO_25"),
	GPIO_UNAVAILABLE("PWR_GPIO_26"),
	GPIO_UNAVAILABLE("PWR_GPIO_27"),
	GPIO_UNAVAILABLE("PWR_GPIO_28"),
	GPIO_UNAVAILABLE("PWR_GPIO_29"),
	GPIO_UNAVAILABLE("PWR_GPIO_30"),
	GPIO_UNAVAILABLE("PWR_GPIO_31"),
};

static int sg200xSetup(void) {
	int i = 0;

	if((sg200x->fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
		wiringXLog(LOG_ERR, "wiringX failed to open /dev/mem for raw memory access");
		return -1;
	}
	for(i = 0; i < SG200X_GPIO_GROUP_COUNT; i++) {
		if((sg200x->gpio[i] = (unsigned char *)mmap(0, sg200x->page_size, PROT_READ | PROT_WRITE, MAP_SHARED, sg200x->fd, sg200x->base_addr[i])) == NULL) {
			wiringXLog(LOG_ERR, "wiringX failed to map The %s %s GPIO memory address", sg200x->brand, sg200x->chip);
			return -1;
		}
	}
	if((pinmux_register_virtual_address = (unsigned char *)mmap(0, sg200x->page_size, PROT_READ | PROT_WRITE, MAP_SHARED, sg200x->fd, PINMUX_BASE)) == NULL) {
		wiringXLog(LOG_ERR, "wiringX failed to map The %s %s CRU memory address", sg200x->brand, sg200x->chip);
		return -1;
	}

	return 0;
}

static char *sg200xGetPinName(int pin) {
	return sg200x->layout[pin].name;
}

static void sg200xSetMap(int *map, size_t size) {
	sg200x->map = map;
	sg200x->map_size = size;
}

static void sg200xSetIRQ(int *irq, size_t size) {
	sg200x->irq = irq;
	sg200x->irq_size = size;
}

struct layout_t *sg200xGetLayout(int i, int *mapping) {
	struct layout_t *pin = NULL;
	unsigned int *grf_reg = NULL;
	unsigned int iomux_value = 0;

	if(mapping == NULL) {
		wiringXLog(LOG_ERR, "The %s %s has not yet been mapped", sg200x->brand, sg200x->chip);
		return NULL;
	}
	if(wiringXValidGPIO(i) != 0) {
		wiringXLog(LOG_ERR, "The %i is not the right GPIO number");
		return NULL;
	}
	if(sg200x->fd <= 0 || sg200x->gpio == NULL) {
		wiringXLog(LOG_ERR, "The %s %s has not yet been setup by wiringX", sg200x->brand, sg200x->chip);
		return NULL;
	}

	pin = &sg200x->layout[mapping[i]];
	if(pin->support == FUNCTION_UNKNOWN) {
		wiringXLog(LOG_ERR, "This pin is currently unavailable");
		return NULL;
	}
	if(pin->gpio_group < 0 || pin->gpio_group >= SG200X_GPIO_GROUP_COUNT) {
		wiringXLog(LOG_ERR, "pin->group out of range: %i, expect 0~3", pin->gpio_group);
		return NULL;
	}

	return pin;
}

#define sg200xGetPinLayout(i) (sg200xGetLayout(i, sg200x->map))
#define sg200xGetIrqLayout(i) (sg200xGetLayout(i, sg200x->irq))

static int sg200xDigitalWrite(int i, enum digital_value_t value) {
	struct layout_t *pin = NULL;
	unsigned int *data_reg = 0;
	uint32_t val = 0;

	if((pin = sg200xGetPinLayout(i)) == NULL) {
		return -1;
	}

	if(pin->mode != PINMODE_OUTPUT) {
		wiringXLog(LOG_ERR, "The %s %s GPIO%d is not set to output mode", sg200x->brand, sg200x->chip, i);
		return -1;
	}

	data_reg = (volatile unsigned int *)(sg200x->gpio[pin->gpio_group] + pin->data.offset + GPIO_SWPORTA_DR);
	if(value == HIGH) {
		*data_reg |= (1 << (pin->data.bit));
	} else if(value == LOW) {
		*data_reg &= ~(1 << (pin->data.bit));
	} else {
		wiringXLog(LOG_ERR, "invalid value %i for GPIO %i", value, i);
		return -1;
	}

	return 0;
}

static int sg200xDigitalRead(int i) {
	struct layout_t *pin = NULL;
	unsigned int *data_reg = NULL;
	uint32_t val = 0;

	if((pin = sg200xGetPinLayout(i)) == NULL) {
		return -1;
	}

	if(pin->mode != PINMODE_INPUT) {
		wiringXLog(LOG_ERR, "The %s %s GPIO%d is not set to input mode", sg200x->brand, sg200x->chip, i);
		return -1;
	}

	data_reg = (volatile unsigned int *)(sg200x->gpio[pin->gpio_group] + pin->data.offset + GPIO_EXT_PORTA);
	val = *data_reg;

	return (int)((val & (1 << pin->data.bit)) >> pin->data.bit);
}

static int sg200xPinMode(int i, enum pinmode_t mode) {
	struct layout_t *pin = NULL;
	unsigned int *pinmux_reg = NULL;
	unsigned int *dir_reg = NULL;
	unsigned int mask = 0;

	if((pin = sg200xGetPinLayout(i)) == NULL) {
		return -1;
	}

	pinmux_reg = (volatile unsigned int *) (pinmux_register_virtual_address + pin->pinmux.offset);
	*pinmux_reg = pin->pinmux.value;

	dir_reg = (volatile unsigned int *)(sg200x->gpio[pin->gpio_group] + pin->direction.offset);
	if(mode == PINMODE_INPUT) {
		*dir_reg &= ~(1 << pin->direction.bit);
	} else if(mode == PINMODE_OUTPUT) {
		*dir_reg |= (1 << pin->direction.bit);
	} else {
		wiringXLog(LOG_ERR, "invalid pin mode %i for GPIO %i", mode, i);
		return -1;
	}

	pin->mode = mode;

	return 0;
}

static int sg200xISR(int i, enum isr_mode_t mode) {
	struct layout_t *pin = NULL;
	char path[PATH_MAX];
	memset(path, 0, sizeof(path));

	if((pin = sg200xGetIrqLayout(i)) == NULL) {
		return -1;
	}

	sprintf(path, "/sys/class/gpio/gpio%d", pin->num);
	if((soc_sysfs_check_gpio(sg200x, path)) == -1) {
		sprintf(path, "/sys/class/gpio/export");
		if(soc_sysfs_gpio_export(sg200x, path, pin->num) == -1) {
			return -1;
		}
	}

	sprintf(path, "/sys/devices/platform/%x.gpio/gpiochip%d/gpio/gpio%d/direction", gpio_register_physical_address[pin->gpio_group], pin->gpio_group, pin->num);
	if(soc_sysfs_set_gpio_direction(sg200x, path, "in") == -1) {
		return -1;
	}

	sprintf(path, "/sys/devices/platform/%x.gpio/gpiochip%d/gpio/gpio%d/edge", gpio_register_physical_address[pin->gpio_group], pin->gpio_group, pin->num);
	if(soc_sysfs_set_gpio_interrupt_mode(sg200x, path, mode) == -1) {
		return -1;
	}

	sprintf(path, "/sys/devices/platform/%x.gpio/gpiochip%d/gpio/gpio%d/value", gpio_register_physical_address[pin->gpio_group], pin->gpio_group, pin->num);
	if((pin->fd = soc_sysfs_gpio_reset_value(sg200x, path)) == -1) {
		return -1;
	}

	pin->mode = PINMODE_INTERRUPT;

	return 0;
}

static int sg200xWaitForInterrupt(int i, int ms) {
	struct layout_t *pin = NULL;

	if((pin = sg200xGetIrqLayout(i)) == NULL) {
		return -1;
	}

	if(pin->mode != PINMODE_INTERRUPT) {
		wiringXLog(LOG_ERR, "The %s %s GPIO %d is not set to interrupt mode", sg200x->brand, sg200x->chip, i);
		return -1;
	}

	return soc_wait_for_interrupt(sg200x, pin->fd, ms);
}

static int sg200xGC(void) {
	struct layout_t *pin = NULL;
	char path[PATH_MAX];
	int i = 0;
	memset(path, 0, sizeof(path));

	if(sg200x->map != NULL) {
		for(i = 0; i < sg200x->map_size; i++) {
			pin = &sg200x->layout[sg200x->map[i]];
			if(pin->mode == PINMODE_OUTPUT) {
				pinMode(i, PINMODE_INPUT);
			} else if(pin->mode == PINMODE_INTERRUPT) {
				sprintf(path, "/sys/class/gpio/gpio%d", pin->num);
				if((soc_sysfs_check_gpio(sg200x, path)) == 0) {
					sprintf(path, "/sys/class/gpio/unexport");
					soc_sysfs_gpio_unexport(sg200x, path, pin->num);
				}
			}

			if(pin->fd > 0) {
				close(pin->fd);
				pin->fd = 0;
			}
		}
	}

	if(pinmux_register_virtual_address != NULL) {
		munmap(pinmux_register_virtual_address, sg200x->page_size);
		pinmux_register_virtual_address = NULL;
	}
	for(i = 0; i < SG200X_GPIO_GROUP_COUNT; i++) {
		if(sg200x->gpio[i] != NULL) {
			munmap(sg200x->gpio[i], sg200x->page_size);
			sg200x->gpio[i] = NULL;
		}
	}

	return 0;
}

static int sg200xSelectableFd(int i) {
	struct layout_t *pin = NULL;

	if((pin = sg200xGetIrqLayout(i)) == NULL) {
		return -1;
	}

	return pin->fd;
}

void sg200xInit(void) {
	soc_register(&sg200x, "Sophgo", "SG200X");

	sg200x->layout = layout;

	sg200x->support.isr_modes = ISR_MODE_RISING | ISR_MODE_FALLING | ISR_MODE_BOTH | ISR_MODE_NONE;
	sg200x->page_size = (1024*4);
	memcpy(sg200x->base_addr, gpio_register_physical_address, sizeof(gpio_register_physical_address));

	sg200x->gc = &sg200xGC;
	sg200x->selectableFd = &sg200xSelectableFd;
	sg200x->pinMode = &sg200xPinMode;
	sg200x->setup = &sg200xSetup;
	sg200x->digitalRead = &sg200xDigitalRead;
	sg200x->digitalWrite = &sg200xDigitalWrite;
	sg200x->getPinName = &sg200xGetPinName;
	sg200x->setMap = &sg200xSetMap;
	sg200x->setIRQ = &sg200xSetIRQ;
	sg200x->isr = &sg200xISR;
	sg200x->waitForInterrupt = &sg200xWaitForInterrupt;
}
