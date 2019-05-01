

#include <msp430.h>
#define uchar unsigned char
#define uint unsigned int

#define CPU_F 1000000  //Define the clock frequency to be 1MHZ
#define delay_nus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))
#define delay_nms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))

uchar xianshi1[]="Tep:00.0 C  ";
uchar xianshi2[]="L:000CM     ";

long current_time;//the current time
long juli;//the current distance
uint wendu;

/* Ports initializations */

#define BZ  BIT5  //Buzzer P25
#define JQ  BIT4  //Relay  P24
#define GM  BIT3  //Photosensitive  P23
#define HW  BIT0   //Infrared  P20

#define DS18B20       BIT3
#define DS18B20_H     P1OUT |= BIT3  //Temperature  P13
#define DS18B20_L     P1OUT &= ~BIT3




#define LED_1 BIT0  //The LED for the time of close distance
#define TA1_1 BIT2  //TA0.1 Ultrasonic ranging module HC-SR04 Echo P22
#define TRIG BIT1   //Ultrasonic ranging module HC-SR04 Trig       P21
#define MIN_UNIT_TO1M 1000  //1 m to 1 mm
/*TIMER0*/
#define SOUR_CLK  1045000  //Set the timer frequency
#define DIV_CLK_1  1       
#define SYS_CLK_SIG_1  SOUR_CLK/DIV_CLK_1
#define MAX_TIME_1 DISTANCE*2/34*SYS_CLK_SIG_1
#ifdef  MAX_TIME_1
#define SYS_CLK SYS_CLK_SIG_1    //SYS_CLK is equal to SYS_CLK_SIG_1
#endif

char flag; //Flag for measuring distance
long time;// The time for the infrared motion detector to detect no motion
long time2;//The time for close distance
void init_timer1()//Initialize timer1
{
  P2SEL |= TA1_1;                       //TA1.1 CCI1B be used   Set the second function
  P2DIR &= ~TA1_1;
  TA1CTL = MC_0 + TASSEL_2;             //Timer1 setup      
  TA1CCTL1 = CCIE + SCCI + CCIS_1 + SCS + CAP + CM_2; //TA0CC1 capture mode + down edge
}

long cal_distance()                     // Calculate the distance by measuring the time
{
 return (long)((340*(0.5000*current_time/SYS_CLK)*MIN_UNIT_TO1M))/10;
}
void delay()    //Delay
{
 unsigned char i,j;
 for(i=124;i>0;i--)
  for(j=8;j>0;j--);
}

void once_pro()                         //Send the ultrasonic sound
{
   if(TA1CCTL1 & COV)
   TA1CCTL1 &= ~COV;//Reset

   TA1R = 0;//Reset the timer
   P2OUT |= TRIG;   
   _EINT();// Initiate the interrupt
   TA1CTL |= MC_2;   //Start counting
   P2OUT &= ~TRIG;   //TRIG stops the module
   while(TA1CCTL1 & CCIFG);          //wait until the interrupt is over
}


/*TIMER0_INT_ISR*/
#pragma vector = TIMER1_A1_VECTOR
__interrupt void capture()
{
 current_time = TA1CCR1;  //Get the count
 TA1CTL &= ~MC_2;         //Stop counting
 TA1CCTL1 &= ~CCIFG;      //Clear the bit
 flag=1;
}




#define LCD_RS_PORT P1OUT    
#define LCD_RS_DDR  P1DIR

#define LCD_EN_PORT P1OUT    
#define LCD_EN_DDR  P1DIR


#define LCD_DATA_PORT P1OUT  
#define LCD_DATA_DDR P1DIR   
#define LCD_RS BIT0
#define LCD_EN BIT1
#define LCD_DATA    BIT7|BIT6|BIT5|BIT4   

void LCD_init(void);
void LCD_en_write1(void);  
void LCD_en_write2(void);  
void LCD_write_command(unsigned char command);
void LCD_write_data(unsigned char data);
void LCD_set_xy (unsigned char x, unsigned char y);
void LCD_write_string(unsigned char X,unsigned char Y, unsigned char *s);
void LCD_write_char(unsigned char X,unsigned char Y, unsigned char data);

