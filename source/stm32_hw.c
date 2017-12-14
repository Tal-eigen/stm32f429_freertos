#include "main.h"

extern volatile uint8_t aRxBuffer[RXBUFFERSIZE];
extern volatile uint8_t parsebuffer[PARSESIZE];

void USART3DMA_Config()
{
   USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);


   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed=GPIO_Fast_Speed;
   GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;

   GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
   GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);

   GPIO_Init(GPIOB, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

    //USART_OverSampling8Cmd(USART1, ENABLE);

   USART_InitStructure.USART_BaudRate=9600;
   USART_InitStructure.USART_WordLength=USART_WordLength_8b;
   USART_InitStructure.USART_StopBits=USART_StopBits_1;
   USART_InitStructure.USART_Parity=USART_Parity_Even;
   USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;

   NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
   //NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;

   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

   NVIC_Init(&NVIC_InitStructure);


   USART_Init(USART3, &USART_InitStructure);


   USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);

   DMA_StructInit(&DMA_InitStructure);

   DMA_InitStructure.DMA_Channel=DMA_Channel_4;
   DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&USART3->DR;
   DMA_InitStructure.DMA_Memory0BaseAddr=(uint32_t)aRxBuffer;
   DMA_InitStructure.DMA_BufferSize=(uint32_t)RXBUFFERSIZE;
   DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
   DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
   DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
   DMA_InitStructure.DMA_Priority=DMA_Priority_High;
   DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Disable;
   DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;
   DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_3QuartersFull;
   DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single;
   DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;

   NVIC_InitStructure.NVIC_IRQChannel=DMA1_Stream1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
   //NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
   NVIC_Init(&NVIC_InitStructure);

   DMA_Init(DMA1_Stream1,&DMA_InitStructure);
   DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);
   DMA_Cmd(DMA1_Stream1,ENABLE);
   USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
   USART_Cmd(USART3,ENABLE);
}

void USART6_Config()
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  USART_InitStructure.USART_BaudRate=9600;
  USART_InitStructure.USART_WordLength=USART_WordLength_8b;
  USART_InitStructure.USART_StopBits=USART_StopBits_1;
  USART_InitStructure.USART_Parity=USART_Parity_Even;
  USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed=GPIO_Low_Speed;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;


  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);

  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
  GPIO_Init(GPIOC,&GPIO_InitStructure);

  USART_Init(USART6,&USART_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel=USART6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;

  NVIC_Init(&NVIC_InitStructure);

  USART_Cmd(USART6,ENABLE);
  USART_ITConfig(USART6,USART_IT_TC,ENABLE);
  USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
}
