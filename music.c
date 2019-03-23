#include "stc15.h"
#include "music.h"
#define FOSC 12000000L //工作频率
#define T1MS (65536-FOSC/1000)      //1T模式


#define Beep P35 //蜂鸣器接口 
void main()
{
	{ //装载曲谱
	opern=liangzhilaohu;
	opern_count_max=sizeof(liangzhilaohu)/sizeof(liangzhilaohu[0]);
	}
	{//T0初始化（蜂鸣器驱动定时器）
	AUXR |= 0x80;                   //定时器0为1T模式
	//  AUXR &= 0x7f;                   //定时器0为12T模式

    TMOD = 0x00;                    //设置定时器为模式0(16位自动重装载)
    TL0 = T1MS;                     //初始化计时值
    TH0 = T1MS >> 8;
    TR0 = 0;                        //关闭定时器 0
    ET0 = 0;                        //关闭定时器0中断
    //EA = 1;

	}
	{//初始化T2(时序定时器)
 	AUXR |= 0x04;                   //定时器2为1T模式
	//  AUXR &= ~0x04;                  //定时器2为12T模式
	T2L = T1MS;                     //初始化计时值
    T2H = T1MS >> 8;
    AUXR |= 0x10;                   //定时器2开始计时
    
    IE2 |= 0x04;                    //开定时器2中断
	}
	EA = 1;
while(1);//无限循环
}

void T1Timer() interrupt  1
{
  Beep=!Beep; //驱动蜂鸣器
}

void T2Timer() interrupt 12
{//进入时序定时器
static unsigned char count;
count++;
if(count>100)//到0.1s
	{
	count=0;
	if(!(current.scale&0xf)) //音阶为0，关闭蜂鸣器
		{
		 TR0=0;
		 Beep=1;//关闭蜂鸣器（实际电平应根据电路确定）
		}
	else //音阶不为0,修改蜂鸣器输出频率
		{
		unsigned int  Tc=65536-FOSC/(scale_table[current.scale>>4][(current.scale&0x0f)-1]*2);//计算定时器0初值
			AUXR |= 0x80;                   //定时器0为1T模式
		//  AUXR &= 0x7f;                   //定时器0为12T模式

   			TMOD = 0x00;                    //设置定时器为模式0(16位自动重装载)
		  	TL0 = Tc;                     //初始化计时值
    		TH0 = Tc >> 8;
    		TR0 = 1;                        //启动定时器 0
   			ET0 = 1;                        //使能定时器0中断

		}

	if(current.time) //时间不为0
		{
		current.time--;//减时间
		}
	else
		{//时间为0，加载下一个音阶
		 if(opern)	//当曲谱存在时
		 	{
			 static unsigned int opern_count;
			 if(opern_count<opern_count_max)//当曲谱未播放完时
			 {
			 current=opern[opern_count];
			 opern_count++;
			 }
			 else //从头播放
			 {
			 opern_count=0;
			 current=opern[0];
			 }
			}
		}
	}


}