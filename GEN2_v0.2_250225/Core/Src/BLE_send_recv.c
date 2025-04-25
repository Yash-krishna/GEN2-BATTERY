/*
 * BLE_send_recv.c
 *
 *  Created on: Feb 13, 2025
 *      Author: adithya.m
 */


//includes
#include "BLE_send_recv.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "fdcan.h"
#include "stm32h5xx_hal_fdcan.h"

//------------function declarations------------------
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */

/* End user code. Do not edit comment generated here */
/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

	/************** Global Variables *********************/

unsigned int scan_off_timer = 0;
unsigned int key_state = KEY_ON;
int prev_state = SCAN_OFF;
extern int scan_state = SCAN_OFF;
int bmcu_scan_rsp = 0;
int totp_received = 0;
int update_broadcast = 0;
unsigned short totp_sent_time =0;
char data_buffer[64];
char dockid_buffer[35]="$,DOCKID,AD0107K185700E5698,#";
char droadcast_buffer[64]="$,BROADON,1234567890,#";
extern int scan_timer = 0;
int ti_data_ready = 0;
int ti_buff = 0;
int totp_buf = 0;
int ack_buff = 0;

/******************** Global Functions ********************/

/* End user code. Do not edit comment generated here */
/***********************************************************************************************************************
Extern variables and functions
***********************************************************************************************************************/
/* Start user code for Extern. Do not edit comment generated here */

	/************** Extern Variables *********************/
//extern unsigned int T_O_D;
//unsigned int totp_status;

//uint8_t               TxData[8];
FDCAN_TxHeaderTypeDef   Txheader;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: scan_state_manager
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void scan_state_manager(void)
{

	  if(key_state == KEY_ON){ 			// checks the key is ON
			  scan_state = SCAN_OFF;
			  scan_timer =0;

		  }
		  else {
			 // the key is OFF
			  if(scan_timer < 360){
				  scan_state = SCAN_ON;

			  }else{
				  scan_state = SCAN_OFF;
				  scan_timer = 360;

			  	  }
			  }
}

