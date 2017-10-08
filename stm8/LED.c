#include "iostm8s103.h"
#include "led.h"

void GPIOPC3_OUT(void)
{
    PC_DDR |= 0x08;    // PC3   ���
    PC_ODR |= 0x08;    // PC3   ����
    PC_CR1 |= 0x08;    // PC3   ����
    //PC_CR2 |= 0x08;  // ����  10MHZ
    PC_CR2 |= 0x00;
}
void GPIOPC4_OUT(void)
{
    PC_DDR |= 0x10;    // PC4   ���
    PC_ODR |= 0x10;    // PC4   ����
    PC_CR1 |= 0x10;    // PC4   ����
    //PC_CR2 |= 0x10;  // ����  10MHZ
    PC_CR2 |= 0x00;
}
void GPIOPC5_OUT(void)
{
    PC_DDR |= 0x20;    // PC5   ���
    PC_ODR |= 0x20;    // PC5   ����
    PC_CR1 |= 0x20;    // PC5   ����
    //PC_CR2 |= 0x20;  // ����  10MHZ
    PC_CR2 |= 0x00;
}
void GPIOPC6_OUT(void)
{
    PC_DDR |= 0x40;    // PC6   ���
    PC_ODR |= 0x40;    // PC6   ����
    PC_CR1 |= 0x40;    // PC6   ����
    //PC_CR2 |= 0x40;  // ����  10MHZ
    PC_CR2 |= 0x00;
}
/*************************************************************
   ��������:
   ��������:
   ��ڲ���:
   ��ڲ��������ͺͺ���:
   ���ڲ���:
   �� ע:
*************************************************************/
void LED_Init(char *buffer)
{
    GPIOPC3_OUT();
    GPIOPC4_OUT();
    GPIOPC5_OUT();
    GPIOPC6_OUT();
    if (buffer[0]=='0') 
    { 
        LED_PC3_OFF; 
    }
    else
    {
        LED_PC3_ON;
    }
    if (buffer[1]=='0') 
    { 
        LED_PC4_OFF; 
    }
    else
    {
        LED_PC4_ON;
    }
    if (buffer[2]=='0') 
    { 
        LED_PC5_OFF; 
    }
    else
    {
        LED_PC5_ON;
    }
    if (buffer[3]=='0') 
    { 
        LED_PC6_OFF; 
    }
    else
    {
        LED_PC6_ON;
    }
}
/*************************************************************
   ��������:
   ��������:
   ��ڲ���:
   ��ڲ��������ͺͺ���:
   ���ڲ���:
   �� ע:
*************************************************************/
void LED_Set(char *buffer)
{
    if ( buffer[0] != buffer[4] )
    {
        if (buffer[4]=='1') 
        {
            LED_PC3_ON;
        }
        else
        {
            LED_PC3_OFF;
        }
        buffer[0] = buffer[4];
    }
    if ( buffer[1] != buffer[5] )
    {
        if (buffer[5]=='1') 
        {
            LED_PC4_ON;
        }
        else
        {
            LED_PC4_OFF;
        }
        buffer[1] = buffer[5];
    }
    if ( buffer[2] != buffer[6] )
    {
        if (buffer[6]=='1') 
        {
            LED_PC5_ON;
        }
        else
        {
            LED_PC5_OFF;
        }
        buffer[2] = buffer[6];
    }
    if ( buffer[3] != buffer[7] )
    {
        if (buffer[7]=='1') 
        {
            LED_PC6_ON;
        }
        else
        {
            LED_PC6_OFF;
        }
        buffer[3] = buffer[7];
    }
}
/* EOF */