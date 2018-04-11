/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "user_app.h"
#include "cpu_utils.h"
#include "httpd.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId userLedTaskHandle;
osThreadId shellTaskHandle;
osThreadId StartTaskHandle;
osThreadId TouchScreenTaskHandle;

/* USER CODE BEGIN Variables */
extern uint32_t tim3_v_old, tim3_v_new;
extern uint32_t ADC_Value[3];
uint32_t idl_cnt = 0;
uint8_t aTxMessage[] = "\r\n*** UART-Hyperterminal communication based on DMA ***\r\n";
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void startUserLed(void const * argument);
void StartShell(void const * argument);
void startTask(void const * argument);
void touchScreenTask(void const * argument);

extern void MX_LWIP_Init(void);
extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationIdleHook(void);
void vApplicationTickHook(void);

/* USER CODE BEGIN 1 */
extern volatile uint32_t ulHighFrequencyTimerTicks;
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 12563;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
__weak void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
	idl_cnt++;
}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
__weak void vApplicationTickHook( void )
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
}
/* USER CODE END 3 */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
	uint32_t tick = HAL_RCC_GetHCLKFreq ();
	//printf ("%d", tick);
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of userLedTask */
  osThreadDef(userLedTask, startUserLed, osPriorityNormal, 0, 256);
  userLedTaskHandle = osThreadCreate(osThread(userLedTask), NULL);

  /* definition and creation of shellTask */
  osThreadDef(shellTask, StartShell, osPriorityNormal, 0, 256);
  shellTaskHandle = osThreadCreate(osThread(shellTask), NULL);

  /* definition and creation of StartTask */
  osThreadDef(StartTask, startTask, osPriorityIdle, 0, 256);
  StartTaskHandle = osThreadCreate(osThread(StartTask), NULL);

  /* definition and creation of TouchScreenTask */
  osThreadDef(TouchScreenTask, touchScreenTask, osPriorityIdle, 0, 256);
  TouchScreenTaskHandle = osThreadCreate(osThread(TouchScreenTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* startUserLed function */
void startUserLed(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();

  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN startUserLed */
	osDelay(100);
	//puts("Hello world!\r");
	//fflush (stdout);
	//putchar('B');
	//uint8_t ch = 'A';
  /* Infinite loop */
	httpd_init ();
	extern int16_t ad0_ad_value, ad1_ad_value, ad2_ad_value;
  for(;;)
  {
    osDelay(1000);
//		if (sys_env.workstep == 10){
//			cy_println ("motor_pwm:%d", motor_pwm_count);
//		}
    //获取CPU使用率并串口打印
    //usage = osGetCPUUsage();
//    cy_println("CPU Usage: %d%%",osGetCPUUsage());
//		cy_println("main_task process time: %d - %d = %d us", tim3_v_new , tim3_v_old , (tim3_v_new - tim3_v_old));
			
		//HAL_GPIO_WritePin (GPIOC, User_Led_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin (GPIOC, User_Led_Pin, GPIO_PIN_SET);
		//HAL_GPIO_TogglePin (GPIOC, User_Led_Pin);//	putchar('A');
	  //HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  }
  /* USER CODE END startUserLed */
}

/* StartShell function */
void StartShell(void const * argument)
{
  /* USER CODE BEGIN StartShell */
  /* Infinite loop */
//	uint16_t rec_size = 0;
	Usart1Type.RX_flag = 0;
  for(;;)
  { 
    osDelay(100);
		if(Usart1Type.RX_flag){    	// Receive flag
			//taskENTER_CRITICAL ();
			//while (Usart1Type.RX_flag);
			//taskEXIT_CRITICAL ();
			Usart1Type.RX_flag = 0;
			fill_rec_buf (Usart1Type.RX_temp);
		}
  }
  /* USER CODE END StartShell */
}

/* startTask function */
void startTask(void const * argument)
{
  /* USER CODE BEGIN startTask */
  HAL_GPIO_WritePin(LAN8720_RST_GPIO_Port, LAN8720_RST_Pin, GPIO_PIN_SET);
  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)aTxMessage, sizeof(aTxMessage));
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE); 
	
	HAL_UART_Receive_DMA(&huart3, touchScreenDataBuffer, TOUCH_SCREEN_DATA_BUF_LEN); 
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE); //
	
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Value, 3);
	coin_init ();
	cy_println ("[Please press ENTER to activate this console]");
	readNandId ();
	HAL_GPIO_WritePin(GPIOC, Nand_WP_Pin, GPIO_PIN_SET);
  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
    normalTask();
  }
  /* USER CODE END startTask */
}

/* touchScreenTask function */
void touchScreenTask(void const * argument)
{
  /* USER CODE BEGIN touchScreenTask */
  /* Infinite loop */
	touchScreenDataFlag = 0;
  for(;;)
  {
    osDelay(100);
		if (touchScreenDataFlag == 1){
			touchresult();//判断触摸 状态的函数
			touchScreenDataFlag =0;
		}
  }
  /* USER CODE END touchScreenTask */
}

/* USER CODE BEGIN Application */
uint8_t pcWriteBuffer[500];
void taskStatus(void )
{
#if 1
	UBaseType_t   ArraySize;
    TaskStatus_t  *StatusArray;
    uint8_t       x;
	uint32_t FreeRTOSRunTimeTicks;

    ArraySize = uxTaskGetNumberOfTasks(); //获取任务个数
    StatusArray = pvPortMalloc(ArraySize*sizeof(TaskStatus_t));
   // while(1)
    {
        if(StatusArray != NULL){ //内存申请成功

            ArraySize = uxTaskGetSystemState( (TaskStatus_t *)  StatusArray,
                                              (UBaseType_t   ) ArraySize,
                                              (uint32_t *    )  &FreeRTOSRunTimeTicks );

            cy_println("        TaskName\tPriority\tTaskNumber\tMinStk\t");
            for(x = 0;x<ArraySize;x++){

                cy_println("%16s\t%d\t\t%d\t\t%d\t%d%%",
                        StatusArray[x].pcTaskName,
                        (int)StatusArray[x].uxCurrentPriority,
                        (int)StatusArray[x].xTaskNumber,
                        (int)StatusArray[x].usStackHighWaterMark,
                        (int)((float)StatusArray[x].ulRunTimeCounter/FreeRTOSRunTimeTicks*100));
            }
            cy_println();
        }
    }
#else
	printf("=================================================\r\n"); 
	vTaskList((char *)&pcWriteBuffer); 
	printf("%s\r\n", pcWriteBuffer); 
	printf("-------------------------------------------------\r\n"); 
	vTaskGetRunTimeStats((char *)&pcWriteBuffer); 
	printf("%s\r\n", pcWriteBuffer);
#endif
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
