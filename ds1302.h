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
/* Date variable cache */
char buf[50];
char day[10];
/* Serial port data cache */
String comdata = "";
int numdata[7] ={0}, j = 0, mark = 0;
/* Create the DS1302 object */
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);


void print_time()
{
    /* Get the current time from DS1302 */
    Time t = rtc.time();
    /* Convert weekly from number to name */
    memset(day, 0, sizeof(day));
    switch (t.day)
    {
    case 1: strcpy(day, "Sunday"); break;
    case 2: strcpy(day, "Monday"); break;
    case 3: strcpy(day, "Tuesday"); break;
    case 4: strcpy(day, "Wednesday"); break;
    case 5: strcpy(day, "Thursday"); break;
    case 6: strcpy(day, "Friday"); break;
    case 7: strcpy(day, "Saturday"); break;
    }
    /* Format the date code into buf for output */
    snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d", day, t.yr, t.mon, t.date, t.hr, t.min, t.sec);
    /* Output date to serial port */
    Serial.println(buf);
}


void setup()
{
    Serial.begin(9600);
    rtc.write_protect(false);
    rtc.halt(false);
}

void loop()
{

    /* When the serial port has data, the data is spliced to the variable comdata */
    while (Serial.available() > 0)
    {
        comdata += char(Serial.read());
        delay(2);
        mark = 1;
    }
    /* Decomposing the comdata string by commas, decomposing the result into a number to numdata [] array */
    if(mark == 1)
    {
        Serial.print("You inputed : ");
        Serial.println(comdata);
        for(int i = 0; i < comdata.length() ; i++)
        {
            if(comdata[i] == ',' || comdata[i] == 0x10 || comdata[i] == 0x13)
            {
                j++;
            }
            else
            {
                numdata[j] = numdata[j] * 10 + (comdata[i] - '0');
            }
        }
        /* Assemble the converted numdata into a time format and write to DS1302 */
        Time t(numdata[0], numdata[1], numdata[2], numdata[3], numdata[4], numdata[5], numdata[6]);
        rtc.time(t);
        mark = 0;j=0;
        /* Empty the comdata variable to wait for the next input */
        comdata = String("");
        /* Empty numdata */
        for(int i = 0; i < 7 ; i++) numdata[i]=0;
    }
