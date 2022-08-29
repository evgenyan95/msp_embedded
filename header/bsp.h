#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx
//#include  <msp430xG46x.h>           // MSP430x4xx -->MSP430FG4618

#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT2
#define RXD BIT1

#define   debounceVal      250
#define   LEDs_SHOW_RATE   0xD9E0  ////0xFFFF  // 62.5 msec
#define   Duty_Cycle_up     0x000E
#define   Duty_Cycle_down   0x0001

#define RGBsArrPort         P2OUT
#define RGBsArrPortDir      P2DIR
#define RGBsArrPortSel      P2SEL
#define RGBmask             0x07

// potentiometer p1.3

extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void LCDconfig(void);
extern void ADCconfig(void);
extern void UARTconfig(void);
extern void startTimerA(void);
extern void stopTimerA(void);
extern void disable_ADC10(void);
extern void ADC10enable(void);
extern void UartRxEnable(void);
extern void UartRxDisable(void);


// LEDs abstraction
//#define LEDsArrPort        P1OUT
//#define LEDsArrPortDir     P1DIR
//#define LEDsArrPortSel     P1SEL

// Switches abstraction
//#define SWsArrPortDir      P2DIR
//#define SWsArrPortSel      P2SEL
//#define SWmask             0x0F

///PWM wave
//#define LEDsArrPort2Dir     P2DIR
//#define PWMPort             P2OUT
//#define PWMmask             0x80

// PushButtons abstraction

// KEY PAD Interrupt Enable
//extern void KeyPadConfig(void);
//extern void keypad_interrupt_enable(void);
//extern void keypad_interrupt_disable(void);



#endif



