#include  "../header/bsp.h"    // private library - BSP layer
#include "../header/LCD.h"

//-----------------------------------------------------------------------------  
//           GPIO configuration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
  // UART
  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;

  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1DIR |= RXLED + TXLED;
  P1OUT &= 0x00;

  // LCD config
  //P2SEL &= ~0xE0;               // P2.6-7 as GPIO for LCD
  //P2DIR |= 0xE0;

  //RGB
  P2DIR |= 0xE0;



  // rotating motor
  P2SEL &= 0x00;
  P2DIR &= 0x00;
  P2DIR |= 0x0F;   // output dir
  P2OUT &= 0x00;

  // Joystick
  P1SEL |= 0x18;  //p1.3 ->  ADC
  P1DIR &= ~0x18; //p1.3 input
  P2IE &= 0x00;
  P2IES &= 0x00;


  //RGBsArrPortSel &= ~0x07;            // GPIO capability
  //RGBsArrPortDir |= 0x07;             // output dir
  //RGBsArrPort &= ~0x07;             // clear all RGBs
  
  _BIS_SR(GIE);                     // enable interrupts globally
}                             
//------------------------------------------------------------------------------------- 
//            Timers configuration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  //TIMER A CONFIG
  TACTL = TASSEL_2 + ID_3;           // SMCLK, divide by 8
  TACCR0 = 0xB0A3;                            // 0.5s - 0xFFFF SMCLK-- 0xB0A3 -- ACLK 0x2C28
} 

//------------------------------------------------------------------------------------- 
//            LCD configuration 
//-------------------------------------------------------------------------------------
void LCDconfig(void){
    lcd_init();    //This function configures LCD
    lcd_clear();
    lcd_home();  
}              

//-------------------------------------------------------------------------------------
//            ADC congiguration
//-------------------------------------------------------------------------------------
void ADCconfig(void){

    ADC10CTL1 = INCH_4 + ADC10DIV_0 + CONSEQ_1 + SHS_0;
    ADC10CTL0 = SREF_0 + ADC10SHT_2  + MSC + ADC10ON;
    ADC10AE0 =  BIT4 + BIT3;
    ADC10DTC1 = 2;
}




//-------------------------------------------------------------------------------------
//            UART configuration
//-------------------------------------------------------------------------------------
void UARTconfig(void){
    UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
    UCA0BR0 = 104;                           //
    UCA0BR1 = 0x00;                           //
    UCA0MCTL = UCBRS0;               //
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

//-------------------------------------------------------------------------------------
//            Timer A
//-------------------------------------------------------------------------------------
void startTimerA(void){
    TACTL |= MC_3 + TACLR;
    TACCTL0 &= ~CCIFG;
    TACCTL0 |= CCIE;
}

void stopTimerA(void){
    TACTL &= ~MC_3;         // turn off TA
    TACCTL0 &= ~CCIE;      // disable interrupt
}



//------------------------------------------------------------------------------
//            ADC10 start
//------------------------------------------------------------------------------
void ADC10enable(void){
    ADC10CTL0 |= ADC10ON + ADC10IE + ENC; // DIGITAL OUTPUT: activate ADC + enable interrupts + enable conversion
    ADC10CTL0 |= ADC10SC;                 // start conversion
}
//------------------------------------------------------------------------------
//            ADC10 disable
//------------------------------------------------------------------------------
void disable_ADC10(void){
    ADC10CTL0 &= ~ADC10ON;                          // de-activate
    ADC10CTL0 &= ~ADC10IE;                          // disable interrupts
    ADC10CTL0 &= ~ENC;                              // disable conversion
    ADC10CTL0 &= ~ADC10SC;                          // stop conversion
}

//------------------------------------------------------------------------------
//            UART Rx Enable Interrupt
//------------------------------------------------------------------------------
void UartRxEnable(void){
    IE2 |= UCA0RXIE;
}

//------------------------------------------------------------------------------
//            UART Rx Disable Interrupt
//------------------------------------------------------------------------------
void UartRxDisable(void){
    IE2 &= ~UCA0RXIE;
}

