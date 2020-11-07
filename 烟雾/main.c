////烟雾
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


#define NODE_TYPE             1         //0:接收节点，1：发送节点

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
       printf("Send:%s\n", StrAdc);   //显示发送烟雾数据
       if(AdcValue<300)Value=2;       //安全
       if(AdcValue>300)Value=3;     //不安全
       sprintf(StrAdc1,"%d\r\n",Value);
       printf("Send:%s\n", StrAdc1);   //显示发送安全数据
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

/*得到ADC值
-------------------------------------------------------*/
int getADC(void) 
{
  unsigned int  value;
  
  P0SEL |= 0x02;
  ADCCON3  = (0xB1);                    //选择AVDD5为参考电压；12分辨率；P0_1  ADC
  
  ADCCON1 |= 0x30;                      //选择ADC的启动模式为手动
  ADCCON1 |= 0x40;                      //启动AD转化             
  
  while(!(ADCCON1 & 0x80));             //等待ADC转化结束
   
  value =  ADCL >> 2;
  value |= (ADCH << 6);                 //取得最终转化结果，存入value中
  
  return ((value) >> 2);        
}