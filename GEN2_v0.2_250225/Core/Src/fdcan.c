/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    fdcan.c
  * @brief   This file provides code for the configuration
  *          of the FDCAN instances.
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
/* Includes ------------------------------------------------------------------*/
#include "fdcan.h"

/* USER CODE BEGIN 0 */
#define LAST_FRAME 37
#define CHECKSUM 1
#define DOCK_ID_CHECKSUM 0
//#define KEY_OFF        0x00
//#define KEY_ON		   0x01


//global variables
FDCAN_TxHeaderTypeDef   TxHeader;
FDCAN_RxHeaderTypeDef   RxHeader;
uint8_t               RxData[8];




collar_t collar; // Collar pointers handle
extern UART_HandleTypeDef huart1;

char buff_segment_id = 0;
char frame_no = 0;
char frame_checksum = 0;
char can_data_valid = 0;
char dockID_checksum =0x00;
char alertData_checksum = 0x00;
char alert_data_valid = 0x00;
extern unsigned int key_state ;
char send_alert_data = 0;
char summaryData_checksum = 0x00;
char summary_data_valid =0x00;
char query_poweron_smry =0;
char get_smry_cnt=0;
char tx_data[8];
char check_can_int =0;
int smry_packetcnt;
int checksum_error=0;
int checksum_success =0;
char totp_status =0;
char totp_rec = 0 ;


unsigned char rx_buffer1[520];
unsigned char rx_buffer2[512];  //live data buffer
unsigned char can_read_buffer[512];
unsigned char dockID_buffer[24];   //dock id can buffer
unsigned char mempcb_can_data[10];  // holds the led & key status data
unsigned char device_time[8] = {0x00};
unsigned char device_date[8]= {0x00};
unsigned char alertData_buffer[384];
unsigned char summaryData_buffer[384];
unsigned char summary_count[8];
unsigned char write_data[512];
unsigned char error_buf[8];
unsigned char totp_ack_buf[2];
unsigned char totp_buffer[8];



void get_batteryid(void);
void tiu_heartbeat_string(unsigned char * hb_string,int maxsize);
void tiu_totp_string(unsigned char * totp_string, int maxsize);
void alert_packet(unsigned char * alert_string, int maxsize);
void histor_packet(unsigned char * history_string, int maxsize);
/* USER CODE END 0 */

FDCAN_HandleTypeDef hfdcan1;