/***********************************************************************************************************************
* Function Name: scan_on_routine
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void scan_on_routine(void)
{

	if(prev_state == SCAN_OFF){ //checks for the prev state of the scan
		totp_received = 0;
		bmcu_scan_rsp = 0;
		initiate_scan_on(); // sends the cmd to TI to start the scan
		prev_state = SCAN_ON; // set the prev state to scan on

	}else{
		if(scan_timer < 360){ // it will check the scan timer it has to be less than 360
				if(totp_received == 1){ //check only if the TOTP is received
					send_totp_to_bmcu();	//collect and send TOTP to BMCU
					totp_sent_time = scan_timer + 5; //time out for receiving BMCU response
					totp_received = 2;  // set this flag to 2 to check the -VE ack from BMCU
					}
				if(totp_received == 2){
						if((bmcu_scan_rsp == -1) ||(scan_timer > totp_sent_time)) {
								//recieved NACK from BMCU
								totp_received = 0;
								bmcu_scan_rsp = 0;
								initiate_scan_on(); //If the -ve ack has received from bmcu the reinit the scan

							}
						if(bmcu_scan_rsp == 1){
							//recieved ACK from BMCU
							scan_state = SCAN_OFF;
							scan_timer = 360;  // set the timer to 360 so not to check the condition again
							initiate_scan_off();//sends scan stop cmd to TI
							}
						}
		}else{
			//scan timed out
			scan_state = SCAN_OFF;
			scan_timer = 360;
			initiate_scan_off();
		}
	}
}

/***********************************************************************************************************************
* Function Name: scan_off_routine
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void scan_off_routine(void)
{
	if(prev_state == SCAN_ON){
		initiate_scan_off(); //sends scan off cmd to TI
		prev_state = SCAN_OFF;
		initiate_broadcast(); //scan is off and key is on then broadast the receiving parameters
	}
	else if(update_broadcast == 1){
		initiate_broadcast();
	}

}

/***********************************************************************************************************************
* Function Name: initiate_broadcast
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void initiate_broadcast(void)
{
		memset(data_buffer,'\0',sizeof(data_buffer));
		memcpy(data_buffer,droadcast_buffer,strlen(droadcast_buffer));
		HAL_UART_Transmit(&huart3,(uint8_t*)data_buffer,strlen(data_buffer),HAL_MAX_DELAY);HAL_UART_Transmit(&huart3,(uint8_t*)data_buffer,strlen(data_buffer),HAL_MAX_DELAY);

}

/***********************************************************************************************************************
* Function Name: initiate_scan_on
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void initiate_scan_on(void){ //sends dock id and scan on to TI

		memset(data_buffer,'\0',sizeof(data_buffer));
		memcpy(data_buffer,dockid_buffer,strlen(dockid_buffer));
		HAL_UART_Transmit(&huart3,(uint8_t*)data_buffer,strlen(data_buffer),HAL_MAX_DELAY);HAL_UART_Transmit(&huart3,(uint8_t*)data_buffer,strlen(data_buffer),HAL_MAX_DELAY);

		//HAL_Delay(1000);
		memset(data_buffer,'\0',sizeof(data_buffer));
		memcpy(data_buffer,"$,SCANON,#",strlen("$,SCANON,#"));
		HAL_UART_Transmit(&huart3,(uint8_t*)data_buffer,strlen(data_buffer),HAL_MAX_DELAY);HAL_UART_Transmit(&huart3,(uint8_t*)data_buffer,strlen(data_buffer),HAL_MAX_DELAY);

		bmcu_scan_rsp = 0;
	}

/***********************************************************************************************************************
* Function Name: initiate_scan_off
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void initiate_scan_off(void){

	memset(data_buffer,'\0',sizeof(data_buffer));
	memcpy(data_buffer,"$,SCANOFF,#",strlen("$,SCANOFF,#"));
	HAL_UART_Transmit(&huart3,(uint8_t*)data_buffer,strlen(data_buffer),HAL_MAX_DELAY);HAL_UART_Transmit(&huart3,(uint8_t*)data_buffer,strlen(data_buffer),HAL_MAX_DELAY);
}

/***********************************************************************************************************************
* Function Name: process_ti_receive
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void process_ti_receive(void){
	  if(ti_data_ready == 1){

		  if(strncmp(ti_buff,"$,TOTP",6)==0){ //$TOTP,1234,# //Check if received data is TOTP
			  memset(totp_buf,'\0',sizeof(totp_buf));
			 // get_field(3,ti_buff,totp_buf);
			  memcpy(totp_buf,ti_buff+6,4); //ti_buff+6 will collect only TOTP from the packet
			  totp_received = 1;
		  }
		  else if (strncmp(ti_buff,"$,SCN",5) == 0){ //checks scan on/off status
			  	  	 memset(ack_buff,'\0',sizeof(ack_buff));
			  	  	 get_field(3,ti_buff,ack_buff);
		  	  	  }
		  else if (strncmp(ti_buff,"$,ADV",5) == 0){ // checks broadcast on/off status
			  	  	 memset(ack_buff,'\0',sizeof(ack_buff));
			  	  	 get_field(3,ti_buff,ack_buff);
		  	  	  }
		  else if (strncmp(ti_buff,"$,HRBT",6) == 0){ //response for TI alive or not
			  	  	 memset(ack_buff,'\0',sizeof(ack_buff));
		 			 get_field(3,ti_buff,ack_buff);
		  	  	  }
		  ti_data_ready=0;
	  }
  }

/***********************************************************************************************************************
* Function Name: send_totp_to_bmcu
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void send_totp_to_bmcu(void)
{

	  Txheader.Identifier = 0x10386070;  //CAN ID for TOTP
	  Txheader.DataLength = 8;			//length of the data
	  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &Txheader,totp_buf);  //send data TO bmcu over CAN

 }
/**************************************************************************************************/





