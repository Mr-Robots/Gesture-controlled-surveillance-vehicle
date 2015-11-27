#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"
#include "UART.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define Temp_sensor_flag	1
#define Light_sensor_flag 2
#define Smoke_sensor_flag 3
#define Transmit_flag			4

#define Right_Motor1	PF3
#define Right_Motor2	PF2
#define	Left_Motor1		PF1
#define	Left_Motor2		PF0

#define	Forward (1<<Right_Motor1)|(1<<Left_Motor1)
#define Reverse	(1<<Right_Motor2)|(1<<Left_Motor2)
#define Right		(1<<Right_Motor1)|(1<<Left_Motor2)
#define Left		(1<<Right_Motor2)|(1<<Left_Motor1)
#define Break		(1<<Right_Motor1)|(1<<Right_Motor2)|(1<<Left_Motor1)|(1<<Left_Motor2)

#define Motor_PORT	GPIO_PORTF_DATA_R

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

volatile unsigned int ADC_value[3];
double GPS_values[2];
volatile unsigned char flag_byte;
char GPS_temp_str[10];
volatile int intr_count,gps_counter;
volatile bool gps_flag;
char GPS_str1[10];

unsigned char status='N';
char GPS_str2[10];


void SysTick_Handler(void)
{
	gps_counter++;
	intr_count++;
	if(intr_count%20==0)
		flag_byte=(1<<(intr_count/20));
	if(gps_counter==10000)
	{
		gps_flag=1;
		gps_counter=0;
	}
	if(intr_count==60)
		intr_count=0;
}

void ADC0_Init(void)
{
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;   // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;         //    allow time for clock to stabilize
  GPIO_PORTE_DIR_R &= ~((1<<1)|(1<<2)|(1<<3));      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= ((1<<1)|(1<<2)|(1<<3));     // 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~((1<<1)|(1<<2)|(1<<3));      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= ((1<<1)|(1<<2)|(1<<3));     // 5) enable analog function on PE2
  SYSCTL_RCGC0_R |= 0x00010000;   // 6) activate ADC0 
  delay = SYSCTL_RCGC2_R;         
  SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1; // 11) channel Ain1 (PE2)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_ACTSS_R |= 0x0008;         // 13) enable sample sequencer 3
}

void Driving_Init(void)
{
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x0F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R |=(1<<PF0)|(1<<PF1)|(1<<PF2)|(1<<PF3) ;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_DEN_R |= (1<<PF0)|(1<<PF1)|(1<<PF2)|(1<<PF3);      
}

void Proximity_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= (1<<PORTA);
	delay = SYSCTL_RCGC2_R;           // allow time for clock to start
	GPIO_PORTA_AMSEL_R &= ~(1<<PA2)|(1<<PA3)|(1<<PA4);
  GPIO_PORTA_PCTL_R=0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTA_DIR_R  &= ~(1<<PA2)|(1<<PA3)|(1<<PA4);
  GPIO_PORTA_AFSEL_R  &= ~(1<<PA2)|(1<<PA3)|(1<<PA4);
  GPIO_PORTA_DEN_R |=(1<<PA2)|(1<<PA3)|(1<<PA4);
}

unsigned long ADC0_Read(unsigned char ch)
{  
	unsigned long result;
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+ch; // 11) channel Ain1 (PE2)
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}

int Gps_Tracker()
{
	int i=0;
	char Msg_ID[10];
	char temp;
	NVIC_ST_CTRL_R = 0x05;
	while(1)
	{
		while(UART1_InChar()!=0x24);
		for(i=0;i<6;i++)Msg_ID[i]=UART1_InChar();
		Msg_ID[6]='\0';
		if(strcmp("GPGGA,",Msg_ID)==0)break;
	}
	for(i=0;i<11;i++)temp=UART1_InChar();
	for(i=0;i<9;i++)GPS_temp_str[i]=UART1_InChar();
	temp=UART1_InChar();
	temp=UART1_InChar();
	if(temp!='N' && temp!='S')return 2;
	temp=UART1_InChar();
	for(i=0;i<10;i++)GPS_str2[i]=UART1_InChar();
	for(i=0;i<9;i++)GPS_str1[i]=GPS_temp_str[i];
	return 1;
}

