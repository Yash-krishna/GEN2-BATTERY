
/*
 * gps.c
 *
 *  Created on: Feb 14, 2025
 *      Author: adithya.m
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gps.h"
#include "usart.h"

char cmd_buf[32]; //used for sending command to the modem
char rmc_Info[20];
char gga_Info[20];
char Data_Received[1024];
char N,k,Status;

extern char Data_Update_flag;
extern char Gps_Data_ready;
extern unsigned int uart2_wr_ptr;
extern UART_HandleTypeDef huart1;

void init_gps(void){
    //print_debug("Init GPS commands\r\n",strlen("Init GPS commands\r\n"));
	//fireAT(600,"AT+MGPSC=1\r\n",cmd_buf);      // Turn on GPS

}
void update_GPS_Strings(char *Actual_data ,int *len) {

	int collect =0;
	char rx_data;
	int ind=0;


	for(int i=0;i<= len;i++){
		if(Actual_data[i] == '$')ind=0;

		Data_Received[ind] = Actual_data[i];
		ind++;

			if(Data_Received[ind-1] == '\n'){

				  if(strncmp(Data_Received,"$GNRMC",6)==0){

					 memset(rmc_Info,'\0',sizeof(rmc_Info));
					 get_field(1,Data_Received, rmc_Info);
					 strcpy(&GPRMC_Data.Message_ID,rmc_Info);
					 HAL_UART_Transmit(&huart1, &GPRMC_Data.Message_ID, 15, 100);

					 memset(rmc_Info,'\0',sizeof(rmc_Info));
					 get_field(2,Data_Received, rmc_Info);
					 strcpy(&GPRMC_Data.UTC_Time,rmc_Info);
					 HAL_UART_Transmit(&huart1, &GPRMC_Data.UTC_Time, 15, 100);

					 memset(rmc_Info,'\0',sizeof(rmc_Info));
					 get_field(3,Data_Received, rmc_Info);
					 strcpy(&GPRMC_Data.Status,rmc_Info);
					 HAL_UART_Transmit(&huart1, &GPRMC_Data.Status, 15, 100);

					 memset(rmc_Info,'\0',sizeof(rmc_Info));
					 get_field(4,Data_Received, rmc_Info);
					 strcpy(&GPRMC_Data.Latitude,rmc_Info);

					 memset(rmc_Info,'\0',sizeof(rmc_Info));
					 get_field(5,Data_Received, rmc_Info);
					 strcpy(&GPRMC_Data.Latitude_direction,rmc_Info);

					 memset(rmc_Info,'\0',sizeof(rmc_Info));
					 get_field(6,Data_Received, rmc_Info);
					 strcpy(&GPRMC_Data.Longitude,rmc_Info);

					 memset(rmc_Info,'\0',sizeof(rmc_Info));
					 get_field(7,Data_Received, rmc_Info);
					 strcpy(&GPRMC_Data.Longitude_direction,rmc_Info);

					 memset(rmc_Info,'\0',sizeof(rmc_Info));
					 get_field(8,Data_Received, rmc_Info);
					 strcpy(&GPRMC_Data.Speed_Over_Ground,rmc_Info);

					 memset(rmc_Info,'\0',sizeof(rmc_Info));
					 get_field(9,Data_Received, rmc_Info);
					 strcpy(&GPRMC_Data.Course_Over_Ground,rmc_Info);

					 memset(rmc_Info,'\0',sizeof(rmc_Info));
					 get_field(10,Data_Received, rmc_Info);
					 strcpy(&GPRMC_Data.Date,rmc_Info);

					 memset(rmc_Info,'\0',sizeof(rmc_Info));
					 get_field(11,Data_Received, rmc_Info);
					 strcpy(&GPRMC_Data.Magnetic_variation,rmc_Info);

					 memset(rmc_Info,'\0',sizeof(rmc_Info));
					 get_field(12,Data_Received, rmc_Info);
					 strcpy(&GPRMC_Data.Magnetic_variation_direction_Checksum,rmc_Info);
					  //data received
					 // collect = 1;
				  }
				  if(strncmp(Data_Received,"$GNGGA",6)==0){

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(1,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.Message_ID,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(2,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.UTC_Time,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(3,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.Latitude,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(4,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.Latitude_direction,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(5,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.Longitude,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(6,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.Longitude_direction,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(7,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.Position_fix_indicator,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(8,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.Satellites_used,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(9,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.HDOP,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(10,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.MSL_Altitude,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(11,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.MSL_Altitude_Units,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(12,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.Geoid_separation,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(10,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.Geoid_separation_Units,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(11,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.Age_of_Diff_corr,gga_Info);

					 memset(gga_Info,'\0',sizeof(gga_Info));
					 get_field(12,Data_Received, gga_Info);
					 strcpy(&GPGGA_Data.Diff_Ref_Station_ID_checksum,gga_Info);

					// Data_Update_instruct(Data_Received);
					  }
			}
			uart2_wr_ptr =0;
	}
}

char Update_gps_data(unsigned char * Actual_data)
{
	unsigned int Address_index;
	unsigned char len;
	char Data_Received[100];
	char N,k,Status;
	char fielddata[20];
	char Member_Info[20];
	char update_struct_flag;
	char GPRMC_ID[] = "$GNRMC";
	char GPGGA_ID[] = "$GNGGA";


//	if((Gps_Data_ready == 1)) //&& (Data_Update_flag == 1)
//	{
		Data_Update_flag = 0;//up
		Gps_Data_ready = 0;//flag indication for

        //copy received data to local varible.
		strcpy(Data_Received,Actual_data);

		len = strlen(Actual_data);
		len += 1;//to return sucessfully after fetching data of last member.
		 for(int i = 1;i<= 15;i++)
		{
			N = i;
			//It will Extract particular data from modem data
			Status = get_field(N,Data_Received, Member_Info);

			//Address_index of member.
			Address_index = 15*(N - 1);

			if(N == 1)
			{
			    //checking which signal received.
				if(strcmp(GPRMC_ID,Member_Info) == 0)
				{
					update_struct_flag = 1;
				}
				else if(strcmp(GPGGA_ID,Member_Info) == 0)
				{
					update_struct_flag = 2;
				}
				else
				{
				    break;
				}
			}

			//protection for next member.
			if(strlen(Member_Info)<15)
			{
				//Update the data based upon the Received Signal.
				if(update_struct_flag == 1)
				{
				//update the value in GPRMC strcture.
					strcpy(&GPRMC_Data.Message_ID[Address_index],Member_Info);
				}
				else if(update_struct_flag == 2)
				{
				    //update the value in GPGGA strcture.
					strcpy(&GPGGA_Data.Message_ID[Address_index],Member_Info);
				}
			}
				if(Status < 0)
				{
					Data_Update_flag = 1;
					update_struct_flag = 0;
					break;
				}
		}
//	}
	return 0;
}


/*void process_gps_data(void)
	{
					rx_data = (uint16_t) READ_REG(huart->Instance->RDR);  //read from Data Register

					if(rx_data == '$'){
							data_receive_flag = 1;
							data_received_count = 0;
						}

						 if(data_receive_flag == 1){
							uart2_rx_buffer[uart2_wr_ptr] = rx_data;
							uart2_wr_ptr++;
							data_received_count++;
						 }

						 if(rx_data == '\n')
						 {
							  data_receive_flag=0;
							  if (Gps_Data_ready == 0)
							  {
								  memset(gps_buff,'\0',sizeof(gps_buff));
								  memcpy(gps_buff, uart2_rx_buffer, data_received_count);
								  Gps_Data_ready = 1;
							  }
							  data_received_count = 0;
							  uart2_wr_ptr = 0;

						 }
						if (strncmp(gps_buff, "$GNRMC", 6) == 0) {
							strncpy(gnrmc_buffer, gps_buff, GNRMC_BUFFER_SIZE); //GNRMC_BUFFER_SIZE this is hard codded
							gnrmc_buffer[GNRMC_BUFFER_SIZE - 1] = '\0'; //  null termination
						}else if(strncmp(gps_buff, "$GNGGA", 6) == 0) {
									strncpy(gngga_buffer, gps_buff	, GNGGA_BUFFER_SIZE);
									gngga_buffer[GNGGA_BUFFER_SIZE - 1] = '\0'; //  null termination
								}

}

*/
