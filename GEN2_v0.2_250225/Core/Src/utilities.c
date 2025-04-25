/*
 * utilities.c
 *
 *  Created on: Feb 6, 2025
 *      Author: adithya.m
 */


//includes
#include "utilities.h"
#include <string.h>



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

int blebackoff_discnt=0;

	/******************** Global Functions ********************/

int str_n_cmp(unsigned char *s1, unsigned char *s2, int n);

/* End user code. Do not edit comment generated here */
/***********************************************************************************************************************
Extern variables and functions
***********************************************************************************************************************/
/* Start user code for Extern. Do not edit comment generated here */

	/************** Extern Variables *********************/

/* End user code. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: get_field
* Description  : This function returns the Nth field in a comma separated record and returns the field in string fielddata,
		 Field numbers start from 1 .. N, ensure record is a null terminated string, TBD - there seems to be a bug if N =1  -- need to check
* Arguments    : char*, char*, char*
* Return Value : char
***********************************************************************************************************************/
char get_field(char N,char * record,char * fielddata)
{
	unsigned int index,i; //len
	char in_c;
	// return the string between the N-1th and Nth comma
	//print_debug(record,(int)strlen(record));
	//give_delay_in_ms(200);
	index = 0;
	//len = (int)(int)strlen(record);
	for (i=0; i<N-1; i++)
		while ((in_c = record[index++]) != ',')
			if (in_c == '\0') return ((char)-1) ; // skip N-1 commas
	 // the data from here to till the next ',' has to be returned
	 i=0;
	 while ( (in_c = record[index++]) != ',') {
		 if((in_c=='\r')||in_c=='\n')
			break;
		 fielddata[i++] = in_c;
		 if (in_c == '\0') return ((char)-1); // we reached end of string before hitting the next comma
	 }
	 fielddata[i] = '\0';
	 return(0);
}
/***********************************************************************************************************************
* Function Name: get_field
* Description  : This function returns the Nth field in a comma separated record and returns the field in string fielddata,
		 Field numbers start from 1 .. N, ensure record is a null terminated string, TBD - there seems to be a bug if N =1  -- need to check
* Arguments    : char*, char*, char*
* Return Value : char
***********************************************************************************************************************/
char get_data_at_index(int index,char * input_buffer,char * outputbuf)
{
    int count = 0;
    int i = 0;
    int start = 0;
    int end = 0;

    // Find the start and end positions of the desired field
    while (input_buffer[i] != '#') {
	    while(input_buffer[i] == '\0'){
		    i++;
	    }
        if (input_buffer[i] == ',' && input_buffer[i + 1] != '\r') {
            count++;
            if (count == index) {
                start = i + 1;
            } else if (count == index + 1) {
                end = i;
                break;
            }
        }
        i++;
    }

    // Copy the data from input_buffer to outputbuf
    memcpy(outputbuf, &input_buffer[start], end - start);

    // Null-terminate the output buffer
    outputbuf[end - start] = '\0';
}

/*
void get_field_hex(int index, unsigned char *input_buffer, unsigned char *outputbuf) {
    int count = 0; // Track the current index in the buffer
    int i,j = 0; // Index for iterating through the buffer

    while (input_buffer[i] != '\0') {
        if (input_buffer[i] == ',') {
            count++; // Increment count when a comma is encountered
            if (count == index) {
                outputbuf[i] = '\0'; // Null-terminate the result string
		//outputbuf[j] = '\0'; // Null-terminate the result string
                break; // Stop when the desired index is reached
            }
        } else if (count == index - 1) {
            outputbuf[i - (index == 1 ? 0 : 1)] = input_buffer[i]; // Copy data to outputbuf
	    //outputbuf[j++] = input_buffer[i]; // Copy data to outputbuf
        }
        i++;
    }
}
*//*
void get_field_hex(int index,char *input_buffer,  char *outputbuf) {
    int count = 0; // Track the current index in the buffer
    int i = 0; // Index for iterating through the buffer
    int start_index = 0; // Index to track the start of the current field

    while (input_buffer[i] != '\0') {
        if (input_buffer[i] == ',') {
            count++; // Increment count when a comma is encountered
            if (count == index) {
                int field_length = i - start_index; // Calculate the length of the field
                strncpy(outputbuf, &input_buffer[start_index], field_length); // Copy the field to outputbuf
                outputbuf[field_length] = '\0'; // Null-terminate the field
                break; // Stop when the desired index is reached
            }
            start_index = i + 1; // Update the start index for the next field
        }
        i++;
    }
}*/
#if 0
/***********************************************************************************************************************
* Function Name: get_config_from_can
* Description  : This function implements get configuration from the CAN.
* Arguments    : unsigned char*, unsigned char*
* Return Value : char
***********************************************************************************************************************/
char get_config_from_can(unsigned char * record, unsigned char * fielddata)
{
	int index,i=0; //len
	int in_c;
	// return the string between the N-1th and Nth comma
	//print_debug(record,(int)strlen(record));
	//give_delay_in_ms(200);
	index = 0;
	//len = (int)(int)strlen(record);
	while ( (in_c = record[index++]) != 0xFF) {
		 if((in_c=='\r')||in_c=='\n')
			break;
		 fielddata[i++] = in_c;
		 if (in_c == '\0') return ((char)-1); // we reached end of string before hitting the next comma
	 }
	 fielddata[i] = '\0';
	 return(0);
}
#endif

