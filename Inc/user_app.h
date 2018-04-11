#ifndef USER_APP
#define USER_APP	

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "usart.h"
#include "spi.h"

#include "yqadc.h"
#include "yqcoin.h"
#include "yqfile.h"
#include "yqscreen.h"
#include "yqsqlite3.h"
#include "yqdrive.h"
#include "my_cmd.h"
#include "coin_config.h"
#include "NAND.h"
#include "fm25w256.h"


#define TRUE 	1   
#define FALSE 	0
#define OK		1
#define FAIL	0

//#define __MY_DEBUG__
#define UART_DEBUG	
	
#ifdef UART_DEBUG
#define dbg_ln(format, arg...) printf("\r\nDEBUG: " format "\nNick-Cmd:", ## arg)
#define dbg(format, arg...) printf("DEBUG: " format "\r\n", ## arg)
#define dbg_null(format, arg...) printf("" format "", ## arg)
#else
#define dbg_ln(format, arg...) 
#define dbg(format, arg...) 
#define dbg_null(format, arg...) 
#endif


#define PC_PRINT	
#ifdef PC_PRINT
#define pc_print(format, arg...) printf("" format "", ## arg)
#define pc_println(format, arg...) printf("" format "\n", ## arg)
#else
#define pc_print(format, arg...) 
#define pc_println(format, arg...) 
#endif

	
#define __USE_COIN_LOG__
#ifdef __USE_COIN_LOG__
#define LOG(format, arg...) coin_log("" format "", ## arg)
#else
#define LOG(format, arg...)
#endif



#define CY_PRINT	
#ifdef CY_PRINT
#define cy_print(format, arg...) printf("" format "", ## arg)
#define cy_println(format, arg...) printf("" format "\r\n", ## arg)
#define cmd(format, arg...) printf("> " format "", ## arg)
#else
#define cy_print(format, arg...) 
#define cy_println(format, arg...) 
#define cmd(format, arg...) 
#endif

#define cy_putc(c) putchar (c)
#define cy_puts(s) fputs (s, stdout)

//#define uart1_sendchar( HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

#define ASSERT(i) \
{\
	if (i)\
	{\
		cy_println(" ASSERT(%d):Error !!! %s, %d", i, __FILE__, __LINE__);\
		while(i);\
	}\
}


#endif
