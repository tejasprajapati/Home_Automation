#include "WiFi_Commands_Prototype.h"
#include <string.h>
#include "stm32f4xx_usart.h"

extern char local_ip[20],idx,response[250];


void disable_echo(void)
{
	UARTSend("ATE0\r\n");
	delay_ms(100);
}
void check_working(void)
{
	UARTSend("AT\r\n");
	delay_ms(500);
}


void restart(void)
{
	UARTSend("AT+RST\r\n");
	delay_ms(500);
}

void firmware_version(void)
{
	UARTSend("AT+GMR\r\n");
		delay_ms(500);
}
void list_ap(void)
{
	UARTSend("AT+CWLAP\r\n");
	delay_ms(5000);
}

void join_ap( char *ssid,  char *pwd)
{
	 char command[50]= "AT+CWJAP=";
	strcat(command,"\"");
	strcat(command,ssid);
	strcat(command,"\"");
	strcat(command,",");
	strcat(command,"\"");
	strcat(command,pwd);
	strcat(command,"\"\r\n");
	UARTSend(command);
	delay_ms(5000);
}


void query_joined_ap(void)
{
	UARTSend("AT+CWJAP?\r\n");
	delay_ms(500);
}

void query_quit_ap(void)
{
	UARTSend("AT+CWQAP=?\r\n");
	delay_ms(500);
}

void quit_ap(void)
{
	UARTSend("AT+CWQAP\r\n");
	delay_ms(500);
}

void get_ip_addr(void)
{
	UARTSend("AT+CIFSR\r\n");
	delay_ms(500);
	char *ptr = NULL;
	ptr = strstr(response,"STAIP,");
	if(ptr)
	{
		ptr = ptr+6;
		while(*ptr!='\r'&& idx<16)
		{
			local_ip[idx++] = *ptr++;
		}
	}
}

void query_parameters_ap(void)
{
	UARTSend("AT+CWSAP?\r\n");
	delay_ms(500);
}

void set_parameter_ap( char*ssid, char*pwd, char*chnl, char*encrpt)
{
	 char command[100]= "AT+CWSAP=";
	strcat(command,"\"");
	strcat(command,ssid);
	strcat(command,"\"");
	strcat(command,",");
	strcat(command,"\"");
	strcat(command,pwd);
	strcat(command,",");
	strcat(command,"\"");
	strcat(command,chnl);
	strcat(command,"\"");
	strcat(command,",");
	strcat(command,"\"");
	strcat(command,encrpt);
	strcat(command,"\"\r\n");
	UARTSend(command);
	delay_ms(2000);
}

void query_wifi_mode(void)
{
	UARTSend("AT+CWMODE?\r\n");
	delay_ms(1000);
}

void wifi_mode(char mode)
{
	UARTSend("AT+CWMODE=");
	UARTSend(mode);
	UARTSend("\r\n");
	delay_ms(1000);
}

void query_set_connection(void)
{
	UARTSend("AT+CIPSTART=?\r\n");
	delay_ms(100);
}

void set_connection( char *id, char *type, char *addr, char *port)
{
	 char command[100]= "AT+CIPSTART=";
//	strcat(command,"\"");
	strcat(command,id);
//	strcat(command,"\"");
	strcat(command,",");
	strcat(command,"\"");
	strcat(command,type);
	strcat(command,"\"");
	strcat(command,",");
	strcat(command,"\"");
	strcat(command,addr);
	strcat(command,"\"");
	strcat(command,",");
//	strcat(command,"\"");
	strcat(command,port);
	strcat(command,"\r\n");
	UARTSend(command);
	delay_ms(2000);
}

void query_connection(void)
{
	UARTSend("AT+CIPMUX?\r\n");
	delay_ms(500);
}

void connection_multiple(unsigned char type)
{
	UARTSend("AT+CIPMUX=");
	UARTSend(type);
	UARTSend("\r\n");
	delay_ms(500);
}

void send_tcp_ip_data(unsigned char multiple, char *id,  char *length)
{
	 char command[50]="AT+CIPSEND=";
	if(multiple != 0)
	{
		strcat(command,"\"");
		strcat(command,id);
		strcat(command,"\",");
		strcat(command,"\"");
		strcat(command,length);
		strcat(command,"\"\r\n");
		UARTSend(command);
	}
	else
	{
		strcat(command,"\"");
		strcat(command,length);
		strcat(command,"\"\r\n");
		UARTSend(command);
	}
	delay_ms(500);
}

void close_connection(unsigned char id)
{
	UARTSend("AT+CIPCLOSE\r\n");
	delay_ms(20);
}

//void UARTSend(const unsigned char *pucBuffer)
//{
//    //
//    // Loop while there are more characters to send.
//    //
//    while(*pucBuffer!= '\0')
//    {
//        USART_SendData(USART2, (uint16_t) *pucBuffer++);
//        /* Loop until the end of transmission */
//        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
//        {
//        }
//    }
//}
//
//void delay(unsigned int count)
//{
//	unsigned int j = 0,k=0;
//	for(j = 0;j<=count;j++)
//	{
//	//	for(k=0;k<=2;k++)
//		{
//			__NOP();
//			__NOP();
//		}
//	}
//}

/*void UARTSend(char *pucBuffer)
{
    //
    // Loop while there are more characters to send.
    //
    while(*pucBuffer != '\0')
    {
        USART_SendData(USART1, (uint16_t) *pucBuffer++);
        /* Loop until the end of transmission
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        }
    }
}

void delay_ms(__IO uint32_t nTime)
{
	__IO uint32_t TimingDelay = 0;
	TimingDelay = nTime;
	for(TimingDelay=0;TimingDelay<nTime;TimingDelay++);
	//while (TimingDelay != 0)
	//	;
}
*/
