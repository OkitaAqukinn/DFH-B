#include "hcsr04.h"
#include "tim.h"
#include "usart.h"
#include "main.h"
uint16_t distance1=0;
uint16_t FLAG;
Hc_Sr04 hc_sr04[7];

uint8_t Ul[4];
uint8_t U2[4];
/**
  * @brief  HAL�����벶��ص�����
  * @param  htim TIM����ṹ��ָ��
  * @retval ��
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &(htim1))
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			HcSr04_CaptureCallBack(&hc_sr04[0]);
    }
	}
}

/**
  * @brief  ����������ص�����
  * @param  hcsr �������ṹ��ָ��
  * @retval ��
  */
void HcSr04_CaptureCallBack(Hc_Sr04 * hcsr)
{
	TIM_HandleTypeDef * timHandle;
	uint32_t tim_channel;
	
	switch(hcsr->Hcsr04_ID)
	{
		case 1:
		{
			timHandle = &htim1;
			tim_channel = TIM_CHANNEL_1;
			break;
		}
	}
	
	//δ��ɲ���
	if(!(hcsr->Capture_State & 0x80))
	{
		//���񵽵͵�ƽ
		if(hcsr->Capture_State & 0x40)
		{
			
			hcsr->Capture_State = 0;//�������
			if(HAL_TIM_ReadCapturedValue(timHandle,tim_channel) < hcsr->cntStart) 
			{
				hcsr->cntContin = 7999 + HAL_TIM_ReadCapturedValue(timHandle,tim_channel) - hcsr->cntStart;
			}
			else hcsr->cntContin = HAL_TIM_ReadCapturedValue(timHandle,tim_channel) - hcsr->cntStart;
			{
				if(hcsr->pointArray ==10)
				{
					hcsr->pointArray = 0;		
				}
				hcsr->cntArray[hcsr->pointArray++] = hcsr->cntContin * HCSR04_Changemm;
				hcsr->distance = Hcsr04_Fliter(hcsr);
			}
			TIM_RESET_CAPTUREPOLARITY(timHandle,tim_channel);
			TIM_SET_CAPTUREPOLARITY(timHandle,tim_channel,TIM_ICPOLARITY_RISING);

		}//���񵽸ߵ�ƽ
		else
		{
			hcsr->Capture_State |= 0x40;
//			__HAL_TIM_SET_COUNTER(timHandle,tim_channel);
			hcsr->cntStart = HAL_TIM_ReadCapturedValue(timHandle,tim_channel);
			TIM_RESET_CAPTUREPOLARITY(timHandle,tim_channel); 
			TIM_SET_CAPTUREPOLARITY(timHandle,tim_channel,TIM_ICPOLARITY_FALLING);
		}
	}
}
/**
  * @brief  ����������
  * @param  ��
  * @retval ��
  */
void HcSr04_Trig(void)
{
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		HcSr04_Delay(500);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
	}
}
/**
  * @brief  �������ṹ���ʼ��
  * @param  hcsr �������ṹ��ָ��
  * @retval ��
  */
void HcSr04_Init(Hc_Sr04 * hc_sr)
{
	for(uint8_t i=0; i<1; i++)
	{
		hc_sr[i].Hcsr04_ID = i+1;
		hc_sr[i].cntStart = 0;
		hc_sr[i].cntContin = 0;
		hc_sr[i].Capture_State = 0;
		hc_sr[i].pointArray = 0;
		for(uint8_t j=0; j<10; j++)
		{
			hc_sr[i].cntArray[j] = 0;
		}
	}
	//�������벶���ж�
	HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_1);
}
/**
  * @brief  HCSR04�˲�+�㷨
  * @param  hcsr �������ṹ��ָ��
  * @retval �˲����ֵ
  */
uint16_t Hcsr04_Fliter(Hc_Sr04 * hcsr)
{
	uint16_t temp;
	//���������٣���ð������On(n2)û����
	for(uint8_t i=0; i<10; i++)
	{
		for(uint8_t j=i+1; j<10; j++)
		{
			if(hcsr->cntArray[i] > hcsr->cntArray[j])
			{
				temp  = hcsr->cntArray[j];
				hcsr->cntArray[j] = hcsr->cntArray[i];
				hcsr->cntArray[i] = temp;
			}
		}
	}
	//ȡ�м�4��ֵ��ֵ
	return (uint16_t) ((hcsr->cntArray[3]+hcsr->cntArray[4]+hcsr->cntArray[5]+hcsr->cntArray[6])/4);
}
/**
  * @brief  ��ǽ���ߺ���
  * @param  hcsr �������ṹ��ָ��
  * @retval ��
  */
void HcSr04_NearWall(Hc_Sr04 * hcsr)
{
	if((hcsr->distance)>190)
	{
		moto_revolve_set(moto_L,front);
	}
	else
	{
		moto_revolve_set(moto_L,back);	
	}
}
/**
  * @brief  ��������ʱ����
  * @param  i ��ʱʱ��
  * @retval ��
  */
void HcSr04_Delay(__IO uint32_t i)
{
	while(i--);
}
/**
  * @brief  ��������ӡ����
  * @param  hcsr �������ṹ��ָ��
  * @retval ��
  */
void HcSr04_USARTPrint(Hc_Sr04 * hcsr)
{
  FLAG = hcsr->distance;   
	printf("Distance is : %d\r\n",(hcsr->distance));
}


	

