#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer

#include "../header/LCD.h"
#include <stdio.h>


unsigned int wait_time = 1000; // The time you have to wait between every two shifts
int frequency_val;
char frequency_arr[3];
unsigned int delay_val;
char op = '0';
char done = '0';


void run_txtFile(void){
    done = '0';
    int i = 0;
    while(write_file[i] != 'Z' ){
        while(write_file[i] != '\n'){
            op = write_file[i];
            if(op == '1')  {
                i++;
                int num = getNum(i);
                i += 2;
                BlinkRGB(num);
            }
            else if(op == '2')  {
                i++;
                int num = getNum(i);
                i += 2;
                rlc_leds(num);
            }
            else if(op == '3')  {
                i++;
                int num = getNum(i);
                i += 2;
                rrc_leds(num);
            }
            else if(op == '4')  {
                i++;
                delay_val = getNum(i);
                delay_val = delay_val * 1000;
                i += 2;
            }
            else if(op == '5')  {
                i++;
                int num = getNum(i);
                i += 2;
                clear_all_leds(num);
            }
            else if(op == '6')  {
                i++;
                int num = getNum(i);
                i += 2;
                stepper_deg(num);
            }
            else if(op == '7')  {
                i++;
                int num_1 = getNum(i);
                i += 2;
                int num_2 = getNum(i);
                i += 2;
                stepper_scan(num_1,num_2);
            }
            else if(op == '8')  {
                donetxt();
                i = -1;
                state = state0;
                break;
            }
            else i++;

        }
        if(write_file[i] == '\n') i += 2;
        if(i == -1)  break;
    }

}



void BlinkRGB(int num){
    char color = 0x20;
    int i;
    for( i = 0 ; i < num ; i++ ){
        P2OUT &=  0x00 ;
        P2OUT |= color;
        DelayMs(50000);
        color +=  0x20;
        if( (P2OUT & 0xE0 ) == 0xE0 )
                  P2OUT &= 0x20;
    }
    P2OUT &= 0x0F;
}

char leds = 0x00;
void rlc_leds(int num){
    int i;
    P1SEL &= ~0xE1;
    P1DIR |= 0xE1;
    P1OUT == leds;
    for( i = 0 ; i < num ; i++ ){
        if(P1OUT == 0x00){  P1OUT &= 0x00;  P1OUT |=  0x01;}
        else if(P1OUT ==  0x01){  P1OUT &= 0x00;  P1OUT |=  0x20;}
        else if(P1OUT ==  0x20){  P1OUT &= 0x00;  P1OUT |=  0x40;}
        else if(P1OUT ==  0x40){  P1OUT &= 0x00;  P1OUT |=  0x80;}
        else if(P1OUT ==  0x80){  P1OUT &= 0x00;  P1OUT |=  0x10;}

        leds = P1OUT;
        DelayMs(delay_val);
    }
    sysConfig();
    P1OUT &= 0x00;
}

void rrc_leds(int num){
    int i;
    P1SEL &= ~0xE1;
    P1DIR |= 0xE1;
    P1OUT == leds;
    for( i = 0 ; i < num ; i++ ){
        if(P1OUT == 0x00){  P1OUT &= 0x00;  P1OUT |=  0x80;}
        else if(P1OUT ==  0x80){  P1OUT &= 0x00;  P1OUT |=  0x40;}
        else if(P1OUT ==  0x40){  P1OUT &= 0x00;  P1OUT |=  0x20;}
        else if(P1OUT ==  0x20){  P1OUT &= 0x00;  P1OUT |=  0x01;}
        else if(P1OUT ==  0x01){  P1OUT &= 0x00;  P1OUT |=  0x80;}


        leds = P1OUT;
        DelayMs(delay_val);
    }
    sysConfig();
    P1OUT &= 0x00;
}

void clear_all_leds(){
    leds = 0x00;

}

int getNum(int i){
    unsigned int a;
    unsigned int b = 0;
    int c;
    unsigned int d,e;
    int qqqq = ((int)write_file[i] - '0')*10;
    int tttt = ((int)write_file[i+1] - '0');

    if (write_file[i] >= '0' && write_file[i] <= '9') a = ((int)write_file[i] - '0');
    else if (write_file[i] >= 'A' && write_file[i] <='F') a = ((int)write_file[i]- 'A' + 10);

    if (write_file[i+1] >= '0' && write_file[i+1] <= '9') b = ((int)write_file[i+1] - '0');
    else if (write_file[i+1] >= 'A' && write_file[i+1] <='F') b = ((int)write_file[i+1]- 'A' + 10);

    c = a*16 + b;
    return c;
}

void frequency_fun(void){
    int a = ((int)frequency_arr[0] - '0')*100;
    int b = ((int)frequency_arr[1] - '0')*10;
    int c = ((int)frequency_arr[2] - '0');
    frequency_val = a+b+c;
    wait_time = (int)fabs(100000.0/frequency_val);
}