/***********************************************************************************************************************
* Function Name: get_config_from_can
* Description  : This function implements get configuration from the CAN.
* Arguments    : unsigned char*, unsigned char*
* Return Value : char
***********************************************************************************************************************/
char get_config_from_can(unsigned char * record, unsigned char * fielddata)
{
	int index,i=0; //len
	int in_c;
	int ff_found=0;
	index = 0;
	//len = (int)(int)strlen(record);
	 while ( (in_c = record[index++]) != 0x00) {
		 if((in_c=='\r')||in_c=='\n')
			break;
		if(in_c==0xFF){
		 ff_found=1;
		}
		if(in_c!=0xFF){
			if(ff_found==1){
			fielddata[i++] = ',';
			ff_found=0;
			}
		 fielddata[i++] = in_c;
    	}
		 if (in_c == '\0') return ((char)-1); // we reached end of string before hitting the next comma
	 }
	 fielddata[i] = '\0';
	 return(0);
}

/***********************************************************************************************************************
* Function Name: conv2hex
* Description  : This function converts hex format.
* Arguments    : char, char*, char*
* Return Value : none
***********************************************************************************************************************/
void conv2hex(char inchar, char *out1, char *out2)	// pass the byte , return out2-lsb out1- msb
{
	char ch1,ch2,ch3,ch='0';
	int i;
	ch1 = inchar & 0x0F;
	ch2 = (inchar & 0xF0)>>4;

	for (i=0;i<2;i++) {
		if (i==0)
			ch3 = ch1;
		else
			ch3 = ch2;
		if (ch3 <=9)
			ch = '0'+ ch3;
		else if (ch3 <=15)
			ch ='A'+ ch3-(10);
		if (i==0)
			*out1 = ch;
		else
			*out2 = ch;
	}
}
/***********************************************************************************************************************
* Function Name: convhextobyte
* Description  : This function converts hex to  byte format.
* Arguments    : char, char, unsigned char*
* Return Value : none
***********************************************************************************************************************/
void convhextobyte(char inchar1,char inchar2, unsigned char *out)
{
	char ch1,ch2='0';
	ch1 = inchar1-'0';
	if(ch1>9)
		ch1=inchar1-'A'+10;
		*out=ch1<<4 ;
		ch2 = inchar2-'0';
	if(ch2>9)
		ch2=inchar2-'A'+10;
		*out=*out+ch2;
}
/***********************************************************************************************************************
* Function Name: convert_bcd
* Description  : This function converts bcd format, for nxp rtc-time convertion.
* Arguments    : short int, unsigned char*, unsigned char*
* Return Value : none
***********************************************************************************************************************/
void convert_bcd(short int in, unsigned char  *out_lsb, unsigned char *out_msb)
{
	short int ch, i;
	ch = in; i=0;
	while (ch >9) {
		ch = ch-10;
		i++;
	}
	*out_lsb =  ((in - i*10) & (unsigned char)0xF) + (unsigned char)'0';
	*out_msb = ((i ) & (unsigned char)0x7) + (unsigned char)'0';

}
/***********************************************************************************************************************
* Function Name: convert_vel_to_integer
* Description  : This function converts ascii to integer.
* Arguments    : char*
* Return Value : int
***********************************************************************************************************************/
/*  if input in ascii ="032.09"
	result 32  -- fraction omitted for land vehicles expect this to be in range 0-200
*/
int convert_vel_to_integer(char *vel)
{
	char tmp[12];
	int i,n;
	n = (int)strlen(vel);

	if (n > 11)
		return(-1); //buff check
	for (i=0;i<n;i++)
		if (vel[i] != '.')
			tmp[i] = vel[i];
		else
			break;
	tmp[i] = '\0';
	n = atoi(tmp);
	return n;
}
/***********************************************************************************************************************
* Function Name: findpat
* Description  : This function is used to find a particular string in a whole string.
* Arguments    : char*, char*, int
* Return Value : int
***********************************************************************************************************************/
/*int findpat(char *buf, char *pat, int buflen)
{

	int i=0,j=0,k;
	int x;
	j = (int)strlen(pat);
	x = 1;
	R_WDT_Restart();

	while (i < buflen ){
		for (k = 0;k < j;k++){
	        	x = x && (buf[i+k] == pat[k]);
			R_WDT_Restart();
		}
		if (x == 1){
	    		return (i);     // string beginning found at buf+i
		}
		i++; x=1;
		R_WDT_Restart();// reset true
	}
	return (-1);               // string not found
}*/
/***********************************************************************************************************************
* Function Name: BCDToDecimal
* Description  : This function is used to convert BCD to Decimal.
* Arguments    : char*, char*, int
* Return Value : int
***********************************************************************************************************************/
unsigned char BCDToDecimal(unsigned char bcdByte)
{
	return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}

