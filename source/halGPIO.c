#include  "../header/halGPIO.h"     // private library - HAL layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "../header/LCD.h"
#include "../header/api.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>




unsigned int wait_t = 1000;



// Joystick
int write_clear = 0; // 0 - Moving without write or clear
                     // 1 - write
                     // 2 - clear
tansmit_ang_or_mode = 0; // 0 - dont trtansmit
                         // 1 - mode trtansmit
                         // 2 - ang trtansmit

int num;
// mator
int Motor_conuter;
char Motor_conuter_string[5] = {'\n','\n','\n','\n','\n'};
char Motor_angel[4] = {'\n','\n','\n','\n'};
int Motor_initialization_state = 0; //0 - Motor rotation to zero angle
                                    //1 - Counting the number of movements per round
int transmit_arrey_index = 0;
float one_tic_size;
float wanted_angle;
float now_tics = 0;
float pi = 3.1415926;



// state mode
int state_mode = 0; // 0 - switch General mode
                    // 1 - switch Local mode of state 1
                    // 4 - switch Local mode of state 4
                    // 5 - switch Local mode of state 5
                    // 6 - switch Local mode of state 5

int conut_tic_mode = 0; // 0 - nothing
                        // 1 - start count

// txtfile ------
int tansmit_flag = 0;
//char test_arr[100];


char *write_file;
int file_num = 0; // can be 0,1 or 2
char Flash_ptr1[100];
char Flash_ptr2[100];
char Flash_ptr3[100];

//char Flash_ptr[3][100];



int Rx_index = 0; // how mane char's get;
int script_mode_state = 0; // 0 - not in state 4
                           // 1 - in state 4 and get file name
                           // 2 - in state 4 and get file size
                           // 3 - in state 4 and get file data
                           // 4 - in state 4 and run the txt file
char file_name_1[50];
char file_name_2[50];
char file_name_3[50];

char file_size_1[5];
char file_size_2[5];
char file_size_3[5];


//char *Flash_ptr1 = (char *) 0x1000;
//char *Flash_ptr2 = (char *) 0x1200;
//char *Flash_ptr3 = (char *) 0x1400;

struct txtFile {
  int NumOfFiles;
  char *name[3];
  //FILE *file[3];
  char *file[3];
  char *file_size[3];
} ;

struct txtFile Files = {.NumOfFiles = 0, .name = {file_name_1, file_name_2, file_name_3}, .file_size = {file_size_1, file_size_2, file_size_3}};
//----------------



//--------------------------------------------------------------------
//             System Configuration
//--------------------------------------------------------------------
void sysConfig(void){
	GPIOconfig();
	TIMERconfig();
    //LCDconfig();
    ADCconfig();
    UARTconfig();
//        KeyPadConfig();

}

//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
	volatile unsigned int j;

	for(j=t; j>0; j--);
}


//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00)
	  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01)
	  _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02)
	  _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
	else if(LPM_level == 0x03)
	  _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04)
	  _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}

