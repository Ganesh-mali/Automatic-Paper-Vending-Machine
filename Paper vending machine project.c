#include <pic.h>
#include <string.h>
#define _XTAL_FREQ   4000000 
void Sensor_scan();
#include "PIC_16F877A_LCD.h"
#include "PIC_16F877A_UART.h"


#define sw_1 RB7	//DEFINE PORTB.7 AS SWITCH
#define sw_2 RB6	//DEFINE PORTB.6 AS SWITCH
#define sw_3 RB5	//DEFINE PORTB.5 AS SWITCH
#define sw_4 RB4	//DEFINE PORTB.4 AS SWITCH

#define Sensor_1 RC0	
#define Sensor_2 RC1	

#define buzzer RB1 	//Define Buzzer
#define green_led RB3	//Define Green_Led
#define red_led RB2  	//Define Red_Led
#define IN1 RA0			//Define Motor Pin
#define IN2 RA3			//Define Motor Pin
#define IN3 RA1			//Define Motor Pin
#define IN4 RA2			//Define Motor Pin

unsigned char data_receive,z=0;
unsigned char RFID_no[13];
unsigned char user_1[]={"14001CBC41F5"};  //Define User_1 Card
unsigned char user_2[]={"14001CB63987"};  //Define User_2 Card
unsigned char user_3[]={"14001B96E67F"};  //Define User_3 Card
unsigned char authorised_person[]={"14001C61BFD6"};	//Define authorised_person Card
unsigned int user_1_amount=10,user_2_amount=10,user_3_amount=10; //Variable For Users Card Default Ammmount
unsigned int recharge_amount=10;
bit recharge_card_exit=0;
bit	smi;
int a,b,c,f;
bit user_1_entry,user_2_entry,user_3_entry,paper_empty=0;	//Bit To Set User Entry


void CARD_SWIPE();	// Function To Detect Card Entry
void user_1_access();	//Fuction To User_1 Access
void user_2_access();	//Fuction To User_2 Access
void user_3_access();	//Fuction To User_3 Access
void CARD_SWIPE_Recharg();	//Function To Recharg The Card
void recharge_card();	//Function To Recharge Card
void Paper_out_process();

void main(void)
{
    TRISB = 0b11110000;	//CONFIGURING PORT B AS Output/Input
	ADCON1 = 0x06;	//Define Port A as Digital.	
	TRISA	= 0x00;	//all ports as outputs	
    buzzer=1;red_led=0;green_led=1;
	__delay_ms(100);
	buzzer=0;red_led=1;green_led=0;
	IN1=0;IN2=0;
	IN3=0;IN4=0;
	init_uart();		// Initialize Uart
	InitLCD();         // Initialize LCD in 4bit mode
	ClearLCDScreen();
	WriteCommandToLCD(0x01);
	WriteStringToLCD("   RFID Based ");// Display The Name Of Projects
	WriteCommandToLCD(0xC0);
	WriteStringToLCD("Paper Vending Sym");
 	__delay_ms(4000);
	WriteCommandToLCD(0x01);

	while(1)
	{	
		CARD_SWIPE();//	Function To Detect Card Entry
	//	Sensor_scan();
		
   	}
	
}

