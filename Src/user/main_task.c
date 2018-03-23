
#include "tim.h"
#include "main.h"
#include "user_app.h"


void main_task(void)
{
	static unsigned int running_state = 0;
	running_state++;
	if (running_state >= 1000){
		running_state = 0;
		HAL_GPIO_TogglePin (GPIOC, User_Led_Pin);
	} 
	if (sys_env.sys_runing_time){
		sys_env.sys_runing_time++;
	}
	
	switch (sys_env.workstep)
	{
		case 10:{        //main  proceed
			cy_ad0_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			cy_ad1_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			cy_ad2_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			//////////////////////////////////////////////////////////////////////
//			cy_precoincount();   //鉴伪、计数
//			IR_detect_func();   //第二个踢币程序
			break;
		}
		case 22:{
			cy_ad0_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			cy_ad1_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			cy_ad2_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			//////////////////////////////////////////////////////////////////////
//			cy_coinlearn();   //特征学习
			break;
		}
		case 103:{
			cy_ad0_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			cy_ad1_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			cy_ad2_valueget();    //check coin wave and get ADSAMPNUM of ad  values
			break;
		}
		default:{
			break;
		}
	}
}

//模拟量采样
uint32_t tim3_v_old = 0, tim3_v_new = 0;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	tim3_v_old = htim3.Instance->CNT;
	sys_env.ad_rd = 1;
	main_task ();
#if 0
	int i, j;
	for (i = 0; i < 10; i++){
		for (j = 0; j < 180; j++)
			sys_env.ad_rd = 1;
	}
#endif
	tim3_v_new = htim3.Instance->CNT;
		if ((tim3_v_new - tim3_v_old > 90) || (tim3_v_new < tim3_v_old)){
			cy_println("main_task use: %d - %d = %d", tim3_v_new, tim3_v_old, tim3_v_new - tim3_v_old);
			cy_println ("task too busy, stop here...!!!");
			while (1);
		}
}








