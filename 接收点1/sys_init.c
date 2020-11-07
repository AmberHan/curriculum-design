#include <ioCC2530.h>             //����CC2530����Ӧ��ͷ�ļ���������SFR�Ķ��壩
#include "sys_init.h"

/*ϵͳʱ�ӳ�ʼ��
-------------------------------------------------------*/
void xtal_init(void)
{
  SLEEPCMD &= ~0x04;              //���ϵ�
  while(!(CLKCONSTA & 0x40));     //���������������ȶ�
  CLKCONCMD &= ~0x47;             //ѡ��32MHz��������
  SLEEPCMD |= 0x04;
}

/*led��ʼ��
-------------------------------------------------------*/
void led_init(void)
{
  P1SEL &= ~0x03;          //P1.0 P1.1Ϊ��ͨ I/O ��
  P1DIR |= 0x03;           //���
  
  D7 = 1;                  //��LED
  D6 = 1;
}

/*�ⲿ�жϳ�ʼ��
-------------------------------------------------------*/
void ext_init(void)
{
    P0SEL &= ~0x10;        //ͨ��IO    
    P0DIR &= ~0x10;        //������  
    P0INP &= ~0x10;        //0:������1:����
    P0IEN |= 0x10;         //��P0���ж�
    PICTL &=~ 0x01;        //�½��ش���    
    P0IFG &= ~0x10;        //P0.4�жϱ�־��0    
    P0IE = 1;              //P0�ж�ʹ��    
    EA = 1;                //���ж�ʹ��    
}

/*timer1��ʼ��
-------------------------------------------------------*/
void time1_init(void)
{
  T1CTL = 0x05;                   //8��Ƶ������ģʽ
  T1STAT= 0x21;                   //ͨ��0,�ж���Ч;�Զ���װģʽ(0x0000->0xffff)
  
  IEN1|=0X02;                     //��ʱ��1�ж�ʹ��
  EA=1;                           //�����ж� 
}

/*uart0��ʼ��
-------------------------------------------------------*/
void uart0_init(unsigned char StopBits,unsigned char Parity)
{
  P0SEL |=  0x0C;                 //��ʼ��UART0�˿�
  PERCFG&= ~0x01;                 //ѡ��UART0Ϊ��ѡλ��һ
  P2DIR &= ~0xC0;                 //P0������Ϊ����0
  U0CSR = 0xC0;                   //����ΪUARTģʽ,����ʹ�ܽ�����
   
  U0GCR = 0x09;
  U0BAUD = 0x3b;                  //����UART0������Ϊ19200bps  
  
  U0UCR |= StopBits|Parity;       //����ֹͣλ����żУ��
}

/*���Ź���ʼ��
-------------------------------------------------------*/
void watchdog_init(void)   
{
  WDCTL = 0x00;                   //���Ź�ģʽ��ʱ����һ��
  WDCTL |= 0x08;                  //�������Ź�
}

/*˯�߶�ʱ���жϳ�ʼ��
-------------------------------------------------------*/
void sleepTimer_init(void)
{
  STIF=0;                         //˯�߶�ʱ���жϱ�־��0   
  STIE=1;                         //��˯�߶�ʱ���ж�   
  EA=1;                           //�����ж�
}

/*��ʱ����
-------------------------------------------------------*/
void halWait(unsigned char wait)
{
  unsigned long largeWait;

  if(wait == 0)
  {return;}
  largeWait = ((unsigned short) (wait << 7));
  largeWait += 114*wait;

  largeWait = (largeWait >> CLKSPD);
  while(largeWait--);

  return;
}