// LCD1602 Initialization
void LCD_init(void)
{

 delay_nms(100);
 P1SEL=0X00;//
 LCD_DATA_DDR|=LCD_DATA;   //Set data as output
 LCD_EN_DDR|=LCD_EN;       //Set EN as output
 LCD_RS_DDR|=LCD_RS;       //Set RS as output
 LCD_en_write1();
 delay_nms(100);
 LCD_write_command(0x28);  // 4-bit data port
 delay_nms(50);
 LCD_write_command(0x28);  // 4-bit data port
 LCD_en_write2();
 delay_nms(50);
 LCD_write_command(0x28); // 4-bit data port
 delay_nms(50);
 LCD_write_command(0x01); // Clear the screen
  delay_nms(50);
 LCD_write_command(0x0c); 
  delay_nms(50);
 LCD_write_command(0x06); 
 delay_nms(50);

}

void LCD_en_write1(void)
{
    LCD_EN_PORT&=~LCD_EN;
    delay_nus(5);
    LCD_EN_PORT|=LCD_EN;
    delay_nus(5);
}


void LCD_en_write2(void)
{
   LCD_EN_PORT|=LCD_EN;
   delay_nus(5);
   LCD_EN_PORT&=~LCD_EN;
   delay_nus(5);
}

void LCD_write_command(unsigned char command)
{
   delay_nus(100);
   P1SEL=0x00;
   LCD_RS_PORT&=~LCD_RS; //RS=0
   LCD_en_write1();
   LCD_DATA_PORT&=0X0f; 
   LCD_DATA_PORT|=command&0xf0; 
   delay_nus(50);
   LCD_en_write2();
   command=command<<4; 
   LCD_en_write1();
   LCD_DATA_PORT&=0x0f; 
   LCD_DATA_PORT|=command&0xf0; 
   delay_nus(50);
   LCD_en_write2();
   delay_nms(5);
}

void LCD_write_data(unsigned char data)
{
   delay_nus(100);
   P1SEL=0x00;
   LCD_RS_PORT|=LCD_RS;      //RS=1
   LCD_en_write1();          
   LCD_DATA_PORT&=0X0f;      
   LCD_DATA_PORT|=data&0xf0; 
   delay_nus(50);
   LCD_en_write2();
   data=data<<4;             
   LCD_en_write1();
   LCD_DATA_PORT&=0X0f;      
   LCD_DATA_PORT|=data&0xf0; 
   LCD_en_write2();
   delay_nms(5);
}

void LCD_set_xy( unsigned char x, unsigned char y )
{
   unsigned char address;
   if (y == 0) address = 0x80 + x;
   else address = 0xc0 + x;
   LCD_write_command( address);
}

void LCD_write_string(unsigned char X,unsigned char Y,unsigned char *s)
{
 LCD_set_xy( X, Y ); //Write the address
    while (*s)          //Write the characters to be displayed
    {
      LCD_write_data( *s );
      s++;

    }
}

