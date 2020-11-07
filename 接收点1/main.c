
#include <iocc2530.h>
#include "hal_mcu.h"
#include "hal_assert.h"
#include "hal_board.h"
#include "sys_init.h"
#include "uart.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include <stdio.h>

#define RF_CHANNEL            38      // 2.4 GHz RF channel

#define PAN_ID                0x2022
#define SEND_ADDR             0x2610
#define RECV_ADDR             0x2630

#define NODE_TYPE             0         //0:���սڵ㣬1�����ͽڵ�

static basicRfCfg_t basicRfConfig;


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
         Uart_Send_String((char *)pRxData);  //���ͽ��յ�������
         printf((char *)pRxData);            //��ʾ���յ�������
        }
    }
}

void main(void)
{
    halMcuInit();

    hal_led_init();
    
    uart0_init(0x00, 0x00);            //��ʼ�����ڣ�����żУ�飬ֹͣλΪ1λ
   
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
