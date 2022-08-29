#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer


extern void BlinkRGB(int num);
extern void countUp(void);
extern void countDown(void);
extern void samplePoten(void);
extern void run_txtFile(void);
extern void frequency_fun(void);




extern char op;
extern char done;
extern int frequency;
extern char frequency_arr[3];
extern unsigned int wait_t;

#endif







