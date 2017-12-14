#include "stm32f4xx_usart.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
//FreeRTOS specific headers
#include "task.h"
#include "semphr.h"
#include "queue.h"



struct Serial
{
}

static void Serial_InitDefault(); /*Initializes the serial peripheral USART3 (PB10,PB11) 
                                    to communicate with AX2550*/