void CARD_SWIPE()
{	
	unsigned char i,ch;

		user_1_entry=0;
		user_2_entry=0;
		WriteCommandToLCD(0x01);
		WriteCommandToLCD(0x80);
		__delay_ms(10);
		WriteStringToLCD("  SWIPE THE CARD");
		WriteCommandToLCD(0xc0);	
		WriteStringToLCD("   For Entry ");

		for(i=0;i<12;i++)
		{
			rxcive_data();
			
			RFID_no[i]=rx_d;
		}
		RFID_no[i++]='\0';
		WriteCommandToLCD(0x01);	
		WriteStringToLCD(RFID_no);
		__delay_ms(100);
		  a=1;b=1;f=1;
		a = strcmp(user_1,RFID_no);__delay_ms(10);
		b = strcmp(user_2,RFID_no);__delay_ms(10);
		c = strcmp(user_3,RFID_no);__delay_ms(10);
		f = strcmp(authorised_person,RFID_no);__delay_ms(10);


		if(a==0)
		{
			WriteCommandToLCD(0x01);
			WriteStringToLCD("user_1");
			WriteCommandToLCD(0xc0);
			WriteStringToLCD(user_1);
			user_1_entry=1;
			buzzer=1;
			green_led=0;
			__delay_ms(400);
			buzzer=0;
			__delay_ms(1000);
			green_led=1;
			user_1_access();		
		}
			
		else if(b==0)
		{
			WriteCommandToLCD(0x01);
			WriteStringToLCD("user_2");

			WriteCommandToLCD(0xc0);
			WriteStringToLCD(user_2);
			user_2_entry=1;
			buzzer=1;
			green_led=0;
			__delay_ms(400);
			buzzer=0;
			__delay_ms(1000);
			green_led=1;
			user_2_access();		
		}
			
		else if(c==0)
		{
			WriteCommandToLCD(0x01);
			WriteStringToLCD("user_3");

			WriteCommandToLCD(0xc0);
			WriteStringToLCD(user_3);
			user_3_entry=1;
			buzzer=1;
			green_led=0;
			__delay_ms(400);
			buzzer=0;
			__delay_ms(1000);
			green_led=1;
			user_3_access();		
		}
		
		else if(f==0)
		{
			WriteCommandToLCD(0x01);
			WriteStringToLCD("authorised_person");
			WriteCommandToLCD(0xc0);
			WriteStringToLCD(authorised_person);
			green_led=0;
			__delay_ms(3000);
			green_led=1;
			WriteCommandToLCD(0x01);
			WriteStringToLCD("Please Wait ");
			__delay_ms(400);
			WriteStringToLCD(".");
			__delay_ms(400);
			WriteStringToLCD(".");
			__delay_ms(400);
			WriteStringToLCD(".");
			__delay_ms(400);
			WriteStringToLCD(".");
			__delay_ms(400);
			WriteStringToLCD(".");
			__delay_ms(400);
			recharge_card();			
		}
		else
		{
			WriteCommandToLCD(0x01);
			WriteStringToLCD("Invalid Card");
			green_led=1;
			red_led=0;
			buzzer=1;
			__delay_ms(2000);
			green_led=1;
			red_led=1;
			buzzer=0;		
		}	
}


void CARD_SWIPE_Recharg()
{
	unsigned char i,ch;

		user_1_entry=0;
		user_2_entry=0;

		for(i=0;i<12;i++)
		{
			rxcive_data();
			RFID_no[i]=rx_d;
		}
		RFID_no[i++]='\0';
		WriteCommandToLCD(0x01);
		WriteStringToLCD(RFID_no);
		__delay_ms(100);
		  a=1;b=1;f=1;
		a = strcmp(user_1,RFID_no);__delay_ms(10);
		b = strcmp(user_2,RFID_no);__delay_ms(10);
		f = strcmp(authorised_person,RFID_no);__delay_ms(10);
 
	
		if(a==0)
		{
			WriteCommandToLCD(0x01);
			WriteStringToLCD("user_1");
			WriteCommandToLCD(0xc0);
			WriteStringToLCD(user_1);
			user_1_entry=1;
			buzzer=1;
			green_led=0;
			__delay_ms(400);
			buzzer=0;
			__delay_ms(1000);
			green_led=1;		
		}
			
		else if(b==0)
		{
			WriteCommandToLCD(0x01);
			WriteStringToLCD("user_2");
			WriteCommandToLCD(0xc0);
			WriteStringToLCD(user_2);
			user_2_entry=1;
			buzzer=1;
			green_led=0;
			__delay_ms(400);
			buzzer=0;
			__delay_ms(1000);
			green_led=1;		
		}
		
		else if(f==0)
		{
			WriteCommandToLCD(0x01);
			WriteStringToLCD("authorised_person");
			WriteCommandToLCD(0xc0);
			WriteStringToLCD(authorised_person);
			green_led=0;
			__delay_ms(3000);
			green_led=1;
			WriteCommandToLCD(0x01);
			WriteStringToLCD("Please Wait ");
			__delay_ms(400);
			WriteStringToLCD(".");
			__delay_ms(400);
			WriteStringToLCD(".");
			__delay_ms(400);
			WriteStringToLCD(".");
			__delay_ms(400);
			WriteStringToLCD(".");
			__delay_ms(400);
			WriteStringToLCD(".");
			__delay_ms(400);
			recharge_card();			
		}
		else
		{
			WriteCommandToLCD(0x01);
			WriteStringToLCD("Invalid Card");
			green_led=1;
			red_led=0;
			buzzer=1;
			__delay_ms(2000);
			green_led=1;
			red_led=1;
			buzzer=0;	
		}	
}


