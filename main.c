/*
 * sudochen@163.com
 * This OS Just for Studing
 * Please contact the author for commercial use
 * All Right Reserved
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY
 */

#include <reg51.h>
#include <string.h>
#include <intrins.h>
#include "STC8X_SYSDEF.H"

sbit Led1=P3^4;
sbit Led2=P3^5;
sbit Led3=P3^6;
sbit Led4=P3^7;

#define ON				0
#define OFF				1
#define CONFIG_TASK_MAX	3
#define FOSC			11059200UL
#define BRT				(256 - FOSC / 115200 / 32)

typedef void (*pfunc)(void);
extern void SCHELDULE(void);

#define schedule() SCHELDULE()

struct task_struct {
	pfunc func;
	unsigned int ticks;
	unsigned char ipc;
	unsigned char sp_top;
	unsigned char sp[20];
};
#define PREFIX idata
#define TO_TASKSTRUCT(p) 	((struct task_struct *)(p))

struct task_struct task_led1;
struct task_struct task_led2;
struct task_struct task_idle;
PREFIX unsigned char pid = 0;
PREFIX unsigned char pid_max = 0;
PREFIX unsigned int task_list[CONFIG_TASK_MAX] = {0};
PREFIX unsigned int current = 0;

/* timer mode 1	and timeout is 10ms
 *
 */
void enable_timer0(void)
{
#ifdef STC_8X
 	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;			//设置定时初始值
	TH0 = 0xb8;			//20ms
	ET0 = 0;			//禁用定时器
	TF0 = 0;			//清除TF0标志
	TR0 = 0;			//定时器0开始计时
#else
	TMOD = 0x01;		// work mode
	TH0  = 0xf0;		// high value
	TL0	 = 0xd8;		// low value
	ET0  = 1;			// enable timer_interrupt
	TR0  = 0;			// disable timer0
#endif
}

#define BUFFER_LEN 16
bit busy;
PREFIX char wptr;
PREFIX char buffer[BUFFER_LEN];
PREFIX char console[BUFFER_LEN];
PREFIX char console_len;

void serial_init(void)		//115200bps@11.0592MHz
{
	SCON = 0x50;		//8???,?????
	AUXR &= 0xBF;		//?????12T??
	AUXR &= 0xFE;		//??1?????1???????
	TMOD &= 0x0F;		//???????
	TL1 = 0xFE;		//???????
	TH1 = 0xFF;		//???????
	ET1 = 0;		//?????%d??
	TR1 = 1;		//???1????
	ES = 1;
	wptr = 0;
	console_len = 0;
}

void uart_irq()
{
	if (TI) {
		TI = 0;
		busy = 0;
	}
	
	if (RI) {
		RI = 0;
		buffer[wptr] = SBUF;
		if (0x0a == buffer[wptr] || 0x0d == buffer[wptr]) {
			buffer[wptr] = 0;
			console_len = wptr + 1;
			wptr = 0;
			memcpy(console, buffer, console_len);
			return;
		}
		wptr ++;
		wptr &= 0x0f;
	}
}

void serial_putc(char dat)
{
	while (busy);
	busy = 1;
	SBUF = dat; 
}

void serial_printf(char *p)
{
	while (*p)
	{
		serial_putc(*p++);
	}
}

void task_init(struct task_struct *task, pfunc func)
{
	task->func = func;
	task->ticks = 0;
	task->ipc = 0;
	task->sp[1] = ((int)func) >> 8;		// hign
	task->sp[0] = ((int)func) ;			// low
	task->sp_top = (unsigned char)(task->sp) + 14;
	task_list[pid_max++] = (unsigned int)task;
}

