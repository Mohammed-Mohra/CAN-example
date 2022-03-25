/*
 * it.c
 *
 *  Created on: Jan 24, 2022
 *      Author: sa
 */
#include "it.h"
extern CAN_HandleTypeDef	hcan1;

void SysTick_Handler()
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void CAN1_TX_IRQHandler()
{
	HAL_CAN_IRQHandler(&hcan1);

}

void CAN1_RX0_IRQHandler()
{
	HAL_CAN_IRQHandler(&hcan1);

}

void CAN1_RX1_IRQHandler()
{
	HAL_CAN_IRQHandler(&hcan1);

}

void CAN1_SCE_IRQHandler()
{
	HAL_CAN_IRQHandler(&hcan1);

}
