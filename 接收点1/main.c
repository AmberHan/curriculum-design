
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

#define NODE_TYPE             0         //0:接收节点，1：发送节点

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
         Uart_Send_String((char *)pRxData);  //发送接收到的数据
         printf((char *)pRxData);            //显示接收到的数据
        }
    }
}

void main(void)
{
    halMcuInit();

    hal_led_init();
    
    uart0_init(0x00, 0x00);            //初始化串口：无奇偶校验，停止位为1位
   
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
