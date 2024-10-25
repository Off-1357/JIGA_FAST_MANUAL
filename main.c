/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include <math.h>
/* Private defines -----------------------------------------------------------*/
#define CCR1_Val  ((uint16_t)2047)
#define CCR2_Val  ((uint16_t)1535)
#define CCR3_Val  ((uint16_t)100)
#define CCR4_Val  ((uint16_t)100)

const uint8_t   ADC_CHANNELS[] = {ADC1_CHANNEL_3,
                                  ADC1_CHANNEL_4};
																																
uint32_t ADCBuffer[NUMBER_OF_CHANNELS];
uint8_t ch_adc_pont = 0;

uint16_t canal_1 = 0;
uint16_t canal_2 = 0;

uint16_t tim_pwm_1ms = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void main(void)
{
  CLK_Config();
 // GPIOInit();
  TIM4Config();
  StartADCs(ADC_CHANNELS[0]);
  
  TIM1Config();
 //TIM1_Config(500);
  while (1)
  {
    VerificaADC();
  }
  
}

void CLK_Config(void)
{
	ErrorStatus status = FALSE;

	CLK_DeInit();
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
	status = CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
}

void GPIOInit(void)
{
  GPIO_Init(TPS1_INPUT_PORT, TPS1_INPUT_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(TPS2_INPUT_PORT, TPS2_INPUT_PIN, GPIO_MODE_IN_FL_NO_IT);
}

void TIM4Config(void)
{
  TIM4_TimeBaseInit(TIM4_PRESCALER_64,250);  //calibrar TIM4 para 50us
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  enableInterrupts();

  TIM4_Cmd(ENABLE);
}

void TIM1Off(void)
{
  //GPIO_Init(TPS2_INPUT_PORT, TPS2_INPUT_PIN, GPIO_MODE_IN_FL_NO_IT);
  TIM1_ITConfig(TIM1_IT_UPDATE, DISABLE);
  TIM1_Cmd(DISABLE);
  TIM1_DeInit();
}

void TIM1Config(void)
{

  TIM1_DeInit();

  TIM1_TimeBaseInit(77, TIM1_COUNTERMODE_UP, 1023, 0);

  /*TIM1_Pulse = CCR3_Val*/
  TIM1_OC3Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,
               CCR3_Val, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_SET,
               TIM1_OCNIDLESTATE_RESET);

  /*TIM1_Pulse = CCR4_Val*/
  TIM1_OC4Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, CCR4_Val, TIM1_OCPOLARITY_HIGH,
               TIM1_OCIDLESTATE_SET);

  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);

  /* TIM1 Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
}

void StartADCs(uint8_t ch)
{
  ADC1_DeInit();
  
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,
            ch,
            ADC1_PRESSEL_FCPU_D12,
            ADC1_EXTTRIG_GPIO,DISABLE, 
            ADC1_ALIGN_RIGHT, 
            ADC1_SCHMITTTRIG_CHANNEL0, 
            DISABLE);
  
  ADC1_StartConversion();
}

void VerificaADC()
{
  if(ADC1_GetFlagStatus(ADC1_FLAG_EOC))
  {
    ADC1_ClearFlag(ADC1_FLAG_EOC);
    ADCBuffer[ch_adc_pont] = ADC1_GetConversionValue();
    
    if(ch_adc_pont == 1)
    {
      
      canal_2 = 1023 - ADCBuffer[0];
      canal_1 = ADCBuffer[0];
      
      TIM1->CCR3H = (uint8_t)(canal_1 >> 8);
      TIM1->CCR3L = (uint8_t)(canal_1);
      
      TIM1->CCR4H = (uint8_t)(canal_2 >> 8);
      TIM1->CCR4L = (uint8_t)(canal_2);
      
      ch_adc_pont = 0;
    }
    else
    {
      ch_adc_pont ++;
    }
   // TIM1_Config(canal_1);
    StartADCs(ADC_CHANNELS[ch_adc_pont]);
  }
}

void TIM1_Config(uint16_t duty)
{

   TIM1_DeInit();

  /* Time Base configuration */
  /*
  TIM1_Period = 4095
  TIM1_Prescaler = 0
  TIM1_CounterMode = TIM1_COUNTERMODE_UP
  TIM1_RepetitionCounter = 0
  */

  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, 4095, 0);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  
  /*
  TIM1_OCMode = TIM1_OCMODE_PWM2
  TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
  TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
  TIM1_Pulse = CCR1_Val
  TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
  TIM1_OCNPolarity = TIM1_OCNPOLARITY_HIGH
  TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
  TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET
  
  */
  TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,
               CCR1_Val, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
               TIM1_OCNIDLESTATE_RESET); 

  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);

  /* TIM1 Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
}

void AtualizaConts(void)
{
  tim_pwm_1ms ++;
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
		
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
