/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "crc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "socket.h"
#include "dhcp.h"
#include "httpServer.h"
#include "W25Qxx.h"
#include "html_pages.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DHCP_SOCKET     0
#define DNS_SOCKET      1
#define HTTP_SOCKET     2
#define SOCK_TCPS       0
#define SOCK_UDPS       1
#define PORT_TCPS       5000
#define PORT_UDPS       3000
#define MAX_HTTPSOCK    6

#define _DHCP_DEBUG_
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t socknumlist[] = {2, 3, 4, 5, 6, 7};
uint8_t RX_BUF[1024];
uint8_t TX_BUF[1024];
wiz_NetInfo net_info = {
    .mac  = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED },
    .dhcp = NETINFO_DHCP
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int cek_LAN(){
	uint8_t tmp;
	ctlwizchip(CW_GET_PHYLINK, (void*) &tmp);
	if(tmp == PHY_LINK_OFF) {
		return -1;
	}else{
		return 0;
	}
}

void wizchipSelect(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
//    LL_GPIO_ResetOutputPin (GPIOB, LL_GPIO_PIN_1);
}

void wizchipUnselect(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
//    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);

}

void wizchipReadBurst(uint8_t* buff, uint16_t len) {
    HAL_SPI_Receive(&hspi2, buff, len, HAL_MAX_DELAY);
}

void wizchipWriteBurst(uint8_t* buff, uint16_t len) {
    HAL_SPI_Transmit(&hspi2, buff, len, HAL_MAX_DELAY);
}

uint8_t wizchipReadByte(void) {
    uint8_t byte;
    wizchipReadBurst(&byte, sizeof(byte));
    return byte;
}

void wizchipWriteByte(uint8_t byte) {
    wizchipWriteBurst(&byte, sizeof(byte));
}

volatile bool ip_assigned = false;

void Callback_IPAssigned(void) {
    ip_assigned = true;
}

void Callback_IPConflict(void) {
    ip_assigned = false;
}

uint8_t dhcp_buffer[1024];
uint8_t dns_buffer[1024];

void W5500Init() {
//	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
//	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_2);
	HAL_Delay(2500);
    // Register W5500 callbacks
    reg_wizchip_cs_cbfunc(wizchipSelect, wizchipUnselect);
    reg_wizchip_spi_cbfunc(wizchipReadByte, wizchipWriteByte);
    reg_wizchip_spiburst_cbfunc(wizchipReadBurst, wizchipWriteBurst);

    uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2};
    wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);

    // set MAC address before using DHCP
    setSHAR(net_info.mac);
    DHCP_init(DHCP_SOCKET, dhcp_buffer);

    reg_dhcp_cbfunc(
        Callback_IPAssigned,
        Callback_IPAssigned,
        Callback_IPConflict
    );
    int a = cek_LAN();
    printf("sini...%i\n", a);
    uint32_t ctr = 1000;
    while((!ip_assigned) && (ctr > 0)) {
        DHCP_run();
        ctr--;
    }

    if(!ip_assigned) {

        return;
        NVIC_SystemReset();
    }

    getIPfromDHCP(net_info.ip);
    getGWfromDHCP(net_info.gw);
    getSNfromDHCP(net_info.sn);

    char charData[200]; // Data holder
    sprintf(charData,"IP:  %d.%d.%d.%d\r\nGW:  %d.%d.%d.%d\r\nNet: %d.%d.%d.%d\r\n",
        net_info.ip[0], net_info.ip[1], net_info.ip[2], net_info.ip[3],
        net_info.gw[0], net_info.gw[1], net_info.gw[2], net_info.gw[3],
        net_info.sn[0], net_info.sn[1], net_info.sn[2], net_info.sn[3]
    );
    HAL_UART_Transmit(&huart1,(uint8_t *)charData,strlen(charData),1000);

    wizchip_setnetinfo(&net_info);

}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
  printf("mulai..\n");
  W25Q_Reset();
  printf("sampai sini..\n");
	uint32_t ID = W25Q_ReadID();

	if (ID) {
		printf("masuk.. 0x%08X\n", ID);
	}
  W5500Init();
	httpServer_init(TX_BUF, RX_BUF, MAX_HTTPSOCK, socknumlist);
	reg_httpServer_cbfunc(NVIC_SystemReset, NULL);
	reg_httpServer_webContent((uint8_t*) "index.html", (uint8_t*) upload_page);
	reg_httpServer_flashContent((uint8_t*) "upload.html",	0x001000);
//	reg_httpServer_flashContent((uint8_t*) "test.html",		0x022000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  for(int i = 0; i < MAX_HTTPSOCK; i++) httpServer_run(i); // HTTP Server handler
	  static uint32_t lsRn = 0;
	  uint32_t tmr = HAL_GetTick();
	  if((tmr-lsRn)>500){
		  lsRn = tmr;
		  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	  }
//	  printf("hallo..\n");
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

/* USER CODE BEGIN 4 */
void UWriteData(const char data)
{
	while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)==RESET);

	huart1.Instance->DR=data;

}

int __io_putchar(int ch)
{
	UWriteData(ch);
	return ch;
}
/* USER CODE END 4 */

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
