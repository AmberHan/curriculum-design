#include <ioCC2530.h>             //引入CC2530所对应的头文件（包含各SFR的定义）

void Uart_Send_char(char ch);
void Uart_Send_String(char *Data);
int Uart_Recv_char(void);