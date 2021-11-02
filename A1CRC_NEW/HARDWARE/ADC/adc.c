#include "adc.h"
#include "delay.h"
#include "usart.h"
#include "RS485.h"
#include "Parse_frame_data.h"

//��ʼ��ADC																   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
  
	/* 1. ADCʱ������ */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	/* 2. ADC��GPIO��ʼ�� */
	//PA1 ��Ϊģ��ͨ����������     adc1  ͨ��10-13
	/*************************************************
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
 *****************************************************/
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//ģ����������AD1->PC2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
  //ȱ�ж�����
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
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	/* 5. ADCУ׼ */
	ADC_ResetCalibration(ADC1);	               //ʹ�ܸ�λУ׼  
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);	               //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1)){};  //�ȴ�У׼����
	
 /* 6. �������ADCת�� */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
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

/*----------------------------------------------------End line-------------------------------------------------------------*/
u8 ADCNS;
void ADC_NS(void)//���ǣ����
{ 
	u16 adc_NS;      
	float temp_NS;
	
	adc_NS=Get_Adc_Average(ADC_Channel_12,10);
	temp_NS=(int)(adc_NS*(3.3/4096)*100);
	
	ADCNS=temp_NS;
 // printf("ǣ����_VOL:= %d\n \r\n",temp_NS);
	delay_ms(5);
}

void ADC_Send_Data(void)
	{
		ADC_NS();
		pack_protocol(6);
		RS485_Send_Data(RS485_TX_BUF,8);
	}


























