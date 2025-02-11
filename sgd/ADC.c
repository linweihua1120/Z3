#include "KTH57XXIIC.h"
#include "MyI2C.h"
#include "main.h"
#include "string.h"
#include "sgd.h"
#include "SEGGER_RTT.h"
//#include "py32f0xx_hal.h"
//#include "py32f0xx_hal_adc.h"
#include "py32f072xx_Start_Kit.h" 

// ADC 处理结构体
ADC_HandleTypeDef        AdcHandle;
// ADC 转换数据
uint16_t                 aADCxConvertedData;
// 电流通道 ADC 转换值
uint16_t                 aADC_CURRENT;
// 高压通道 ADC 转换值
uint16_t                 aADC_HV;
// 电池通道 ADC 转换值
uint16_t                 aADC_BAT;
// 温度通道 ADC 转换值
uint16_t                 aADC_TMP;
// 采样缓冲区大小，定义为 5
#define NUM_SAMPLES 5 

// 当前通道编号，0 表示 VBAT，1 表示 V1V5
int currentCH = 0; 

// 数据缓冲区，用于存储最近的采样值
static uint16_t adcBuffer[NUM_SAMPLES] = {0}; 
// 当前索引位置，用于循环覆盖缓冲区
static uint8_t index = 0;                    
// 已采集样本数
static uint8_t count = 0;                    

// 函数声明，用于设置 ADC 通道
void sgdAdcSetChannle(int ch);

/**
 * @brief 错误处理函数
 * @param None
 * @retval None
 */
void APP_ErrorHandler() {
    // 此处可添加具体的错误处理代码
}
/**
 * @brief 更新 ADC 缓冲区并计算最近 5 次采样的平均值
 * @param newAdcValue 新采样值
 * @return 最近 5 次采样的平均值
 */
uint16_t updateAdcAndGetAverage(uint16_t newAdcValue) {
    // 将新采样值存入缓冲区
    adcBuffer[index] = newAdcValue;
    
    // 更新索引，确保循环覆盖缓冲区
    index = (index + 1) % NUM_SAMPLES;
    
    // 增加采样计数，但不能超过缓冲区大小
    if (count < NUM_SAMPLES) {
        count++;
    }

    // 计算平均值
    uint32_t sum = 0;
    for (uint8_t i = 0; i < count; i++) {
        sum += adcBuffer[i];
    }

    return (uint16_t)(sum / count);
}

/**
 * @brief ADC 转换完成回调函数
 * @param hadc：ADC 处理结构体指针
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
//    // 获取当前时间（单位：10ms）
     int now = getTick() ;
//    
    // 获取 ADC 转换值
    aADCxConvertedData = HAL_ADC_GetValue(hadc);

    // 根据当前通道编号，将转换值存储到相应的变量中
    if (currentCH == 1) {
        aADC_HV = aADCxConvertedData;
    } else if (currentCH == 0) {
        aADC_TMP = aADCxConvertedData;
    } else if (currentCH == 4) {
        aADC_BAT = aADCxConvertedData;
    } else if (currentCH == 9) {
        aADC_CURRENT = aADCxConvertedData;
    } 
     //   sgdAdcSetChannle(9);

    // 根据当前通道编号，切换到下一个通道并启动 ADC 转换
	switch (currentCH)
		{
			case 0:
		        currentCH = 1;
				break;
			case 1:
		        currentCH = 4;
				break;
			case 4:
		        currentCH = 0;
				break;
		   default:
		        currentCH = 1;
				break;
			
			
		}
		sgdAdcSetChannle(currentCH);
		SEGGER_RTT_printf(0, "currentCH = %03d  aADC_HV = %05d    aADC_TMP = %05d    aADC_BAT = %05d  now = %u\r\n ",currentCH,aADC_HV,aADC_TMP,aADC_BAT,now);
//    if (currentCH == 4) {
//        currentCH = 1;
//        sgdAdcSetChannle(currentCH);
//       // HAL_ADC_Start_IT(&AdcHandle);
//    } else if (currentCH == 1) {
//        currentCH = 9;
//        sgdAdcSetChannle(currentCH);
//       // HAL_ADC_Start_IT(&AdcHandle);
//    } else if (currentCH == 9) {
//        currentCH = 0;
//        sgdAdcSetChannle(currentCH);
//      // HAL_ADC_Start_IT(&AdcHandle);
//    } else {
//        currentCH = 4;
//        sgdAdcSetChannle(currentCH);
//       // HAL_ADC_Start_IT(&AdcHandle);
//    }
}

/**
 * @brief 设置 ADC 通道
 * @param ch 通道编号
 * @retval None
 */
void sgdAdcSetChannle(int ch) {
    ADC_ChannelConfTypeDef   sConfig = {0};  
    sConfig.Channel      = ch;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
    /* ADC 通道配置 */
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK) {
        APP_ErrorHandler();
    }
}

/**
 * @brief ADC 初始化函数
 * @param None
 * @retval None
 */
