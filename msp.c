/*
 * msp.c
 *
 *  Created on: Jan 24, 2022
 *      Author: sa
 */
#include "stm32f4xx_hal.h"


void HAL_MspInit(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MspInit could be implemented in the user file
   */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	SCB->SHCSR |= 0x7 << 16;	//enable memory fault, bus fault and usage fault

	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);

}


void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	__HAL_RCC_CAN1_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef	can_mx;
	can_mx.Mode = GPIO_MODE_AF_PP;
	can_mx.Alternate = GPIO_AF9_CAN1;
	can_mx.Pin = GPIO_PIN_8| GPIO_PIN_9;
	can_mx.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &can_mx);
/*
	HAL_NVIC_SetPriority(CAN1_TX_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 15, 0);*/
	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
}
