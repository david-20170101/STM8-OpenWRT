#include "iostm8s103.h"
#include "dht11.h"

void GPIOPD4_OUT(void)
{
    PD_DDR |= 0x10;    // PD4   ���
    PD_ODR |= 0x10;    // PD4   ����
    PD_CR1 |= 0x10;    // PD4   ����
    //PD_CR2 |= 0x10;  // ����  10MHZ
    PD_CR2 |= 0x00;
}
void GPIOPD4_IN(void)
{
    PD_DDR |= 0x00;  // PD4   ����
    PD_CR1 |= 0x00;  // PD4   
    PD_CR2 |= 0x10;  // �������� 
}
void Delayms( unsigned int time )
{
    unsigned int i;
    while( time-- )
    {
        for( i = 900; i > 0; i-- )
        {
            _asm("nop");
            _asm("nop");
            _asm("nop");
            _asm("nop");
        }
    }
}
void Delayus( unsigned int time )
{
    unsigned int temp = time;
    if( time == 10 )
    {
        temp = 8;
    }
    if( time == 20 )
    {
        temp = 20;
    }
    if( time == 30 )
    {
        temp = 32;
    }
    if( time == 40 )
    {
        temp = 42;
    }
    if( time == 50 )
    {
        temp = 54;
    }
    while( temp-- )
    {
        _asm("nop");
        _asm("nop");
        _asm("nop");
        _asm("nop");
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
unsigned char uc_ComDHT11(void)
{
    unsigned char i, ucComData = 0;
    for( i = 0; i < 8; i++ )
    {
        //���յ���Ӧ������50us�ĵ͵�ƽ��ʾ�������ݵĿ�ʼ��
        //��������ֻҪ��ʱ������50us����
        while(!((PD_IDR & 0x10) == 0x10));
        //λ���ݡ�0���ĸ�ʽΪ��50΢��ĵ͵�ƽ��26--28΢��ĸߵ�ƽ��
        //λ���ݡ�1���ĸ�ʽΪ��50΢��ĵ͵�ƽ��70΢��ĸߵ�ƽ��
        Delayus( 30 );
        if((PD_IDR & 0x10) == 0x10) // �ߵ�ƽΪ1���͵�ƽΪ0
        {
            ucComData |= ( 1 << ( 7 - i ) );
            while((PD_IDR & 0x10) == 0x10);
        }
        else
        {
            ucComData |= ( 0 << ( 7 - i ) );
            while(!((PD_IDR & 0x10) == 0x10));
        }
    }
    return ucComData;
}
/*************************************************************
   ��������:
   ��������:
   ��ڲ���:
   ��ڲ��������ͺͺ���:
   ���ڲ���:
   �� ע:
*************************************************************/
void ReadTempAndHumi( float *pTempValue, float *pHumiValue )
{
    unsigned char ucDHT11Value[5] = { 0x00 };

    GPIOPD4_OUT();       //�����������ģʽ����ʱ����ߵ�ƽ
    DATA_DQ_LOW;         //��ʱ�����������ģʽ���������ͣ�ODR����Ϊ0
    Delayms(180);        //����18����
    DATA_DQ_HIGH;        //�ͷ����ߡ�ODR����Ϊ1
    GPIOPD4_IN();        //��������
    Delayus(35);         //�ͷ������Ժ�ȴ�35΢��DHT�ᷢ����Ӧ�ź�
    if(!((PD_IDR & 0x10) == 0x10))
    {
        while(!((PD_IDR & 0x10) == 0x10));   //80us��Ӧ���ź� DHT11 TO MCU
        while((PD_IDR & 0x10) == 0x10);      //80us��֪ͨ�ź� DHT11 TO MCU
        ucDHT11Value[0]	= uc_ComDHT11( ); // ʪ��H
        ucDHT11Value[1]	= uc_ComDHT11( ); // ʪ��L
        ucDHT11Value[2]	= uc_ComDHT11( ); // �¶�H
        ucDHT11Value[3]	= uc_ComDHT11( ); // �¶�L
        ucDHT11Value[4]	= uc_ComDHT11( ); // Check sum
        GPIOPD4_OUT();
        if( ( ucDHT11Value[0] + ucDHT11Value[1] + ucDHT11Value[2] +
              ucDHT11Value[3] ) == ucDHT11Value[4] )
        {
            *pHumiValue	= ucDHT11Value[0] + ucDHT11Value[1] / 10;
            *pTempValue	= ucDHT11Value[2] + ucDHT11Value[4] / 10;
            TempData = ucDHT11Value[2];
            HumiData = ucDHT11Value[0];
        }
    }
}
/* EOF */