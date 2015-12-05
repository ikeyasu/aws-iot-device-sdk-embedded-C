/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
/*
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/**
 * @file timer.c
 * @brief Linux implementation of the timer interface.
 */

#include "espressif/esp_common.h"
#include <stddef.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

#include "timer_platform.h"

char expired(Timer* timer) {
	uint32_t now = sdk_system_get_time();
	if (now < timer->start_time_micro_sec)
		return 1; // overflow
  return timer->end_time_micro_sec - now < 0;
}

void countdown_ms(Timer* timer, unsigned int timeout_milli_sec) {
	uint32_t now = sdk_system_get_time();
	timer->start_time_micro_sec = now;
	timer->end_time_micro_sec = now + (timeout_milli_sec * 1000);
}

void countdown(Timer* timer, unsigned int timeout_sec) {
	uint32_t now = sdk_system_get_time();
	timer->start_time_micro_sec = now;
	timer->end_time_micro_sec = now + (timeout_sec * 1000 * 1000);
}

int left_micro_sec(Timer* timer) {
	uint32_t now = sdk_system_get_time();
	if (timer->end_time_micro_sec < timer->start_time_micro_sec) {
		// overflow
		if (now < timer->start_time_micro_sec) {
			return now + (UINT32_MAX - timer->start_time_micro_sec);
		}
	  return (UINT32_MAX - now) + timer->start_time_micro_sec;
	}
	if (now < timer->start_time_micro_sec) {
		return 0;
	}
	return timer->end_time_micro_sec - now;
}

int left_ms(Timer* timer) {
	return left_micro_sec(timer) / 1000;
}

void InitTimer(Timer* timer) {
	timer->start_time_micro_sec = 0;
	timer->end_time_micro_sec = 0;
}
