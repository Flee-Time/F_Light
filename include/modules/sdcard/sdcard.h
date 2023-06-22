#ifndef __SDCARD_H
#define __SDCARD_H

#include "modules/fatfs/fatfs.h"
#include "stm32f4xx_hal.h"

FRESULT mount_sd();
FRESULT unmount_sd();
uint32_t getSize_sd();
uint32_t getFreeSpace_sd();

#endif