//---------------------------------------------------------------------
//            UART Rx interrupt service routine
//---------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  stopTimerA();
  //disable_ADC10();
  //RgbPlayer(0);

  if (state_mode == 0) {
      char Rx_buf = UCA0RXBUF;
      switch(UCA0RXBUF) {
      case '0':
          state = state0; // sleep mode
          break;

      case '1':
          state_mode = 1;
          conut_tic_mode = 1;
          state = state1; // motor initionls
          break;

      case '2':
          state = state2; // pointer follows Joystick
          break;

      case '3':
          state = state3; // pointer go to ang zero
          break;

      case '4':
          state_mode = 4;  // input txt file
          script_mode_state = 1; // input file name

          //if(file_num == 2) write_file =  Flash_ptr3;
          //else if(file_num == 1) { write_file =  Flash_ptr2;  file_num = file_num + 1;}
          //else if(file_num == 0) { write_file =  Flash_ptr1; file_num = file_num + 1;}
          write_file =  Flash_ptr1;


          //Files.NumOfFiles = file_num+1;

          break;

      case '5':
            state_mode = 5;
            state = state4; // mouse control
            break;

      case '6':
          state_mode = 6;


      default:
          break;
      }
  }

  else if(state_mode == 1){
      char ttt = UCA0RXBUF;  // for debug
  switch(conut_tic_mode) {
       case 1:
           Motor_conuter = 0;
           conut_tic_mode = 2; // the next mode is sleep
           state = state5; // count tics
           break;

       case 2:
           conut_tic_mode = 3; //
           state_mode = 0; // end the count and wait for new func from PC
           state = state0; // sleep
           break;

       case 3:
           state_mode = 0; // end the count and wait for new func from PC
           conut_tic_mode = 0; //
           state = state0; // sleep
           break;

       default:
           break;
       }
   }

  else if(state_mode == 4){
        conut_tic_mode = 0;
        char Rx_buf = UCA0RXBUF;
       // if(file_num = 0) write_file =  Flash_ptr1;
       // else if(file_num = 1) write_file =  Flash_ptr2;
       // else write_file =  Flash_ptr3;

    switch(script_mode_state) {
     case 1:
         if(Rx_buf == 'Z'){
             script_mode_state = 2;
             Rx_index = 0;
            // Files.file[file_num] = fopen (Files.name[file_num], "w");
         }
         else{
             Files.name[file_num][Rx_index] = Rx_buf;
             write_file[Rx_index] =  Files.name[file_num][Rx_index];
             Rx_index++;
         }
         break;

     case 2:
         if(Rx_buf == 'Z'){
             script_mode_state = 3;
             Rx_index = 0;
            // Flash_ptr1 = (char*) malloc (44 * sizeof(char));
         }
         else{
             Files.file_size[file_num][Rx_index] = Rx_buf;
             write_file[Rx_index] =  Files.file_size[file_num][Rx_index];
             //Files.file[file_num] = Flash_ptr1;
             Rx_index++;
         }
         break;

     case 3:
         if(Rx_buf == 'Z'){
              script_mode_state = 4;
              write_file[Rx_index] = 'Z';
              Files.file[file_num] = write_file;
              Rx_index = 0;
         }
         else{
            //Files.file[file_num][Rx_index] = Rx_buf;
            // test_arr[Rx_index] =  Files.file[file_num][Rx_index];
             write_file[Rx_index] = Rx_buf;

             Rx_index++;
         }
         break;

     case 4:
         //file_num = file_num + 1; // can be 1,2 or 3
         state_mode = 0; // end the count and wait for new func from PC
         script_mode_state = 0; //
         state = state6; // run txt file function
         Rx_index = 0;
         break;

     default:
         break;
     }
 }

  else if(state_mode == 5){
      char Rx_buf = UCA0RXBUF;
      state_mode = 0;
      state = state0;
  }

  else if(state_mode == 6){
        char Rx_buf = UCA0RXBUF;
        if(Rx_buf == 'Z'){
              Rx_index = 0;
              state_mode = 0;
              frequency_fun();
              state = state0;
         }
         else{
             frequency_arr[Rx_index] = Rx_buf;
             Rx_index++;
         }
    }

   switch(lpm_mode){
    case mode0:
     LPM0_EXIT; // must be called from ISR only
     break;

    case mode1:
     LPM1_EXIT; // must be called from ISR only
     break;

    case mode2:
     LPM2_EXIT; // must be called from ISR only
     break;

    case mode3:
     LPM3_EXIT; // must be called from ISR only
     break;

    case mode4:
     LPM4_EXIT; // must be called from ISR only
     break;
   }

}