void LCD_write_char(unsigned char X,unsigned char Y,unsigned char data)
{
   LCD_set_xy( X, Y ); //Write the address
   LCD_write_data( data);
}

 // DS18B20 Initialization

 void B20_init(void);
 uchar B20_readB(void);
 void B20_writeB(uchar wrd);
 uint Read_temp(void);



 void B20_init(void)
 {
  P1DIR |= DS18B20;                     //Set as output
  P1REN &= ~DS18B20;
  DS18B20_L;                         
  __delay_cycles(600); // Wait for 60ms
  DS18B20_H;                         
  __delay_cycles(60);// Wait for 60ms
  P1DIR &= ~DS18B20;                     //Set as input
  P1REN |=DS18B20;
  while((P1IN &(DS18B20)));                 
  while(!(P1IN &(DS18B20)));                  
 }

 uchar B20_readB(void)
 {
  uchar i,retd=0;
  for(i=0;i<8;i++)         
  {
   retd>>=1;            
   P1DIR |=DS18B20;        
   P1REN &= ~DS18B20;
   DS18B20_L;                
   DS18B20_H;            

   __delay_cycles(5); 
   P1DIR &=~DS18B20;        
   P1REN |=DS18B20;

   if(P1IN&DS18B20)        
   {
    retd|=0x80;           
   }

   __delay_cycles(50);
  }
  return retd;            
 }

 void B20_writeB(uchar wrd)
 {
  uchar i;
  for(i=0;i<8;i++)          
 {
   P1DIR |=DS18B20;        
   P1REN &= ~DS18B20;
   DS18B20_L;                
   __delay_cycles(1);
   if(wrd&0x01)           
   {
    DS18B20_H;                
  }
   else
   {
    DS18B20_L;              
   }
  __delay_cycles(50);  
   DS18B20_H;               
   wrd>>=1;            
  }
  __delay_cycles(50);
 }

 uint Read_temp(void)
 {
  uchar templ,temph;
  uint temp=0;
  B20_init();            //Initialization

 B20_writeB(0xcc);      //Skip the ROM
 B20_writeB(0x44);      //Start to tranform the temperature
 B20_init();            //Initialization
 B20_writeB(0xcc);      //Skip the ROM
 B20_writeB(0xbe);      //Read the register
 templ=B20_readB();     
 temph=B20_readB();     
 temp=templ+temph*256;  
 temp*=0.625;
  return temp;            //Return the 16-bit variable
}


void main()//main function
{
  WDTCTL = WDTPW + WDTHOLD;              // Stop the watchdog timer
  DCOCTL = 0;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  P1OUT = 0;
  P2OUT = 0;
  P1REN =0;
  P2REN =0;

  P2REN |= TA1_1; //Input Ultrasonic sound signal
  P1DIR = LED_1;//LED_1 as output
  P2DIR|=TRIG; //TriG as output
  P2DIR|=BZ; //BZ as output
  P2DIR|=JQ; //JQ as output
  P2DIR&=~HW; //HW as input
  P2DIR&=~GM; //GM as input


  P2OUT |= HW;
  P2OUT |= GM;

  P2SEL &=~TRIG;
  P2SEL &=~BZ;
  P2SEL &=~HW;
  P2SEL &=~JQ;
  P2SEL &=~GM;


  init_timer1();//Initialize timer1
  LCD_init();//Initialize the LCD
  while(1)
  {
   int c = 15;
   while(c--)
   delay();
    wendu=Read_temp();

   if(TA1CCTL1 & CCIFG)
   TA1CCTL1 &= ~CCIFG;//Reset
   once_pro();//Start to measure the distance

   if((P2IN & GM))//Photosensitive 
   {
    if((P2IN & HW))//Infrared
    time=0;

    if(time2<50)
    {
      if(time<100)
      P2OUT|=JQ;
      else
      P2OUT&=~JQ;
    }
    else//Turn off the LED
    {
     time2=50;
     P2OUT&=~JQ;
     P1OUT |= LED_1;
    }

    time++;
    if(time>=100)
    time=100;
   }
   else
   P2OUT&=~JQ;

   if(flag)
   {
    flag=0;
    juli=cal_distance();//Calculate the distance
    if(juli<30)//Turn on the buzzer if the distance is smaller than 30cm
    {
     P2OUT^=BZ; //Turn on the buzzer
     time2++;
    }
    else
    {
     time2=0;
     P2OUT|=BZ;
     P1OUT &=~LED_1; 
    }
   }

 //Convert the data to LCD
   xianshi1[4]=wendu/100+0x30;
   xianshi1[5]=wendu/10%10+0x30;
   xianshi1[7]=wendu%10+0x30;
   xianshi1[8]=0xdf;

  xianshi2[2]=juli/100+0x30;
  xianshi2[3]=juli/10%10+0x30;
  xianshi2[4]=juli%10+0x30;

  LCD_write_string(0,0,xianshi1);//The first line display
  LCD_write_string(0,1,xianshi2);//The second line display

  }
}
