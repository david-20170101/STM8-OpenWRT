#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*************************�궨��******************************/
#define LED_PB4_OFF    PB_ODR|=0x10   //С��Ϩ��
#define LED_PB4_ON     PB_ODR&=0xEF   //С�Ƶ���
#define LED_PB5_OFF    PB_ODR|=0x20   //С��Ϩ��
#define LED_PB5_ON     PB_ODR&=0xDF   //С�Ƶ���
#define LED_PC3_OFF    PC_ODR|=0x08   //С��Ϩ��
#define LED_PC3_ON     PC_ODR&=0xF7   //С�Ƶ���
#define LED_PC4_OFF    PC_ODR|=0x10   //С��Ϩ��
#define LED_PC4_ON     PC_ODR&=0xEF   //С�Ƶ���

void LED_Init(char *buffer);
void LED_Set(char *buffer);

#ifdef __cplusplus
}
#endif

#endif
/* EOF */