#include <msp430.h>
#include <stdbool.h>

//All Defines
#define SYS_CLK	1000000

#define ADC_CHANNELS 3  // Globals for transmit UART communication
#define X_neutral	570
#define Y_neutral	540
#define Z_neutral	500
#define error_radius	150

//All Global Variables
int a,b;
volatile unsigned int ADC_values[ADC_CHANNELS];
volatile unsigned long  ADC_avg[ADC_CHANNELS];
volatile unsigned char int_ctr;
unsigned char previous_state='N', drive_state='N';
unsigned int Print_counter;
volatile bool adc_flag,avg_flag;

// Function prototypes
void ConfigureAdc(void);
void UART_Transmitchar(unsigned char);
void UART_Init(unsigned long long);
void UART_TransmitNumber(long int);

void main(void)
{
	 WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	 /* Use Calibration values  for 1MHz    Clock   DCO*/
	 DCOCTL =   0;
	 BCSCTL1    =   CALBC1_1MHZ;
	 DCOCTL =   CALDCO_1MHZ;

	 ConfigureAdc();					// ADC set-up function call
	 UART_Init(38400);

	while(1)
	{

		if(adc_flag)
		{
			adc_flag=0;
			ADC_avg[0]+=ADC_values[0];
			ADC_avg[1]+=ADC_values[1];
			ADC_avg[2]+=ADC_values[2];

			ADC10CTL0 &= ~ENC;
			while (ADC10CTL1 & BUSY);
			ADC10SA = (unsigned int)ADC_values;
			__delay_cycles(1000);				// Wait for ADC Ref to settle
			ADC10CTL0 |= ENC + ADC10SC;			// Sampling and conversion start
		}

		if(avg_flag)
		{
			avg_flag=0;
			ADC_avg[0]/=100;
			ADC_avg[1]/=100;
			ADC_avg[2]/=100;

			a= ((ADC_avg[0]-X_neutral) - (ADC_avg[1]-Y_neutral));
			b= ((ADC_avg[0]-X_neutral) + (ADC_avg[1]-Y_neutral));

			if(abs(ADC_avg[0]-X_neutral)<error_radius && abs(ADC_avg[1]-Y_neutral)<error_radius)
				drive_state='N';

			else if(a<0 && b>0)
				drive_state='F';
			else if(a>0 && b<0)
				drive_state='B';
			else if(a>0 && b>0)
				drive_state='R';
			else if(a<0 && b<0)
				drive_state='L';
		}

		if((Print_counter==60) || drive_state!=previous_state)
		{
			UART_Transmitchar(drive_state);
			previous_state=drive_state;
			Print_counter=0;
		}else Print_counter++;

	}


}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
	 int_ctr++;
	 if(int_ctr==100)
	 {
		avg_flag=1;
		int_ctr=0;
	 }
	 adc_flag=1;
}

// Function containing ADC set-up
void ConfigureAdc(void)
{
	ADC10CTL1 = INCH_5 + ADC10DIV_0 + CONSEQ_3 + SHS_0; //Multi-channel repeated conversion starting from channel 5<br />
	ADC10CTL0 = SREF_1 + ADC10SHT_2 + MSC + ADC10ON + ADC10IE + REFON + REF2_5V;
	ADC10AE0 = BIT5 + BIT4 + BIT3;
	ADC10DTC1 = ADC_CHANNELS;
	P1SEL |= BIT3 + BIT4 + BIT5;					// ADC input pin P1.3

	__enable_interrupt();			// Enable interrupts.

	ADC10CTL0 &= ~ENC;
	while (ADC10CTL1 & BUSY);
		ADC10SA = (unsigned int)ADC_values;
	__delay_cycles(1000);				// Wait for ADC Ref to settle
	ADC10CTL0 |= ENC + ADC10SC;			// Sampling and conversion start
	__bis_SR_register(GIE);	// Low Power Mode 0 with interrupts enabled
}

void UART_Init(unsigned long long baud)
{
	/* Configure   Pin Muxing  P1.1    RXD and P1.2    TXD */
   	P1SEL  =   BIT1    |   BIT2    ;
   	P1SEL2 =   BIT1    |   BIT2;

   	/* Place   UCA0    in  Reset   to  be  configured  */
   	UCA0CTL1   =   UCSWRST;

    /* Configure   */
    UCA0CTL1   |=  UCSSEL_2;   //  SMCLK
    UCA0BR0    =   (SYS_CLK/baud);    //  1MHz    9600
    UCA0BR1    =   0;  //  1MHz    9600
    UCA0MCTL   =   UCBRS0; //  Modulation  UCBRSx  =   1

    /* Take    UCA0    out of  reset   */
    UCA0CTL1   &=  ~UCSWRST;
}

void UART_Transmitchar(unsigned char data)
{
	while  (!(IFG2&UCA0TXIFG));    //  USCI_A0 TX  buffer  ready?
		UCA0TXBUF  =   data;  //  TX  -&amp;gt;   RXed    character
}

void UART_TransmitNumber(long int num)
{
	if(num<0)
	{
		UART_Transmitchar('-');
		num=(-1)*num;
	}

	if(num >= 10)
	{
		UART_TransmitNumber(num/10);
		num = num%10;
	}
	UART_Transmitchar(num+'0'); // n is between 0 and 9
}



