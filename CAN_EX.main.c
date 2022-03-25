/*
 * PLL.main.c
 *
 *  Created on: Jan 26, 2022
 *      Author: sa
 */


/*
 * main.c
 *
 *  Created on: Jan 24, 2022
 *      Author: sa
 */

#include "stm32f4xx_hal.h"
#include <string.h>
#include "main.h"
#include "it.h"
char message_recieve[8];

uint32_t mailbox;
char recieve_data[100];
void Error_Handler();
void UART2_init();
CAN_HandleTypeDef	hcan1;
void can_init();
void can_message_send();
void can_message_recieve();
uint8_t count = 0;
void SystemClockConfig(uint8_t systick_clk_val);
void can_config_filter();
char convert_to_lower_case(char ch);

int main(void)
{
	HAL_Init();
	SystemClockConfig(SYS_CLK_50MH);


	can_init();

	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO0_FULL | CAN_IT_RX_FIFO0_OVERRUN | CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_RX_FIFO1_FULL |CAN_IT_RX_FIFO1_OVERRUN);

	can_config_filter();

	if(HAL_CAN_Start(&hcan1) != HAL_OK)
		Error_Handler();

	can_message_send();


	while(1);
	return 0;
}

void can_config_filter()
{
	CAN_FilterTypeDef		can_filter;
	can_filter.FilterIdHigh = 0x0000;
	can_filter.FilterIdLow = 0x0000;
	can_filter.FilterMaskIdLow = 0x0000;
	can_filter.FilterMaskIdHigh = 0x0000;
	can_filter.FilterFIFOAssignment =  CAN_FILTER_FIFO0;
	can_filter.FilterBank = 0;
	can_filter.FilterActivation = CAN_FILTER_ENABLE;
	can_filter.FilterMode = CAN_FILTERMODE_IDMASK;

	if(HAL_CAN_ConfigFilter(&hcan1, &can_filter) != HAL_OK)
		Error_Handler();
}

void can_init()
{
	hcan1.Instance = CAN1;
	hcan1.Init.Prescaler = 5;
	hcan1.Init.Mode = CAN_MODE_LOOPBACK; // Make sure about this
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
	hcan1.Init.AutoRetransmission = ENABLE;
	hcan1.Init.TransmitFifoPriority = ENABLE;
	 if(HAL_CAN_Init(&hcan1) != HAL_OK)
		 Error_Handler();
}

void can_message_send()
{
	char data_send[] = "Mohammed";
	CAN_TxHeaderTypeDef		hcan_tx;

	hcan_tx.StdId = 0x65D;
	hcan_tx.IDE = CAN_ID_STD;
	hcan_tx.DLC = strlen(data_send);
	hcan_tx.RTR = CAN_RTR_DATA;

	if(HAL_CAN_AddTxMessage(&hcan1, &hcan_tx, (uint8_t*)data_send, &mailbox) == !HAL_OK)
		Error_Handler();

}


void SystemClockConfig(uint8_t systick_clk_val)
{
	// Configuring the Clock of the device to HSE
	RCC_ClkInitTypeDef	rcc_clk_init;
	rcc_clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |\
			RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;


	RCC_OscInitTypeDef 	rcc_hse;
	memset(&rcc_hse,0,sizeof(rcc_hse));
	rcc_hse.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	rcc_hse.HSEState = RCC_HSE_ON;

	switch(systick_clk_val)
	{
	case SYS_CLK_50MH:
		rcc_hse.PLL.PLLM = 8;
		rcc_hse.PLL.PLLN = 100;
		rcc_hse.PLL.PLLP = RCC_PLLP_DIV2;
		rcc_hse.PLL.PLLSource = RCC_PLLSOURCE_HSE;
		rcc_hse.PLL.PLLState = RCC_PLL_ON;

		rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
		rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
		rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
		rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

		if(HAL_RCC_OscConfig(&rcc_hse) != HAL_OK)
			Error_Handler();

		if(HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_ACR_LATENCY_1WS) != HAL_OK)
			Error_Handler();
		break;
	case SYS_CLK_84MH:
		// To enable the microcontroller to work in full capacity u have to enable certain settings in power mode
		// First enabel the clock for
		rcc_hse.PLL.PLLM = 8;
		rcc_hse.PLL.PLLN = 84;
		rcc_hse.PLL.PLLP = RCC_PLLP_DIV2;
		rcc_hse.PLL.PLLSource = RCC_PLLSOURCE_HSE;
		rcc_hse.PLL.PLLState = RCC_PLL_ON;

		rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
		rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

		if(HAL_RCC_OscConfig(&rcc_hse) != HAL_OK)
			Error_Handler();

		if(HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_ACR_LATENCY_2WS) != HAL_OK)
			Error_Handler();
		break;
	case SYS_CLK_120MH:
		rcc_hse.PLL.PLLM = 8;
		rcc_hse.PLL.PLLN = 120;
		rcc_hse.PLL.PLLP = RCC_PLLP_DIV2;
		rcc_hse.PLL.PLLSource = RCC_PLLSOURCE_HSI;
		rcc_hse.PLL.PLLState = RCC_PLL_ON;

		rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
		rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

		if(HAL_RCC_OscConfig(&rcc_hse) != HAL_OK)
			Error_Handler();

		if(HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_ACR_LATENCY_4WS) != HAL_OK)
			Error_Handler();
		break;
	}



	HAL_SYSTICK_Config(HAL_RCC_GetSysClockFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}


void Error_Handler()
{
	while(1);
}




void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{


}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{

}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{

}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef 	rx_message;

	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0,&rx_message, (uint8_t*)message_recieve) != HAL_OK)
		Error_Handler();

}
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{


}


