////����
#include <iocc2530.h>
#include "hal_mcu.h"
#include "hal_assert.h"
#include "hal_board.h"

#include "hal_rf.h"
#include "basic_rf.h"
#include <stdio.h>
int getADC(void);

#define RF_CHANNEL            38      // 2.4 GHz RF channel


#define PAN_ID                0x2022
#define SEND_ADDR             0x2620
#define RECV_ADDR             0x2630


#define NODE_TYPE             1         //0:���սڵ㣬1�����ͽڵ�

static basicRfCfg_t basicRfConfig;


void rfSendData(void)
{
 //   uint8 pTxData[] = {'H', 'e', 'l', 'l', 'o', ' ', 'c', 'c', '2', '5', '3', '0', '\r', '\n'};
    uint8 ret;   
    uint8 StrAdc[10],StrAdc1[10];
    int AdcValue,Value=0; 
    
    // Keep Receiver off when not needed to save power
    basicRfReceiveOff();
    
    // Main loop
    while (TRUE) {
       AdcValue = getADC();
       sprintf(StrAdc,"%d\r\n",AdcValue);
       printf("Send:%s\n", StrAdc);   //��ʾ������������
       if(AdcValue<300)Value=2;       //��ȫ
       if(AdcValue>300)Value=3;     //����ȫ
       sprintf(StrAdc1,"%d\r\n",Value);
       printf("Send:%s\n", StrAdc1);   //��ʾ���Ͱ�ȫ����
       ret = basicRfSendPacket(0xffff, StrAdc1, sizeof StrAdc1);       
       if (ret == SUCCESS) {
          hal_led_on(1);
          halMcuWaitMs(100);
          hal_led_off(1);
          halMcuWaitMs(900); 
       } else {
          hal_led_on(1);
          halMcuWaitMs(1000);
          hal_led_off(1);
       }
       
    }
}

void rfRecvData(void)
{
  uint8 pRxData[128];
  int rlen;
  
   basicRfReceiveOn();

    // Main loop
    while (TRUE) {
        while(!basicRfPacketIsReady());
        rlen = basicRfReceive(pRxData, sizeof pRxData, NULL);
        if(rlen > 0) {
          pRxData[rlen] = 0;
          printf("My Address %u , recv:", RECV_ADDR);  
          printf((char *)pRxData);
            
        }
    }
}

void main(void)
{
    halMcuInit();

    hal_led_init();
    
    hal_uart_init();
    if (FAILED == halRfInit()) {
        HAL_ASSERT(FALSE);
    }

    // Config basicRF
    basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
#ifdef SECURITY_CCM
    basicRfConfig.securityKey = key;
#endif

    
    // Initialize BasicRF
#if NODE_TYPE
    basicRfConfig.myAddr = SEND_ADDR;
#else
    basicRfConfig.myAddr = RECV_ADDR; 
#endif
    
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }
#if NODE_TYPE
  rfSendData();
#else
  rfRecvData();   
#endif
}

/*�õ�ADCֵ
-------------------------------------------------------*/
int getADC(void) 
{
  unsigned int  value;
  
  P0SEL |= 0x02;
  ADCCON3  = (0xB1);                    //ѡ��AVDD5Ϊ�ο���ѹ��12�ֱ��ʣ�P0_1  ADC
  
  ADCCON1 |= 0x30;                      //ѡ��ADC������ģʽΪ�ֶ�
  ADCCON1 |= 0x40;                      //����ADת��             
  
  while(!(ADCCON1 & 0x80));             //�ȴ�ADCת������
   
  value =  ADCL >> 2;
  value |= (ADCH << 6);                 //ȡ������ת�����������value��
  
  return ((value) >> 2);        
}