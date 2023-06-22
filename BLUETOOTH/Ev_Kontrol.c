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
int i=0,kosul=0;

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
	//PA9->TX-------------------------------------------------
	GPIOAyar.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOAyar.GPIO_Pin=GPIO_Pin_9;
	GPIOAyar.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOAyar);
	//PA10->RX-----------------------------------------------
	GPIOAyar.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIOAyar.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIOAyar);
	//LEDLER VE MOTOR KABLOLARI------------------------------
	GPIOAyar.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOAyar.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_8 |GPIO_Pin_9|GPIO_Pin_5|GPIO_Pin_6;
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
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //Data register not empty interrupt
	USART_Cmd(USART1,ENABLE);
}
void NVIC_Config(void)
{
	NVICAyar.NVIC_IRQChannel=USART1_IRQn;
	NVICAyar.NVIC_IRQChannelCmd=ENABLE;
	NVICAyar.NVIC_IRQChannelPreemptionPriority=0;
	NVICAyar.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVICAyar);
}
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		char bellek=USART_ReceiveData(USART1);
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

void evkontrol(int kontrol)
{
	switch(kontrol)
	{
	case 1:
		 GPIO_SetBits(GPIOB,GPIO_Pin_0);
		 sprintf(gonder,"SALON LAMBA1 AKTIF\n");
		 usartTransmit(gonder);
		 delay_ms(20);
		 for(int a=0; a<50;a++)
		 	 {
				 text[a]='\0';
				 gonder[a]='\0';
			 }
		 break;
	case 2:
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		sprintf(gonder,"SALON LAMBA2 AKTIF\n");
		usartTransmit(gonder);
		delay_ms(20);
		for(int a=0; a<50;a++)
			{
				text[a]='\0';
				gonder[a]='\0';
			}
		 break;
	case 3:
		 GPIO_SetBits(GPIOB,GPIO_Pin_2);
		 sprintf(gonder,"SALON ISITICI ACIK\n");
		 usartTransmit(gonder);
		 delay_ms(20);
		 for(int a=0; a<50;a++)
		 	 {
				  text[a]='\0';
				  gonder[a]='\0';
			 }
		 break;
	case 4:
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	    sprintf(gonder,"SALON LAMBA1 KAPALI\n");
		usartTransmit(gonder);
		delay_ms(20);
		for(int a=0; a<50;a++)
			 {
				  text[a]='\0';
				  gonder[a]='\0';
			 }
		break;
	case 5:
		 GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		 sprintf(gonder,"SALON LAMBA2 KAPALI\n");
		 usartTransmit(gonder);
		 delay_ms(20);
		 for(int a=0; a<50;a++)
			 {
				   text[a]='\0';
				   gonder[a]='\0';
			 }
		 break;
	case 6:
		 GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		 sprintf(gonder,"ISITICI KAPALI\n");
		 usartTransmit(gonder);
		 delay_ms(20);
		 for(int a=0; a<50;a++)
			 {
				   text[a]='\0';
				   gonder[a]='\0';
			 }
		 break;
	case 7:
		GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2);
	    sprintf(gonder,"EV KAPALI\n");
	    usartTransmit(gonder);
		delay_ms(20);
		for(int a=0; a<50;a++)
			 {
			 	    text[a]='\0';
			 		gonder[a]='\0';
			 }
		 break;
	}
}
void garajkontrol(int adim)
{
	switch(adim)
	{
	case 0:
		GPIO_SetBits(GPIOB,GPIO_Pin_8); //IN 1
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		break;
	case 1:
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		GPIO_SetBits(GPIOB,GPIO_Pin_9);	//IN 2
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		break;
	case 2:
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);
		GPIO_SetBits(GPIOB,GPIO_Pin_5);	//IN 3
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		break;
	case 3:
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		GPIO_SetBits(GPIOB,GPIO_Pin_6);	//IN 4
		break;
	}
}
void garajayar(float aci,int yon,int hiz)
{
	float adimAcisi=0.703125; //  360%512=0.703125 her bir adımdaki yaptığı açı
	int adimSayisi=(int)(aci/adimAcisi); //İstenilen açı için kaç adım lazım

	for(int a=0; a<=adimSayisi;a++)
	{
		if(yon==0)
		{
			for(int step=0; step<4; step++)  //KAPIYI AÇ
			{
				garajkontrol(step);
				delay_ms(hiz);
			}
		}
		else if(yon==1)
		{
			for(int step=4; step>=0; step--)  //KAPIYI KAPA
			{
				garajkontrol(step);
				delay_ms(hiz);
			}
		}
	}
	for(int a=0; a<50;a++)
		{
			text[a]='\0';
			gonder[a]='\0';
		}
}
int main(void)
{
	rccConfig();
	gpioConfig();
	usartConfig();
	NVIC_Config();
	SysTick_Config(SystemCoreClock/1000);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2|GPIO_Pin_8 | GPIO_Pin_9
			|GPIO_Pin_5|GPIO_Pin_6);
  while (1)
  {
	 if((strcmp(text,"LAMBA1 AC")==0))
	 {
		 kosul=1;
		 evkontrol(kosul);
		 kosul=0;
	 }
	 else if((strcmp(text,"LAMBA2 AC")==0))
	 {
		 kosul=2;
		 evkontrol(kosul);
		 kosul=0;
	 }
	 else if((strcmp(text,"ISITICI AC")==0))
	 {
		 kosul=3;
		 evkontrol(kosul);
		 kosul=0;
	 }
	 else if((strcmp(text,"LAMBA1 KAPA")==0))
	 {
		 kosul=4;
		 evkontrol(kosul);
		 kosul=0;
	 }
	 else if((strcmp(text,"LAMBA2 KAPA")==0))
	 {
		 kosul=5;
		 evkontrol(kosul);
		 kosul=0;
	 }
	 else if((strcmp(text,"ISITICI KAPA")==0))
	 {
		 kosul=6;
		 evkontrol(kosul);
		 kosul=0;
	 }
	 else if((strcmp(text,"EVI KAPA")==0))
	 {
		 kosul=7;
		 evkontrol(kosul);
		 kosul=0;
	  }
	 else if((strcmp(text,"GARAJ AC")==0))
	 {
		 sprintf(gonder,"GARAJ ACILIYOR\n");
		 usartTransmit(gonder);
		 garajayar(180,0,10);
		 GPIO_ResetBits(GPIOB,GPIO_Pin_8| GPIO_Pin_9|GPIO_Pin_5|GPIO_Pin_6);
		 delay_ms(200);
	 }
	 else if((strcmp(text,"GARAJ KAPA")==0))
	 {
		 sprintf(gonder,"GARAJ KAPANIYOR\n");
		 usartTransmit(gonder);
		 garajayar(180,1,10);
		 GPIO_ResetBits(GPIOB,GPIO_Pin_8 | GPIO_Pin_9|GPIO_Pin_5|GPIO_Pin_6);
		 delay_ms(200);
	 }

  }

}
