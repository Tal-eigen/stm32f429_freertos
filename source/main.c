#include "main.h"
#include <string.h>

uint32_t ticksms;
uint32_t ticksled;

char aRxBuffer[RXBUFFERSIZE];
char parsebuffer[PARSESIZE];

void randominit()
{
  parsebuffer[0]=0;
  int i;
  for(i=1;i<PARSESIZE;i++)
  {
    if(i%10==0)
    {
      parsebuffer[i]='O';
      parsebuffer[i+1]='K';
      ++i;
    }
    else
    {
      parsebuffer[i]=i;
    }

  }
}
volatile size_t writes,reads,parse_iter,write_length;

bool reset,sync,synced;

SemaphoreHandle_t parse;


extern void USART3DMA_Config();
extern void USART6_Config();

void  *memcpy(void *dest, const void *src, size_t count)
{
  /* This would be a prime candidate for reimplementation in assembly */
  char *in_src = (char*)src;
  char *in_dest = (char*)dest;

  while(count--)
    *in_dest++ = *in_src++;
  return dest;
}

static void Putc_ax2550(volatile char c)
{
  if(USART3->CR1 & USART_CR1_UE)
  {
    USART3->DR = (uint16_t)(c & 0x01FF);
    while(!(USART3->SR & USART_FLAG_TXE));
  }
}

static void Putc_term(volatile char c)
{
  if(USART6->CR1 & USART_CR1_UE)
  {
    USART6->DR = (uint16_t)(c & 0x01FF);
    while(!(USART6->SR & USART_FLAG_TXE));
  }
}
static void Puts_term(volatile char* c)
{
  while(*c != 0)
    Putc_term(*c++);
}
void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3,USART_IT_IDLE))
  {
    volatile uint32_t dummy;
    dummy=USART3->SR;
    dummy=USART3->DR;
 
    DMA1_Stream1->CR &= ~DMA_SxCR_EN;
  }
}

void USART6_IRQHandler(void)
{
  if(USART_GetITStatus(USART6,USART_IT_TC))
    USART_ClearITPendingBit(USART6,USART_IT_TC);
  if(USART_GetITStatus(USART6,USART_IT_RXNE))
  {
    char c=USART_ReceiveData(USART6);
    if(c=='R' || c=='r')
      reset=1;
    else if(c=='S' || c=='s')
      sync=1;
  }
}

void DMA1_Stream1_IRQHandler(void)
{
  volatile size_t len,copyindex;
  uint8_t* ptr;
  if(DMA1->LISR & DMA_FLAG_TCIF1)
    DMA1->LIFCR = DMA_FLAG_TCIF1;
  len = (uint32_t)RXBUFFERSIZE - DMA1_Stream1->NDTR;
  copyindex=(uint32_t)PARSESIZE - writes;

  if(copyindex > len)
    copyindex=len;
  else if(copyindex < len)
  {
    writes=0;
    copyindex=len;
  }
  ptr=aRxBuffer;
  memcpy(&parsebuffer[writes],ptr,len);

  writes+=copyindex;
  write_length += len;
  len-=copyindex;
  ptr+=copyindex;
  BaseType_t xHigherPriorityTaskWoken;

  xHigherPriorityTaskWoken=pdFALSE;

  xSemaphoreGiveFromISR(parse,&xHigherPriorityTaskWoken);

  DMA1->LIFCR=DMA_FLAG_DMEIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1;
  DMA1_Stream1->M0AR=(uint32_t)aRxBuffer;
  DMA1_Stream1->NDTR=(uint32_t)RXBUFFERSIZE;
  DMA1_Stream1->CR |= DMA_SxCR_EN;



  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

GPIO_InitTypeDef GPIOG_Init;
void prvGetRegistersFromStack(uint32_t* pulFaultStackAddress)
{
  volatile uint32_t r0;
  volatile uint32_t r1;
  volatile uint32_t r2;
  volatile uint32_t r3;
  volatile uint32_t r12;
  volatile uint32_t lr;
  volatile uint32_t pc;
  volatile uint32_t psr;

  r0=pulFaultStackAddress[0];
  r1=pulFaultStackAddress[1];
  r2=pulFaultStackAddress[2];
  r3=pulFaultStackAddress[3];

  r12=pulFaultStackAddress[4];
  lr=pulFaultStackAddress[5];
  pc=pulFaultStackAddress[6];
  psr=pulFaultStackAddress[7];

  for(;;);
}

static int BufferCmp(char* pBuffer1,char* pBuffer2,size_t pBufferLength)
{
  while(pBufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {

      return -1;
    }
    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}


size_t str_len(const char* s)
{
  size_t i;
  while(*s != 0)
  {
    ++s;
    ++i;
  }
  return i;
}

static void* BufferSearch(char* bf1,size_t size,char* bf2,size_t length)
{
  const unsigned char* haystack=(const unsigned char*)bf1;
  const unsigned char* needle=(const unsigned char*)bf2;

  if(length==0)
    return 0;
  while(length <= size)
  {
    if(!BufferCmp(haystack,needle,length))
      return (void*) haystack;
    haystack++;
    size--;
  }
  return NULL;
}


void GPIO_Config()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
  GPIOG_Init.GPIO_Pin=GPIO_Pin_14 | GPIO_Pin_13;
  GPIOG_Init.GPIO_Mode=0x01;
  GPIOG_Init.GPIO_Speed=0x02;
  GPIOG_Init.GPIO_OType=0x00;
  GPIOG_Init.GPIO_PuPd=GPIO_PuPd_UP;

  GPIO_Init(GPIOG,&GPIOG_Init);

}


void vTask_parse(void* pvParameters)
{
  const TickType_t MaxBlock =pdMS_TO_TICKS(20);
  char comm[2]={'O','K'};
  for(;;)
  {
    //GPIOG->ODR ^= GPIO_Pin_13;
    if(xSemaphoreTake(parse,MaxBlock)==pdPASS)
    {

      if(!synced)
      {
        __disable_irq();
        if(!BufferSearch(&parsebuffer[0],PARSESIZE,comm,2))
        {
          Putc_ax2550('\r');
          //GPIOG->ODR ^= GPIO_Pin_13;
        }
        else
        {
          Puts_term("Synced\r\n");
          synced=1;
          GPIOG->ODR ^= GPIO_Pin_13;
        }
        __enable_irq();
      }
    }

  }
}

void vTask2(void* pvParameters)
{
  for(;;)
  {
    GPIOG->ODR ^= GPIO_Pin_14;
    while(reads!=writes)
    {
      Putc_term(parsebuffer[reads++]);
      if(reads>PARSESIZE)
        reads=0;
    }
    vTaskDelay(pdMS_TO_TICKS(30));
  }
}

int main()
{
  parse=xSemaphoreCreateBinary();
  GPIO_Config();
  synced=0;
  USART6_Config();
  USART3DMA_Config();

  xTaskCreate(vTask_parse,"parse",256,NULL,1,NULL);
  xTaskCreate(vTask2,"vTask2",100,NULL,1,NULL);

  vTaskStartScheduler();

  for(;;);
}

void vApplicationTickHook(void) {
}

/* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created.  It is also called by various parts of the
   demo application.  If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
void vApplicationMallocFailedHook(void) {
  taskDISABLE_INTERRUPTS();
  for(;;);
}

/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
   task.  It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()).  If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
void vApplicationIdleHook(void) {
}

void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName) {
  (void) pcTaskName;
  (void) pxTask;
  /* Run time stack overflow checking is performed if
     configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     function is called if a stack overflow is detected. */
  taskDISABLE_INTERRUPTS();
  for(;;);
}
