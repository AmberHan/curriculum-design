
#include <iocc2530.h>
#include <stdio.h>


void hal_led_init(void)
{
    P1SEL &= ~0x0D;          //设置P1.0 p1.2 p1.3为普通 I/O 口
    P1DIR |= 0x0D;          //设置P1.0  P1.2 p1.3 为输出

    P2SEL &= ~0x01;
    P2DIR |= 0x01;
    
    P1 |= 0x0D;
    P2_0 |= 0x01;
}

void hal_led_off(int leds)
{
  if (leds & 0x01) {
    P1_0 |= 0x01;
  }
  if (leds & 0x02) {
    P1_2 |= 0x04;
  }
  if (leds & 0x04) {
    P1_3 |= 0x08;
  }
  if (leds & 0x08) {
    P2_0 |= 0x01;
  }
}
void hal_led_on(int leds)
{
    if (leds & 0x01) {
    P1_0 &= ~0x01;
  }
  if (leds & 0x02) {
    P1_2 &= ~0x04;
  }
  if (leds & 0x04) {
    P1_3 &= ~0x08;
  }
  if (leds & 0x08) {
    P2_0 &= ~0x01;
  }
}



/****************************************************************
*函数功能 ：初始化串口1										
*入口参数 ：无												
*返 回 值 ：无							
*说    明 ：19200-8-n-1						
****************************************************************/
void hal_uart_init(void)
{
    PERCFG = 0x00;				//位置1 P0口
    P0SEL |= 0x0C;				//P0用作串口
    P2DIR &= ~0XC0;                             //P0优先作为串口0

    U0CSR |= 0x80;				//UART方式
    U0CSR |= 0X40;				//允许接收
    
    U0GCR = 0x09;				//baud_e
    U0BAUD |= 0x3b;				//波特率设为19200
    UTX0IF = 0;
}

__near_func int putchar(int ch)
{
    U0DBUF = ch;
    while(UTX0IF == 0);
    UTX0IF = 0;
    return ch;
}