void sgdADCInit(void) {
    ADC_ChannelConfTypeDef   sConfig = {0};  
    RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInit = {0};
  
    // 使能 ADC 时钟
    __HAL_RCC_ADC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    // 使能 GPIOB 时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();                              

    // 配置 GPIO 为模拟输入模式
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    // 初始化 ADC 相关 GPIO 引脚
    GPIO_InitStruct.Pin = PIN_ADC_BAT;
    HAL_GPIO_Init(PORT_ADC_BAT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = PIN_ADC_HV;
    HAL_GPIO_Init(PORT_ADC_HV, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = PIN_ADC_TMP;
    HAL_GPIO_Init(PORT_ADC_TMP, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = PIN_ADC_CURRENT;
    HAL_GPIO_Init(PORT_ADC_CURRENT, &GPIO_InitStruct);

    // 设置 ADC 中断优先级
    HAL_NVIC_SetPriority(ADC_COMP_IRQn, 0, 0); 
    // 使能 ADC 中断
    HAL_NVIC_EnableIRQ(ADC_COMP_IRQn);         

    // 配置 ADC 时钟源
    RCC_PeriphCLKInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    RCC_PeriphCLKInit.ADCClockSelection    = RCC_ADCCLKSOURCE_PCLK_DIV4;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInit);

    // 配置 ADC 处理结构体
    AdcHandle.Instance = ADC1;
    AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;             // 12 位分辨率
    AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;            // 数据右对齐
    AdcHandle.Init.ScanConvMode          = ADC_SCAN_DISABLE;               // 禁用扫描模式
    AdcHandle.Init.ContinuousConvMode    = DISABLE;                        // 单模式
    AdcHandle.Init.NbrOfConversion       = 1;                              // 转换通道数为 1
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;                        // 禁用不连续模式
    AdcHandle.Init.NbrOfDiscConversion   = 1;                              // 不连续模式短序列长度为 1
    AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;             // 软件触发

    // 初始化 ADC
    if (HAL_ADC_Init(&AdcHandle) != HAL_OK) {
        APP_ErrorHandler();
    }

    // 配置 ADC 通道
    sConfig.Channel      = CHANNEL_ADC_BAT;
    sConfig.Rank         = ADC_REGULAR_RANK_5;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK) {
        APP_ErrorHandler();
    }

    sConfig.Channel      = CHANNEL_ADC_HV;
    sConfig.Rank         = ADC_REGULAR_RANK_2;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK) {
        APP_ErrorHandler();
    }

    sConfig.Channel      = CHANNEL_ADC_CURRENT;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK) {
        APP_ErrorHandler();
    }

    sConfig.Channel      = CHANNEL_ADC_TMP;
    sConfig.Rank         = ADC_REGULAR_RANK_10;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK) {
        APP_ErrorHandler();
    }

    // 执行 ADC 校准
    if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK) {
        APP_ErrorHandler();
    }

    // 设置初始通道
    sgdAdcSetChannle(1);
}

/**
 * @brief 读取 ADC 值
 * @param None
 * @retval None
 */
void sgdReadAdc(void) {
    // 启动 ADC 中断转换
    HAL_ADC_Start_IT(&AdcHandle);
}

/**
 * @brief 获取电池电压
 * @param None
 * @retval 电池电压值
 */
int sgdGetBatteryV(void) {
    return (unsigned int)((aADCxConvertedData * 4860) / 4096);
}

/**
 * @brief 获取 ADC 电流原始值
 * @param None
 * @retval ADC 电流原始值
 */
int sgdGetADCCurrentres(void) {
    return (unsigned int)(aADC_CURRENT);
}

/**
 * @brief 获取 ADC 电流值（单位：mA）
 * @param None
 * @retval ADC 电流值（单位：mA）
 */
int sgdGetADCCurrentresmA(void) {
    return (unsigned int)((aADC_CURRENT * 5000 * 4) / 4096);
}

/**
 * @brief 获取 ADC 原始值
 * @param None
 * @retval ADC 原始值
 */
int sgdGetADCres(void) {
    return (unsigned int)((aADCxConvertedData * 4700) / 4096);
}

/**
 * @brief 获取 ADC 温度原始值（整型）
 * @param None
 * @retval ADC 温度原始值（整型）
 */
int sgdGetADCTempresINT() {
    return (unsigned int)(aADCxConvertedData);
}

/**
 * @brief 获取 ADC 温度值
 * @param None
 * @retval ADC 温度值
 */
int sgdGetADCTempres(void) {
    return (unsigned int)((aADCxConvertedData * 5000) / 4096);
}

/**
 * @brief 获取 ADC 电池值
 * @param None
 * @retval ADC 电池值
 */
int sgdGetADCBat(void) {
    return (unsigned int)((aADC_BAT * 5000 * 2) / 4096);
}

/**
 * @brief 获取 ADC 高压值
 * @param None
 * @retval ADC 高压值
 */
int sgdGetADCHV(void) {
    return (unsigned int)((aADC_HV * 5000) / 4096);
}

/**
 * @brief 获取 ADC 高压值（单位：mV）
 * @param None
 * @retval ADC 高压值（单位：mV）
 */
int sgdGetADCHVmV(void) {
    return (unsigned int)((aADC_HV * 5 * 200) / 4096);
}

/**
 * @brief 获取 ADC 1.5V 参考值
 * @param None
 * @retval ADC 1.5V 参考值
 */
int sgdGetADC1v5res(void) {
    return (unsigned int)((aADC_HV * 4700) / 4096);
}



