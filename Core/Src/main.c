/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RX_BUFFER_SIZE 32
#define FLAG_1 (1U << 0)
#define FLAG_2 (1U << 1)
#define FLAG_3 (1U << 2)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Led_Handler */
osThreadId_t Led_HandlerHandle;
const osThreadAttr_t Led_Handler_attributes = {
  .name = "Led_Handler",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Message_Sender */
osThreadId_t Message_SenderHandle;
const osThreadAttr_t Message_Sender_attributes = {
  .name = "Message_Sender",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Message_receive */
osThreadId_t Message_receiveHandle;
const osThreadAttr_t Message_receive_attributes = {
  .name = "Message_receive",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myQueue01 */
osMessageQueueId_t myQueue01Handle;
const osMessageQueueAttr_t myQueue01_attributes = {
  .name = "myQueue01"
};
/* Definitions for myTimer01 */
osTimerId_t myTimer01Handle;
const osTimerAttr_t myTimer01_attributes = {
  .name = "myTimer01"
};
/* Definitions for ReceiverSem */
osSemaphoreId_t ReceiverSemHandle;
const osSemaphoreAttr_t ReceiverSem_attributes = {
  .name = "ReceiverSem"
};
/* Definitions for LedEvent */
osEventFlagsId_t LedEventHandle;
const osEventFlagsAttr_t LedEvent_attributes = {
  .name = "LedEvent"
};
/* Definitions for SenderEvent */
osEventFlagsId_t SenderEventHandle;
const osEventFlagsAttr_t SenderEvent_attributes = {
  .name = "SenderEvent"
};
/* USER CODE BEGIN PV */
char* Commands[]={
"LED ON\n","LED OFF\n","TOGGLE LED \n ","ECHO \n","HELP"
};
char* messages[]={
"HELLO\n","YOU CAN TAP HELP TO SEE THE COMMANDS \n"
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
void StartDefaultTask(void *argument);
void LED(void *argument);
void Sender(void *argument);
void Receiver(void *argument);
void Callback01(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char RxBuffer[RX_BUFFER_SIZE];
char RxChar;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
if(huart->Instance==USART2){
RxBuffer[0]=RxChar;

osSemaphoreRelease(ReceiverSemHandle);
}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_USART2_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of ReceiverSem */
  ReceiverSemHandle = osSemaphoreNew(1, 0, &ReceiverSem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of myTimer01 */
  myTimer01Handle = osTimerNew(Callback01, osTimerOnce, NULL, &myTimer01_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myQueue01 */
  myQueue01Handle = osMessageQueueNew (16, sizeof(char*), &myQueue01_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Led_Handler */
  Led_HandlerHandle = osThreadNew(LED, NULL, &Led_Handler_attributes);

  /* creation of Message_Sender */
  Message_SenderHandle = osThreadNew(Sender, NULL, &Message_Sender_attributes);

  /* creation of Message_receive */
  Message_receiveHandle = osThreadNew(Receiver, NULL, &Message_receive_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of LedEvent */
  LedEventHandle = osEventFlagsNew(&LedEvent_attributes);

  /* creation of SenderEvent */
  SenderEventHandle = osEventFlagsNew(&SenderEvent_attributes);

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
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
    osDelay(1000);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_LED */
/**
* @brief Function implementing the Led_Handler thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED */
void LED(void *argument)
{
  /* USER CODE BEGIN LED */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
  /* Infinite loop */
  for(;;)
  {uint32_t flags=osEventFlagsWait(LedEventHandle, FLAG_1|FLAG_2|FLAG_3, osFlagsWaitAny, osWaitForever);
     osTimerStop(myTimer01Handle);//if the previous command was a toggle we need to stop the timer
  if(flags&FLAG_1) //led on
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
  else if(flags&FLAG_2) //led off
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
  else if(flags&FLAG_3)//start timer and toggle led
	  osTimerStart(myTimer01Handle,1000);
  osEventFlagsClear(LedEventHandle, flags);



  }
  /* USER CODE END LED */
}

/* USER CODE BEGIN Header_Sender */
/**
* @brief Function implementing the Message_Sender thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Sender */
void Sender(void *argument)
{
  /* USER CODE BEGIN Sender */
  /* Infinite loop */
  HAL_UART_Transmit(&huart2, messages[0], strlen(messages[0]), 10);
  HAL_UART_Transmit(&huart2, messages[1], strlen(messages[1]), 10);
  for(;;)
  {uint32_t flags=osEventFlagsWait(SenderEventHandle, FLAG_1|FLAG_2,osFlagsWaitAny, osWaitForever);
  if(flags&FLAG_1){
	  char* c;
	  osMessageQueueGet(myQueue01Handle, &c, 0, osWaitForever);
	  HAL_UART_Transmit(&huart2, c, strlen(c), 10);

  }
  else if(flags&FLAG_2) //send possible commands
  { HAL_UART_Transmit(&huart2, Commands[0], strlen(Commands[0]), 10);
  HAL_UART_Transmit(&huart2, Commands[1], strlen(Commands[1]), 10);
  HAL_UART_Transmit(&huart2, Commands[2], strlen(Commands[2]), 10);
  HAL_UART_Transmit(&huart2, Commands[3], strlen(Commands[3]), 10);
  }
  osEventFlagsClear(LedEventHandle, flags);
  }

  /* USER CODE END Sender */
}

/* USER CODE BEGIN Header_Receiver */
/**
* @brief Function implementing the Message_receive thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Receiver */
void Receiver(void *argument)
{
  /* USER CODE BEGIN Receiver */
   HAL_UART_Receive_IT(&huart2, &RxChar, 1);

  /* Infinite loop */
  for(;;)
  {uint8_t i=0;
  osSemaphoreAcquire(ReceiverSemHandle, osWaitForever);
  HAL_StatusTypeDef status;
  do {
      status = HAL_UART_Receive(&huart2, (uint8_t *)&RxChar, 1, 10);
      if (status == HAL_OK && i < RX_BUFFER_SIZE - 1) {
    	  i++;
          RxBuffer[i] = RxChar;
      }
  } while (status == HAL_OK && i < RX_BUFFER_SIZE - 1);
  RxBuffer[i+1] = '\0';
  if(strncmp(RxBuffer,Commands[0],6)==0)//led on
	  osEventFlagsSet(LedEventHandle, FLAG_1);
  else if(strncmp(RxBuffer,Commands[1],7)==0)//led off
	  osEventFlagsSet(LedEventHandle, FLAG_2);
  else if(strncmp(RxBuffer,Commands[2],10)==0)//toggle led
	  osEventFlagsSet(LedEventHandle, FLAG_3);
  else if((strncmp(RxBuffer,Commands[3],4)==0))//echo
  { osEventFlagsSet(SenderEventHandle, FLAG_1);
  	  char* c=(char*)malloc((i-4)*sizeof(char));
  	  strncpy(c,RxBuffer+5,i-4);
  	  c[i-4]='\0';
       osMessageQueuePut(myQueue01Handle, &c, 0, 0);}//if we received echo the custom message start from the index 5

  else if(strcmp(RxBuffer,Commands[4])==0) //help
	  osEventFlagsSet(SenderEventHandle,FLAG_2);
  HAL_UART_Receive_IT(&huart2, &RxChar, 1);//restart the receiving
  /* USER CODE END Receiver */
}}

/* Callback01 function */
void Callback01(void *argument)
{
  /* USER CODE BEGIN Callback01 */
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	osTimerStart(myTimer01Handle, 1000);
  /* USER CODE END Callback01 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

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
#ifdef USE_FULL_ASSERT
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