/***********************************************************************************************************************
* Function Name: conv2dec
* Description  : This function is used to convert Decimal format.
* Arguments    : uint8_t, uint8_t*, uint8_t*
* Return Value : none
***********************************************************************************************************************/
void conv2dec(unsigned char inchar, unsigned char *out1, unsigned char *out2)
{
	char ch1,ch2,ch3,ch='0';
	char i,a=0,b=0;
	unsigned char x=0;
	x=inchar;

        if(x>=90)
		a=9;
        else if((x>=80)&& (x<=89))
		a=8;
        else if((x>=70)&& (x<=79))
		a=7;
        else if((x>=60)&& (x<=69))
		a=6;
        else if((x>=50)&& (x<=59))
		a=5;
        else if((x>=40)&& (x<=49))
		a=4;
        else if((x>=30)&& (x<=39))
		a=3;
        else if((x>=20)&& (x<=29))
		a=2;
        else if((x>=10)&& (x<=19))
		a=1;

	b=(x-(a*10));
	ch1 = b;
	ch2 = a;

	for (i=0;i<2;i++) {
	    if (i==0)
		ch3 = ch1;
	    else
		ch3 = ch2;
	    if (ch3 <=9)
		ch = '0'+ ch3;
	    else if (ch3 <=15)
		ch ='A'+ ch3-(10);
	    if (i==0)
		*out1 = ch;
	    else
		*out2 = ch;
	}
}
/***********************************************************************************************************************
* Function Name: bin2dec
* Description  : This function converts an in put byte stream in to an output ascii hex stream,
		 ensure that outbuf is twice the size of inbuf.
* Arguments    : uint8_t*, int8_t, uint8_t*
* Return Value : none
***********************************************************************************************************************/
void bin2dec(unsigned int *inbuf, char size, unsigned int *outbuf)
{
	int i = 0, j = 0;
	unsigned char hexchar1,hexchar2;

	for (i=0,j=0;i<size; i++,j=j+2) {
		conv2dec(inbuf[i],&hexchar1,&hexchar2);
		outbuf[j] =  hexchar2; // MSB
		outbuf[j+1] = hexchar1; // LSB
	}
	outbuf[j] = '\0';

}
/***********************************************************************************************************************
* Function Name: convert_knots_to_km_meter
* Description  : This function is used to convert Knots to KM.
* Arguments    : char*, unsigned long*, uint32_t
* Return Value : int
***********************************************************************************************************************/
int convert_knots_to_km_meter(char *vel,unsigned long *km,unsigned int *meter)
{
	char tmp[8];
	int i,j;
	unsigned int n,k10;
	n = (int)strlen(vel);
	if (n > 11)
		return(-1);	//buff check
	for (i=0;i<n;i++){
		if (vel[i] != '.'){ 	//checking for the character by character in string for '.'.
			tmp[i] = vel[i];	//assigning string charater to the temporary variable.
			j=1;
		}else{
		    	j=0;	//updating the variable if the character has '.' in the string.
			break;	//breaking out of the loop.
		}
	}
	if (j==1){ 	//if j=1 then the strings does not have '.'.
		tmp[i] = '0';	//last character is updated by the null character.
	}else{	//string has the '.' so the next charater is updated to the temporary variable.
        	tmp[i] =vel[i+1];
        }
	tmp[i+1] = '\0';
	k10 = atoi(tmp);	//converting string to integer. k10 is 10 times actual speed in knot
	n=(k10>>2)-(k10>>4) - (k10 >> 9);    // kmph = 1.85 x knots => kmph = 0.185 * k10  = (0.25 - 0.0625 - 0.0025) x knots
	*km=n;  // 0.25 -> >> 2,  0.0625 -> >> 4   0.002 -> >> 9
	n= k10<<4; // n x16 -> 16 x 10x knots  - to meter per sec  - 0.185 x 1000/3600 = 0.0514*16
	*meter=(n>>5)+(n>>6)+(n>>8)+ (n>>10); // 0.0514 = 0.03125 + 0.01562 + 0.00390+ 0.000975
	return *km;
}
/***********************************************************************************************************************
* Function Name: convert_hdop
* Description  : This function is used to convert hdop.
* Arguments    : char*, unsigned long*, uint32_t
* Return Value : int
***********************************************************************************************************************/
int convert_hdop(char *vel,unsigned long *hdop)
{
	char tmp[8];
	int i,j;
	unsigned int n;
	n = (int)strlen(vel);
	if (n > 11)
		return(-1);				//buff check
	for (i=0;i<n;i++){
		if (vel[i] != '.'){			//checking for the character by character in string for '.'.
			tmp[i] = vel[i];		//assigning string charater to the temporary variable.
			j=1;
		}else{
		    	j=0;				//updating the variable if the character has '.' in the string.
			break;				//breaking out of the loop.
		}
	}
	if (j==1){					//if j=1 then the strings does not have '.'.
		tmp[i] = '0';				//last character is updated by the null character.
	}else{						//string has the '.' so the next charater is updated to the temporary variable.
        	tmp[i] =vel[i+1];
        }
	tmp[i+1] = '\0';
	*hdop = atoi(tmp);
	return 0;
}
/***********************************************************************************************************************
* Function Name: blebackoff-flags
* Description  : blebackoff time functions.
* Arguments    :
* Return Value :
***********************************************************************************************************************/
void set_blebackoff(int x)
{
	blebackoff_discnt = x;
}
void decr_blebackoff(void)
{
	blebackoff_discnt--;
}
int get_blebackoff(void)
{
	return blebackoff_discnt;
}
void clear_blebackoff(void)
{
	blebackoff_discnt=0;
}

