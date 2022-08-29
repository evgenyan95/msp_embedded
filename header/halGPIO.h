#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp.h"    		// private library - BSP layer
#include  "../header/app.h"    		// private library - APP layer

#define song1_length 54
#define song2_length 47
#define song3_length 40

extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern void sysConfig(void);

extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void enable_interrupts();
extern void disable_interrupts();

extern __interrupt void USCI0RX_ISR(void);
extern __interrupt void USCI0RX_ISR(void);
extern __interrupt void TIMER1_A1_ISR (void);
extern __interrupt void ADC10_ISR(void);
//extern __interrupt void PBs_handler(void);
//extern __interrupt void KEY_PAD_handler (void);
//extern void play(int KB);
//
//extern void DMAConfig(unsigned int *song, unsigned int *song_2d, unsigned int song_size);

extern char Flash_ptr1[100];
extern char *write_file;




//extern void getDelay(void);
//extern void displayVoltage(unsigned int mem);


extern void sysConfig(void);
extern void CountMotorTic(void);
extern void MovePointerToZero(void);
extern void pointer_follows(void);
extern void move_motor_to_zero_otomat(void);
extern void stepper_deg(int num);
extern void stepper_scan(int num_1,int num_2);
extern void donetxt();
extern void mouse_control(void);



#endif







