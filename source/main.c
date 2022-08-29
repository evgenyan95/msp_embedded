#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer
#include "../header/LCD.h"

#include <stdio.h>

enum FSMstate state;
enum SYSmode lpm_mode;

void main(void){
  
    state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  
  while(1){
	switch(state){
	    case state0: ////Idle state                
            enterLPM(lpm_mode);
            break;
		 
	    case state1: 
	        MovePointerToZero();
		break;
			 
	    case state2:
	        pointer_follows();
		break;  
		     
	    case state3:
	        move_motor_to_zero_otomat();
		break;   
                
        case state4:
            P2IE |= 0x10;
            mouse_control();
            P2IE &= 0x10;
		break;


		// HELP states
        case state5:
            CountMotorTic();
        break;

        case state6:
            IE2 |= UCA0TXIE; // ack to PC , start run txt file
            run_txtFile();
        break;



	}
  }
}
  
  
  
  
  
  
