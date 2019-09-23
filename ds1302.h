#ifndef_ds1302_1022
#defing_ds1302_1022
#include "stdio.h"
sbit T_RST=P3^7; //DS1302接线
sbit T_CLK=P3^5; // DS1302接线 
sbit T_IO=P3^6; // DS1302接线 
sbit ACC0=ACC^0;
sbit ACC7=ACC^7;
void delay(unsigned int a)
{
while(a--); 
}
void write_byte(unsigned char ucode)
{
unsigned char i;
ACC=ucode;
T_RST=1;
for(i=8;i>0;i--)
{ 
T_IO=ACC0;
T_CLK=0; 
T_CLK=1;
ACC=ACC>>1;
}
}
unsigned char read_byte(void)
{
unsigned char i;
T_RST=1;
for(i=0;i<8;i++)
{
ACC=ACC>>1;
T_IO=1;
T_CLK = 1;
T_CLK = 0;
ACC7 = T_IO;
}
return(ACC);

}
void write_1302(unsigned char addr,unsigned char ucode)
{
T_RST=0; //停止工作
T_CLK=0; 
T_RST=1; //重新工作
write_byte(addr); //写入地址
delay(1);
write_byte(ucode);
T_RST=0;
T_CLK=1;
}
unsigned char read_1302(unsigned char addr)
{
unsigned char ucode;
T_RST=0; //停止工作
T_CLK=0; 
T_RST=1; //重新工作
write_byte(addr); //写入地址
delay(1);
ucode=read_byte();
T_RST=0;
T_CLK=1; //停止工作
return(ucode);
}
#endif