/* FDCAN1 init function */
void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = ENABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 8;
  hfdcan1.Init.NominalSyncJumpWidth = 1;
  hfdcan1.Init.NominalTimeSeg1 = 4;
  hfdcan1.Init.NominalTimeSeg2 = 1;
  hfdcan1.Init.DataPrescaler = 24;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 1;
  hfdcan1.Init.DataTimeSeg2 = 1;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */
  TxHeader.IdType = FDCAN_EXTENDED_ID;
    RxHeader.IdType = FDCAN_EXTENDED_ID;
    HAL_FDCAN_ActivateNotification(&hfdcan1,HAL_FDCAN_RxFifo0Callback,0);

  /* USER CODE END FDCAN1_Init 2 */

}

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspInit 0 */

  /* USER CODE END FDCAN1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_HSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN1 clock enable */
    __HAL_RCC_FDCAN_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**FDCAN1 GPIO Configuration
    PB10     ------> FDCAN1_TX
    PB12     ------> FDCAN1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* FDCAN1 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspInit 1 */

  /* USER CODE END FDCAN1_MspInit 1 */
  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspDeInit 0 */

  /* USER CODE END FDCAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_FDCAN_CLK_DISABLE();

    /**FDCAN1 GPIO Configuration
    PB10     ------> FDCAN1_TX
    PB12     ------> FDCAN1_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_12);

    /* FDCAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

  /* USER CODE END FDCAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  /* Prevent unused argument(s) compilation warning */
	unsigned int canid;
	 if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	 {
		 if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK){
			 Error_Handler();
		}
		check_can_int = 0;
		canid = RxHeader.Identifier;
		switch(canid)
		{
			case 0x18227060:  //Live packet ID
				//Copy last 7 bytes of data from received CAN frame to rx_buffer1  & calculate checksum
				frame_no = RxData[0]; //frame number received
				memcpy(&rx_buffer1[((frame_no-1)*7) + buff_segment_id * 259 ],RxData+1,7); //copy to the given segment

				frame_checksum = RxData[0] ^ frame_checksum;  //calculate checksum using the frame no.
				if(RxData[0] == LAST_FRAME){                 //check the below condition only when 37th frame is received.
					if(frame_checksum == CHECKSUM){
						can_data_valid = 1;
						frame_checksum= 0;
						checksum_success++;
						if(!buff_segment_id){
							buff_segment_id = 1;
						}else{
							buff_segment_id = 0;
						}
					}else{
						frame_checksum= 0;
						checksum_error ++;
					}
				}
				break;

			case 0x10217060:          //membrane pcb data & key on/off
				memcpy(mempcb_can_data,RxData,8);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, mempcb_can_data[0] & 0x1);    //LED 3
			    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (mempcb_can_data[0] & 0x4)<<3);  // LED 2							              //LED 5
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (mempcb_can_data[0] & 0x10) << 1 );   //LED 5
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (mempcb_can_data[0] & 0x40) << 1); //LED 4
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, mempcb_can_data[1] & 0x1);  //LED 1
			    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (mempcb_can_data[1] & 0x4) << 3); //LED 6
			    //key_state =  mempcb_can_data[1] & 0x10;//key status
			    if(mempcb_can_data[1] == 0x10)
			    {
			    	key_state = 1;

			    }
			    else if (mempcb_can_data[1] == 0x00)
			    {
			    	key_state = 0;

			    }

				break;

			case 0x10377060:         //get dock id
				frame_no = RxData[0];               //frame number received
				memcpy(&dockID_buffer[(frame_no -1)*7],RxData+1,7); //copy 7 bytes of data from each frame

				dockID_checksum = RxData[0] ^ dockID_checksum; //calculate checksum using frame no 0th byte
				if(frame_no >= 0x03){  //last frame detected
					frame_no = 0;
					if(dockID_checksum == DOCK_ID_CHECKSUM){  //validate checksum
						dockID_checksum = 0 ;
						dockID_buffer[20] = 0x00; //
					}else{
						dockID_checksum = 0 ;
					}
				}
				break;

			case 0x10367060:   //Alert data 37 frames
					frame_no = RxData[0];               //frame number received
					memcpy(&alertData_buffer[(frame_no -1)*7],RxData+1,7); //copy 7 bytes of data from each frame

					alertData_checksum = RxData[0] ^ alertData_checksum;  //calculate checksum using frame no 0th byte
					if(frame_no >= 0x25){   //last frame detected
								frame_no = 0;
								if(alertData_checksum == 1){   //data is valid of checksum match
									alertData_checksum = 0;
									alert_data_valid = 1;
								}else{
									alertData_checksum = 0;
								}
					}
					break;

			case 0x18327060: // summary data 37 frames
							frame_no = RxData[0];               //frame number received
							memcpy(&summaryData_buffer[(frame_no -1)*7],RxData+1,7); //copy 7 bytes of data from each frame

							summaryData_checksum = RxData[0] ^ summaryData_checksum;  //calculate checksum using frame no 0th byte
							if(frame_no >= 0x25){   //last frame detected
									frame_no = 0;
									if(summaryData_checksum == 1){   //data is valid of checksum match
													summaryData_checksum = 0;
													summary_data_valid = 1;
									}else{
													summaryData_checksum = 0;
									}
							}
							break;


			/*case 0x102C7060:  // SCU Response to TIU query -> for summary data on bootup

				if((RxData[5] == 0x06) && (RxData[0] == 0xF2)){
					get_smry_cnt=1;
					memcpy(summary_count,RxData,8);  		//get no.of summary packet available.
					break;
				}
				frame_no = RxData[0];               //frame number received
				memcpy(&summaryData_buffer[(frame_no -1)*7],RxData+1,7); //copy 7 bytes of data from each frame

				summaryData_checksum = RxData[0] ^ summaryData_checksum;
				if(frame_no >= 0x24){
						frame_no = 0;
						if(summaryData_checksum == SUMMARY_DATA_CHECKSUM){
									summaryData_checksum = 0;
									get_smry_cnt = 1;
						}else{
									summaryData_checksum = 0;
						}
				}
				break;*/


			case 0x10397060:    //totp acknowledgent from BMCU on swap
				 totp_ack_buf[0] = RxData[0];
				 totp_rec =1;
				break;

			default:
			break;
		}
	 }
}

//function that checks the received summary data


