////
#include "main.h"
#include "user_app.h"

s_coin_env coin_env;// (std_ad0-40)  //波形下降 阀值  990   950  当前基准下降40  
uint32_t ADC_Value[3];


uint32_t adtime = 0;    //定时中断里 计时

//////////////////////////////////////////////////////
 uint16_t ad0_mintemp = 0;    // = AD0STDVALUE;		//作为 与ad1_ad_value 值进行比较的值，为了取得波形最高值
 uint16_t ad0_maxtemp = 0;   //波形恢复  参考比较值
 uint16_t wave0down_flagone = 0;
 uint16_t wave0up_flagone = 0;    //波形上升 参考比较值
 uint16_t wave0up_flag = 0;
 uint16_t wave0down_flagtwo =0;   //双峰时，用于判断第二个峰
 uint32_t ch0_count =0;  //通道0 通过的硬币计数 
 uint32_t ch0_pre_count =0;  //通道0 来硬币 标记  
 uint32_t ch0_coin_come =0;  //通道 来硬币 标记
 volatile uint16_t blockflag = 0;      //堵币标志变量
 uint16_t ad0_min = 0;     //贮存每枚硬币过去后的 最大值
 
 
//int16_t coin_env.ad0_averaged_value = 0;    //在运行过程中 是ADSAMPNUM个 value的求和值
 int16_t ad0_ad_value = 0;  //在运行过程中 是ADSAMPNUM个 value的和值的平均值


 int16_t ad0_temp_value = 0;  //在求平均值时用到的临时 变量
 int16_t ad0_samp_number = 0;   // ad value sample times 
 int16_t ad1_temp_value = 0;  //在求平均值时用到的临时 变量
 int16_t ad1_samp_number = 0;   // ad value sample times 
 int16_t ad2_temp_value = 0;  //在求平均值时用到的临时 变量
 int16_t ad2_samp_number = 0;   // ad value sample times 
 
 int16_t ad0_ad_value_buf[ADSAMPNUM];	//ad value sample  ADSAMPNUM times 
 int16_t ad1_ad_value_buf[ADSAMPNUM];	//ad value sample  ADSAMPNUM times 
 int16_t ad2_ad_value_buf[ADSAMPNUM];	//ad value sample  ADSAMPNUM times 



 uint16_t ad1_ad_value= 0;	//ad value sample  ADSAMPNUM times 
 uint16_t ad1_mintemp = 0;// = AD1STDVALUE;		//作为 与ad1_ad_value 值进行比较的值，为了取得波形最高值
 uint16_t ad1_maxtemp = 0;   //波形恢复  参考比较值
 uint16_t ad1_min = 0;     //贮存每枚硬币过去后的 最大值
 uint32_t ch1_coin_come =0;  //通道0 来硬币 标记


 uint16_t ad2_ad_value = 0;	//ad value sample  ADSAMPNUM times 
 uint16_t ad2_mintemp = 0;		//作为 与ad2_ad_value 值进行比较的值，为了取得波形最高值
 uint16_t wave2up_flagone =0;    //波形上升 参考比较值
 uint32_t ch2_coin_come =0;  //通道0 来硬币 标记
 uint16_t ad2_min = 0;     //贮存每枚硬币过去后的 最大值
 uint32_t ch1_count1temp =0;  //通道1 来硬币 标记  


//模拟转换  0  函数
//目前采用算术平均滤波算法进行处理 N= 10 后加一个消抖计数滤波器进行处理，判断波形的趋势

void coin_env_init (void)
{
	int i;
	int8_t *p = (int8_t *) &coin_env;
	for (i = 0; i < sizeof(s_coin_env); i++)
	{
		*(p++) = 0;
	}	
}
extern int print_ad0, print_ad1, print_ad2;
//模拟量采样

extern uint32_t ADC_Value[3];
#define GET_AD_VALUE(x) ADC_Value[x]

