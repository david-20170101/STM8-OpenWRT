/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

#include "iostm8s103.h"
#include "stdio.h"
#include "string.h"
#include "dht11.h"
#include "led.h"
#include "code.h"

#define MAXMESG 16
#define uchar unsigned char
#define uint  unsigned int

typedef enum { FAILED = 0, PASSED = !FAILED } TestStatus;
/* Private function prototypes -----------------------------------------------*/
TestStatus Buffercmp(uchar *pBuffer1, uchar *pBuffer2, uint Buffer1Length, uint BufferLength);
void UART1_sendchar(uchar data);
void UART1_SendString(uchar *Data,uint len);
void ByteToHexStr(uchar byte_arr[], uint arr_len);
void UART1_Init(void);
void CLK_HSI_Init(void);

uchar RxBuffer1[MAXMESG];
uint  len=0;
uchar DHT11[8]="AT+DHT11";
uchar PB4[6]="AT+PB4";
uchar PB5[6]="AT+PB5";
uchar PC3[6]="AT+PC3";
uchar PC4[6]="AT+PC4";
static char DHT11_TxBuffer1[40];
static char LED_Buffer[8]={'0','0','0','0','0','0','0','0'};
static uchar UniqueID[12]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

uchar TempData,HumiData;

/*****************************************************************/
void ByteToHexStr(uchar byte_arr[], uint arr_len)
{
    uint i=0;
    for (;i<arr_len;i++)
    {
        char hex1;
        char hex2;
        int value=byte_arr[i];   //ֱ�ӽ�unsigned char��ֵ�����͵�ֵ��ϵͳ������ǿ��ת��
        int v1=value/16;
        int v2=value % 16;       //����ת����ĸ
        if (v1>=0&&v1<=9)
            hex1=(char)(48+v1);
        else
            hex1=(char)(55+v1);  //������ת����ĸ
        if (v2>=0&&v2<=9)
            hex2=(char)(48+v2);
        else
            hex2=(char)(55+v2);  //����ĸ���ӳɴ�
        UART1_sendchar('0');
        UART1_sendchar('x');
        UART1_sendchar(hex1);
        UART1_sendchar(hex2);
        UART1_sendchar(' ');
    }
    //return hexstr;
} 
//�����Ϳ���ͨ��һ��������һ��BYTE����ת��һ��16���Ƶ�string�����ˡ�
/*****************************************************************/
void CLK_HSI_Init(void)
{
    CLK_ECKCR  = 0x00;         //Disable extern HSE
    CLK_ICKCR  = 0x01;         //ebale HSI
    while(!(CLK_ICKCR&0x02));  //Waiting for Ready on HSI
    CLK_SWR    = 0xE1;         //chose HSI 16MHZ
    //CLK_CKDIVR = 0x18;       //Fmaster=16MHZ/8 Fcpu=Fmaster=2MHZ
    CLK_CKDIVR = 0x00;         //Fmaster=16MHZ
}
/*****************************************************************/
void UART1_Init(void)
{
    UART1_CR1=0x00; //����M�ֳ���8λ����λ
    UART1_CR2=0x00; 
    UART1_CR3=0x00; //1λֹͣλ

    // ���ò����ʣ�����ע�����¼��㣺
    // (1) ������дBRR2
    // (2) BRR1��ŵ��Ƿ�Ƶϵ���ĵ�11λ����4λ��
    // (3) BRR2��ŵ��Ƿ�Ƶϵ���ĵ�15λ����12λ���͵�3λ
    // ����0λ
    // ������ڲ�����λ9600ʱ����Ƶϵ��=2000000/9600=208
    // ��Ӧ��ʮ��������Ϊ00D0��BBR1=0D,BBR2=00
    // ������ڲ�����λ9600ʱ����Ƶϵ��=16000000/9600=1666
    // ��Ӧ��ʮ��������Ϊ0682��BBR1=68,BBR2=02
    // ������ڲ�����λ115200ʱ����Ƶϵ��=2000000/115200=17
    // ��Ӧ��ʮ��������Ϊ0011��BBR1=01,BBR2=01
    // ������ڲ�����λ115200ʱ����Ƶϵ��=16000000/115200=138
    // ��Ӧ��ʮ��������Ϊ008A��BBR1=08,BBR2=0A
    UART1_BRR2=0x02;
    UART1_BRR1=0x68;
    //UART1_BRR2=0x0A;
    //UART1_BRR1=0x08;

    UART1_CR2=0x2c;       //������գ����ͣ��������ж�
}
/*****************************************************************/