/***********************************************************************************************************************
* Function Name: store_data
* Description  : This function creates packet using the received can data & stores to flash
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
//Function gets the valid CAN data from isr, creates packet & writes to external flash
void store_data(void){

	int packetlen=0;

	//checks for new live data received & packetizes
	if(can_data_valid){   // all 37 frames have been received copy to main buffer for further processing
		read_tcu_time(device_time,device_date);    //Get current date & time
				  can_data_valid = 0;
				  strncpy(rx_buffer2,"$$H",3);
				  get_devid(rx_buffer2+3);				// device id is 10 chars long
				  strncpy(rx_buffer2+13,device_date,6);	   // HHMMSS
				  strncpy(rx_buffer2+19,device_time,6); 	//YYMMDD
				  if(buff_segment_id){  //segment 0 =>  0- 258
						  //copy from the Upper section of 518 i.e. 0 - 258 & update the segment id to 1
						  memcpy(rx_buffer2+25,rx_buffer1+0,259); //holds received can data
						  memset(rx_buffer1,0x00,259);           //clear segment 0 of rx buffer1 0-258
				   }else{  //segment 1  => 259-517
						  //copy from the lower section of 518 i.e. 259 - 517 & update the segment id to 0
						  memcpy(rx_buffer2+25,rx_buffer1+259,259);
						  memset(rx_buffer1+259,0x00,259);      ////clear segment 1 of rx buffer1 259-517
				  }
				  //Write 512 bytes of data to external flash & read back, followed by write & read pointer handling
				  memset(error_buf,0x00,sizeof(error_buf));
				  sprintf((char *)error_buf,"%.6d",checksum_error);  //RX Checksum error count
			      strncpy(rx_buffer2+284,error_buf,6);
				  strncpy(rx_buffer2+290,",",1);
				  memset(error_buf,0x00,sizeof(error_buf));
				  sprintf((char *)error_buf,"%.6d",checksum_success);   //rx checksum success count
				  strncpy(rx_buffer2+291,error_buf,6);
				  strcpy((char*)rx_buffer2+297,"$*");   //terminator
				  HAL_UART_Transmit(&huart1,rx_buffer2,300,100);

				  //if(flash_state == FLASH_READY_STATE ){
				           //update_flash(rx_buffer2,&collar.wptr,&collar.rptr,HISTORYDATAFLASHSTART,HISTORYDATAFLASHEND,WRITE_SIZE); //actual packet size - 292 bytesz
				  //}
	}

	//creates alert packet if valid data received from can
	if(alert_data_valid){
			alert_data_valid =0;
			memset(write_data,0x00,sizeof(write_data));
			alert_packet(write_data,sizeof(write_data));  //create $$A packet
			HAL_UART_Transmit(&huart1,write_data,279,100); //Push Alert packet to UART
			collar.alertwptr = collar.wptr;
			send_alert_data = 1;
	}
	/*if(send_alert_data){  //pointer handling if alert packet is received
			collar.alertrptr = collar.alertwptr - 15360; //512*30 reduce pointers by 30 packets

			//boundary condition
			// If alert read pointer is  less than startpointer(4096) then do following calculation.
			if (collar.alertrptr <=  HISTORYDATAFLASHSTART ){
					collar.alertrptr = (HISTORYDATAFLASHEND  +  collar.alertrptr);							// ex:5971968 +(- 11264) = 5960704
			}
			collar.alertwptr =  (collar.alertwptr  + 15360);
			// If alert write pointer is  greater than endpointer(5971968) then do following calculation.
			if (collar.alertwptr  >= HISTORYDATAFLASHEND ){
					 collar.alertwptr = HISTORYDATAFLASHSTART + (collar.alertwptr -HISTORYDATAFLASHEND);		//ex: 4096 +(5969920 - 5971968) = 2048
			}
			send_alert_data=0;
	}*/

	//if summary data is received generate packet, store to flash read back & send over UART
	if(summary_data_valid){
			packetlen=0;
			summary_data_valid =0;
			memset(write_data,0x00,sizeof(write_data));
			summary_packet(write_data,sizeof(write_data));  //create $$S packet
			//if(flash_state == FLASH_READY_STATE ){
				//	update_flash(write_data,&collar.summarywptr,&collar.summaryrptr,SUMMARYDATASTART,SUMMARYDATAEND,WRITE_SIZE); //actual packet size - 292 bytesz
			//}
	}

if(check_can_int >=30){  //Heartbeat event
			check_can_int =0;
			tiu_heartbeat_string(write_data,sizeof(write_data)); //create $$B packet
}

	if(totp_rec){  //when TOTP Ack is received from BMCU
		totp_rec = 0;
		memset(write_data,0x00,sizeof(write_data));
		tiu_totp_string(write_data,sizeof(write_data));   //create $$T packet
		//if(flash_state == FLASH_READY_STATE ){
				//	update_flash(write_data,&collar.totp_wptr,&collar.totp_rptr,TOTP_STORE_START,TOTP_STORE_END,256); //actual packet size - 292 bytesz
		//}
	}


}




