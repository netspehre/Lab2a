#define USE_STDPERIPH_DRIVER
#include "stm32f10x.h"
int last_button_state, new_button_state;
int main(void)
{
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
					
	/* Configure PC.12 as output push-pull (LED) */
	GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_SET);
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin =  GPIO_Pin_12,
		.GPIO_Mode = GPIO_Mode_Out_PP,
		.GPIO_Speed = GPIO_Speed_50MHz,
	};
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	last_button_state = GPIOA->IDR & 0x00000001;

	while(1) {
		new_button_state = GPIOA->IDR & 0x00000001;
		if(new_button_state ^ last_button_state) {
			if(new_button_state) {
				GPIOC->ODR = GPIOC->ODR ^ 0x00001000;
			}
		}
		last_button_state = new_button_state;
	}
}