//---------------------------------------------------------------------
//            UART Tx interrupt service routine
//---------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if(conut_tic_mode == 3){
        UCA0TXBUF = Motor_conuter_string[transmit_arrey_index];
        transmit_arrey_index++;
        if (Motor_conuter_string[transmit_arrey_index] == '\n') {                 // TX over?
          IE2 &= ~UCA0TXIE;   // Disable USCI_A0 TX interrupt
          transmit_arrey_index = 0;
        }
    }

    else if(state == state6 && (op == '0' || done == '1')){
       UCA0TXBUF = 'a';//Motor_conuter_string[transmit_arrey_index];
       IE2 &= ~UCA0TXIE;    // Disable USCI_A0 TX interrupt
    }

    else if(state == state6 && tansmit_flag == 1 ){
        UCA0TXBUF = Motor_angel[transmit_arrey_index];
        transmit_arrey_index++;
        //if (Motor_angel[transmit_arrey_index] == '\n') {                 // TX over?
        if (transmit_arrey_index == 4) {                 // TX over?
            UCA0TXBUF = '\n';
            UCA0TXBUF = '\n';
          IE2 &= ~UCA0TXIE;   // Disable USCI_A0 TX interrupt
          transmit_arrey_index = 0;
        }                // Disable USCI_A0 TX interrupt
      }

    else if(state == state6 ) {
                UCA0TXBUF = 'Z';
                IE2 &= ~UCA0TXIE;}

    else if(state == state4 && tansmit_ang_or_mode == 1 ) {
              if(write_clear == 0) UCA0TXBUF = 'n';
              if(write_clear == 1) UCA0TXBUF = 'w';
              if(write_clear == 2) UCA0TXBUF = 'c';
              IE2 &= ~UCA0TXIE;}

    else if(state == state4 && tansmit_ang_or_mode == 2 ) {
            UCA0TXBUF = Motor_angel[transmit_arrey_index];
            transmit_arrey_index++;
            //if (Motor_angel[transmit_arrey_index] == '\n') {                 // TX over?
            if (transmit_arrey_index == 4) {                 // TX over?
                UCA0TXBUF = '\n';
                UCA0TXBUF = '\n';
              IE2 &= ~UCA0TXIE;   // Disable USCI_A0 TX interrupt
              transmit_arrey_index = 0;
           }
    }

    else if(state == state0) {
        UCA0TXBUF = 'a';
        IE2 &= ~UCA0TXIE;}

}



//*********************************************************************
//            TA0_A1 Interrupt vector
//*********************************************************************
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
    //TACCTL0 &= ~CCIFG;
    LPM0_EXIT;
//    return;
}


//*********************************************************************
//            ADC10 Interrupt Service Routine
//*********************************************************************
#pragma vector=ADC10_VECTOR
 __interrupt void ADC10_ISR(void){
    // displayVoltage(ADC10MEM);
 }



void MovePointerToZero(void){
    while(conut_tic_mode == 1){
        P2OUT &= 0x00;
        P2OUT |= 0x01;
        delay(wait_t);
        P2OUT &= 0x00;
        P2OUT |= 0x02;
        delay(wait_t);
        P2OUT &= 0x00;
        P2OUT |= 0x04;
        delay(wait_t);
        P2OUT &= 0x00;
        P2OUT |= 0x08;
        delay(wait_t);
    }
}

void CountMotorTic(void){
    while(conut_tic_mode == 2){
           P2OUT &= 0x00;
           P2OUT |= 0x01;
           Motor_conuter++;
           delay(wait_t);
           P2OUT &= 0x00;
           P2OUT |= 0x02;
           Motor_conuter++;
           delay(wait_t);
           P2OUT &= 0x00;
           P2OUT |= 0x04;
           Motor_conuter++;
           delay(wait_t);
           P2OUT &= 0x00;
           P2OUT |= 0x08;
           Motor_conuter++;
           delay(wait_t);
       }
    Motor_conuter_To_Motor_conuter_string();
    IE2 |= UCA0TXIE;
}

void Motor_conuter_To_Motor_conuter_string(void){
    //int num = Motor_conuter;
    num = Motor_conuter;
    int i;
    for(i = 3 ; i >= 0 ; i--){
        char c =  num%10 + '0';
        Motor_conuter_string[i] = c;
        num = num/10;
    }
}


