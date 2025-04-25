/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    fdcan.h
  * @brief   This file contains all the function prototypes for
  *          the fdcan.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FDCAN_H__
#define __FDCAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
extern unsigned char rx_buffer1[520];
extern char frame_checksum;
extern char buff_segment_id;
extern char frame_no;
extern char can_data_valid ;
extern unsigned char dockID_buffer[24];
extern unsigned char can_read_buffer[512];
extern int write_pointer;
extern int read_pointer;
extern char query_poweron_smry;
extern char get_smry_cnt;
extern char check_can_int ;
extern char totp_status;
void get_devid(unsigned char *bnum);
void store_data(void);


typedef struct {
	unsigned long rptr;
	unsigned long wptr;
	unsigned long summarywptr;
	unsigned long summaryrptr;
	unsigned long alertwptr;
	unsigned long alertrptr;
	unsigned long totp_wptr;
	unsigned long totp_rptr;
	unsigned long dpwptr;
    unsigned long dprptr;
}collar_t;

/* USER CODE END Includes */

extern FDCAN_HandleTypeDef hfdcan1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_FDCAN1_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __FDCAN_H__ */

