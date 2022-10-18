#include "reg51.h" 


#ifndef STC_8X
#define STC_8X

sfr ISP_CONTR = 0xC7;
//VCC = 1.19 * 1024 / ADC15

sfr     WDT_CONTR   =   0xc1;
sfr     CCON        =   0xd8;
sbit    CF          =   CCON^7;
sbit    CR          =   CCON^6;
sbit    CCF3        =   CCON^3;
sbit    CCF2        =   CCON^2;
sbit    CCF1        =   CCON^1;
sbit    CCF0        =   CCON^0;
sfr     CMOD        =   0xd9;
sfr     CL          =   0xe9;
sfr     CH          =   0xf9;
sfr     CCAPM0      =   0xda;
sfr     CCAP0L      =   0xea;
sfr     CCAP0H      =   0xfa;
sfr     PCA_PWM0    =   0xf2;
sfr     CCAPM1      =   0xdb;
sfr     CCAP1L      =   0xeb;
sfr     CCAP1H      =   0xfb;
sfr     PCA_PWM1    =   0xf3;
sfr     CCAPM2      =   0xdc;
sfr     CCAP2L      =   0xec;
sfr     CCAP2H      =   0xfc;
sfr     PCA_PWM2    =   0xf4;
sfr     CCAPM3      =   0xdd;
sfr     CCAP3L      =   0xed;
sfr     CCAP3H      =   0xfd;
sfr     PCA_PWM3    =   0xf5;


sfr INTCLKO = 0x8F;
#define EX2 0x10
#define EX3 0x20
#define EX4 0x40

sfr WKTCL = 0xAA;   //唤醒定时器低字节
sfr WKTCH = 0xAB;   //唤醒定时器高字节


sfr ADC_CONTR = 0xBC;   //带AD系列
sfr ADC_RES   = 0xBD;   //带AD系列
sfr ADC_RESL  = 0xBE;   //带AD系列
sfr ADCCFG = 0xDE;


sfr TH2  = 0xD6;
sfr TL2  = 0xD7;
sfr IE2   = 0xAF;
sfr INT_CLKO = 0x8F;
sfr AUXR = 0x8E;
sfr P_SW1 = 0xA2;


sfr AUXR1 = 0xA2;

sfr P_SW2 = 0xBA;
sfr S2CON = 0x9A;
sfr S2BUF = 0x9B;


sfr P4   = 0xC0;
sfr P5   = 0xC8;
sfr P6   = 0xE8;
sfr P7   = 0xF8;
sfr P1M1 = 0x91;    //PxM1.n,PxM0.n     =00--->Standard,    01--->push-pull
sfr P1M0 = 0x92;    //                  =10--->pure input,  11--->open drain
sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sfr P2M1 = 0x95;
sfr P2M0 = 0x96;
sfr P3M1 = 0xB1;
sfr P3M0 = 0xB2;
sfr P4M1 = 0xB3;
sfr P4M0 = 0xB4;
sfr P5M1 = 0xC9;
sfr P5M0 = 0xCA;
sfr P6M1 = 0xCB;
sfr P6M0 = 0xCC;
sfr P7M1 = 0xE1;
sfr P7M0 = 0xE2;

sbit P00 = P0^0;
sbit P01 = P0^1;
sbit P02 = P0^2;
sbit P03 = P0^3;
sbit P04 = P0^4;
sbit P05 = P0^5;
sbit P06 = P0^6;
sbit P07 = P0^7;
sbit P10 = P1^0;
sbit P11 = P1^1;
sbit P12 = P1^2;
sbit P13 = P1^3;
sbit P14 = P1^4;
sbit P15 = P1^5;
sbit P16 = P1^6;
sbit P17 = P1^7;
sbit P20 = P2^0;
sbit P21 = P2^1;
sbit P22 = P2^2;
sbit P23 = P2^3;
sbit P24 = P2^4;
sbit P25 = P2^5;
sbit P26 = P2^6;
sbit P27 = P2^7;
sbit P30 = P3^0;
sbit P31 = P3^1;
sbit P32 = P3^2;
sbit P33 = P3^3;
sbit P34 = P3^4;
sbit P35 = P3^5;
sbit P36 = P3^6;
sbit P37 = P3^7;
sbit P40 = P4^0;
sbit P41 = P4^1;
sbit P42 = P4^2;
sbit P43 = P4^3;
sbit P44 = P4^4;
sbit P45 = P4^5;
sbit P46 = P4^6;
sbit P47 = P4^7;
sbit P50 = P5^0;
sbit P51 = P5^1;
sbit P52 = P5^2;
sbit P53 = P5^3;
sbit P54 = P5^4;
sbit P55 = P5^5;
sbit P56 = P5^6;
sbit P57 = P5^7;

#endif
