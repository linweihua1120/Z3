/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @brief   Main program body
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
#include "usb_config.h"
#include "app_bootloader.h"
#include "app_wdg.h"
#include "app_usart.h"
#include "app_i2c.h"
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_USBInit(void);

// Function to print an array of integers
void print_int_array(unsigned BufferIndex, const char *prefix, const uint8_t *array, size_t size) {
    // Print the prefix (optional)
    if (prefix != NULL) {
        SEGGER_RTT_printf(BufferIndex, "\r\n%s  LEN=%d\n", prefix ,size);
    }

    // Print the array elements
    for (size_t i = 0; i < size; i++) {
        SEGGER_RTT_printf(BufferIndex, " %x ",  array[i]);
    }
	SEGGER_RTT_printf(BufferIndex, "\r\n");
}

//   void sgd1msConfig(void)
//{
//	NVIC_SetPriority(SysTick_IRQn, 0);
//	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
//}

/**
  * @brief  应用程序入口函数.
  * @retval int
  */
  
extern uint32_t uwTick;
static void APP_PvdConfig(void)
{
  /* Enable PWR clock and GPIOB clock */
  GPIO_InitTypeDef  GPIO_InitStruct = {0};
  PWR_PVDTypeDef    sConfigPVD      = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Initialize PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  sConfigPVD.Mode = PWR_PVD_MODE_IT_RISING_FALLING;     /* Configure PVD for rising/falling edge interrupt */
  sConfigPVD.PVDFilter = PWR_PVD_FILTER_NONE;           /* Disable filtering */
  sConfigPVD.PVDLevel = PWR_PVDLEVEL_7;/* */            /* This parameter is invalid as PB07 is used as the detection source */
  sConfigPVD.PVDSource = PWR_PVD_SOURCE_VCC;           /* PVD detection for PB07 */
  HAL_PWR_ConfigPVD(&sConfigPVD);  

  /* Initialize PVD */
  HAL_NVIC_SetPriority(PVD_IRQn, 0, 0);                 /* Interrupt priority setting */
  HAL_NVIC_EnableIRQ(PVD_IRQn);
}

