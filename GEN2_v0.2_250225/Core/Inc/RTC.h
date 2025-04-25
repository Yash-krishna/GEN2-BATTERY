/*
 * RTC.h
 *
 *  Created on: Feb 26, 2025
 *      Author: raksh
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_



/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

typedef struct {
    unsigned char seconds;
    unsigned char minutes;
    unsigned char hours;
    unsigned char day;
    unsigned char month;
    unsigned short year;
} SoftRTC;

typedef struct{
	unsigned char curr_hour;
	unsigned char curr_min;
	unsigned char curr_sec;
} ext_time;

//Timer source
typedef enum
{
    SCUTIME=0x01,SERVERTIME,GPSTIME,TIMER
} time_source_t;
/***********************************************************************************************************************
FUNCTION DEFINITIONS
***********************************************************************************************************************/

	/*************** Void Functions *********************/
void Update_rtc(void);
void RTC_Init(void);
unsigned long get_time_of_day_in_seconds(char *tod); // function to provide time of day to app stack
void get_tcu_time(char *c_time,char *c_date,unsigned long *tod); // function to provide date and time to app stack
int set_tcu_time(char *c_time,char *c_date, time_source_t src); // function to set date and time by app stack
int convert_strtime_TO_timeofday(char * stime, unsigned long * tim);  // conversion utility to convert time in hh:mm:ss to time of day in seconds
unsigned int calculate_nextdate(char *current_date, char *next_date); //Calculates next day date once roll over happens
int validate_date_time(char *current_time, char *current_date);//validate date and time and provide return value accordingly
int is_leap_year(int year); //checks the leap year
void convert_tod_to_time(char *nTtime, long unsigned int time_of_day); //convert time of day into hhmmss
void get_rtc_time(char *c_time, char *c_date);
int set_time_of_day_in_seconds(char *tod);
void set_current_datetime(char *ext_time, char *ext_date);
inline void check_tod_rollover(void);
void read_tcu_time(char *curr_time,char *curr_date);


#endif /* INC_RTC_H_ */