void recharge_card()
{
	recharge_card_exit=0;
	WriteCommandToLCD(0x01);
	WriteStringToLCD("Recharg Card");
	WriteCommandToLCD(0xc0);
	WriteStringToLCD("Swipe user Card");
	 CARD_SWIPE_Recharg();
	WriteCommandToLCD(0x01);
	if(user_1_entry==1)
	{
		while(recharge_card_exit==0)
		{
			WriteCommandToLCD(0x80);
			WriteStringToLCD(" S2>+ S3>- S4>S");

			if(sw_2==0)
			{if(user_1_amount<100)user_1_amount=user_1_amount+recharge_amount;__delay_ms(1);}

			if(sw_3==0)
			{if(user_1_amount>1)user_1_amount=user_1_amount-recharge_amount;__delay_ms(1);}

			if(sw_4==0)
			{recharge_card_exit=1;}

			WriteCommandToLCD(0xc0);
			WriteStringToLCD("User_1 : ");
			WriteDataToLCD(user_1_amount/100+0x30);

			WriteDataToLCD((user_1_amount/10)%10+0x30);

			WriteDataToLCD(user_1_amount%10+0x30);
			WriteStringToLCD("Rs");

			__delay_ms(1);
		}
	}

	else if(user_2_entry==1)
	{
		
		while(recharge_card_exit==0)
		{
			WriteCommandToLCD(0x80);
			WriteStringToLCD(" S2>+ S3>- S4>S");
			if(sw_2==0)
			{if(user_2_amount<100)user_2_amount=user_2_amount+recharge_amount;__delay_ms(1);}

			if(sw_3==0)
			{if(user_2_amount>1)user_2_amount=user_2_amount-recharge_amount;__delay_ms(1);}

			if(sw_4==0)
			{recharge_card_exit=1;}

			WriteCommandToLCD(0xc0);
			WriteStringToLCD("User_2 : ");
			WriteDataToLCD(user_2_amount/100+0x30);

			WriteDataToLCD((user_2_amount/10)%10+0x30);

			WriteDataToLCD(user_2_amount%10+0x30);
			WriteStringToLCD("Rs");
		}
	}
	else if(user_3_entry==1)
	{
		
		while(recharge_card_exit==0)
		{
			WriteCommandToLCD(0x80);
			WriteStringToLCD(" S2>+ S3>- S4>S");
			if(sw_2==0)
			{if(user_3_amount<100)user_3_amount=user_3_amount+recharge_amount;__delay_ms(1);}

			if(sw_3==0)
			{if(user_3_amount>1)user_3_amount=user_3_amount-recharge_amount;__delay_ms(1);}

			if(sw_4==0)
			{recharge_card_exit=1;}

			WriteCommandToLCD(0xc0);
			WriteStringToLCD("User_3 : ");
			WriteDataToLCD(user_3_amount/100+0x30);

			WriteDataToLCD((user_3_amount/10)%10+0x30);

			WriteDataToLCD(user_3_amount%10+0x30);
			WriteStringToLCD("Rs");
			__delay_ms(1);
		}
	}
	else
	{
		WriteCommandToLCD(0x01);
		WriteStringToLCD("invalid Card");
	}


}

void user_1_access()
{
	int d;
	if(user_1_amount!=0)
	{
		WriteCommandToLCD(0x01);
		WriteStringToLCD("user_1");

		WriteCommandToLCD(0xC0);
		WriteStringToLCD("Card Bal: ");
		d=user_1_amount;
		WriteDataToLCD(((d/100)%10)+0x30);
		WriteDataToLCD(((d/10)%10)+0x30);
		WriteDataToLCD((d%10)+0x30);	
		WriteStringToLCD(" Rs");
		__delay_ms(1000);
		WriteCommandToLCD(0x01);
		WriteStringToLCD("Paper Cost: ");
		d=2;
		//WriteDataToLCD(((d/100)%10)+0x30);
		WriteDataToLCD(((d/10)%10)+0x30);
		WriteDataToLCD((d%10)+0x30);	
		WriteStringToLCD("Rs");
		WriteCommandToLCD(0xc0);
		WriteStringToLCD("Current Bal: ");

		user_1_amount=(user_1_amount-2);
		d=user_1_amount;
		WriteDataToLCD(((d/100)%10)+0x30);

		WriteDataToLCD(((d/10)%10)+0x30);

		WriteDataToLCD((d%10)+0x30);
		WriteStringToLCD(" Rs");
		
		__delay_ms(1000);
		Paper_out_process();
	}
	else
	{WriteCommandToLCD(0x01);	WriteStringToLCD("Insufficent Bal");WriteCommandToLCD(0xc0);	
	WriteStringToLCD("Please Recharge");buzzer=1;__delay_ms(2000);	buzzer=0;}
	
}