int IsSoftReset(){

	return RCC->CSR;
}
int main(void)
{
        SEGGER_RTT_printf(0, "\r\nmain  start  11-28  ----------------------------\r\n");
		 sgd1msConfig( );
  /* 初始化IWDG和WWDG */
  APP_WDG_Init();
  
  /* 初始化所有外设，Flash接口，SysTick */
  HAL_Init();  
  
  /* Configure user Button */
  //BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
	APP_PvdConfig();
  /* Check if the USER Button is pressed */
//  if (BSP_PB_GetState(BUTTON_USER) == 0x00)
//  {
//    APP_Bootloader_Go(APP_ADDR);
//  }
  
  /* 系统时钟配置 */
  APP_SystemClockConfig();

//  /* 初始化USART1外设 */
//  APP_USART_InitRx(USART1);
//  
//  /* 初始化USART2外设 */
//  APP_USART_InitRx(USART2);
//  
//  /* 初始化USART3外设 */
//  APP_USART_InitRx(USART3);
//  
//  /* 初始化USART4外设 */
//  APP_USART_InitRx(USART4);
//  
//  /* 初始化I2C外设 */
//  APP_I2C_Init();

//  uint8_t buff[16];
//
//  uint8_t* p = (uint8_t*) 0x8004000;
//	uint32_t startTick =  uwTick;
//  md5_sign(p,25006,buff);
//	uint32_t endTick =  uwTick;
//        SEGGER_RTT_printf(0, "\r\ startTick  = %d  end = %d gone = %d\r\n",startTick,endTick, endTick - startTick);
//	print_int_array(0,"md5:", buff,16);
//	print_int_array(0,"md5 sourece:", p,256);
		 // SEGGER_RTT_printf(0, "\r\nBOOTLOAD_MODE = %d	loop\r\n ",BOOTLOAD_MODE);
	/* 初始化USB外设 */
	  /* 初始化USB外设 */
//			  APP_USBInit();
//			  
//			  APP_Bootloader_Init();
//			//	sgd1msConfig( );
//			  SEGGER_RTT_printf(0, "\r\n start 111	loop\r\n ");
//			   sgdInit();
//			  /* 无限循环 */
//			  while (1)
//			  {
//				APP_Bootloader_ProtocolDetection();
//			  }


  sgd1msConfig( );
 
//HAL_PWR_EnterSTOPMode(0,0);
//
SEGGER_RTT_printf(0, "\r\n 11-28 APP MODE	reset  reason = %x\r\n ",IsSoftReset());
//int res  = IsSoftReset();
//if(res & 0x10000000)
//{
//	sgdOnSleepStart();
//}

  /* Configure PA08 pin as MCO1 function to output the system clock */
//  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_8);
//	while(1){
//		;
//	}

sgdInit();
while(1)
	{
	
		sgdLoop();
	}
//	APP_USBInit();
//	
//	APP_Bootloader_Init();
//  //  sgd1msConfig( );
//	SEGGER_RTT_printf(0, "\r\n start 111  loop\r\n ");
//	 sgdInit();
//	/* 无限循环 */
//	while (1)
//	{
//	  APP_Bootloader_ProtocolDetection();
//	}
//	SEGGER_RTT_printf(0, "\r\n11-19 E 	APP MODE \r\n ");
//			 sgdInit();
//			  while(1)
//				  {
//					  sgdLoop();
//				  }

//	if(BOOTLOAD_MODE == 1) {
//		SEGGER_RTT_printf(0, "\r\n APP_Bootloader_Go %x \r\n ",APP_ADDR);
//		if(isGoBootload()){
//			
//			  /* 初始化USB外设 */
//			  APP_USBInit();
//			  
//			  APP_Bootloader_Init();
//			  sgd1msConfig( );
//			  SEGGER_RTT_printf(0, "\r\n start 111	loop\r\n ");
//			  // sgdInit();
//			  /* 无限循环 */
//			  while (1)
//			  {
//				APP_Bootloader_ProtocolDetection();
//			  }
//
//			}
//		else
//			{
//			  	SEGGER_RTT_printf(0, "\r\n APP_Bootloader_Go %x \r\n ",APP_ADDR);
//	    		APP_Bootloader_Go(APP_ADDR);
//
//			}
//	}
//	else{
//			  SEGGER_RTT_printf(0, "\r\n  APP MODE \r\n ");
//		   sgdInit();
//			while(1)
//				{
//					sgdLoop();
//				}
//	}
}

/**
  * @brief  USB外设初始化函数
  * @param  无
  * @retval 无
  */
static void APP_USBInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  __HAL_RCC_USB_CLK_ENABLE();
  
  __HAL_RCC_GPIOA_CLK_ENABLE();

  hid_custom_dfu_init();

  /* 使能USB中断 */
  NVIC_EnableIRQ(USBD_IRQn);
}

/**
  * @brief  系统时钟配置函数
  * @param  无
  * @retval 无
  */

static void APP_SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Oscillator configuration */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE; /* Select oscillator HSE, HSI, LSI, LSE */
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                          /* Enable HSI */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                          /* HSI 1 frequency division */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz; /* Configure HSI clock 24MHz */
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                         /* Close HSE */
  /*RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;*/
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                         /* Close LSI */
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                         /* Close LSE */
  /*RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM;*/
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;                     /* Close PLL */
  /*RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_NONE;*/
  /*RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;*/
  /* Configure oscillator */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  /* Clock source configuration */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* Choose to configure clock HCLK, SYSCLK, PCLK1 */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI; /* Select HSISYS as the system clock */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;     /* AHB clock 1 division */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;      /* APB clock 1 division */
  /* Configure clock source */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    APP_ErrorHandler();
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  输出产生断言错误的源文件名及行号
  * @param  file：源文件名指针
  * @param  line：发生断言错误的行号
  * @retval 无
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* 用户可以根据需要添加自己的打印信息,
     例如: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* 无限循环 */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
