/**
  ******************************************************************************
  * @file    Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to configure the CAN peripheral
  *          to send and receive CAN frames in normal mode. The sent frames
  *          are used to control Leds by pressing Tamper push-button.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Structs.h"
#include "Open_SAE_J1939.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY_PRESSED     0x01
#define KEY_NOT_PRESSED 0x00
#define BROADCAST		1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t ubKeyNumber = 0x0;
CAN_HandleTypeDef     CanHandle;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;
J1939 j1939         = {0};
uint8_t               CurrentLed = 0;
volatile uint16_t 	  bytes_sent = 0;
volatile uint8_t      current_package = 1;
#ifdef BROADCAST
uint8_t DA = 0xFF;
#else
uint8_t DA = 128;
#endif

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);
static void CAN_Config(void);
static void LED_Display(uint8_t LedStatus);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
#ifdef BROADCAST
	j1939.this_ecu_tp_cm.control_byte = CONTROL_BYTE_TP_CM_BAM;
	  j1939.this_ecu_tp_cm.PGN_of_the_packeted_message = 0xF0FF;
#else
  j1939.this_ecu_tp_cm.control_byte = CONTROL_BYTE_TP_CM_RTS;
  j1939.this_ecu_tp_cm.PGN_of_the_packeted_message = 0xD080;
#endif
  j1939.this_ecu_tp_cm.total_message_size = 50;
  j1939.this_ecu_tp_cm.number_of_packages = 8;
  j1939.this_ecu_tp_cm.from_ecu_address = 0;
  for (uint8_t i = 0; i < j1939.this_ecu_tp_cm.total_message_size; i++){
    j1939.this_ecu_tp_dt.data[i] = i;
  }
  j1939.this_ecu_tp_dt.sequence_number = 0;
  j1939.this_ecu_tp_dt.from_ecu_address = 0;

  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  
  /* STM32F7xx HAL library initialization:
       - Configure the Flash ART accelerator on ITCM interface
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 216 MHz */
  SystemClock_Config();

  /* Configure LED1, LED2, LED3 and LED4 */
#ifdef STM32F723xx
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_GPIO);
#else
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Configure Tamper push-button */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);
#endif

  /* Configure the CAN peripheral */
  CAN_Config();

  /* Infinite loop */

  /* Load your ECU information */
  Open_SAE_J1939_Startup_ECU(&CanHandle, &TxMailbox, &j1939);

  while(1) {

	/* Your application code here */
	HAL_Delay(5000);

	SAE_J1939_Send_Transport_Protocol_Connection_Management(&CanHandle, &TxMailbox, &j1939, DA);
#ifdef BROADCAST
	bytes_sent = 0;
	current_package = 1;
	HAL_Delay(50);
    SAE_J1939_Send_Transport_Protocol_Data_Transfer(&CanHandle, &TxMailbox, &j1939, DA);
	HAL_Delay(50);
    SAE_J1939_Send_Transport_Protocol_Data_Transfer(&CanHandle, &TxMailbox, &j1939, DA);
	HAL_Delay(50);
    SAE_J1939_Send_Transport_Protocol_Data_Transfer(&CanHandle, &TxMailbox, &j1939, DA);
	HAL_Delay(50);
    SAE_J1939_Send_Transport_Protocol_Data_Transfer(&CanHandle, &TxMailbox, &j1939, DA);
	HAL_Delay(50);
    SAE_J1939_Send_Transport_Protocol_Data_Transfer(&CanHandle, &TxMailbox, &j1939, DA);
	HAL_Delay(50);
    SAE_J1939_Send_Transport_Protocol_Data_Transfer(&CanHandle, &TxMailbox, &j1939, DA);
	HAL_Delay(50);
    SAE_J1939_Send_Transport_Protocol_Data_Transfer(&CanHandle, &TxMailbox, &j1939, DA);
	HAL_Delay(50);
    SAE_J1939_Send_Transport_Protocol_Data_Transfer(&CanHandle, &TxMailbox, &j1939, DA);
#endif
  }      
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock sstartource            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            PLL_R                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef  ret = HAL_OK;
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
#ifndef STM32F723xx
  RCC_OscInitStruct.PLL.PLLR = 7;
#endif
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 216 MHz Frequency */  
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2; 
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }  
}

