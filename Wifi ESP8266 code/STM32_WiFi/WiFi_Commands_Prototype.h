#include "stm32f4xx.h"

void disable_echo(void);
void check_working(void);
void restart(void);
void firmware_version(void);
void list_ap(void);
void join_ap( char *,  char *);
void query_joined_ap(void);
void query_quit_ap(void);
void quit_ap(void);
void get_ip_addr(void);
void query_parameters_ap(void);
void set_parameter_ap(char*, char*, char*, char*);
void wifi_mode(char);
void query_wifi_mode(void);
void query_set_connection(void);
void set_connection( char *, char *, char *, char *);
void query_connection(void);
void connection_multiple(unsigned char);
//void check_joined_devices_ip(void);
//void check_connection_status(void);
void send_tcp_ip_data(unsigned char, char*,  char*);
void close_connection(unsigned char);
//void check_baudrate(void);
//void check_own_ip(void);
void rx_ip_data(void);
void UARTSend(char *);
void delay_ms(__IO uint32_t);
