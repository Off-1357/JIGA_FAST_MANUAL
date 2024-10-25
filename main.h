#include "stm8s.h"
#include "stdbool.h"

#define TPS1_INPUT_PORT 									GPIOD
#define TPS1_INPUT_PIN 										GPIO_PIN_2 

#define TPS2_INPUT_PORT 									GPIOC
#define TPS2_INPUT_PIN 										GPIO_PIN_4

#define TPS1_OUTPUT_PORT 									GPIOD
#define TPS1_OUTPUT_PIN 									GPIO_PIN_4 

#define TPS2_OUTPUT_PORT 									GPIOC
#define TPS2_OUTPUT_PIN 									GPIO_PIN_3

#define LED_GREEN_OUT_PORT			 					GPIOC
#define LED_GREEN_OUT_PIN 								GPIO_PIN_5 // led

#define LED_RED_OUT_PORT			 						GPIOC
#define LED_RED_OUT_PIN 									GPIO_PIN_6 // led

#define FAST_10_BUTTON_PORT								GPIOD
#define FAST_10_BUTTON_PIN								GPIO_PIN_3

#define FAST_11_BUTTON_PORT								GPIOC
#define FAST_11_BUTTON_PIN								GPIO_PIN_7

#define FAST_12_BUTTON_PORT								GPIOA
#define FAST_12_BUTTON_PIN								GPIO_PIN_3

#define FAST_13_BUTTON_PORT								GPIOB
#define FAST_13_BUTTON_PIN								GPIO_PIN_5



#define NUMBER_OF_CHANNELS								2
#define GAP_TICKS													800

typedef enum
{
	SELECT_PRODUCT,
	ESPERA_ZERO,
	TEST_UART_CHANNEL,
	TEST_TPS1_CHANNEL,
	TEST_TPS2_CHANNEL,
	TEST_VMAX_VALUE,
	TEST_VMIN_VALUE,
	SHOW_RESULTS
}JigaStatus_typedef;

typedef enum
{
	FAST10,
	FAST11,
	FAST12,
	FAST13
}Product_typedef;

typedef enum
{
	NO_ERROR,
	UART_ERROR,
	TPS1_ERROR,
	TPS2_ERROR
}Error_typedef;

typedef enum
{
	START_VARIABLES,
	TEST_START_VALUE,
	WAIT_A_LITTLE,
	TEST_GRADIENT
}TPSTest_typedef;

typedef enum
{
	NEGATIVE_EDGE,
	POSITIVE_EDGE
}TPSPulseStatus_typedef;

void GPIOInit(void);
void TIM4Config(void);
void TIM1Config(void);
void TIM1Off(void);
void CLK_Config(void);
void StartADCs(uint8_t ch);
void UART_Config(void);
void SendByte(char i);
void PrintString(char* str);
uint8_t TestUartBuffer(void);
void ClearUartBuffer(void);
void ResetTPSVars(void);
void VerificaADC(void);
void AtualizaConts(void);
void TIM1_Config(uint16_t duty);
void TIM2_Config(uint16_t duty);
void ProcessJiga(void);
void TPS1_Test(void);
void TPS2_Test(void);
void TPS2_12_Test(void);
void TPS2_13_Test(void);
void TestTPS2Pulse(void);
