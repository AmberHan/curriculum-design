#include <iocc2530.h>
#include "hal_mcu.h"
#include "hal_assert.h"
#include "hal_board.h"
#include "sys_init.h"
#include "uart.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include <stdio.h>
#include <string.h>
uint8 sim800c_send_cmd(char *cmd,char ack,int waittime);//定义声明
void sim800c_init(void);
//uint8 sim800c_send_cmd1(char *cmd,char ack,int waittime);//定义声明

#define RF_CHANNEL            38      // 2.4 GHz RF channel
#define PAN_ID                0x2022
#define SEND_ADDR             0x2620
#define RECV_ADDR             0x2630
#define NODE_TYPE             0         //0:接收节点，1：发送节点

static basicRfCfg_t basicRfConfig;
static  char haoma[]="00310038003800350032003700300033003000370033";
static char ppp[150];
static char pp[150];
static  char a=(char)0x0A;//短信验证

void rfRecvData(void)
{
  uint8 pRxData[128];
  int rlen;
  
   basicRfReceiveOn();

    // Main loop
    while (TRUE) {
  //      char key=0;
        while(!basicRfPacketIsReady());
        rlen = basicRfReceive(pRxData, sizeof pRxData, NULL);
        if(rlen > 0) {
        pRxData[rlen] = 0;
        printf((char *)pRxData);            //显示接收到的数据
        if(strstr(pRxData,"1")>0){
             hal_led_off(1);
             sim800c_send_cmd(ppp,a,20);//发短信给18852703073      
             halMcuWaitMs(1000);
             hal_led_off(1);
             Uart_Send_String("6CE8610F4E004E0B54E6FF0C67094EBA51654FB5FF01FF01");//注意一下哦，有人入侵！！
             sim800c_send_cmd(pp,'+',20);//0x1A结束符,回应ACK结束
             hal_led_on(1);
             }//注意一下哦，有人入侵！！
         if(strstr(pRxData,"3")>0){
             hal_led_off(1);
             sim800c_send_cmd(ppp,a,20);//发短信给18852703073      
             halMcuWaitMs(1000);
             hal_led_off(1);
             Uart_Send_String("6CE8610F4E004E0B54E6FF0C70DF96FE8D856807FF01FF01"); //注意一下哦，烟雾超标！！
             sim800c_send_cmd(pp,'+',20);//0x1A结束符,回应ACK结束
             hal_led_on(1);
             } //注意一下哦，烟雾超标！！
        }
    }
}


void main(void)
{
    halMcuInit();                       //初始化时钟
    hal_led_init();                     //初始化灯
    uart0_init(0x00, 0x00);            //初始化串口：无奇偶校验，停止位为1位
    sim800c_init();

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

uint8 sim800c_send_cmd(char *cmd,char ack,int waittime){
  char ch;
  Uart_Send_String((char *)cmd);  //发送AT指令
  for(;waittime>0;waittime--){
  ch = Uart_Recv_char();
  if(ch==ack) {hal_led_on(1);return 1;}
  }
  return 0;
}

//短信初始化，闪烁三下初始化
void sim800c_init(void){
  sprintf((char *)ppp,"AT+CMGS=\"%s\"\r\n",haoma);
  sprintf((char *)pp,"%c\r\n",0x1A);
  sim800c_send_cmd("AT+CMGF=1\r\n",'O',20); //设置短信模式为文本模式
  halMcuWaitMs(1000);
  hal_led_off(1);
  sim800c_send_cmd("AT+CSMP=17,167,1,8\r\n",'O',30);//设置中英文模式
  halMcuWaitMs(1000);
  hal_led_off(1);
  sim800c_send_cmd("AT+CSCS=\"UCS2\"\r\n",'O',30);//设置为 UCS2 字符集编码
  halMcuWaitMs(1000);
  hal_led_off(1);
}

/*
uint8 sim800c_send_cmd1(char *cmd,char ack,int waittime){
  char ch;
  Uart_Send_String((char *)cmd);  //发送AT指令
  for(;waittime>0;waittime--){
  ch = Uart_Recv_char();
  while(1)Uart_Send_char(ch);
  if(ch==ack) {hal_led_on(1);return 1;}
  }
  return 0;
}*/