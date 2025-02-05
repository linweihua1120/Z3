/**
  ******************************************************************************
  * @file    py32f07x_hal_msp.c
  * @author  MCU Application Team
  * @brief   This file provides code for the MSP Initialization
  *          and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External functions --------------------------------------------------------*/

void HAL_TIM_OnePulse_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct = {0};

  /* Enable TIM3 clock */
  __HAL_RCC_TIM3_CLK_ENABLE();

  __HAL_RCC_GPIOB_CLK_ENABLE();
////  
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  GPIO_InitStruct.Alternate = GPIO_AF2_TIM17;
//  /* TIM3_CH3 */
//  GPIO_InitStruct.Pin = GPIO_PIN_9;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief Initialize TIM1 related MSP
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  __HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_TIM15_CLK_ENABLE();
	__HAL_RCC_TIM16_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE(); 
	__HAL_RCC_GPIOB_CLK_ENABLE();    
	__HAL_RCC_GPIOC_CLK_ENABLE();    
	__HAL_RCC_GPIOF_CLK_ENABLE();        
	__HAL_RCC_COMP1_CLK_ENABLE();                 /* Enable COMP1 clock */           

	GPIO_InitTypeDef   GPIO_InitStruct = {0};
	/* Common configuration for all channels */
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

//  /* TIM17_CH1 */
//  GPIO_InitStruct.Alternate = GPIO_AF2_TIM17;
//  GPIO_InitStruct.Pin = GPIO_PIN_9;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//	
//  /* TIM14_CH1 */
//  GPIO_InitStruct.Alternate = GPIO_AF11_TIM14;
//  GPIO_InitStruct.Pin = GPIO_PIN_0;
//  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
//	
//  /* TIM3_CH3 */
//  GPIO_InitStruct.Alternate = GPIO_AF4_TIM14;
//  GPIO_InitStruct.Pin = GPIO_PIN_4;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	
	
	//TIM16_CH1N 
//	GPIO_InitStruct.Alternate = GPIO_AF2_TIM16;
//	GPIO_InitStruct.Pin = GPIO_PIN_7;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//TIM15_CH1N
	GPIO_InitStruct.Alternate = GPIO_AF3_TIM15;
//	GPIO_InitStruct.Alternate = GPIO_AF1_TIM15;
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//	//TIM15_CH1N
//	GPIO_InitStruct.Alternate = GPIO_AF11_TIM15;
//	GPIO_InitStruct.Pin = GPIO_PIN_13;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	//TIM15_CH1
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM15;
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	//TIM1_CH1
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//TIM1_CH2
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	
	//TIM2_CH3
//	GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
//	GPIO_InitStruct.Pin = GPIO_PIN_2;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//TIM2_CH4
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	//TIM1_CH1N 
//GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
//		//GPIO_InitStruct.Alternate = GPIO_AF5_TIM17;
//	GPIO_InitStruct.Pin = GPIO_PIN_7;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pull = GPIO_PULLUP;

	//TIM15_BKIN
//	GPIO_InitStruct.Alternate = GPIO_AF11_TIM15;
//	GPIO_InitStruct.Pin = GPIO_PIN_8;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//TIM1_BKIN
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* PA0 configured as analog input */
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;            /* analog Mode */
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;                 /* No Pull */
	HAL_GPIO_Init(GPIOA,  &GPIO_InitStruct);            /* GPIO initialization */


	GPIO_InitStruct.Alternate = GPIO_AF7_COMP1;            /* analog Mode */
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;            /* analog Mode */
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOB,  &GPIO_InitStruct);            /* GPIO initialization */
	
//	GPIO_InitStruct.Alternate = GPIO_AF7_COMP2;            /* analog Mode */
//	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;            /* analog Mode */
//	GPIO_InitStruct.Pin = GPIO_PIN_7;
//	HAL_GPIO_Init(GPIOA,  &GPIO_InitStruct);            /* GPIO initialization */
	
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
	
	/* TIM3_CH2 */
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	HAL_NVIC_EnableIRQ(ADC_COMP_IRQn);            /* Enable COMP interrupt */
	HAL_NVIC_SetPriority(ADC_COMP_IRQn, 1, 1);

	/* Enable TIM1 interrupt*/
	HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE(); 
}

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