void user_2_access()
{
	int d;
	if(user_2_amount!=0)
	{
		WriteCommandToLCD(0x01);
		WriteStringToLCD("user_2");

		WriteCommandToLCD(0xC0);
		WriteStringToLCD("Card Bal: ");
		d=user_2_amount;
		WriteDataToLCD(((d/100)%10)+0x30);
		WriteDataToLCD(((d/10)%10)+0x30);
		WriteDataToLCD((d%10)+0x30);	
		WriteStringToLCD(" Rs");
		__delay_ms(1000);
		WriteCommandToLCD(0x01);
		WriteStringToLCD("Paper Cost: ");
		d=2;
		//WriteDataToLCD(((d/100)%10)+0x30);
		WriteDataToLCD(((d/10)%10)+0x30);
		WriteDataToLCD((d%10)+0x30);	
		WriteStringToLCD("Rs");
		WriteCommandToLCD(0xc0);
		WriteStringToLCD("Current Bal: ");

		user_2_amount=(user_2_amount-2);
		d=user_2_amount;
		WriteDataToLCD(((d/100)%10)+0x30);

		WriteDataToLCD(((d/10)%10)+0x30);

		WriteDataToLCD((d%10)+0x30);
		WriteStringToLCD(" Rs");
		
		__delay_ms(1000);
		Paper_out_process();
	}
	else
	{WriteCommandToLCD(0x01);	WriteStringToLCD("Insufficent Bal");WriteCommandToLCD(0xc0);	
	WriteStringToLCD("Please Recharge");buzzer=1;__delay_ms(2000);	buzzer=0;}
	
}


void user_3_access()
{
	int d;
	if(user_3_amount!=0)
	{
		WriteCommandToLCD(0x01);
		WriteStringToLCD("user_3");

		WriteCommandToLCD(0xC0);
		WriteStringToLCD("Card Bal: ");
		d=user_3_amount;
		WriteDataToLCD(((d/100)%10)+0x30);
		WriteDataToLCD(((d/10)%10)+0x30);
		WriteDataToLCD((d%10)+0x30);	
		WriteStringToLCD(" Rs");
		__delay_ms(1000);
		WriteCommandToLCD(0x01);
		WriteStringToLCD("Paper Cost: ");
		d=2;
		//WriteDataToLCD(((d/100)%10)+0x30);
		WriteDataToLCD(((d/10)%10)+0x30);
		WriteDataToLCD((d%10)+0x30);	
		WriteStringToLCD("Rs");
		WriteCommandToLCD(0xc0);
		WriteStringToLCD("Current Bal: ");

		user_3_amount=(user_3_amount-2);
		d=user_3_amount;
		WriteDataToLCD(((d/100)%10)+0x30);

		WriteDataToLCD(((d/10)%10)+0x30);

		WriteDataToLCD((d%10)+0x30);
		WriteStringToLCD(" Rs");
		
		__delay_ms(1000);
		Paper_out_process();
	}
	else
	{WriteCommandToLCD(0x01);	WriteStringToLCD("Insufficent Bal");WriteCommandToLCD(0xc0);	
	WriteStringToLCD("Please Recharge");buzzer=1;__delay_ms(2000);	buzzer=0;}
	
}

void Paper_out_process()
{
	WriteCommandToLCD(0x01);
	WriteStringToLCD("  Please Take ");
	WriteCommandToLCD(0xc0);
	WriteStringToLCD("     Paper");
	green_led=0;buzzer=1;__delay_ms(400);green_led=1;buzzer=0;
	buzzer=0;red_led=0;green_led=1;
	__delay_ms(2000);
	IN1=0;IN2=1;IN3=0;IN4=1;__delay_ms(10000);
	IN1=0;IN2=0;IN3=0;IN4=0;
	WriteCommandToLCD(0x01);
	WriteStringToLCD(" Process Stop");
	green_led=0;buzzer=1;__delay_ms(400);green_led=1;buzzer=0;
	buzzer=0;red_led=1;green_led=1;
	__delay_ms(2000);
}

void Sensor_scan()
{
	if(Sensor_1==1 || Sensor_2==1)
	{paper_empty=1;}
	else
	{paper_empty=0;}
	
	while(paper_empty==1)
	 {
		WriteCommandToLCD(0x01);
		WriteStringToLCD("Please feed ");
		WriteCommandToLCD(0xc0);	
		WriteStringToLCD("Paper in tray");
		red_led=0;green_led=1;
		buzzer=1;__delay_ms(2000);buzzer=0;
		Sensor_scan();
		if(paper_empty==0)
		{
			WriteCommandToLCD(0x80);
			WriteStringToLCD("  SWIPE THE CARD");
			WriteCommandToLCD(0xc0);	
			WriteStringToLCD("   For Entry ");
			red_led=1;green_led=0;
		}
	}
}