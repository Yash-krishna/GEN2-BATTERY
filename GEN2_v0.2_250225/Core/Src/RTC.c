/*
 * RTC.c
 *
 *  Created on: Feb 26, 2025
 *      Author: raksh
 */

#include "RTC.h"
#include <string.h>
#include <stdio.h>
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

SoftRTC rtc;
ext_time curtime;
int hours_in_seconds = 0;
int minutes_in_seconds = 0;
int seconds = 0;
//extern static char nTtime;  // holds the current TCU time in hhmmss  format
//extern static char nTdate;  // holds the curren the TCU date in ddmmyy format
extern unsigned int time_of_day;  //holds the curent time of day is seconds - midnight 12 AM is 0
time_source_t curr_time_src = TIMER;  //  this is the source of current time.
char nTtime[8];  // holds the current TCU time in hhmmss  format
char nTdate[8]="120225";  // holds the curren the TCU date in ddmmyy format


	/******************** Global Functions ********************/

/* End user code. Do not edit comment generated here */
/***********************************************************************************************************************
Extern variables and functions
***********************************************************************************************************************/
/* Start user code for Extern. Do not edit comment generated here */

	/************** Extern Variables *********************/
//extern unsigned int T_O_D;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: convert_tod_to_time
* Description  : Converts time of the day into hh:mm:ss
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void convert_tod_to_time(char *nTtime, long unsigned int time_of_day)
{
  int c_hour,c_minutes,c_seconds;

  //calculate hours, minutes and seconds
  c_hour = time_of_day / 3600;
  c_minutes = (time_of_day % 3600) / 60;
  c_seconds = time_of_day % 60;

 sprintf(nTtime,"%02d%02d%02d",c_hour,c_minutes,c_seconds);

}

//Checks for time_of_day in seconds to time and if time_of_Daya is greater than 86400 then updates the next date
/*
inline void check_tod_rollover(void){

	 char input_date[6];

	if(time_of_day >= 86400)
	{
		// nTdate updated accordingly inside the function
		strncpy(input_date,nTdate,6);
		//sending current from nTdate and updating nextdate into nTdate itself
		calculate_nextdate(input_date,nTdate);
		//reset nTtime and tod
		memset(nTtime,'0',sizeof(nTtime)); //resetting time to 00:00:00
		time_of_day=0;
	}
}
*/
//converts the external passed time from HHMMSS to seconds
/*
int set_time_of_day_in_seconds(char *tod){
	int seconds,minutes,hours;

	 hours = (tod[0] - '0') * 10 + (tod[1] - '0'); // Convert HH
	 minutes = (tod[2] - '0') * 10 + (tod[3] - '0'); // Convert MM
	 seconds = (tod[4] - '0') * 10 + (tod[5] - '0'); // Convert SS
	return (hours * 3600) + (minutes * 60) + seconds;
}

//Copies and sets the external time and date to assigned variables
void set_current_datetime(char *ext_time, char *ext_date)
{
	strncpy(nTtime,ext_time,6);
	strncpy(nTdate,ext_date,6);
}
*/

/***********************************************************************************************************************
* Function Name: calculate_nextdate
* Description  : Calculates next date once roll over happens
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned int calculate_nextdate(char *current_date, char *next_date)
{
	unsigned int c_month,c_day,c_year;
	char s_date[8]={'\0'};
	char temp_day[3],temp_month[3],temp_year[3];

	memset(temp_day,0x00,sizeof(temp_day));
	strncpy(temp_day, current_date, 2);

	memset(temp_month,0x00,sizeof(temp_month));
	strncpy(temp_month, current_date+2, 2);

	memset(temp_year,0x00,sizeof(temp_year));
	strncpy(temp_year, current_date+4, 2);

	//date - ddmmyy
	c_day = atoi(temp_day);
	c_month = atoi(temp_month);
	c_year = atoi(temp_year);

        // Adjust for month and year changes using switch statements
        switch (c_month) {
            case 1: // January
            case 3: // March
            case 5: // May
            case 7: // July
            case 8: // August
            case 10: // October
            case 12: // December
                if(c_day==31){
			c_day=1; c_month++;
		}
	        else {
			c_day++;
		}
	        // Adjust for year change
	        if (c_month > 12) {
	            c_month = 1;
	            c_year++;
	        }
                break;

            case 2: // February

                if (is_leap_year(c_year)) {
                    if(c_day==29){
			c_day=1;  c_month++;
		     }
		     else{
		        c_day++;
		     }
                } else {
                    	if(c_day==28){
		          c_day=1;  c_month++;
		         }
		         else{
			    c_day++;
			    }
                }
                break;
            case 4: // April
            case 6: // June
            case 9: // September
            case 11: // November

		        if(c_day==30){
			    c_day=1;  c_month++;
	        	}
	        	else{
		    	c_day++;
	        	}
            }
	sprintf(next_date,"%02d%02d%02d",c_day,c_month,c_year);
    return 0;
    }



/***********************************************************************************************************************
* Function Name: is_leap_year
* Description  : Function to calculate leap year divisible by 4 and year not divisible by 100 or year divisble by 400 then its leap year
* Arguments    : year
* Return Value : 1 - leap year, 0 - not leap year
***********************************************************************************************************************/
int is_leap_year(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}


