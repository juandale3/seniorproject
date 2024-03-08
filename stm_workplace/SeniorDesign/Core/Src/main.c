/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "flow_controller.h"
#include "project_calls.h"
#include "vacuum_gauge.h"
#include "stepperMotor.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
  enum State {
      START,
	  VAC_ACHIEVMENT_TEST_INIT,
      VAC_ACHIEVMENT_TEST,
	  SPECIAL_TEST_INIT,
	  SPECIAL_TEST,
	  WARM_UP_INIT,
      WARM_UP,
      LOAD_TEST_INIT,
	  LOAD_TEST,
      OPERATION_TEST_INIT,
	  OPERATION_TEST,
	  ULTIMATE_MEASURE_TEST_INIT,
      ULTIMATE_MEASURE_TEST,
      IDLE,
      FAIL_STATE,
      STOP
  };
  enum Status {
        IN_PROGRESS,
		COMPLETE,
		FAILURE
    };

  typedef struct{
	  uint8_t stateList[20];
	  uint8_t* currentState;
	  enum State eNextState;
	  uint8_t VATI[8];
	  uint8_t STI[9];
	  uint8_t WUI[8];
	  uint8_t LTI[9];
	  uint8_t OTI[9];
	  uint8_t UMTI[10];
	  enum Status pumpStatus;
  }Test;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PUMP1 0
#define PUMP2 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

DAC_HandleTypeDef hdac;

TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* Definitions for stateMachine */
osThreadId_t stateMachineHandle;
const osThreadAttr_t stateMachine_attributes = {
  .name = "stateMachine",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sendData */
osThreadId_t sendDataHandle;
const osThreadAttr_t sendData_attributes = {
  .name = "sendData",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal1,
};
/* USER CODE BEGIN PV */

Test pumpTestsParameters[2] = {
    {
        .stateList = {0}, // Initialize all elements to 0
        .currentState = NULL, // Initialize pointer to NULL
        .eNextState = START,
        .VATI = {0}, // Initialize all elements to 0
        .STI = {0}, // Initialize all elements to 0
        .WUI = {0}, // Initialize all elements to 0
        .LTI = {0}, // Initialize all elements to 0
        .OTI = {0}, // Initialize all elements to 0
        .UMTI = {0}, // Initialize all elements to 0
        .pumpStatus = IN_PROGRESS
    },
    {
        .stateList = {0}, // Initialize all elements to 0
        .currentState = NULL, // Initialize pointer to NULL
        .eNextState = START,
        .VATI = {0}, // Initialize all elements to 0
        .STI = {0}, // Initialize all elements to 0
        .WUI = {0}, // Initialize all elements to 0
        .LTI = {0}, // Initialize all elements to 0
        .OTI = {0}, // Initialize all elements to 0
        .UMTI = {0}, // Initialize all elements to 0
        .pumpStatus = IN_PROGRESS
    }
};



uint8_t pump = 0;
float volts = 0;
float dacVolts = 0;
float flowRate = 0;
float vacuumScale = 0;
char msg[68];

uint8_t tx_buffer[20];
uint8_t tx_buffer_size = 0;



uint8_t hours;
uint8_t minutes;
uint8_t seconds;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_ADC1_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM10_Init(void);
static void MX_TIM7_Init(void);
void StartDefaultTask(void *argument);
void StartTask02(void *argument);

/* USER CODE BEGIN PFP */
void resetTime();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END SysInit */



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	pumpTestsParameters[0].currentState = &pumpTestsParameters[0].stateList[0];
	pumpTestsParameters[1].currentState = &pumpTestsParameters[1].stateList[0];
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_TIM10_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  // HAL_TIM_Base_Start_IT(&htim7);

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of stateMachine */
  stateMachineHandle = osThreadNew(StartDefaultTask, NULL, &stateMachine_attributes);

  /* creation of sendData */
  sendDataHandle = osThreadNew(StartTask02, NULL, &sendData_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osThreadSuspend(sendDataHandle);

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	  /*
	  vacuumGaugeADC(&hadc1);
	  volts = adcGet(&hadc1);
	  vacuumScale = readVacuum(volts);
	  sprintf(msg, "Vacuum Gauge\t\tVolts: %.3f V\tVacuum:\t\t%1.0f\tkPa\r\n", volts, vacuumScale);
	  printMsg(msg, &huart3);
	  HAL_Delay(1000);
	*/


	  /*
	  flowControllerADC(&hadc1);
	  volts = adcGet(&hadc1);
	  flowRate = readFlow(volts);
	  sprintf(msg, "Flow Controller\t\tVolts: %.3f V\tFlow Rate:\t%1.0f\tL/min\r\n", volts, flowRate);
	  printMsg(msg, &huart3);
	  dacSet(&hdac, DAC_CHANNEL_1, setFlowRate(volts, 50));
	  HAL_Delay(1000);
	  */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 9600 - 1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 5 - 1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 48000 - 1;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 2000 - 1;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PF4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PF5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_MDC_Pin RMII_RXD0_Pin RMII_RXD1_Pin */
  GPIO_InitStruct.Pin = RMII_MDC_Pin|RMII_RXD0_Pin|RMII_RXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_REF_CLK_Pin RMII_MDIO_Pin RMII_CRS_DV_Pin */
  GPIO_InitStruct.Pin = RMII_REF_CLK_Pin|RMII_MDIO_Pin|RMII_CRS_DV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : RMII_TXD1_Pin */
  GPIO_InitStruct.Pin = RMII_TXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(RMII_TXD1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DI3_Pin DI2_Pin DI1_Pin */
  GPIO_InitStruct.Pin = DI3_Pin|DI2_Pin|DI1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_TX_EN_Pin RMII_TXD0_Pin */
  GPIO_InitStruct.Pin = RMII_TX_EN_Pin|RMII_TXD0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart3)
    {
        // Echo received data back
        //HAL_UART_Transmit(&huart3, (uint8_t*)tx_buffer, tx_buffer_size, HAL_MAX_DELAY);
        // Start a new receive operation
        //HAL_UART_Receive_IT(&huart3, (uint8_t*)rx_buffer, 5);
    }
}
void resetTime(){
	seconds = 0;
	minutes = 0;
	hours = 0;
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the stateMachine thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	  switch(pumpTestsParameters[pump].eNextState) {
	  			case START:
	  				osThreadSuspend(sendDataHandle);
					volts = setFlowRate(0);
					flowControllerADC(&hadc1);
					dacSet(&hdac, DAC_CHANNEL_1, volts);

					stepperOpen();
					HAL_TIM_Base_Start_IT(&htim7);

//	  				pumpTestsParameters[0].stateList[0] = START;
//	  				pumpTestsParameters[0].stateList[1] = IDLE;
//	  				//pumpTestsParameters[0].stateList[1] = VAC_ACHIEVMENT_TEST_INIT;
//	  				pumpTestsParameters[0].stateList[2] = SPECIAL_TEST_INIT;
//	  				pumpTestsParameters[0].stateList[3] = WARM_UP_INIT;
//	  				pumpTestsParameters[0].stateList[4] = LOAD_TEST_INIT;
//	  				pumpTestsParameters[0].stateList[5] = OPERATION_TEST_INIT;
//	  				pumpTestsParameters[0].stateList[6] = VAC_ACHIEVMENT_TEST_INIT;
//	  				pumpTestsParameters[0].stateList[7] = ULTIMATE_MEASURE_TEST_INIT;
//	  				pumpTestsParameters[0].stateList[8] = IDLE;
//	  				pumpTestsParameters[0].stateList[9] = 0;

					// sends current State
					HAL_UART_Transmit(&huart3, (uint8_t*)&pumpTestsParameters[pump].eNextState, 1, HAL_MAX_DELAY);

					// Receives State List
					HAL_UART_Receive(&huart3, (uint8_t*)&pumpTestsParameters[pump].stateList, 20, HAL_MAX_DELAY);

					pumpTestsParameters[pump].eNextState = *(pumpTestsParameters[pump]).currentState++;
					//pumpTestsParameters[pump].eNextState = START;

	  				break;
	  			case VAC_ACHIEVMENT_TEST_INIT:

//					pumpTestsParameters[0].VATI[0] = VAC_ACHIEVMENT_TEST_INIT;
//					pumpTestsParameters[0].VATI[1] = PUMP1;
//					pumpTestsParameters[0].VATI[2] = 1;		// TIMER HOURS
//					pumpTestsParameters[0].VATI[3] = 0;		// TIMER MINUTES
//					pumpTestsParameters[0].VATI[4] = 0;		// Stepper motor open
//					pumpTestsParameters[0].VATI[5] = 1;		// flow controller closed
//					pumpTestsParameters[0].VATI[6] = 3;		// mTorr
//					pumpTestsParameters[0].VATI[7] = 50;		// temperature in C

					// sends current State
					HAL_UART_Transmit(&huart3, (uint8_t*)&pumpTestsParameters[pump].eNextState, 1, HAL_MAX_DELAY);

					// Receives Parameters
					HAL_UART_Receive(&huart3, (uint8_t*)&pumpTestsParameters[0].VATI[0], 8, HAL_MAX_DELAY);

	  				flowStateClose();
	  				stepperOpen();
	  				solenoidOpen();
	  				solenoidClose();
	  				vacuumGaugeADC(&hadc1);

	  				pumpTestsParameters[pump].eNextState = VAC_ACHIEVMENT_TEST;
	  				HAL_TIM_Base_Start_IT(&htim10);	// Starts this timer
	  				resetTime();					// Resets timer
	  				osThreadResume(sendDataHandle);	// Starts data Transfer
	  				break;
	  			case VAC_ACHIEVMENT_TEST:
	  				volts = adcGet(&hadc1);
	  				vacuumScale = readVacuum(volts);

	  				vacuumScale = 1000; // Remove this once STM is connected to hardware

	  				if(vacuumScale <= (float)pumpTestsParameters[pump].VATI[6] / 1000.0){	// Success
	  					HAL_TIM_Base_Stop_IT(&htim10);
		  				osThreadSuspend(sendDataHandle);
		  				pumpTestsParameters[pump].eNextState = *(pumpTestsParameters[pump]).currentState++;
	  				}else if(25 >= pumpTestsParameters[pump].VATI[7]){	// if current temp is >= temp limit
	  					pumpTestsParameters[pump].eNextState = FAIL_STATE;
	  					pumpTestsParameters[pump].pumpStatus = FAILURE;
	  					break;
	  				}else if(pumpTestsParameters[pump].VATI[2] == hours && pumpTestsParameters[pump].VATI[3] == minutes){
	  					pumpTestsParameters[pump].eNextState = FAIL_STATE;
	  					pumpTestsParameters[pump].pumpStatus = FAILURE;
	  					break;
	  				}

	  				osDelay(100); // Checks condition every 100 ms

	  				break;
	  			case SPECIAL_TEST_INIT:
//					pumpTestsParameters[0].STI[0] = SPECIAL_TEST_INIT;
//					pumpTestsParameters[0].STI[1] = PUMP1;
//					pumpTestsParameters[0].STI[2] = 0;		// TIMER HOURS
//					pumpTestsParameters[0].STI[3] = 10;		// TIMER MINUTES
//					pumpTestsParameters[0].STI[4] = 0;		// Stepper motor open
//					pumpTestsParameters[0].STI[5] = 3;		// flow controller controlled
//					pumpTestsParameters[0].STI[6] = 0;		// mTorr
//					pumpTestsParameters[0].STI[7] = 50;		// temperature in C
//					pumpTestsParameters[0].STI[8] = 50;		// Flow Rate

					// sends current State
					HAL_UART_Transmit(&huart3, (uint8_t*)&pumpTestsParameters[pump].eNextState, 1, HAL_MAX_DELAY);

					// Receives Parameters
					HAL_UART_Receive(&huart3, (uint8_t*)&pumpTestsParameters[0].STI[0], 9, HAL_MAX_DELAY);

	  				flowStateControl();
	  				stepperOpen();
	  				solenoidOpen();
	  				solenoidClose();
	  				flowControllerADC(&hadc1);


	  				pumpTestsParameters[pump].eNextState = SPECIAL_TEST;
	  				HAL_TIM_Base_Start_IT(&htim10);	// Starts this timer
	  				resetTime();					// Resets timer
	  				osThreadResume(sendDataHandle);	// Starts data Transfer
	  				dacSet(&hdac, DAC_CHANNEL_1, setFlowRate(pumpTestsParameters[pump].STI[8]));
	  				break;

	  			case SPECIAL_TEST:
//	  				sprintf(msg,"SPECIAL_TEST Scanning\r\n");
//					printMsg(msg, &huart3);
//					osDelay(1000);

	  				flowControllerADC(&hadc1);
					volts = adcGet(&hadc1);
					flowRate = readFlow(volts);

					vacuumGaugeADC(&hadc1);
	  				volts = adcGet(&hadc1);
	  				vacuumScale = readVacuum(volts);

	  				flowRate = 0;		// Remove this Once STM is connected to the hardware
	  				vacuumScale = 1000;	// Remove this Once STM is connected to the hardware

	  				if((uint8_t)flowRate == pumpTestsParameters[pump].STI[8]){	// success
	  					HAL_TIM_Base_Stop_IT(&htim10);
						osThreadSuspend(sendDataHandle);
						pumpTestsParameters[pump].eNextState = *(pumpTestsParameters[pump]).currentState++;
	  				}else if(25 >= pumpTestsParameters[pump].STI[7]){	// if current temp is >= temp limit
	  					pumpTestsParameters[pump].eNextState = FAIL_STATE;
	  					pumpTestsParameters[pump].pumpStatus = FAILURE;
	  					break;
	  				}else if(pumpTestsParameters[pump].STI[2] == hours && pumpTestsParameters[pump].STI[3] == minutes){
	  					pumpTestsParameters[pump].eNextState = FAIL_STATE;
						pumpTestsParameters[pump].pumpStatus = FAILURE;
						break;
	  				}
	  				osDelay(100);	// Checks condition every 100 ms
					break;
	  			case WARM_UP_INIT:

//	  				pumpTestsParameters[0].WUI[0] = WARM_UP_INIT;
//					pumpTestsParameters[0].WUI[1] = PUMP1;
//					pumpTestsParameters[0].WUI[2] = 3;		// TIMER HOURS
//					pumpTestsParameters[0].WUI[3] = 0;		// TIMER MINUTES
//					pumpTestsParameters[0].WUI[4] = 0;		// Stepper motor open
//					pumpTestsParameters[0].WUI[5] = 0;		// flow controller open
//					pumpTestsParameters[0].WUI[6] = 0;		// mTorr
//					pumpTestsParameters[0].WUI[7] = 100;	// temperature in C

					// sends current State
					HAL_UART_Transmit(&huart3, (uint8_t*)&pumpTestsParameters[pump].eNextState, 1, HAL_MAX_DELAY);

					// Receives Parameters
					HAL_UART_Receive(&huart3, (uint8_t*)&pumpTestsParameters[0].WUI[0], 8, HAL_MAX_DELAY);

	  				flowStateOpen();
	  				stepperOpen();
	  				solenoidOpen();
	  				solenoidClose();
	  				vacuumGaugeADC(&hadc1);

	  				pumpTestsParameters[pump].eNextState = WARM_UP;
	  				HAL_TIM_Base_Start_IT(&htim10);	// Starts this timer
	  				resetTime();					// Resets timer
	  				osThreadResume(sendDataHandle);	// Starts data Transfer
	  				break;
	  			case WARM_UP:

	  				// record internal/external temperatures
	  				if(pumpTestsParameters[pump].WUI[2] == hours && pumpTestsParameters[pump].WUI[3] == minutes){
	  					// Success
	  					HAL_TIM_Base_Stop_IT(&htim10);
	  					osThreadSuspend(sendDataHandle);
	  					pumpTestsParameters[pump].eNextState = *(pumpTestsParameters[pump]).currentState++;
	  				}else if(25 >= pumpTestsParameters[0].WUI[7]){	// if current temp is >= temp limit
	  					pumpTestsParameters[pump].eNextState = FAIL_STATE;
	  					pumpTestsParameters[pump].pumpStatus = FAILURE;
	  					break;
	  				}
	  				osDelay(100);	// Checks condition every 100 ms
	  				break;
	  			case LOAD_TEST_INIT:

//					pumpTestsParameters[0].LTI[0] = LOAD_TEST_INIT;
//					pumpTestsParameters[0].LTI[1] = PUMP1;
//					pumpTestsParameters[0].LTI[2] = 8;		// TIMER HOURS
//					pumpTestsParameters[0].LTI[3] = 0;		// TIMER MINUTES
//					pumpTestsParameters[0].LTI[4] = 0;		// Stepper motor open
//					pumpTestsParameters[0].LTI[5] = 3;		// flow controller Controlled
//					pumpTestsParameters[0].LTI[6] = 0;		// mTorr
//					pumpTestsParameters[0].LTI[7] = 100;	// temperature in C
//	  				pumpTestsParameters[0].LTI[8] = 50;		// Flow Rate

					// sends current State
					HAL_UART_Transmit(&huart3, (uint8_t*)&pumpTestsParameters[pump].eNextState, 1, HAL_MAX_DELAY);

					// Receives Parameters
					HAL_UART_Receive(&huart3, (uint8_t*)&pumpTestsParameters[0].LTI[0], 8, HAL_MAX_DELAY);

	  				stepperOpen();
	  				solenoidOpen();
	  				solenoidClose();
	  				flowControllerADC(&hadc1);

	  				pumpTestsParameters[pump].eNextState = LOAD_TEST;
	  				dacSet(&hdac, DAC_CHANNEL_1, setFlowRate(pumpTestsParameters[pump].LTI[8]));
	  				HAL_TIM_Base_Start_IT(&htim10);	// Starts this timer
	  				resetTime();					// Resets timer
	  				osThreadResume(sendDataHandle);	// Starts data Transfer

	  				break;
	  			case LOAD_TEST:

					volts = adcGet(&hadc1);
					flowRate = readFlow(volts);

					flowRate = 0;	// Remove this Once STM is connected to the hardware

	  				if(pumpTestsParameters[pump].LTI[2] == hours && pumpTestsParameters[pump].LTI[3] == minutes){
//	  					// Success
	  					HAL_TIM_Base_Stop_IT(&htim10);
	  					osThreadSuspend(sendDataHandle);
	  					pumpTestsParameters[pump].eNextState = *(pumpTestsParameters[pump]).currentState++;
	  				}else if(25 >= pumpTestsParameters[0].LTI[7]){	// if current temp is >= temp limit
	  					pumpTestsParameters[pump].eNextState = FAIL_STATE;
	  					pumpTestsParameters[pump].pumpStatus = FAILURE;
	  					break;
	  				}

	  				osDelay(100);	// Checks condition every 100 ms
	  				break;
	  			case OPERATION_TEST_INIT:

//					pumpTestsParameters[0].OTI[0] = OPERATION_TEST_INIT;
//					pumpTestsParameters[0].OTI[1] = PUMP1;
//					pumpTestsParameters[0].OTI[2] = 4;		// TIMER HOURS
//					pumpTestsParameters[0].OTI[3] = 0;		// TIMER MINUTES
//					pumpTestsParameters[0].OTI[4] = 0;		// Stepper motor open
//					pumpTestsParameters[0].OTI[5] = 3;		// flow controller Controlled
//					pumpTestsParameters[0].OTI[6] = 0;		// mTorr
//					pumpTestsParameters[0].OTI[7] = 100;	// temperature in C
//	  				pumpTestsParameters[0].OTI[8] = 0;		// Flow Rate

					// sends current State
					HAL_UART_Transmit(&huart3, (uint8_t*)&pumpTestsParameters[pump].eNextState, 1, HAL_MAX_DELAY);

					// Receives Parameters
					HAL_UART_Receive(&huart3, (uint8_t*)&pumpTestsParameters[0].OTI[0], 8, HAL_MAX_DELAY);

	  				stepperOpen();
	  				solenoidOpen();
	  				solenoidClose();
	  				flowControllerADC(&hadc1);

	  				pumpTestsParameters[pump].eNextState = OPERATION_TEST;
	  				//dacSet(&hdac, DAC_CHANNEL_1, setFlowRate(pumpTestsParameters[pump].LTI[8]));
	  				HAL_TIM_Base_Start_IT(&htim10);	// Starts this timer
	  				resetTime();					// Resets timer
	  				osThreadResume(sendDataHandle);	// Starts data Transfer

	  				break;
	  			case OPERATION_TEST:

	  				flowControllerADC(&hadc1);
					volts = adcGet(&hadc1);
					flowRate = readFlow(volts);

					vacuumGaugeADC(&hadc1);
					volts = adcGet(&hadc1);
					vacuumScale = readVacuum(volts);

	  				if(pumpTestsParameters[pump].OTI[2] == hours && pumpTestsParameters[pump].OTI[3] == minutes){
	  					// Success
	  					HAL_TIM_Base_Stop_IT(&htim10);
	  					osThreadSuspend(sendDataHandle);
	  					pumpTestsParameters[pump].eNextState = *(pumpTestsParameters[pump]).currentState++;
	  				}else if(25 >= pumpTestsParameters[0].OTI[7]){	// if current temp is >= temp limit
	  					pumpTestsParameters[pump].eNextState = FAIL_STATE;
	  					pumpTestsParameters[pump].pumpStatus = FAILURE;
	  					break;
	  				}
	  				osDelay(100);
	  				break;
	  			case ULTIMATE_MEASURE_TEST_INIT:

//					pumpTestsParameters[0].UMTI[0] = ULTIMATE_MEASURE_TEST_INIT;
//					pumpTestsParameters[0].UMTI[1] = PUMP1;
//					pumpTestsParameters[0].UMTI[2] = 3;		// TIMER HOURS
//					pumpTestsParameters[0].UMTI[3] = 0;		// TIMER MINUTES
//					pumpTestsParameters[0].UMTI[4] = 3;		// Stepper motor Controlled
//					pumpTestsParameters[0].UMTI[5] = 0;		// flow controller open
//					pumpTestsParameters[0].UMTI[6] = 0;		// mTorr
//					pumpTestsParameters[0].UMTI[7] = 100;	// temperature in C
//	  				pumpTestsParameters[0].UMI[8] = 50;		// Flow Rate
//					pumpTestsParameters[0].UMTI[9] = 15;	// Pressure in kPA

					// sends current State
					HAL_UART_Transmit(&huart3, (uint8_t*)&pumpTestsParameters[pump].eNextState, 1, HAL_MAX_DELAY);

					// Receives Parameters
					HAL_UART_Receive(&huart3, (uint8_t*)&pumpTestsParameters[0].UMTI[0], 9, HAL_MAX_DELAY);

					stepperStep(800);				// close valve half way
					HAL_TIM_Base_Start_IT(&htim7);	// starts closing
	  				stepperOpen();
	  				solenoidOpen();
	  				solenoidClose();
	  				flowControllerADC(&hadc1);



					pumpTestsParameters[pump].eNextState = ULTIMATE_MEASURE_TEST;
	  				//dacSet(&hdac, DAC_CHANNEL_1, setFlowRate(pumpTestsParameters[pump].LTI[8]));
	  				HAL_TIM_Base_Start_IT(&htim10);	// Starts this timer
	  				resetTime();					// Resets timer
	  				osThreadResume(sendDataHandle);	// Starts data Transfer

	  				break;
	  			case ULTIMATE_MEASURE_TEST:


	  				if(pumpTestsParameters[pump].UMTI[2] == hours && pumpTestsParameters[pump].UMTI[3] == minutes){
	  					// Success
	  					HAL_TIM_Base_Stop_IT(&htim10);
	  					osThreadSuspend(sendDataHandle);
	  					pumpTestsParameters[pump].eNextState = *(pumpTestsParameters[pump]).currentState++;
	  				}else if(25 >= pumpTestsParameters[0].UMTI[7]){	// if current temp is >= temp limit
	  					pumpTestsParameters[pump].eNextState = FAIL_STATE;
	  					pumpTestsParameters[pump].pumpStatus = FAILURE;
	  					break;
	  				}
	  				osDelay(100);	// Checks condition every 100 ms
	  				break;
	  			case IDLE:

					osDelay(3000);

	  				break;
	  			case FAIL_STATE:
	  				HAL_UART_Transmit(&huart3, (uint8_t*)&pumpTestsParameters[pump].eNextState, 1, HAL_MAX_DELAY);
	  				HAL_TIM_Base_Stop_IT(&htim10);
	  				osThreadSuspend(sendDataHandle);
	  				pumpTestsParameters[pump].eNextState = *(pumpTestsParameters[pump]).currentState++;
	  				break;
	  			case STOP:
	  				pumpTestsParameters[pump].eNextState = STOP;
	  				HAL_Delay(5000);
	  				break;
	  			default:
	  				pumpTestsParameters[pump].eNextState = STOP;
	  }
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the sendData thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
//	sprintf(msg,"Sending Data\r\n");
//	printMsg(msg, &huart3);



    switch(pumpTestsParameters[pump].eNextState){
    case VAC_ACHIEVMENT_TEST:
    	tx_buffer[0] = VAC_ACHIEVMENT_TEST;
    	tx_buffer[1] = pump;
    	tx_buffer[2] = hours;
    	tx_buffer[3] = minutes;
    	tx_buffer[4] = seconds;
    	tx_buffer[5] = (uint8_t) vacuumScale;	// mTorr
    	tx_buffer[6] = 25;						// temperature in C
    	tx_buffer_size = 7;
    	break;
    case SPECIAL_TEST:
    	tx_buffer[0] = SPECIAL_TEST;
    	tx_buffer[1] = pump;
    	tx_buffer[2] = hours;
    	tx_buffer[3] = minutes;
    	tx_buffer[4] = seconds;
    	tx_buffer[5] = (uint8_t) vacuumScale;	// mTorr
    	tx_buffer[6] = 25;						// temperature in C
    	tx_buffer[7] = (uint8_t) flowRate;		// L/min
    	tx_buffer_size = 8;
    	break;
    case WARM_UP:
    	tx_buffer[0] = WARM_UP;
    	tx_buffer[1] = pump;
    	tx_buffer[2] = hours;
    	tx_buffer[3] = minutes;
    	tx_buffer[4] = seconds;
    	tx_buffer[5] = 25;						// temperature in C
    	tx_buffer_size = 6;
    	break;
    case LOAD_TEST:
    	tx_buffer[0] = LOAD_TEST;
    	tx_buffer[1] = pump;
    	tx_buffer[2] = hours;
    	tx_buffer[3] = minutes;
    	tx_buffer[4] = seconds;
    	tx_buffer[5] = (uint8_t) flowRate;		// L/min
    	tx_buffer[6] = 25;						// temperature in C
    	tx_buffer_size = 7;
    	break;
    case OPERATION_TEST:
    	tx_buffer[0] = OPERATION_TEST;
    	tx_buffer[1] = pump;
    	tx_buffer[2] = hours;
    	tx_buffer[3] = minutes;
    	tx_buffer[4] = seconds;
    	tx_buffer[5] = (uint8_t) vacuumScale;	// mTorr
    	tx_buffer[6] = 25;						// temperature in C
    	tx_buffer[7] = (uint8_t) flowRate;		// L/min
    	tx_buffer_size = 8;
    	break;
    case ULTIMATE_MEASURE_TEST:
    	tx_buffer[0] = ULTIMATE_MEASURE_TEST;
    	tx_buffer[1] = pump;
    	tx_buffer[2] = hours;
    	tx_buffer[3] = minutes;
    	tx_buffer[4] = seconds;
    	tx_buffer[5] = (uint8_t) vacuumScale;	// mTorr
    	tx_buffer[6] = 25;						// temperature in C
    	tx_buffer[7] = (uint8_t) flowRate;		// L/min
    	tx_buffer_size = 8;
    	break;
    case IDLE:
    	tx_buffer[0] = IDLE;
    	tx_buffer[1] = pump;
    	tx_buffer[2] = hours;
    	tx_buffer[3] = minutes;
    	tx_buffer[4] = seconds;
    	tx_buffer[5] = (uint8_t) vacuumScale;	// mTorr
    	tx_buffer[6] = 25;						// temperature in C
    	tx_buffer[7] = (uint8_t) flowRate;		// L/min
    	tx_buffer_size = 8;
    	break;
    case FAIL_STATE:
    	tx_buffer[0] = FAIL_STATE;
    	tx_buffer[1] = pump;
    	tx_buffer[2] = hours;
    	tx_buffer[3] = minutes;
    	tx_buffer[4] = seconds;
    	tx_buffer[5] = (uint8_t) vacuumScale;	// mTorr
    	tx_buffer[6] = 25;						// temperature in C
    	tx_buffer[7] = (uint8_t) flowRate;		// L/min
    	tx_buffer_size = 8;
    default:
    	break;

    }
    HAL_UART_Transmit_IT(&huart3, (uint8_t*)tx_buffer, tx_buffer_size);
    osDelay(1000);
  }
  /* USER CODE END StartTask02 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if(htim == &htim10) { // This a timer period = 1 sec
  		seconds++;
  		if(seconds == 60){
  			seconds = 0;
  			minutes++;
  			if(minutes == 60){
  				minutes = 0;
  				hours++;
  			}
  		}
  	}
  if(htim->Instance == TIM7) { // This is for PWM
	  currPos = toggleCount/2;
	if(steps != currPos || (toggleCount%2) != 0){
		if(steps > currPos ){
			HAL_GPIO_WritePin(dirGroup, dirPin, RESET);
			toggleCount++;
		}
		else {
			HAL_GPIO_WritePin(dirGroup, dirPin, SET);
			toggleCount--;
		}
		HAL_GPIO_TogglePin(pulGroup, pulPin);
		HAL_TIM_Base_Start_IT(&htim7);
	}else{	// Current Position == Steps
		HAL_TIM_Base_Stop_IT(&htim7);
	}
	 //HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_5);
  }

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
