#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>
#include <string.h>

GPIO_InitTypeDef GPIOAyar;    
USART_InitTypeDef USARTAyar;
NVIC_InitTypeDef NVICAyar;

uint32_t zaman;
char text[50];
char gonder[50];
int i=0;

void rccConfig(void)
{
			RCC_HSICmd(DISABLE);
			RCC_HSEConfig(RCC_HSE_ON);
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
			RCC_PCLK1Config(RCC_HCLK_Div2);
			RCC_PCLK2Config(RCC_HCLK_Div1);
			RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
			RCC_PLLCmd(ENABLE);
}
void gpioConfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//PA9->TX------------------------------------------------
	GPIOAyar.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOAyar.GPIO_Pin=GPIO_Pin_9;
	GPIOAyar.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOAyar);
	//PA10->RX-----------------------------------------------
	GPIOAyar.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIOAyar.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIOAyar);
	//LEDLER-------------------------------------------------
	GPIOAyar.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOAyar.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIOAyar.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOAyar);
}

void usartConfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USARTAyar.USART_BaudRate=9600; 
	USARTAyar.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USARTAyar.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USARTAyar.USART_Parity=USART_Parity_No;
	USARTAyar.USART_StopBits=USART_StopBits_1;
	USARTAyar.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USARTAyar);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //Data register meşgul interruptı
	USART_Cmd(USART1,ENABLE);
}
void NVIC_Config(void)
{
	NVICAyar.NVIC_IRQChannel=USART1_IRQn;   //Kanal tanımlaması
	NVICAyar.NVIC_IRQChannelCmd=ENABLE;
	NVICAyar.NVIC_IRQChannelPreemptionPriority=0;  //Öncelik ayarlamaları
	NVICAyar.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVICAyar);
}
void USART1_IRQHandler(void)      //interrupt
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)) //interrupt kontrolü
	{
		char bellek=USART_ReceiveData(USART1);    //Usart1 den veri okuma
		if(bellek != '\n'&& i<50)                 
		{
			text[i]=bellek;
			i++;
		}
		else
			i=0;
	}
}
void usartTransmit(char *string)
{
	while(*string)
	{
		while(!(USART1->SR & 0x00000040)); // veri gönderimi bitmeden sonraki kısma geçmemesi için
		USART_SendData(USART1,*string);
		string++;
	}
}


void delay_ms(uint32_t sayac)
{
	zaman=sayac;
	while(zaman);
}
void SysTick_Handler()
{
	if(zaman>0)
	{
		zaman--;
	}
}
int main(void)
{
	rccConfig();
	gpioConfig();
	usartConfig();
	NVIC_Config();
	SysTick_Config(SystemCoreClock/1000);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2);


  while (1)
  {
	 if((strcmp(text,"LED1 ON")==0))   //Gelen verinin string biçiminden karşılaştırılması.
	 {
		 GPIO_SetBits(GPIOB,GPIO_Pin_0);
		 sprintf(gonder,"LED1 AKTIF\n");    //Tırnak içindeki yazı gonder'in içine gömüldü
		 usartTransmit(gonder);
		 delay_ms(20);
		 for(int a=0; a<50;a++)             // stringlerin içerisi komple temizlendi
		 	 {
		 		 text[a]='\0';
		 		 gonder[a]='\0';
		 	 }
	 }
	 else if((strcmp(text,"LED2 ON")==0))   //Gelen verinin string biçiminden karşılaştırılması.
	 {
		 GPIO_SetBits(GPIOB,GPIO_Pin_1);
		 sprintf(gonder,"LED2 AKTIF\n");
		 usartTransmit(gonder);
		 delay_ms(20);
		 for(int a=0; a<50;a++)
		 	 {
		 		 text[a]='\0';
		 		 gonder[a]='\0';
		 	 }
	 }
	 else if((strcmp(text,"LED3 ON")==0))   //Gelen verinin string biçiminden karşılaştırılması.
	 {
		 GPIO_SetBits(GPIOB,GPIO_Pin_2);
		 sprintf(gonder,"LED3 AKTIF\n");
		 usartTransmit(gonder);
		 delay_ms(20);
		 for(int a=0; a<50;a++)
		 	 {
		 		 text[a]='\0';
		 		 gonder[a]='\0';
		 	 }
	 }
	 else if((strcmp(text,"LED1 OFF")==0))  //Gelen verinin string biçiminden karşılaştırılması.
	 {
		 GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		 sprintf(gonder,"LED1 PASIF\n");
		 usartTransmit(gonder);
		 delay_ms(20);
		 for(int a=0; a<50;a++)
		 	 {
		 		 text[a]='\0';
		 		 gonder[a]='\0';
		 	 }
	 }
	 else if((strcmp(text,"LED2 OFF")==0))   //Gelen verinin string biçiminden karşılaştırılması.
	 {
		 GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		 sprintf(gonder,"LED2 PASIF\n");
		 usartTransmit(gonder);
		 delay_ms(20);
		 for(int a=0; a<50;a++)
		 	 {
		 		 text[a]='\0';
		 		 gonder[a]='\0';
		 	 }
	 }
	 else if((strcmp(text,"LED3 OFF")==0))  //Gelen verinin string biçiminden karşılaştırılması.
	 {
		 GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		 sprintf(gonder,"LED3 PASIF\n");
		 usartTransmit(gonder);
		 delay_ms(20);
		 for(int a=0; a<50;a++)
		 	 {
		 		 text[a]='\0';
		 		 gonder[a]='\0';
		 	 }
	 }
	 else if((strcmp(text,"LEDLER ON")==0))  //Gelen verinin string biçiminden karşılaştırılması.
		 {
			 GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2);
			 sprintf(gonder,"LEDLER AKTIF\n");
			 usartTransmit(gonder);
			 delay_ms(20);
			 for(int a=0; a<50;a++)
			 	 {
			 		 text[a]='\0';
			 		 gonder[a]='\0';
			 	 }
		 }
	 else if((strcmp(text,"LEDLER OFF")==0))  //Gelen verinin string biçiminden karşılaştırılması.
	 		 {
	 			 GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2);
	 			 sprintf(gonder,"LEDLER PASIF\n");
	 			 usartTransmit(gonder);
	 			 delay_ms(20);
	 			 for(int a=0; a<50;a++)
	 			 	 {
	 			 		 text[a]='\0';
	 			 		 gonder[a]='\0';
	 			 	 }
	 		 }

  }

}