/***********************************************************************************************************************
* Function Name: get_time_of_day_in_seconds
* Description  : Gets time of the day in seconds
* Arguments    : tod
* Return Value : time of the day in seconds
***********************************************************************************************************************/
/*unsigned long int get_time_of_day_in_seconds(char *tod)
{
	return time_of_day;
}*/

/***********************************************************************************************************************
* Function Name: get_tcu_time
* Description  : Provides current date, time and time of the day to app stack
* Arguments    : c_time, c_date, tod
* Return Value : None
***********************************************************************************************************************/
/*void get_tcu_time(char *c_time,char *c_date,unsigned long *tod)
{
    //update the current date into c_date
    strcpy(c_date,nTdate);

    //update the current time into c_time
    strcpy(c_time,nTtime);

   //update the tod with time of day in seconds
    *tod = get_time_of_day_in_seconds(c_time);

}*/


/***********************************************************************************************************************
* Function Name: convert_strtime_TO_timeofday
* Description  : convert time in hh:mm:ss to time of day in seconds
* Arguments    : stime, tim
* Return Value : Error -1, Success  1
***********************************************************************************************************************/
int convert_strtime_TO_timeofday(char *stime, unsigned long int* tim)
{
	unsigned char tim_hr = 0, tim_min = 0, tim_sec = 0;
	char timrtcstr[3];


		//Hour calculating in seconds
		timrtcstr[0] = stime[0];
		timrtcstr[1] = stime[1];
		timrtcstr[2] = 0x00;
		tim_hr = atoi(timrtcstr);
		if (tim_hr > 23){
			return -1;
		}


		// Min calculating in seconds
		timrtcstr[0] = stime[2];
		timrtcstr[1] = stime[3];
		timrtcstr[2] = 0x00;
		tim_min = atoi(timrtcstr);
		if (tim_min > 59){
			return -1; }


		// calculating in seconds
		timrtcstr[0] = stime[4];
		timrtcstr[1] = stime[5];
		timrtcstr[2] = 0x00;
		tim_sec = atoi(timrtcstr);
		if (tim_sec > 59){
			return -1;
		}


		*tim = (long int)(tim_hr)*(3600)+ (long int)(tim_min)*(60)+ (long int)(tim_sec);
		return 1;


}

/***********************************************************************************************************************
* Function Name: set_tcu_time
* Description  : Set time, date, time of day and time source
* Arguments    : c_time, c_date, tod, src
* Return Value : return 1 if successful
***********************************************************************************************************************/
int set_tcu_time(char *c_time, char *c_date,time_source_t src )
{
   int ret = 0;
   char output_time[6],input_date[6];
   memset(output_time,0x00,sizeof(output_time));
   memset(input_date,0x00,sizeof(input_date));
   curr_time_src = time_of_day;
   //condition to check if the current source has higher or equal priority to the new source
//   if(curr_time_src > src)
//   {
//   	return -1;
//   }

   switch(src) {
/*	case TIMER :  // this is increment of time from timer
	//if (tod < time of day) then date change is triggered
	// change the time of day
	// convert and save as nT time, and nT date
	// if hours have rolled over, then get_next_date
	// update ntdate and Nttime
	time_of_day++;
	if(time_of_day >= 86400)
	{
	    // nTdate updated accordingly inside the function
	    strncpy(input_date,nTdate,6);
	    //sending current from nTdate and updating nextdate into nTdate itself
	    calculate_nextdate(input_date,nTdate);
	    //reset nTtime and tod
	    memset(nTtime,'0',sizeof(nTtime)); //resetting time to 00:00:00
	    time_of_day=0;
	}
	else
	{
	   //convert the time of day into hhmmss and update it in nTtime
	   convert_tod_to_time(nTtime, time_of_day);
	}
	break;*/
	case SCUTIME :
	// validate c_time and c_date
	// copy c_time and c_date in to nTdate and nTtime
	// calculate time of day and update it
	// convert_strtime_TO_timeofday
	//set time_soure = SCUTIME

		// validate c_time and c_date  ddmmyy and hhmmss
		ret = validate_date_time(c_time,c_date);
		if(ret == 1){
	   		//Copy date from c_date into nTdate
	   		strncpy(nTdate,c_date,6);
	  		 //copy time from c_time into nTtime
   	  		 strncpy(nTtime,c_time,6);
			 // calculate time of day and update it in time_of_day variable
			convert_strtime_TO_timeofday(c_time, &time_of_day);
			//set time_soure
			curr_time_src=src;

			}
		else{

		}
   		break;
	case GPSTIME :
	// copy c_time and c_date in to nTdate and nTtime
	// before that validate c_time and c_date
	// calculate time of day and update it
	// convert_strtime_TO_timeofday
	//set time_source to GPSTIME
		// validate c_time and c_date input format from GPS is ddmmyy and hhmmss
		ret = validate_date_time(c_time,c_date);

		if(ret == 1){
	   		//Copy date from c_date into nTdate
	   		strncpy(nTdate,c_date,6);

	  		 //copy time from c_time into nTtime
   	  		 strncpy(nTtime,c_time,6);
			// calculate time of day and update it in time_of_day variable
			convert_strtime_TO_timeofday(c_time, &time_of_day);

			//set time_soure
			curr_time_src=src;
		}
		else{
		}
	break;
	case SERVERTIME:
	// copy c_time and c_date in to nTdate and nTtime
	// before that validate c_time and c_date
	// calculate time of day and update it
	// convert_strtime_TO_timeofday
	//set time_source to SERVERTIME
		// validate c_time and c_date dd/mm/yy and hh:mm:ss
		ret = validate_date_time(c_time,c_date);

		if(ret == 1){
	   		//Copy date from c_date into nTdate
	   		strncpy(nTdate,c_date,6);
	  		 //copy time from c_time into nTtime
   	  		 strncpy(nTtime,c_time,6);
			 // calculate time of day and update it in time_of_day variable
			convert_strtime_TO_timeofday(c_time, &time_of_day);
			//set time_soure
			curr_time_src=src;
		}
		else{}
	break;
	default :
	break;
   }

   return ret;

}


