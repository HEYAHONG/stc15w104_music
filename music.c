#include "stc15.h"
#include "music.h"
#define FOSC 12000000L //����Ƶ��
#define T1MS (65536-FOSC/1000)      //1Tģʽ


#define Beep P35 //�������ӿ� 
void main()
{
	{ //װ������
	opern=liangzhilaohu;
	opern_count_max=sizeof(liangzhilaohu)/sizeof(liangzhilaohu[0]);
	}
	{//T0��ʼ����������������ʱ����
	AUXR |= 0x80;                   //��ʱ��0Ϊ1Tģʽ
	//  AUXR &= 0x7f;                   //��ʱ��0Ϊ12Tģʽ

    TMOD = 0x00;                    //���ö�ʱ��Ϊģʽ0(16λ�Զ���װ��)
    TL0 = T1MS;                     //��ʼ����ʱֵ
    TH0 = T1MS >> 8;
    TR0 = 0;                        //�رն�ʱ�� 0
    ET0 = 0;                        //ʹ�ܶ�ʱ��0�ж�
    //EA = 1;

	}
	{//��ʼ��T2(ʱ��ʱ��)
 	AUXR |= 0x04;                   //��ʱ��2Ϊ1Tģʽ
	//  AUXR &= ~0x04;                  //��ʱ��2Ϊ12Tģʽ
	T2L = T1MS;                     //��ʼ����ʱֵ
    T2H = T1MS >> 8;
    AUXR |= 0x10;                   //��ʱ��2��ʼ��ʱ
    
    IE2 |= 0x04;                    //����ʱ��2�ж�
	}
	EA = 1;
while(1);//����ѭ��
}

void T1Timer() interrupt  1
{
  Beep=!Beep; //����������
}

void T2Timer() interrupt 12
{//����ʱ��ʱ��
static unsigned char count;
count++;
if(count>100)//��0.1s
	{
	count=0;
	if(!(current.scale&0xf)) //����Ϊ0���رշ�����
		{
		 TR0=0;
		 Beep=1;//�رշ�������ʵ�ʵ�ƽӦ���ݵ�·ȷ����
		}
	else //���ײ�Ϊ0,�޸ķ��������Ƶ��
		{
		unsigned int  Tc=65536-FOSC/(scale_table[current.scale>>4][(current.scale&0x0f)-1]*2);//���㶨ʱ��0��ֵ
			AUXR |= 0x80;                   //��ʱ��0Ϊ1Tģʽ
		//  AUXR &= 0x7f;                   //��ʱ��0Ϊ12Tģʽ

   			TMOD = 0x00;                    //���ö�ʱ��Ϊģʽ0(16λ�Զ���װ��)
		  	TL0 = Tc;                     //��ʼ����ʱֵ
    		TH0 = Tc >> 8;
    		TR0 = 1;                        //������ʱ�� 0
   			ET0 = 1;                        //ʹ�ܶ�ʱ��0�ж�

		}

	if(current.time) //ʱ�䲻Ϊ0
		{
		current.time--;//��ʱ��
		}
	else
		{//ʱ��Ϊ0��������һ������
		 if(opern)	//�����״���ʱ
		 	{
			 static unsigned int opern_count;
			 if(opern_count<opern_count_max)//������δ������ʱ
			 {
			 current=opern[opern_count];
			 opern_count++;
			 }
			 else //��ͷ����
			 {
			 opern_count=0;
			 current=opern[0];
			 }
			}
		}
	}


}