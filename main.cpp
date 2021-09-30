#include <msp430g2553.h>
int tempo = 0;
int main(void)
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR |= (BIT0 + BIT6);
  P1OUT &= ~(BIT0 + BIT6);

  P1REN |= BIT3;
  P1OUT |= BIT3;

  P1IES |= BIT3;
  P1IFG &= BIT3;
  P1IE |= BIT3;

  TA0CCR0 = 50000 - 1;
  TA0CTL = TASSEL_2 + ID_0 + MC_1;
  TA0CCTL0 = CCIE;
  _BIS_SR(GIE);
  for (;;);
}

#pragma vector = PORT1_VECTOR
__interrupt void portal(void)
{
  tempo = 0;
  P1OUT ^= BIT6;
  P1IFG &= ~BIT3;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer(void)
{
  tempo++;
  if (tempo >= 1200)
  {
    P1OUT |= BIT0;
    __delay_cycles(4000000);
    P1OUT &= ~BIT0;
    tempo = 0;
  }
  TA0CCTL0 &= ~CCIFG;
}