int main()
{
	float temp=0;
	unsigned char temp1=0;
	int temp2=0;
	unsigned long long gps_lat,gps_lon;
	PLL_Init();
	SysTick_Init();
	Driving_Init();
	Proximity_Init();
	//SysTick_Wait_ms(1000);
	UART1_Init(1);
	UART2_Init();
	UART5_Init();
	ADC0_Init();
	

	while(1)
	{	
		if((UART5_FR_R&UART_FR_RXFE) == 0)
    {
			status=(unsigned char)(UART5_DR_R&0xFF);
			switch(status)
			{
				case 'N':
						Motor_PORT|=Break; 
						UART5_OutChar(status);
						break;
				case 'F':
						Motor_PORT=0x00;
						Motor_PORT|=Forward;
						UART5_OutChar(status);
						break;
				case 'B':
						Motor_PORT=0x00;
						Motor_PORT|=Reverse;
						UART5_OutChar(status);
						break;
				case 'R':
						Motor_PORT=0x00;
						Motor_PORT|=Right;
						UART5_OutChar(status);
						break;
				case 'L':
						Motor_PORT=0x00;
						Motor_PORT|=Left;
						UART5_OutChar(status);
						break;
			}
		}
		
//		if(gps_flag)
//		{
//			gps_flag=0;
//			temp2=Gps_Tracker();
//			NVIC_ST_CTRL_R = 0x00000007;
//		}
		
		switch(flag_byte)
		{
			case 0x02: 
								ADC_value[0]=ADC0_Read(0);
								flag_byte=0;
								break;
			case 0x04: 
								ADC_value[1]=ADC0_Read(1);
								flag_byte=0;
								break;
			case 0x08: 
								ADC_value[2]=ADC0_Read(2);
								flag_byte=(1<<Transmit_flag);
								break;
			default: break;
		}
		
		if(flag_byte==(1<<Transmit_flag))
		{
			flag_byte&=~(1<<Transmit_flag);
			
			UART2_OutChar(0xEF);
			UART2_OutChar(ADC_value[0] & 0x00FF);		//Temp. sensor. PE3
			UART2_OutChar(((ADC_value[0] & 0xFF00) >> 8));
			UART2_OutChar(ADC_value[1] & 0x00FF);		//Gas Sensor. 	PE2
			UART2_OutChar(((ADC_value[1] & 0xFF00) >> 8));
			UART2_OutChar(ADC_value[2] & 0x00FF);		//LDR.  				PE1
			UART2_OutChar(((ADC_value[2] & 0xFF00) >> 8));
			
			/*Conversion
			GPS_values[0]=(GPS_str1[0]-'0')*10+(GPS_str1[1]-'0');
			temp=(GPS_str1[2]-'0')*10+(GPS_str1[3]-'0')+(GPS_str1[5]-'0')*0.1+(GPS_str1[6]-'0')*0.01+(GPS_str1[7]-'0')*0.001+(GPS_str1[8]-'0')*0.0001;
			GPS_values[0]+=temp/60;
			GPS_values[1]=(GPS_str2[0]-'0')*100+(GPS_str2[1]-'0')*10+(GPS_str2[2]-'0');
			temp=(GPS_str2[3]-'0')*10+(GPS_str2[4]-'0')+(GPS_str2[6]-'0')*0.1+(GPS_str2[7]-'0')*0.01+(GPS_str2[8]-'0')*0.001+(GPS_str2[9]-'0')*0.0001;
			GPS_values[1]+=temp/60;
			//Conversion
			*/
			gps_lat=21.1629044*100000;
			gps_lon=72.7895564*100000;
			
			//gps_lat=GPS_values[0]*100000;
			//gps_lon=GPS_values[1]*100000;
			
			UART2_OutChar(gps_lat & 0x0000FF);
			UART2_OutChar((gps_lat & 0x00FF00) >> 8);
			UART2_OutChar((gps_lat & 0xFF0000) >> 16);
			UART2_OutChar(gps_lon & 0x0000FF);
			UART2_OutChar((gps_lon & 0x00FF00) >> 8);
			UART2_OutChar((gps_lon & 0xFF0000) >> 16);
			temp1=GPIO_PORTA_DATA_R&((1<<PA2)|(1<<PA3)|(1<<PA4));
			UART2_OutChar(temp1);
			UART2_OutChar(0xFF);
		}
	}
}

