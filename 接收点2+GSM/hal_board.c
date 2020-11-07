
#include <iocc2530.h>
#include <stdio.h>


void hal_led_init(void)
{
    P1SEL &= ~0x0D;          //����P1.0 p1.2 p1.3Ϊ��ͨ I/O ��
    P1DIR |= 0x0D;          //����P1.0  P1.2 p1.3 Ϊ���

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
*�������� ����ʼ������1										
*��ڲ��� ����												
*�� �� ֵ ����							
*˵    �� ��19200-8-n-1						
****************************************************************/
void hal_uart_init(void)
{
    PERCFG = 0x00;				//λ��1 P0��
    P0SEL |= 0x0C;				//P0��������
    P2DIR &= ~0XC0;                             //P0������Ϊ����0

    U0CSR |= 0x80;				//UART��ʽ
    U0CSR |= 0X40;				//�������
    
    U0GCR = 0x09;				//baud_e
    U0BAUD |= 0x3b;				//��������Ϊ19200
    UTX0IF = 0;
}

__near_func int putchar(int ch)
{
    U0DBUF = ch;
    while(UTX0IF == 0);
    UTX0IF = 0;
    return ch;
}