/***********************************************************************************************************************
* Function Name: convert_string_to_HEX
* Description  : This function is used to convert string to HEX.
* Arguments    : long int, unsigned char*,unsigned char*
* Return Value : unsigned int
***********************************************************************************************************************/
unsigned int convert_string_to_HEX(long int data_len, unsigned char *data,unsigned char *string)
{
	const char ascTable[17] = {"0123456789ABCDEF"};
	unsigned char *tmp_p = string;
	long int i, pos;
	pos = 0;
	for(i = 0; i < data_len; i++)
	{
	   tmp_p[pos++] = ascTable[data[i] >> 4];
	   tmp_p[pos++] = ascTable[data[i] & 0x0f];
	}
    	return pos;
}
/***********************************************************************************************************************
* Function Name: convhextodec
* Description  : This function is used to convert HEX to Decimal.
* Arguments    : char*, char*
* Return Value : none
***********************************************************************************************************************/
void convhextodec(char *hex,char *out)
{
	long long decimal = 0, base = 1;
	long int i = 0,length; //value
	length = strlen(hex);

	for(i = length--; i >= 0; i--){
		if(hex[i] >= '0' && hex[i] <= '9'){
	    		decimal += (hex[i] - 48) * base;
	    		base *= 16;
		}
		else if(hex[i] >= 'A' && hex[i] <= 'F'){
	   		decimal += (hex[i] - 55) * base;
	    		base *= 16;
		}
		else if(hex[i] >= 'a' && hex[i] <= 'f'){
	    		decimal += (hex[i] - 87) * base;
	    		base *= 16;
		}
	}
    	sprintf(out,"%ld",decimal);
}