main()
{
    float fTempDataValue = 0;
    float fHumiDataValue = 0;
    CLK_HSI_Init();
    UART1_Init();
    LED_Init(LED_Buffer);
    //encrypt((uchar*)UniqueID);
    //ByteToHexStr((uchar*)UniqueID, sizeof(UniqueID));
    //decrypt((uchar*)UniqueID);

    _asm("rim");      //��ȫ���жϣ�simΪ���ж�
    //_asm("sim");    //��ȫ���ж�
    while (1)
    {
        uint TP=TempData;
        uint HM=HumiData;
        ReadTempAndHumi( &fTempDataValue, &fHumiDataValue );
        sprintf( DHT11_TxBuffer1,"{\"temperature\":%d,\"humidity\":\"%d%%\"}\r\n",TP,HM);
        LED_Set(LED_Buffer);
        Delayms(5000);
   }
}

/***************************************************/
void UART1_sendchar(uchar data)
{
    while((UART1_SR & 0x80)==0x00);
    UART1_DR = data;
}
/***************************************************/
void UART1_SendString(uchar *Data,uint len)
{
    uint i=0;
    for(;i<len;i++)
    UART1_sendchar(Data[i]);
}
/***************************************************/
/**
  * @brief Compares two buffers.
  * @param[in] pBuffer1 First buffer to be compared.
  * @param[in] pBuffer2 Second buffer to be compared.
  * @param[in] BufferLength Buffer's length
  * @retval TestStatus Status of buffer comparison
  * - PASSED: pBuffer1 identical to pBuffer2
  * - FAILED: pBuffer1 differs from pBuffer2
  * @par Required preconditions:
  * None
  */  
TestStatus Buffercmp(uchar *pBuffer1, uchar *pBuffer2, uint Buffer1Length, uint BufferLength)
{
    if (Buffer1Length != BufferLength)
        {
            return FAILED;
        }	
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }
        pBuffer1++;
        pBuffer2++;
    }
    return PASSED;
}
/***************************************************/
//�жϽ��յ������ݣ��������AT�����ԭ�����ͻ�ȥ��
@far @interrupt void UART1_Recv_IRQHandler (void)
{
    uchar ch;
    ch = UART1_DR;
    //if (!UniqueIDcmp((uchar*)UniqueID)){ return; }
    if (len==MAXMESG) 
    {
        len=0;
    }
    RxBuffer1[len]=ch;
    if (RxBuffer1[len]==0x0a)
    {
        if (RxBuffer1[len-1]==0x0d)
        {
            if (Buffercmp( DHT11, RxBuffer1, sizeof(DHT11), len-1 ))
            {
                UART1_SendString( DHT11_TxBuffer1, sizeof(DHT11_TxBuffer1) );
            }
            else
            if (Buffercmp( PB4, RxBuffer1, sizeof(PB4), len-1 ))
            {
                if (LED_Buffer[4] == '0')
                {
                    LED_Buffer[4]='1';
                }
                else
                {
                    LED_Buffer[4]='0';
                }
            }
            else
            if (Buffercmp( PB5, RxBuffer1, sizeof(PB5), len-1 ))
            {
                if (LED_Buffer[5] == '0')
                {
                    LED_Buffer[5]='1';
                }
                else
                {
                    LED_Buffer[5]='0';
                }
            }
            else
            if (Buffercmp( PC3, RxBuffer1, sizeof(PC3), len-1 ))
            {
                if (LED_Buffer[6] == '0')
                {
                    LED_Buffer[6]='1';
                }
                else
                {
                    LED_Buffer[6]='0';
                }
            }
            else
            if (Buffercmp( PC4, RxBuffer1, sizeof(PC4), len-1 ))
            {
                if (LED_Buffer[7] == '0')
                {
                    LED_Buffer[7]='1';
                }
                else
                {
                    LED_Buffer[7]='0';
                }
            }
            else
            {
                UART1_SendString( RxBuffer1, len+1 );
            }
            len=0;
        }
        else
        {
            len = len+1;
        }
    }
    else
    {
        len = len+1;
    }
    return;
}
/* EOF */