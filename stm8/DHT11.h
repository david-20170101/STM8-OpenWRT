#ifndef __DHT11_H__
#define __DHT11_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*************************�궨��******************************/
#define DATA_DQ_HIGH   PD_ODR|=0x10  
#define DATA_DQ_LOW    PD_ODR&=0xEF  

extern unsigned char TempData;
extern unsigned char HumiData;

void ReadTempAndHumi( float *pTempValue, float *pHumiValue ); //��ȡ�¶Ⱥ�ʪ��ֵ
void Delayms( unsigned int time );                            //��ʱ����

#ifdef __cplusplus
}
#endif

#endif
/* EOF */