#if 0
/***********************************************************************************************************************
* Function Name: convert_lat
* Description  : This function is used to convert lat value like 01234.3214 to 12xxxxxx.
* Arguments    : char*, long*
* Return Value : unsigned int
***********************************************************************************************************************/
unsigned int convert_lat( char *inbuf,unsigned long *geo_lat)
{
	unsigned int temp;
	unsigned int tempf,temp2;
	char outbuf[12];
	//char buf[50];
	//int n;
	memset(outbuf,'\0',sizeof(outbuf));
	//inbuf is in ddmm.mmmm format
	outbuf[0] = inbuf[0]; //d
	outbuf[1] = inbuf[1]; //d
	outbuf[2] = '\n';
	outbuf[3] = inbuf[2]; //m
	outbuf[4] = inbuf[3]; //m
	outbuf[5] = '\n';
	// skipping inbuf[4] which is '.'
	outbuf[6] = inbuf[5]; //m
	outbuf[7] = inbuf[6]; //m
	outbuf[8] = inbuf[7]; //m

        if(inbuf[8] == '\0'){
		outbuf[9] = '0';
        }else{
		outbuf[9] = inbuf[8]; //m
        }
        outbuf[10] = '\n';
	temp  = atoi(outbuf); // this is degreee
	temp = temp*60; //degree * 60
	temp2 = atoi(outbuf+3); //minute before '.'
	temp2 = temp2+temp; //adding degree and minute
	temp2 = temp2*10000; //left shift 4 time to add after '.' minute
	tempf = atoi(outbuf+6); // getting minute after '.'
	tempf = tempf+temp2; // combining all converted minutes to buffer.
	*geo_lat=tempf;
        return(tempf);
}
/***********************************************************************************************************************
* Function Name: convert_lon
* Description  : This function is used to convert lon value like 01234.3214 to 12xxxxxx.
* Arguments    : char*, long*
* Return Value : unsigned int
***********************************************************************************************************************/
unsigned int convert_lon(char *inbuf,unsigned long *geo_lon)
{

	unsigned int temp ;
	unsigned int tempf,temp2;
	char outbuf[16];//int n;

    // skipping inbuf[0] which is '0' always for indian longitude
        outbuf[0] = inbuf[1]; //d
        outbuf[1] = inbuf[2]; //d
        outbuf[2] = '\n';
        outbuf[3] = inbuf[3]; //m
        outbuf[4] = inbuf[4]; //m
        outbuf[5] = '\n';
        // skipping inbuf[5] which is '.'
        outbuf[6] = inbuf[6]; //m
        outbuf[7] = inbuf[7]; //m
        outbuf[8] = inbuf[8]; //m

        if(inbuf[9] == '\0'){
		outbuf[9] = '0';
	}else{
        	outbuf[9] = inbuf[9]; //m
        }
        outbuf[10] = '\n';
        temp  = atoi(outbuf); // this is degreee
        temp = temp*60; //degree * 60
        temp2 = atoi(outbuf+3); //minute before '.'
	temp2 = temp2+temp; //adding degree and minute
	temp2 = temp2*10000; //left shift 4 time to add after '.' minute
	tempf = atoi(outbuf+6); // getting minute after '.'
	tempf = tempf+temp2; // combining all converted minutes to buffer.
        *geo_lon=tempf;
        return(tempf);

}
#endif


