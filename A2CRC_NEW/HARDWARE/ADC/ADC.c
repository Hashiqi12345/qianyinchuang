 #include "adc.h"
 #include "delay.h"
 #include "usart.h"
 #include "Debug_PIN.h"
#include "Execute_function.h"
#include "RS485.h"
#include "Parse_frame_data.h"

/******************************************************
Ӳ���ӿ�˵����
PC2/3		--->		ADC12/13			�ɼ�����DCģ��ĵ�ѹ
*******************************************************/

//��ʼ��ADC															   
void Adc_Init(void)
{    
    ADC_InitTypeDef ADC_InitStructure;       
	  GPIO_InitTypeDef GPIO_InitStructure;
	
		/* 1. ADCʱ������ */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1, ENABLE); //��ADCʱ��
		RCC_ADCCLKConfig(RCC_PCLK2_Div6); //����ʱ��(12MHz),F1������14MHZ
	
		/* 2. ADC��GPIO��ʼ�� */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
		/* 3. ADC�������� */
  ADC_DeInit(ADC1);  //��λADC1 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
		
		/* 4. ʹ��ADC */
		ADC_Cmd(ADC1, ENABLE); 											//ADCʹ��
		
		/* 5. ADCУ׼ */
		ADC_ResetCalibration(ADC1);									//ʹ�ܸ�λУ׼  
		while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
		ADC_StartCalibration(ADC1);									//����ADУ׼
		while(ADC_GetCalibrationStatus(ADC1)); 			//�ȴ�У׼����
		 
		/* 6. �������ADCת�� */ 
		ADC_SoftwareStartConvCmd(ADC1, ENABLE); 		//��Ϊ�ǳ���ת��ģʽ������ֱ������ת������һֱ����ADCת��
}



//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
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

	//���DCDģ���ѹֵ
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
		//�Զ��Ƚϣ���ѹֵ�仯��ˢ�������ѹֵ����(����)
    u16 ADC_DCL;      
    float temp_DCL=0,i=0,temp_a,temp_b,a,b;
		
		while(Get_Adc_Average(ADC_Channel_12,10))
		{
			ADC_DCL=Get_Adc_Average(ADC_Channel_12,10);//ͨ��12�õ���ѹֵ
			temp_DCL=(float)ADC_DCL*(3.323/4096)*10;
			//������λС����ֵ��a��׼���Ƚ�
			temp_a=(int)(temp_DCL*100+0.5);
			a= temp_a/100.0;
			//�Ƚ�
			if(b!=a)
				{
					i=temp_DCL;
					printf("DCL= %0.2f\n \r\n",i);
					//������λС����ֵ��b
				  temp_b=(int)(i*100+0.5);
				  b= temp_b/100.0;
			 	  delay_ms(100);
				}
		}
**************************************************/


//�Ҳ�DCRģ���ѹֵ
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
//�Զ��Ƚϣ���ѹֵ�仯��ˢ�������ѹֵ����		
		u16 ADC_DCR;      
    float temp_DCR=0,i=0,temp_a,temp_b,a,b;
		
		while(Get_Adc_Average(ADC_Channel_13,10))
		{
			ADC_DCR=Get_Adc_Average(ADC_Channel_13,10);//ͨ��12�õ���ѹֵ
			temp_DCR=(float)ADC_DCR*(3.344/4096)*10;
			
			temp_a=(int)(temp_DCR*100+0.5);
			a= temp_a/100.0;//������λС����ֵ��a��׼���Ƚ�
			
			if(b!=a)//�Ƚ�
				{
					i=temp_DCR;
					printf("DCR= %0.2f\n \r\n",i);
					
				  temp_b=(int)(i*100+0.5);//������λС����ֵ��b
				  b= temp_b/100.0;
			 	  delay_ms(100);
				}
		}
***************************************************************/
 

	 

	

