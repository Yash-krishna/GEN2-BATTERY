/*
 * utilities.h
 *
 *  Created on: Feb 6, 2025
 *      Author: adithya.m
 */

#ifndef INC_UTILITIES_H_
#define INC_UTILITIES_H_


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

/***********************************************************************************************************************
FUNCTION DEFINITIONS
***********************************************************************************************************************/

	/*************** Void Functions *********************/

void conv2hex(char inchar,char *out1,char * out2);
void clear_blebackoff(void);
void set_blebackoff(int x);
void decr_blebackoff(void);
void convhextodec(char *hex,char *out);
void convhextobyte(char inchar1,char inchar2, unsigned char *out);
void convert_bcd(short int in, unsigned char *out_lsb, unsigned char *out_msb);

	/*************** Integer Functions *****************/

int convert_vel_to_integer(char *vel);
int findpat(char *buf, char *pat, int buflen);
int get_blebackoff(void);
int convert_hdop(char *vel,unsigned long *hdop);
int convert_knots_to_km_meter(char *vel,unsigned long *km,unsigned int *meter);

	/*************** Character Functions ***************/

unsigned int convert_string_to_HEX(long int data_len, unsigned char *data,unsigned char *string);
unsigned int convert_lat( char *inbuf,unsigned long *geo_lat);
unsigned int convert_lon(char *inbuf,unsigned long *geo_lon);

char get_field(char N,char * record,char * fielddata);
char get_data_at_index(int index,char * input_buffer,char * outputbuf);
//char get_field_hex(char N,char * record,char * fielddata);
//void get_field_hex(int index,char *input_buffer,  char *outputbuf);
//void get_field_hex(int index, unsigned char *input_buffer, unsigned char *outputbuf);
char get_config_from_can(unsigned char * record, unsigned char * fielddata);

unsigned char BCDToDecimal(unsigned char bcdByte);
int asciiToHex(char asciiChar);

#endif /* INC_UTILITIES_H_ */