/***********************************************************************************************************************
* Function Name: convert_lat
* Description  : This function is used to convert lat value like 01234.3214 to 12xxxxxx.
* Arguments    : char*, unsigned long*
* Return Value : unsigned int
***********************************************************************************************************************/
unsigned int convert_lat( char *inbuf,unsigned long *geo_lat)
{

	long int temp;
        long int tempf,temp2;
        char outbuf[12];
	//char buf[50];
	//int n;

    memset(outbuf,'\0',sizeof(outbuf));
// inbuf is in ddmm.mmmm format
    	outbuf[0] = inbuf[0]; //d 1
    	outbuf[1] = inbuf[1]; //d 2
    	outbuf[2] = '\n';
    	outbuf[3] = inbuf[2]; //m 5
    	outbuf[4] = inbuf[3]; //m  4
    	outbuf[5] = '\n';
    	// skipping inbuf[4] which is '.'
	if(inbuf[5] == '\0')
        {
            outbuf[6] = '0';
        }
	else
	{
		outbuf[6] = inbuf[5]; //m  0
	}
    	if(inbuf[6] == '\0')
        {
            outbuf[7] = '0';
        }
	else
	{
		outbuf[7] = inbuf[6]; //m  5
	}
    	if(inbuf[7] == '\0')
        {
            outbuf[8] = '0';
        }
	else
	{
		outbuf[8] = inbuf[7]; //m  6
	}

	 if(inbuf[8] == '\0')
        {
            outbuf[9] = '0';
        }
        else
        {
            outbuf[9] = inbuf[8]; //m
        }
    	outbuf[10] = '\n';
	temp  = atol(outbuf); // this is degreee
	temp = temp*60; //degree * 60
	temp2 = atol(outbuf+3); //minute before '.'
        temp2 = temp2+temp; //adding degree and minute
    	temp2 = temp2*10000; //left shift 4 time to add after '.' minute
    	tempf = atol(outbuf+6); // getting minute after '.'
    	tempf = tempf+temp2; // combining all converted minutes to buffer.
   	*geo_lat=tempf;
        return(tempf);
}
/***********************************************************************************************************************
* Function Name: convert_lon
* Description  : This function is used to convert lon value like 01234.3214 to 12xxxxxx.
* Arguments    : char*, unsigned long*
* Return Value : unsigned int
***********************************************************************************************************************/
unsigned int convert_lon(char *inbuf,unsigned long *geo_lon)
{
	long int temp ;
	long int tempf,temp2;
	char outbuf[16];

	// skipping inbuf[0] which is '0' always for indian longitude
	    outbuf[0] = inbuf[1]; //d
	    outbuf[1] = inbuf[2]; //d
	    outbuf[2] = '\n';
	    outbuf[3] = inbuf[3]; //m
	    outbuf[4] = inbuf[4]; //m
	    outbuf[5] = '\n';
	    // skipping inbuf[5] which is '.'
	    if(inbuf[6] == '\0')
	    {
		outbuf[6] = '0';
	    }
	    else
	    {
		outbuf[6] = inbuf[6]; //m
	    }

	    if(inbuf[7] == '\0')
	    {
		outbuf[7] = '0';
	    }
	    else
	    {
		outbuf[7] = inbuf[7]; //m
	    }

	    if(inbuf[8] == '\0')
	    {
		outbuf[8] = '0';
	    }
	    else
	    {
		outbuf[8] = inbuf[8]; //m
	    }

	    if(inbuf[9] == '\0')
	    {
		outbuf[9] = '0';
	    }
	    else
	    {
		outbuf[9] = inbuf[9]; //m
	    }
	    outbuf[10] = '\n';
	    temp  = atol(outbuf); // this is degreee
	    temp = temp*60; //degree * 60
	    temp2 = atol(outbuf+3); //minute before '.'
            temp2 = temp2+temp; //adding degree and minute
    	    temp2 = temp2*10000; //left shift 4 time to add after '.' minute
    	    tempf = atol(outbuf+6); // getting minute after '.'
    	    tempf = tempf+temp2; // combining all converted minutes to buffer.
   	    *geo_lon=tempf;
	    return(tempf);

}

/***********************************************************************************************************************
* Function Name: str_n_cmp
* Description  : This function is used to compare n no of char in a given string like strncmp.
* Arguments    : unsigned char*, unsigned char*, int
* Return Value : int
***********************************************************************************************************************/
int str_n_cmp(unsigned char *s1, unsigned char *s2, int n)
{
	unsigned char u1, u2;

  	while (n-- > 0){
		R_WDT_Restart();
      		u1 = *s1++;
		u2 = *s2++;
		if (u1 != u2)
			return u1 - u2;
		if (u1 == '\0')
			return 0;
    	}
  	return 0;
}

int asciiToHex(char asciiChar) {
    // Convert ASCII hex digit to decimal
    int decimalValue;
    if (asciiChar >= '0' && asciiChar <= '9') {
        decimalValue = asciiChar - '0';
    } else if (asciiChar >= 'A' && asciiChar <= 'F') {
        decimalValue = asciiChar - 'A' + 10; // Adjust for A-F characters
    } else if (asciiChar >= 'a' && asciiChar <= 'f') {
        decimalValue = asciiChar - 'a' + 10; // Adjust for a-f characters
    } else {
        // Invalid input character
        return -1; // Or handle the error in your specific way
    }

    return decimalValue;
}