void move_motor_to_zero_otomat(void){
   while(now_tics != 0){
       P2OUT &= 0x00;
       P2OUT |= 0x01;
       delay(wait_t);
       P2OUT &= 0x00;
       P2OUT |= 0x02;
       delay(wait_t);
       P2OUT &= 0x00;
       P2OUT |= 0x04;
       delay(wait_t);
       P2OUT &= 0x00;
       P2OUT |= 0x08;
       delay(wait_t);
      now_tics += 4;
      if(now_tics > Motor_conuter) now_tics = 0;
   }
      P2OUT &= 0x00;
      state = state0;
}


void move_4_tics_left(void){

       P2OUT &= 0x00;
       P2OUT |= 0x01;
       delay(wait_t);
       P2OUT &= 0x00;
       P2OUT |= 0x02;
       delay(wait_t);
       P2OUT &= 0x00;
       P2OUT |= 0x04;
       delay(wait_t);
       P2OUT &= 0x00;
       P2OUT |= 0x08;
       delay(wait_t);
      now_tics += 4;
      if(now_tics > Motor_conuter) now_tics = 0;
      P2OUT &= 0x00;
      if(state == state6 && tansmit_flag == 1){
          int i;
          int now_tics_num = now_tics;
          float pre_ang = (float)now_tics_num/(float)Motor_conuter * 360.0;
          int ang = (int)(round(pre_ang));
          for(i = 2 ; i >= 0 ; i--){
                 char c =  ang%10 + '0';
                 Motor_angel[i] = c;
                 ang = ang/10;
             }
          IE2 |= UCA0TXIE;
      }
}

void move_4_tics_right(void){

       P2OUT &= 0x00;
       P2OUT |= 0x08;
       delay(wait_t);
       P2OUT &= 0x00;
       P2OUT |= 0x04;
       delay(wait_t);
       P2OUT &= 0x00;
       P2OUT |= 0x02;
       delay(wait_t);
       P2OUT &= 0x00;
       P2OUT |= 0x01;
       delay(wait_t);
      now_tics -= 4;
      if(now_tics < 0) now_tics = Motor_conuter;
      P2OUT &= 0x00;
      if(state == state6 && tansmit_flag == 1){
         int i;
         int now_tics_num = now_tics;
         float pre_ang = (float)now_tics_num/(float)Motor_conuter * 360.0;
         int ang = (int)(round(pre_ang));
         for(i = 2 ; i >= 0 ; i--){
                char c =  ang%10 + '0';
                Motor_angel[i] = c;
                ang = ang/10;
            }

         IE2 |= UCA0TXIE;
     }

}

float angel;
float pp;
float a;
float b;
void pointer_follows(void){

    unsigned int adc_res[2];
    int wanted_tics;
    int tics_to_rot;
    int i;

    one_tic_size = (float)Motor_conuter/950.0;
    while(state == state2){
        ADC10CTL0 &= ~ENC;
        while(ADC10CTL1 & BUSY);
        ADC10CTL0 |= ENC + ADC10SC;
        ADC10SA = (unsigned int)adc_res;
        a = (float)adc_res[1] -476.0;
        b = (float)adc_res[0] -476.0;
        pp = a/b ;
        if(!(466 < adc_res[0] && adc_res[0] < 486 && 457 < adc_res[1] && adc_res[1] < 477)){
            angel =  fabs(atan( pp )); // atan(Y/X)
            angel = 360 * angel/(2*pi);
            if(adc_res[0] > 476 && adc_res[1] > 476) wanted_tics = (angel/360) * Motor_conuter + 0.25 * Motor_conuter;
            else if(adc_res[0] < 476 && adc_res[1] < 476) wanted_tics = (angel/360) * Motor_conuter + 0.75 * Motor_conuter;
            else if(adc_res[0] < 476 && adc_res[1] > 476) wanted_tics = 0.25 * Motor_conuter - (angel/360) * Motor_conuter + 0.5 * Motor_conuter;
            else if(adc_res[0] > 476 && adc_res[1] < 476) wanted_tics = 0.25 * Motor_conuter - (angel/360) * Motor_conuter ;

            tics_to_rot = (wanted_tics - now_tics)/4;

            if(tics_to_rot <  Motor_conuter && tics_to_rot > 0 ){
                for(i = 0 ; i < tics_to_rot ; i++){
                    move_4_tics_left();
                    }
            }

            if(tics_to_rot <  Motor_conuter && tics_to_rot < 0 ){
                for(i = 0 ; i < abs(tics_to_rot) ; i++){
                    move_4_tics_right();
                    }
            }
         } // if

      } // while

} // pointer_follows


