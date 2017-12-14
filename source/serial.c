#include "include/serial.h"

static void Serial_InitDefault()
{
  USART_InitTypeDef USART_Init_Default;
  NVIC_InitTypeDef NVIC_USART;
  DMA_InitTypeDef DMA_USART;
  GPIO_InitTypeDef GPIO_USART;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

  //Configure Usart Init Structure
  USART_Init_Default.USART_BaudRate=9600;
  USART_Init_Default.USART_WordLength=USART_WordLength_8b;
  USART_Init_Default.USART_StopBits=USART_StopBits_1;
  USART_Init_Default.USART_Parity=USART_Parity_Even;
  USART_Init_Default.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
  USART_Init_Default.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;

  //Configure NVIC Init Structure
  NVIC_USART.NVIC_IRQChannel=USART3_IRQn;
  NVIC_USART.NVIC_IRQChannelCmd=ENABLE;
  NVIC_USART.NVIC_IRQChannelPreemptionPriority=1;
  NVIC_USART.NVIC_IRQChannelSubPriority=1;

  //Config GPIO Init Structure
  GPIO_USART.GPIO_Pin=GPIO_Pin_10;
  GPIO_USART.GPIO_Mode=GPIO_Mode_AF;
  GPIO_USART.GPIO_Speed=GPIO_Fast_Speed;
  GPIO_USART.GPIO_OType=GPIO_OType_PP;
  GPIO_USART.GPIO_PuPd=GPIO_PuPd_UP;

  GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);

  GPIO_Init(GPIOB,&GPIO_USART);
  GPIO_USART.GPIO_Pin=GPIO_Pin_11;

  GPIO_Init(GPIOB,&GPIO_USART);

  NVIC_Init(&NVIC_USART);

  USART_Init(USART3,&USART_Init_Default);
  USART_Cmd(USART3,ENABLE);

}
