/* input 'l' or 'L' to trigger LED */
/* input 'b' or 'B' to show button-pressed times*/

#define USE_STDPERIPH_DRIVER
#include "stm32f10x.h"
#include "stm32_p103.h"

void Output(char *Buffer);
void MyItoA(int Value, char *Buffer);
void myDelay(unsigned long delay );

volatile const char String1[]="Hello!\n";
volatile char String2[16];
volatile char in_char;
volatile int Counter;


void Output(char *Buffer)
{
    int I=0;
    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
    for (I=0; Buffer[I]!='\0'; I++)
    {
        USART_SendData(USART2, Buffer[I]);
    }
    USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
}

void MyItoA(int Value, char *Buffer)
{
    static int I=0;
    if (Value==0)
    {
        I=0;
    }
    else
    {
        MyItoA(Value/10,Buffer);
        Buffer[I++]=Value%10+'0';
    }
    Buffer[I]='\0';
    return;
}

void myDelay(unsigned long delay )
{
    while(delay) delay--;
}

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        Counter++;
        Output(String1);
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        in_char = USART_ReceiveData(USART2);
        USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
    }

    if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
    {

        if (in_char=='L' || in_char=='l' )
        {
            GPIOC->ODR = GPIOC->ODR ^ 0x00001000;
        }
        else if (in_char=='B' || in_char=='b' )
        {
 	    Output("Button has been pressed ");
            MyItoA(Counter,String2);
            Output(String2);
            Output(" times.\n");
        }
        else
            Output("Invalid Input;press \'l\' or \'b\'\n");
        USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    }
}

int main(void)
{
    init_led();
    init_button();
    enable_button_interrupts();
    init_rs232();
    enable_rs232_interrupts();
    enable_rs232();
    while(1) 
    {
          EXTI_GenerateSWInterrupt(EXTI_Line0);
          myDelay(50000000);
    }
}