// for state 6
void stepper_deg(int num){
    float d = (float)num/360.0;
    int new_tics = fabs((float)Motor_conuter * d);
    float tics_to_move = new_tics - now_tics;
    if( (fabs(tics_to_move) < Motor_conuter/2 && tics_to_move < 0) || (fabs(tics_to_move) > Motor_conuter/2 && tics_to_move > 0) ){
        tansmit_flag = 1;
        while(now_tics != new_tics && now_tics != new_tics+1 && now_tics != new_tics-1 && now_tics != new_tics+2)
            move_4_tics_right();
   }
    else if((fabs(tics_to_move) > Motor_conuter/2 && tics_to_move < 0) || (fabs(tics_to_move) < Motor_conuter/2 && tics_to_move > 0)){
        tansmit_flag = 1;
        while(now_tics != new_tics && now_tics != new_tics+1 && now_tics != new_tics-1 && now_tics != new_tics+2)
            move_4_tics_left();
      }
    else tansmit_flag = 0;
    P2OUT &= 0x00;
}


void stepper_scan(int num_1,int num_2){
    stepper_deg(num_1);
    stepper_deg(num_2);
    float d1 = (float)num_1/360.0;
    float d2 = (float)num_2/360.0;

}

void donetxt(){
    done = '1';
    state_mode = 0; // end the count and wait for new func from PC
    IE2 |= UCA0TXIE;

}

void mouse_control(void){
    unsigned int adc_res[2];

    int i;
    while(state == state4){
        int final_ang;
        ADC10CTL0 &= ~ENC;
        while(ADC10CTL1 & BUSY);
        ADC10CTL0 |= ENC + ADC10SC;
        ADC10SA = (unsigned int)adc_res;
        a = (float)adc_res[1] -476.0;
        b = (float)adc_res[0] -476.0;
        pp = a/b ;
        if(!(466 < adc_res[0] && adc_res[0] < 486 && 457 < adc_res[1] && adc_res[1] < 477)){
            angel =  fabs(atan( pp )); // atan(Y/X)
            angel = 360 * angel/(2*pi);
            if(adc_res[0] > 476 && adc_res[1] > 476) final_ang = angel  + (0.25 * 360);
            else if(adc_res[0] < 476 && adc_res[1] < 476) final_ang = angel + (0.75 * 360);
            else if(adc_res[0] < 476 && adc_res[1] > 476) final_ang = (0.25 * 360) - angel + (0.5 * 360);
            else if(adc_res[0] > 476 && adc_res[1] < 476) final_ang = 0.25 * 360 - angel;

            for(i = 2 ; i >= 0 ; i--){
                 char c =  final_ang%10 + '0';
                 Motor_angel[i] = c;
                 final_ang = final_ang/10;
             }
            tansmit_ang_or_mode = 2; // ang  tansmit
            IE2 |= UCA0TXIE;
        }

      } // while
}
//*********************************************************************
//            Port2 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT2_VECTOR
  __interrupt void PBs_handler(void){

  delay(debounceVal);
  P2IFG &= 0x00;

  if(write_clear == 0) write_clear = 1;
  else if(write_clear == 1) write_clear = 2;
  else if (write_clear == 2) write_clear = 0;
  tansmit_ang_or_mode = 1; // mode  tansmit
  IE2 |= UCA0TXIE; // send write or clear mode to pc
  }