char task_switch(void)
{
	// NOW SP is address of "task switch function"
	// SP - 2 is the current task runtime address
	TO_TASKSTRUCT(current)->sp_top = (unsigned char)(SP - 2);
	
	if (F0) {
		for (pid = 0; pid < pid_max; pid++) {
			if (task_list[pid]) {
				if (TO_TASKSTRUCT(task_list[pid])->ticks) {
					TO_TASKSTRUCT(task_list[pid])->ticks--;
				}
			}
		}
		F0 = 0;
	}

	for (pid = 0; pid < pid_max; pid++) {
		if (task_list[pid]) {
			if (!TO_TASKSTRUCT(task_list[pid])->ticks) {
				current = task_list[pid];
				break;
			}
		}
	}

	return TO_TASKSTRUCT(current)->sp_top;
}

void task_start(void)
{
	// when task_init we set task sp_top is sp+14
	// SP = sp+1, the address of the function of task[0] start address
	SP = (unsigned char )((TO_TASKSTRUCT(task_list[0])->sp_top) - 13);
	current = (unsigned int)task_list[0];
	TF0 = 0;
	TR0 = 1;
	ET0 = 1;
}


void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}


void delay_500ms(void)	//@11.0592MHz
{
	PREFIX unsigned char i, j, k;
	i = 8;
	j = 3;
	k = 67;
	do {
		do {
			while (--k);
		} while (--j);
	} while (--i);
}

void sleep(int ticks)
{
	EA = 0;
	TO_TASKSTRUCT(current)->ticks = ticks;
	EA = 1;
	schedule();
}

void func_led1(void)
{
	PREFIX unsigned char c = 0;
	PREFIX unsigned int task_led2 = task_list[1];

	while(1) {
		Led1=ON;
		sleep(25);
		Led1=OFF;
		sleep(25);		
		if (++c > 10) {
			c = 0;
			EA = 0;
			TO_TASKSTRUCT(task_led2)->ipc = 1;
			EA = 1;
			serial_printf("PID: 0 Led2 Turn over\n\r");
			
		}
	}
}

void func_led2(void)
{
	PREFIX unsigned char ipc = 0;
	PREFIX unsigned char recv = 0;
	PREFIX char buff[16];
	PREFIX unsigned char i = 0;
	while(1) {
		EA = 0;
		ipc = TO_TASKSTRUCT(current)->ipc;
		if (ipc) {
			TO_TASKSTRUCT(current)->ipc = 0;
		}
		EA = 1;
		if (ipc) {
			serial_printf("PID: 1 Recive [counter] Message\n\r");
			Led2=!Led2;
			if (!Led2) {
				serial_printf("PID: 1 Led2 Turn On\n\r");
			} else {
				serial_printf("PID: 1 Led2 Turn Off\n\r");
			}
		}
		EA = 0;
		recv = console_len;
		memcpy(buff, console, console_len);
		console_len = 0;
		EA = 1;
		
		if (recv) {
			serial_printf(buff);
			if (!strcmp(buff, "LED3ON")) {
				Led3 = ON;
				serial_printf(buff);
			}
			if (!strcmp(buff, "LED3OFF")) {
				Led3 = OFF;
				serial_printf(buff);
			}
		}
		sleep(1);	
	}
}

void func_idle(void)
{
	while(1) {
		Led4 = !Led4;
		Delay10ms();
	}
}

void main()
{
	PREFIX int i = 0;
	
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M1 = 0x00;
	P3M0 = 0x00;   	//设置为准双向口
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	
	enable_timer0();
	serial_init();
	
	for (i = 0; i < CONFIG_TASK_MAX; i++) {
		task_list[i] = 0;
	}
	
	for (i = 0; i < 10; i++) {
		Led1=!Led1;
		Led2=!Led2;
		Led3=!Led3;
		Led4=!Led4;
		delay_500ms();
	}
	for (i = 0; i < 10; i++) {
		delay_500ms();
	}

	Led1 = OFF;
	Led2 = OFF;
	Led3 = OFF;
	Led4 = OFF;

	task_init(&task_led1, func_led1);
	task_init(&task_led2, func_led2);
	task_init(&task_idle, func_idle);

	task_start();
	EA = 1;

	while(1);
}