/***********************************************************************************************************************
* Function Name: query_summary_data
* Description  : This function sends query frame to BMCU for summary data
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/




//Function creates the heartbeat packet
/***********************************************************************************************************************
* Function Name: tiu_heartbeat_string
* Description  : Function creates the heartbeat packet
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void tiu_heartbeat_string(unsigned char * hb_string,int maxsize){

	int packetlen=0;
   // get_batteryid();
	//get_latlon_fr_hb (c_lat,c_lon);
	read_tcu_time(device_time,device_date);    //Get current date & time
	memset(hb_string,0x00,maxsize);
	strncpy(hb_string,"$$B",3);  		// Start of packet
	get_devid(hb_string+3);				// device id is 10 chars long
	strncpy(hb_string+13,device_date,6); 	//YYMMDD
	strncpy(hb_string+19,device_time,6);	// hhmmss
	strncpy(hb_string+25,",",1);
	strncpy(hb_string+26,"61680.6168",strlen("61680.6168"));
	strncpy(hb_string+36,",",1);
	strncpy(hb_string+37,"61680.6168",strlen("61680.6168"));
	strncpy(hb_string+47,",",1);
	strncpy(hb_string+48,"112233445566778899",strlen("112233445566778899"));  //Battery ID
	strncpy(hb_string+67,",",1);
	packetlen=strlen(hb_string);
	memcpy(hb_string+packetlen,"$*",2);
}




/***********************************************************************************************************************
* Function Name: tiu_totp_string
* Description  : Function creates the TOTP packet
* Arguments    : unsigned char * totp_string, int maxsize
* Return Value : None
***********************************************************************************************************************/
void tiu_totp_string(unsigned char * totp_string, int maxsize){
	read_tcu_time(device_time,device_date);    //Get current date & time
	memset(totp_string,0x00,maxsize);
	strncpy(totp_string,"$$T",3);  		    // Start of packet
	get_devid(totp_string+3);				// device id is 10 chars long
	strncpy(totp_string+13,device_date,6); 	//YYMMDD
	strncpy(totp_string+19,device_time,6);	// hhmmss
	if(totp_ack_buf[0]==0xAA){              //TOTP Match
		strncpy(totp_string+25,"1",1);
		totp_ack_buf[0]=0x00;
		//strncpy(totp_string+26,dock_totp,18);  // 18 bytes received DOCK ID &  TOTP (BLE)
		strncpy(totp_string+26,"123456789ABCDE7123",18);  // 18 bytes received DOCK ID &  TOTP (BLE)
	}else if(totp_ack_buf[0]==0xFF){       //TOTP Failure
		strncpy(totp_string+25,"0",1);
		//strncpy(totp_string+26,dock_totp,18);   // 18 bytes received DOCK ID &  TOTP (BLE)
		strncpy(totp_string+26,"123456789ABCDE0103",18);  // 18 bytes received DOCK ID &  TOTP (BLE)
		totp_ack_buf[0]=0x00;
	}else{
		strncpy(totp_string+25,"3",1);
		totp_ack_buf[0]=0x00;
	}
	strncpy(totp_string+44,(char *)(dockID_buffer),14);  //14 Bytes DOCK ID queried via CAN
	strncpy(totp_string+58,"$*",2);
}



/***********************************************************************************************************************
* Function Name: summary_packet
* Description  : Function creates the summary packet
* Arguments    : unsigned char * summary_string, int maxsize
* Return Value : None
***********************************************************************************************************************/
void summary_packet(unsigned char * summary_string, int maxsize){

	int packetlen =0;
	read_tcu_time(device_time,device_date);    //Get current date & time
	strncpy(summary_string,"$$S",3);  		// Start of packet
	get_devid(summary_string+3);				// device id is 10 chars long
	strncpy(summary_string+13,device_date,6); 	//YYMMDD
	strncpy(summary_string+19,device_time,6);	// hhmmss	collar_tmp_date collar_tmp_time
	packetlen=strlen(summary_string);
	memcpy(summary_string+25,summaryData_buffer,259);              //can_test_buf
	memcpy(summary_string+284,"$*",2);
}


/***********************************************************************************************************************
* Function Name: alert_packet
* Description  : Function creates the alert packet
* Arguments    : unsigned char * alert_string, int maxsize
* Return Value : None
***********************************************************************************************************************/
void alert_packet(unsigned char * alert_string, int maxsize){
	int packetlen =0;
	read_tcu_time(device_time,device_date);    //Get current date & time
	strncpy(alert_string,(unsigned char*)"$$A",3);  		// Start of packet -> header
	get_devid(alert_string+3);				// device id is 10 chars long
	strncpy(alert_string+13,device_date,6); 	//YYMMDD
    strncpy(alert_string+19,device_time,6);	   // HHMMSS
	packetlen=strlen(alert_string);
	memcpy(alert_string+packetlen,alertData_buffer,259);  //copy CAN data
	memcpy(alert_string+284,"$*",2);
}

void get_devid(unsigned char *bnum){
	strncpy(bnum,"1023456789",strlen("1023456789"));
}

//function sends the received TOTP to BMCU

/* USER CODE END 1 */