void AD_Sample_All (void)
{
	
	//PreProcess AD 0 Start*********************************************************************
	ad0_temp_value = ad0_ad_value_buf[ad0_samp_number];//old ad0_samp_number = 0，第一次采样值,步骤2
	coin_env.ad0_averaged_value -= ad0_temp_value;//求和减去第一次AD值
	//Read AD 0 ////////////////////////////////////////////////////////////////////////////////////////
	ad0_ad_value_buf[ad0_samp_number] = GET_AD_VALUE(0);
	//PreProcess AD 0 End**********************************************************************
	//Process AD 0 Start ***********************************************************************
	/*coin_env.ad0_averaged_value 求和*/
	coin_env.ad0_averaged_value += ad0_ad_value_buf[ad0_samp_number];//coin_env.ad0_averaged_value 均值后的数				
	ad0_ad_value = (coin_env.ad0_averaged_value)/ADSAMPNUM;   //求当前数组总和后求平均值
	Detect_AD_Value_buf_p[detect_sample_data_buf_index].AD0 = ad0_ad_value; // save ad to buffer
	ad0_samp_number++;
	if( (ad0_samp_number > (ADSAMPNUM-1)) ){   //如果采集够了ADSAMPNUM个数据10
			ad0_samp_number = 0;
	}
	//Process AD 0 End ***********************************************************************
	
	//PreProcess AD 1 Start*********************************************************************
	ad1_temp_value = ad1_ad_value_buf[ad1_samp_number];//old ad1_samp_number = 0，第一次采样值,步骤2
	coin_env.ad1_averaged_value -= ad1_temp_value;//求和减去第一次AD值
	//Read AD 1 ////////////////////////////////////////////////////////////////////////////////////////
	ad1_ad_value_buf[ad1_samp_number] =  GET_AD_VALUE(1);
	//PreProcess AD 1 End**********************************************************************
	//Process AD 1 Start ***********************************************************************
	/*coin_env.ad1_averaged_value 求和数*/
	coin_env.ad1_averaged_value += ad1_ad_value_buf[ad1_samp_number];//coin_env.ad1_averaged_value 均值后的数				
	ad1_ad_value = (coin_env.ad1_averaged_value)/ADSAMPNUM;   //求当前数组总和后求平均值
	Detect_AD_Value_buf_p[detect_sample_data_buf_index].AD1 = ad1_ad_value; // save ad to buffer
	ad1_samp_number++;
	if( (ad1_samp_number > (ADSAMPNUM-1)) ){   //如果采集够了ADSAMPNUM个数据10
		ad1_samp_number = 0;
	}
	//Process AD 1 End ***********************************************************************
	
	//PreProcess AD 2 Start*********************************************************************
	ad2_temp_value = ad2_ad_value_buf[ad2_samp_number];//old ad2_samp_number = 0，第一次采样值,步骤2
	coin_env.ad2_averaged_value -= ad2_temp_value;//求和减去第一次AD值
	//Read AD 2 ////////////////////////////////////////////////////////////////////////////////////////
	ad2_ad_value_buf[ad2_samp_number] =  GET_AD_VALUE(2);
	//PreProcess AD 2 End**********************************************************************
	//Process AD 2 Start ***********************************************************************
	/*coin_env.ad2_averaged_value 求和数*/
	coin_env.ad2_averaged_value += ad2_ad_value_buf[ad2_samp_number];//coin_env.ad2_averaged_value 均值后的数				
	ad2_ad_value = (coin_env.ad2_averaged_value)/ADSAMPNUM;   //求当前数组总和后求平均值
	Detect_AD_Value_buf_p[detect_sample_data_buf_index].AD2 = ad2_ad_value; // save ad to buffer
	ad2_samp_number++;
	if( (ad2_samp_number > (ADSAMPNUM-1)) ){   //如果采集够了ADSAMPNUM个数据10
		ad2_samp_number = 0;
	}
	//Process AD 2 End ***********************************************************************
	
//		cy_println ("AD0:	%d,	AD1:	%d,	AD2:	%d", ad0_ad_value,
//																						 ad1_ad_value, 
//																						 ad2_ad_value);

	
	detect_sample_data_buf_index++;
	if (detect_sample_data_buf_index >= DATA_BUF_LENGTH){
		detect_sample_data_buf_index = 0;
	}
}

uint32_t coin_cross_time = 0;
	