/***********************************************************************************************************************
* Function Name: validate_date_time
* Description  : validate given date and time is valid or not
* Arguments    : time, date
* Return Value : 1 - valid, 0 - invalid
***********************************************************************************************************************/
int validate_date_time(char *current_time, char *current_date){


	unsigned int c_month,c_day,c_year,c_hr,c_min,c_sec;
	char temp_day[3],temp_month[3],temp_year[3];
	char temp_hr[3],temp_min[3],temp_sec[3];

	memset(temp_day,0x00,sizeof(temp_day));
	strncpy(temp_day, current_date, 2);

	memset(temp_month,0x00,sizeof(temp_month));
	strncpy(temp_month, current_date+2, 2);

	memset(temp_year,0x00,sizeof(temp_year));
	strncpy(temp_year, current_date+4, 2);


	memset(temp_hr,0x00,sizeof(temp_hr));
	strncpy(temp_hr, current_time, 2);

	memset(temp_min,0x00,sizeof(temp_min));
	strncpy(temp_min, current_time+2, 2);

	memset(temp_sec,0x00,sizeof(temp_sec));
	strncpy(temp_sec, current_time+4, 2);

	//date - ddmmyy
	c_day = atoi(temp_day);
	c_month = atoi(temp_month);
	c_year = atoi(temp_year);

	c_hr = atoi(temp_hr);
	c_min = atoi(temp_min);
	c_sec = atoi(temp_sec);


	if((c_year > 99)|| (c_month > 12) || (c_month < 1)){
		// month or year is invalid. Do not update date and time
		return 0;
	}

	if((c_hr > 23) || (c_min > 59) || (c_sec > 59)){
		// invalid time
		return 0;
	}

	   // Adjust for month and year changes using switch statements
        switch (c_month) {
            case 1: // January
            case 3: // March
            case 5: // May
            case 7: // July
            case 8: // August
            case 10: // October
            case 12: // December

                if(c_day>31){
			//invalid date
			return 0;
		}
	        else {

		}
                break;

            case 2: // February

                if (is_leap_year(c_year)) {
                    if(c_day>29){
                       //invalid date
			return 0;
		     }
		    else{
			//valid day
		    }
                } else {
                    	if(c_day>28){
                    	   //tested
			    return 0;
		         }
		         else{
			    //valid day
			 }
                }
                break;
            case 4: // April
            case 6: // June
            case 9: // September
            case 11: // November

		        if(c_day>30){
		            //invalid date
			    return 0;
	        	}
	        	else{
		    	//valid
	        	}
            }
    return 1;
 }

void read_tcu_time(char *curr_time,char *curr_date){

	   //convert the time of day into hhmmss and update it in nTtime
	   convert_tod_to_time(nTtime, time_of_day);
	   strcpy(curr_time,nTtime);
	   strcpy(curr_date,nTdate);

}


/*
void update_time_of_day(){

	hours_in_seconds = curtime.curr_hour * 3600;
	minutes_in_seconds = curtime.curr_min * 60;
	seconds 			= curtime.curr_sec ;

		T_O_D = hours_in_seconds + minutes_in_seconds + seconds;
}
*/