/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
static void CAN_Config(void)
{
  CAN_FilterTypeDef  sFilterConfig;

  /*##-1- Configure the CAN peripheral #######################################*/
  CanHandle.Instance = CANx;

  CanHandle.Init.TimeTriggeredMode = DISABLE;
  CanHandle.Init.AutoBusOff = DISABLE;
  CanHandle.Init.AutoWakeUp = DISABLE;
  CanHandle.Init.AutoRetransmission = ENABLE;
  CanHandle.Init.ReceiveFifoLocked = DISABLE;
  CanHandle.Init.TransmitFifoPriority = DISABLE;
  CanHandle.Init.Mode = CAN_MODE_NORMAL;
  CanHandle.Init.SyncJumpWidth = CAN_SJW_1TQ;
  CanHandle.Init.TimeSeg1 = CAN_BS1_10TQ; // Orig=6TQ, New=8TQ
  CanHandle.Init.TimeSeg2 = CAN_BS2_7TQ; // Orig=2TQ, New=7TQ
  CanHandle.Init.Prescaler = 12; // Orig=6, New=4

  if (HAL_CAN_Init(&CanHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the CAN Filter ###########################################*/
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;

  if (HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig) != HAL_OK)
  {
    /* Filter configuration Error */
    Error_Handler();
  }

  /*##-3- Start the CAN peripheral ###########################################*/
  if (HAL_CAN_Start(&CanHandle) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /*##-4- Activate CAN RX notification #######################################*/
  if (HAL_CAN_ActivateNotification(&CanHandle, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
  {
    /* Notification Error */
    Error_Handler();
  }

  /*##-5- Configure Transmission process #####################################*/
  TxHeader.StdId = 0x321;
  TxHeader.ExtId = 0x01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 2;
  TxHeader.TransmitGlobalTime = DISABLE;
}

/**
 *
 */
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	if (bytes_sent < j1939.this_ecu_tp_cm.total_message_size)
	{
		SAE_J1939_Send_Transport_Protocol_Data_Transfer(&CanHandle, &TxMailbox, &j1939, DA);
	}
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	if (bytes_sent < j1939.this_ecu_tp_cm.total_message_size)
	{
		SAE_J1939_Send_Transport_Protocol_Data_Transfer(&CanHandle, &TxMailbox, &j1939, DA);
	}
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	if (bytes_sent < j1939.this_ecu_tp_cm.total_message_size)
	{
		SAE_J1939_Send_Transport_Protocol_Data_Transfer(&CanHandle, &TxMailbox, &j1939, DA);
	}
}

/**
  * @brief  Rx Fifo 0 message pending callback
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

  /* Read incoming message */
  if (Open_SAE_J1939_Listen_For_Messages(hcan, &TxMailbox, &j1939))
  {
	  // Got a new message
	  CurrentLed = (CurrentLed + 1) % 2;
#ifdef STM32F723xx
	  if (CurrentLed == 0){
		  LED_Display(5);
	  }
	  else {
		  LED_Display(6);
	  }
#else
	  LED_Display(1);
#endif
  }
  else
  {
	  LED_Display(0);
  }

}

/**
  * @brief  Turns ON/OFF the dedicated LED.
  * @param  LedStatus: LED number from 0 to 3
  * @retval None
  */
void LED_Display(uint8_t LedStatus)
{

#ifdef STM32F723xx
  /* Turn OFF all LEDs */
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);

  switch(LedStatus)
  {
    case (5):
      /* Turn ON LED5 */
      BSP_LED_On(LED5);
      break;

    case (6):
      /* Turn ON LED6 */
      BSP_LED_On(LED6);
      break;
    default:
      break;
  }

#else

  BSP_LED_Off(LED1);
  BSP_LED_Off(LED2);
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);

  switch(LedStatus)
  {
    case (1):
      /* Turn ON LED1 */
      BSP_LED_On(LED1);
      break;

    case (2):
      /* Turn ON LED2 */
      BSP_LED_On(LED2);
      break;

    case (3):
      /* Turn ON LED3 */
      BSP_LED_On(LED3);
      break;

    case (4):
      /* Turn ON LED4 */
      BSP_LED_On(LED4);
      break;
    default:
      break;
  }
#endif
}


/**
  * @brief  Configure the MPU attributes
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

