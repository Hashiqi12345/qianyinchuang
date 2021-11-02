 #include "adc.h"
 #include "delay.h"
 #include "usart.h"
 #include "Debug_PIN.h"
#include "Execute_function.h"
#include "RS485.h"
#include "Parse_frame_data.h"

/******************************************************
硬件接口说明：
PC2/3		--->		ADC12/13			采集两个DC模块的电压
*******************************************************/

//初始化ADC															   
void Adc_Init(void)
{    
    ADC_InitTypeDef ADC_InitStructure;       
	  GPIO_InitTypeDef GPIO_InitStructure;
	
		/* 1. ADC时钟配置 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1, ENABLE); //打开ADC时钟
		RCC_ADCCLKConfig(RCC_PCLK2_Div6); //配置时钟(12MHz),F1不超过14MHZ
	
		/* 2. ADC的GPIO初始化 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
		/* 3. ADC参数配置 */
  ADC_DeInit(ADC1);  //复位ADC1 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
		
		/* 4. 使能ADC */
		ADC_Cmd(ADC1, ENABLE); 											//ADC使能
		
		/* 5. ADC校准 */
		ADC_ResetCalibration(ADC1);									//使能复位校准  
		while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
		ADC_StartCalibration(ADC1);									//开启AD校准
		while(ADC_GetCalibrationStatus(ADC1)); 			//等待校准结束
		 
		/* 6. 软件启动ADC转换 */ 
		ADC_SoftwareStartConvCmd(ADC1, ENABLE); 		//因为是持续转换模式，所以直接启动转换，将一直进行ADC转换
}



//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

	//左侧DCD模块电压值
u16 adc_DCL;
u16 temp_DCL;
void ADC_DCL(void)
	{      
	  adc_DCL=Get_Adc_Average(ADC_Channel_12,10);
	  temp_DCL=(int)adc_DCL*(3.3/4096)*100;
	  //printf("DCL= %0.2f\n \r\n",temp_DCL);
    delay_ms(250);	
	}
		/***************************************
		//自动比较，电压值变化，刷新输出电压值数据(可用)
    u16 ADC_DCL;      
    float temp_DCL=0,i=0,temp_a,temp_b,a,b;
		
		while(Get_Adc_Average(ADC_Channel_12,10))
		{
			ADC_DCL=Get_Adc_Average(ADC_Channel_12,10);//通道12得到电压值
			temp_DCL=(float)ADC_DCL*(3.323/4096)*10;
			//保留两位小数赋值给a，准备比较
			temp_a=(int)(temp_DCL*100+0.5);
			a= temp_a/100.0;
			//比较
			if(b!=a)
				{
					i=temp_DCL;
					printf("DCL= %0.2f\n \r\n",i);
					//保留两位小数赋值给b
				  temp_b=(int)(i*100+0.5);
				  b= temp_b/100.0;
			 	  delay_ms(100);
				}
		}
**************************************************/


//右侧DCR模块电压值
	u16 adc_DCR;
 	u16 temp_DCR;
  void ADC_DCR(void)
	{
	  adc_DCR=Get_Adc_Average(ADC_Channel_13,10);
	  temp_DCR=(int)adc_DCR*(3.3/4096)*100;
	  //printf("DCR= %0.2f\n \r\n",temp_DCR);
    delay_ms(250);
	}
/**********************************************************
//自动比较，电压值变化，刷新输出电压值数据		
		u16 ADC_DCR;      
    float temp_DCR=0,i=0,temp_a,temp_b,a,b;
		
		while(Get_Adc_Average(ADC_Channel_13,10))
		{
			ADC_DCR=Get_Adc_Average(ADC_Channel_13,10);//通道12得到电压值
			temp_DCR=(float)ADC_DCR*(3.344/4096)*10;
			
			temp_a=(int)(temp_DCR*100+0.5);
			a= temp_a/100.0;//保留两位小数赋值给a，准备比较
			
			if(b!=a)//比较
				{
					i=temp_DCR;
					printf("DCR= %0.2f\n \r\n",i);
					
				  temp_b=(int)(i*100+0.5);//保留两位小数赋值给b
				  b= temp_b/100.0;
			 	  delay_ms(100);
				}
		}
***************************************************************/
 

	 

	

