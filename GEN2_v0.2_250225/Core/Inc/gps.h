/*
 * gps.h
 *
 *  Created on: Feb 26, 2025
 *      Author: raksh
 */

#ifndef INC_GPS_H_
#define INC_GPS_H_

void update_GPS_Strings(char *Actual_data ,int *len);
void init_gps(void);
char Update_gps_data(unsigned char * Actual_data);

typedef struct {
    char Message_ID[15];
    char UTC_Time[15] ;
    char Status[15];
    char Latitude[15];
    char Latitude_direction[15];
    char Longitude[15];
    char Longitude_direction[15];
    char Speed_Over_Ground[15];
    char Course_Over_Ground[15];
    char Date[15];
    char Magnetic_variation[15];
    char Magnetic_variation_direction_Checksum[15];
}GPRMC_typ;

static GPRMC_typ GPRMC_Data;

typedef struct {
    char Message_ID[15];
    char UTC_Time[15] ;
    char Latitude[15];
    char Latitude_direction[15];
    char Longitude[15];
    char Longitude_direction[15];
    char Position_fix_indicator[15];
    char Satellites_used[15];
    char HDOP[15];
    char MSL_Altitude[15];
    char MSL_Altitude_Units[15];
    char Geoid_separation[15];
    char Geoid_separation_Units[15];
    char Age_of_Diff_corr[15];
    char Diff_Ref_Station_ID_checksum[15];
}GPGGA_typ;

static GPGGA_typ GPGGA_Data;


#endif /* INC_GPS_H_ */