void cy_ad0_valueget(void)
{
	switch(coin_env.ad0_step)
	{ 
		case 0: 
		{
			ch0_count = 0;
			ch0_pre_count = ch0_count;
			
			ch0_coin_come = 0;
			
			wave0down_flagone = 0;
			wave0up_flagone = 0;
			wave0up_flag = 0;
			wave0down_flagtwo = 0;	
			
			detect_sample_data_buf_index = 0;
			
			ad0_samp_number = 0;
			coin_env.ad0_averaged_value = 0;

			ad0_ad_value = AD0STDSET;
			ad0_mintemp = AD0STDSET;//900
			ad0_min = AD0STDSET;//900
			
			blockflag = ADBLOCKT;	   //使用鉴伪传感器 报堵币 堵币时间5
			coin_env.ad0_step = 2;
			sys_env.AD_buf_index = 0;
			Detect_AD_Value_buf_p = Detect_AD_Value_buf[sys_env.AD_buf_index];
			break;
		}
		/*读取通道0,ad0_ad_value采样10次作为均值*/
		case 2:
		{
//			ad0_ad_value_buf[ad0_samp_number] = ReadAdc0();
			ad0_ad_value_buf[ad0_samp_number] = GET_AD_VALUE(0);
			coin_env.ad0_averaged_value += ad0_ad_value_buf[ad0_samp_number];   //求当前数组总和
			ad0_samp_number++;
			blockflag = ADBLOCKT;//堵币时间 400ms
			if( (ad0_samp_number > (ADSAMPNUM-1)) ) {  //如果采集够了ADSAMPNUM个数据 10
				ad0_samp_number = 0;
				ad0_ad_value = coin_env.ad0_averaged_value / ADSAMPNUM;   //求当前数组的平均值
				coin_env.ad0_step = 3; //10 次才会跳到步骤3
				dbg ("go to  coin_env.ad0_step %d", coin_env.ad0_step);
			}
			break;
		}
/*采样对比,旧采样10次值-旧采样某次值+当前采样值/均次*/
		case 3: 
		{
			AD_Sample_All ();
			blockflag = ADBLOCKT;	   //使用鉴伪传感器 报堵币
			if( (ad0_ad_value < coin_env.std_down_value0)){  //std_value	standard value for detect  从平均值的监测改为当前值的监测
				wave0down_flagone++;    //小于参考值  则表示有硬币过来
			}else{// if((ad0_ad_value >= coin_env.std_down_value0)){/*均值大于等于参考值时*/
				wave0down_flagone = 0;
				detect_sample_data_buf_index = 0;	
			}
			/*采样值小于参考值,持续性WAVE_DOWN_TO_STD_N次以上（消抖处理）*/	
			if(  wave0down_flagone > WAVE_DOWN_TO_STD_N){     //WAVE_DOWN_TO_STD_N 4
				//coin_env.ad0_step  = 9;
				coin_cross_time = 0;//开始计时,计算硬币通过传感器的时间
				wave0down_flagone = 0;
				
				ad0_mintemp = ad0_ad_value;
				ch0_coin_come++;  //通道 0 检测到来硬币  通知其它通道开始采样
				coin_env.ad0_step  = 10;
			}
			break;
		} 
		case 10:						//ad0 value get
		{
			AD_Sample_All ();

			if( ad0_ad_value < ad0_mintemp ){  //波形持续下降中此处使用的 比较参数非常重要，原来是ad0_ad_value_buf[ad0_samp_number] 试一下ad0_ad_value
				ad0_mintemp = ad0_ad_value;	//交换最小值
				coin_env.AD_min_index[0] = detect_sample_data_buf_index;
				wave0up_flagone = 0;
			}else if((ad0_ad_value > ad0_mintemp )&&((ad0_ad_value - ad0_mintemp ) > WAVE_UP_VALUE)){    //波形    回升  
				wave0up_flagone++;
			}else{ 
				wave0up_flagone = 0;
			}
			/*持续性WAVE_GO_UP_N次,波形回升*/
			if( wave0up_flagone  > WAVE_GO_UP_N){     //确认波形峰值
				//coin_env.ad0_step = 19;	
				wave0up_flagone = 0;
					
				ad0_min = ad0_mintemp;
				ad0_mintemp = AD0STDSET;//900
				ch0_count++;    //if ch0_count != ch_counttemp ,则表示通道0采集到一枚
				ad0_maxtemp = ad0_ad_value ;//ad0_ad_value求和均值
				coin_env.ad0_step = 25; ///25
			}
			break;
		}
		case 25:						//read in ad value ADSAMPNUM times 
		{
			AD_Sample_All ();
			/*均值大于参考值,表示一枚已经过去*/
			if( (ad0_ad_value >= coin_env.std_up_value0) ){    //如果 波形恢复到参考值，表示一枚结束
				wave0up_flag++;
				wave0down_flagtwo = 0;
				ad0_maxtemp = ad0_ad_value ;
			}else if( 	(ad0_ad_value < coin_env.std_down_value0) && 
						(ad0_ad_value < ad0_maxtemp) && 
						((ad0_maxtemp - ad0_ad_value) > WAVE_COIN_TWO_DOWN_VALUE) ){ 
				wave0up_flag = 0;
				wave0down_flagtwo++;//考虑连币情况,连币时，波谷过去后还没恢复到参考值还会再来一个波谷,
			}else if(ad0_ad_value >= ad0_maxtemp){/*如果波形还在上升，更新确定最大值*/		
				wave0up_flag = 0;
				wave0down_flagtwo = 0;
				ad0_maxtemp = ad0_ad_value ;
			}
					
			/*已经恢复参考值,返回初始测量*/
			if( wave0up_flag > WAVE_UP_TO_STD_N){// WAVE_UP_TO_STD_N 2
				coin_env.ad0_step = 3; //
				sys_env.coin_cross_time = coin_cross_time;//硬币出来了，这里统计硬币经过的时间
				blockflag = ADBLOCKT;      //堵币时间复位
				wave0up_flag =0;
				wave0down_flagtwo = 0;
				if (sys_env.AD_buf_sending == 0){
					sys_env.AD_buf_sending = 1;
					sys_env.AD_data_len = detect_sample_data_buf_index;
					sys_env.Detect_AD_buf_p = Detect_AD_Value_buf_p;
					sys_env.AD_buf_index++;
					sys_env.AD_buf_index %= AD_BUF_GROUP_LEN;
					Detect_AD_Value_buf_p = Detect_AD_Value_buf[sys_env.AD_buf_index];
				}
				memset (Detect_AD_Value_buf_p, 0, sizeof (AD_Value) * DATA_BUF_LENGTH);
				detect_sample_data_buf_index = 0;	
				break;
			}
			/*连币双波谷,返回状态10*/
			if( (wave0down_flagtwo > WAVE_DOWN_TWO_N)) {    //表示双峰来临 WAVE_DOWN_TWO_N 8
				coin_env.ad0_step = 10;
				blockflag = ADBLOCKT;      //使用鉴伪传感器 报堵币 5
				wave0up_flag =0;
				wave0down_flagtwo = 0;	
				sys_env.coin_cross_time = coin_cross_time;//硬币出来了，这里统计硬币经过的时间
				coin_cross_time = 0;
				ad0_mintemp = ad0_ad_value;
				ch0_coin_come++;  //通道 0 检测到连币  通知其它通道开始采样
				break;
			}								
			break;
		}
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////


//模拟转换  1  函数
void cy_ad1_valueget(void)
{					
	switch(coin_env.ad1_step)
	{
		case 0: 
		{
			ch1_coin_come = ch0_coin_come;
			ad1_ad_value = AD1STDSET;
			ad1_mintemp = AD1STDSET;
			ad1_min = AD1STDSET;
			ad1_samp_number = 0;
			coin_env.ad1_averaged_value = 0;

			coin_env.ad1_step = 2;
			break;
		}
		case 2:
		{
			//**********************************************************************
			//cy_println ("Start Sample AD1");
			//*************************************************************************
//			ad1_ad_value_buf[ad1_samp_number] = ReadAdc1();
			ad1_ad_value_buf[ad1_samp_number] = GET_AD_VALUE(1);
			coin_env.ad1_averaged_value = coin_env.ad1_averaged_value + ad1_ad_value_buf[ad1_samp_number];   //求当前数组总和
			ad1_samp_number++;
			if( (ad1_samp_number > (ADSAMPNUM-1)) ){//如果采集够了ADSAMPNUM个数据 10
			
				ad1_samp_number = 0;
				ad1_ad_value = coin_env.ad1_averaged_value/ADSAMPNUM;   //求当前数组的平均值
				coin_env.ad1_step = 3; //10 次才会跳到步骤3
				dbg ("go to  coin_env.ad1_step %d", coin_env.ad0_step);
			}
			break;
		}
		case 3://等待触发硬币检测
		{						
			if(( ch1_coin_come != ch0_coin_come)){//硬币来了,+1值不一样
				coin_env.ad1_step = 5;
				ad1_mintemp =  ad1_ad_value;
				ch1_coin_come  =  ch0_coin_come;
				break;
			}
			break;
		}
		case 5://进入硬币检测
		{						
			if(ad1_mintemp > ad1_ad_value){
				ad1_mintemp =  ad1_ad_value;
				coin_env.AD_min_index[1] = detect_sample_data_buf_index;
			}
			/**/
 			if((ch0_pre_count != ch0_count)){//如果0通道出来了。
				coin_env.ad1_step = 3;
  				ad1_min = ad1_mintemp;
				ad1_mintemp = AD1STDSET;
			}
 			break;
		}
	}
}

//模拟转换  2  函数
void cy_ad2_valueget(void)
{
	switch(coin_env.ad2_step)
	{
		/*初始赋值*/
		case 0: 
		{	
			ch2_coin_come = ch0_coin_come;
			
			ad2_ad_value = AD2STDSET;//900
			ad2_mintemp = AD2STDSET;//900
			ad2_min = AD2STDSET;//900
			
			ad2_samp_number = 0;
			coin_env.ad2_averaged_value = 0;
			
			coin_env.ad2_step = 2;
			break;
		}
		case 2:
		{
			//**********************************************************************
			//cy_println ("Start Sample AD2");
//			ad2_ad_value_buf[ad2_samp_number] = ReadAdc2();
			ad2_ad_value_buf[ad2_samp_number] = GET_AD_VALUE(2);
			coin_env.ad2_averaged_value = coin_env.ad2_averaged_value + ad2_ad_value_buf[ad2_samp_number];   //求当前数组总和
			ad2_samp_number++;
			if( (ad2_samp_number > (ADSAMPNUM-1)) ){   //如果采集够了ADSAMPNUM个数据 10
				ad2_samp_number = 0;
				ad2_ad_value = coin_env.ad2_averaged_value/ADSAMPNUM;   //求当前数组的平均值
				coin_env.ad2_step = 3; //10 次才会跳到步骤3
					
				dbg ("go to  coin_env.ad2_step %d", coin_env.ad0_step);
			}
			//*************************************************************************
			break;
		}
		case 3://等待触发硬币检测			
		{			
			if(( ch2_coin_come != ch0_coin_come)){
				coin_env.ad2_step	 = 5;
				ad2_mintemp = ad2_ad_value; 
				ch2_coin_come = ch0_coin_come;
				break;
			}
			break;
		}
		case 5://进入硬币 检测 					
		{

			if( ad2_mintemp > ad2_ad_value ){
				ad2_mintemp = ad2_ad_value;
				coin_env.AD_min_index[2] = detect_sample_data_buf_index;
			}
				
			if((ch0_pre_count != ch0_count)){
				coin_env.ad2_step	 = 3;
				ad2_min =  ad2_mintemp;
				ad2_mintemp = AD2STDSET;//900
			}
			break;
		}
	}
	return;
}

	
////////////////////////////////////////////////////////////////////////////////
int16_t std_ad0 = 0;
int16_t std_ad1 = 0;
int16_t std_ad2 = 0;




void adcsininget(uint16_t ad0,uint16_t ad1,uint16_t ad2)
{
	
	coin_env.std_down_value0 = (ad0 - WAVE_DOWN_VALUE);  //波形下降 阀值 40  	
	coin_env.std_up_value0 = (ad0 - WAVE_UP_VALUE);  //波形恢复 20
	cy_println( "coin_env.std_down_value0: %d", coin_env.std_down_value0);
	cy_println( "coin_env.std_up_value0: %d", coin_env.std_up_value0);
}
void update_std_ad_x (void)
{	
	uint16_t is;
		////////////////////////////////////
	uint16_t ad0_std[AD0STDNUM];
	uint16_t ad2_std[AD2STDNUM];
	uint16_t ad1_std[AD1STDNUM];

	for(is=0;is<AD0STDNUM;is++)
	{
		while (sys_env.ad_rd == 0);
		sys_env.ad_rd = 0;
		ad0_std[is] = GET_AD_VALUE(0);
		ad1_std[is] = GET_AD_VALUE(1);
		ad2_std[is] = GET_AD_VALUE(2);	
	}
	std_ad0 = (ad0_std[2]  +ad0_std[4]+ad0_std[5]+ad0_std[7]+ad0_std[9])/5;
	std_ad1  = (ad1_std[2] +ad1_std[3] +ad1_std[4]+ad1_std[5]+ad1_std[6]+ad1_std[7]+ad1_std[8]+ad1_std[9])/8;
	std_ad2 = (ad2_std[2]+ad2_std[3]+ad2_std[4] +ad2_std[5] +ad2_std[6] +ad2_std[7]+ad2_std[8] +ad2_std[9])/8;
}


///////////////////////////////////////

 s_coin_compare_value coin_cmp_value[COIN_TYPE_NUM];

void print_std_value(void)    //  检测 基准值   有不大偏差进行补偿
{
	update_std_ad_x ();
}



uint16_t adstd_offset()    //  检测 基准值   有不大偏差进行补偿
{
	uint16_t is;
		////////////////////////////////////
	int16_t std0_offset, std1_offset, std2_offset;

	update_std_ad_x ();
		
	dbg ("real     std0 = %4d          std1 = %4d     std2 = %4d", std_ad0, std_ad1, std_ad2);
	//如果偏差在200 可以进行补偿，否则可能传感器出问题了，给出报警提示
	if( ( ((std_ad0) < (pre_value.country[coinchoose].coin[0].data.std0 + OFFSETMAX))) && 
		  ((std_ad0) > (pre_value.country[coinchoose].coin[0].data.std0 - OFFSETMIN)) ){        
	}else {
		return 0;
	}	

	
	/////////////   
	for (is = 0; is < COIN_TYPE_NUM; is++){ //补偿值
		std0_offset = std_ad0 - pre_value.country[coinchoose].coin[is].data.std0;
		std1_offset = std_ad1 - pre_value.country[coinchoose].coin[is].data.std1;
		std2_offset = std_ad2 - pre_value.country[coinchoose].coin[is].data.std2;
	#ifdef SAMPLE_METHOD_0
		coin_cmp_value[is].compare_max0 = (pre_value.country[coinchoose].coin[is].data.max0 + pre_value.country[coinchoose].coin[is].data.offsetmax0 + 
										  std0_offset);
		coin_cmp_value[is].compare_min0 = (pre_value.country[coinchoose].coin[is].data.min0 + pre_value.country[coinchoose].coin[is].data.offsetmin0 + 
										  std0_offset);
		
		coin_cmp_value[is].compare_max1 = (pre_value.country[coinchoose].coin[is].data.max1 + pre_value.country[coinchoose].coin[is].data.offsetmax1 + 
										  std1_offset - std0_offset);
		coin_cmp_value[is].compare_min1 = (pre_value.country[coinchoose].coin[is].data.min1 + pre_value.country[coinchoose].coin[is].data.offsetmin1 + 
										  std1_offset - std0_offset);
		
		coin_cmp_value[is].compare_max2 = (pre_value.country[coinchoose].coin[is].data.max2 + pre_value.country[coinchoose].coin[is].data.offsetmax2 + 
										  std2_offset - std1_offset);
		coin_cmp_value[is].compare_min2 = (pre_value.country[coinchoose].coin[is].data.min2 + pre_value.country[coinchoose].coin[is].data.offsetmin2 + 
										  std2_offset - std1_offset);
	#endif
	#ifdef SAMPLE_METHOD_1
		coin_cmp_value[is].compare_max0 = (pre_value.country[coinchoose].coin[is].data.max0 + pre_value.country[coinchoose].coin[is].data.offsetmax0 + 
										  std0_offset);
		coin_cmp_value[is].compare_min0 = (pre_value.country[coinchoose].coin[is].data.min0 + pre_value.country[coinchoose].coin[is].data.offsetmin0 + 
										  std0_offset);
		
		coin_cmp_value[is].compare_max1 = (pre_value.country[coinchoose].coin[is].data.max1 + pre_value.country[coinchoose].coin[is].data.offsetmax1 + 
										  std1_offset);
		coin_cmp_value[is].compare_min1 = (pre_value.country[coinchoose].coin[is].data.min1 + pre_value.country[coinchoose].coin[is].data.offsetmin1 + 
										  std1_offset);
		
		coin_cmp_value[is].compare_max2 = (pre_value.country[coinchoose].coin[is].data.max2 + pre_value.country[coinchoose].coin[is].data.offsetmax2 + 
										  std2_offset);
		coin_cmp_value[is].compare_min2 = (pre_value.country[coinchoose].coin[is].data.min2 + pre_value.country[coinchoose].coin[is].data.offsetmin2 + 
										  std2_offset);
		
		if (coin_cmp_value[is].compare_max0 < 10){
			coin_cmp_value[is].compare_max0 = 10;
		}
		if (coin_cmp_value[is].compare_max1 < 10){
			coin_cmp_value[is].compare_max1 = 10;
		}
		if (coin_cmp_value[is].compare_max2 < 10){
			coin_cmp_value[is].compare_max2 = 10;
		}
	
	#endif
	}		
	adcsininget(std_ad0,std_ad1,std_ad2);//AD 波形进入 的阀值

	return 1;
	////////////////////////////////////
}

AD_Value Detect_AD_Value_buf[AD_BUF_GROUP_LEN][DATA_BUF_LENGTH];
AD_Value *Detect_AD_Value_buf_p;
AD_Value NG_value_buf[NG_BUF_LENGTH];
AD_Value GOOD_value_buf[NG_BUF_LENGTH];

volatile uint32_t ng_value_index = 0;
volatile uint32_t good_value_index = 0;
volatile uint32_t  sample_data_buf_index = 0;
volatile uint32_t detect_sample_data_buf_index = 0;
 
/////////////////////////////////

void send_sample_data (AD_Value ad_value_buf[DATA_BUF_LENGTH], int counter)
{
	uint16_t  minTempAD0 = 1000;
	uint16_t  minTempAD1 = 1000;
	uint16_t  minTempAD2 = 1000;
	int i = 0;
	int H_min_index = 0;
	int M_min_index = 0;
	int L_min_index = 0;
	
	cy_print ("start\n");
	for (i = 0; i < counter; i++){
		if (minTempAD0 > ad_value_buf[i].AD0){
			minTempAD0 = ad_value_buf[i].AD0;
			H_min_index = i;
		}
		if (minTempAD1 > ad_value_buf[i].AD1){
			minTempAD1 = ad_value_buf[i].AD1;
			M_min_index = i;
		}
		if (minTempAD2 > ad_value_buf[i].AD2){
			minTempAD2 = ad_value_buf[i].AD2;
			L_min_index = i;
		}
		cy_print("%d	%d	%d	\n",ad_value_buf[i].AD0, ad_value_buf[i].AD1, ad_value_buf[i].AD2);
	}
	cy_print ("end\n");
	
	if ( minTempAD0 > 0){
		minTempAD0 = ad_value_buf[H_min_index].AD0;
		minTempAD1 = ad_value_buf[H_min_index].AD1;
		minTempAD2 = ad_value_buf[H_min_index].AD2;
	}else if (minTempAD1 > 0){
		minTempAD0 = ad_value_buf[M_min_index].AD0;
		minTempAD1 = ad_value_buf[M_min_index].AD1;
		minTempAD2 = ad_value_buf[M_min_index].AD2;
	}else{
		minTempAD0 = ad_value_buf[L_min_index].AD0;
		minTempAD1 = ad_value_buf[L_min_index].AD1;
		minTempAD2 = ad_value_buf[L_min_index].AD2;
	}
//	dgus_tf1word(ADDR_STDH, (minTempAD0)); //	high frequence
//	dgus_tf1word(ADDR_STDM,  minTempAD1); //	middle frequence
//	dgus_tf1word(ADDR_STDL, (minTempAD2)); //	low frequence	
}






void setStdValue (void)
{
	
	update_std_ad_x ();
//	cy_print("A0 :%d   A1 :%d  A2 :%d  \r\n",std_ad0,std_ad1,std_ad2);
	adcsininget(std_ad0,std_ad1,std_ad2);//AD 波形进入 的阀值
}

uint16_t cy_adstd_adj(void)    //基准调试
{
	if (adtime == 0){
		adtime = ADADJACTERTIME;
		if (sys_env.ad_rd == 1){
			sys_env.ad_rd = 0;
			std_ad0 = ad0_ad_value;
			std_ad1 = ad1_ad_value;
			std_ad2 = ad2_ad_value;
			dgus_tf1word(ADDR_STDH, (std_ad0)); //	high frequence
			dgus_tf1word(ADDR_STDM, (std_ad1)); //	middle frequence
			dgus_tf1word(ADDR_STDL, (std_ad2)); //	low frequence	
			cy_println (">%4d %4d %4d",std_ad0,std_ad1,std_ad2);
		}
	}
	return 1;
}


