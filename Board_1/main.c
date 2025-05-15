//***********************  main.c  ***********************
// Brief desicription of program:
// - Initializes an ESP8266 module to act as a WiFi client
//   and fetch weather data from openweathermap.org
//
//*********************************************************
/* to make this work you must
 Step 1) Change line 60 of this program with directions in lines 61-63
 Step 2) Add correct AT commands ("yourATcommand"s)in esp8266.c file 
 Step 3) Run a terminal emulator like Putty
         115200 bits/sec, 8 bit, 1 stop, no flow control
 Step 4) Set line 77 to match baud rate of your ESP8266 (9600 or 115200)
 Step 5) Set parameters of your AP in lines 41-42 of esp8266.c
 Step 6) For ESP8266_Reset function in esp8266.c, some ESP8266 respond with "ok", others with "ready"
         try different strings like "ready" and "ok"
 Step 7) For ESP8266_SetWifiMode function in esp8266.c, some ESP8266 respond with "ok", others with "no change"
         try different strings like "no change" and "ok"
 Step 8) choose correct part: 1 or 2 
 Example
 AT+GMR version 0018000902 uses "ready" and "no change"
 AT+GMR version:0.60.0.0 uses "ready" and "ok"
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "pll.h"
#include "UART.h"
#include "esp8266.h"
#include "LED.h"

#define part 2

void PortF_Init(void){
	//Initialize PF3, PF2, and PF1 as outputs
	//Initialize PF4 and PF0 as inputs
	volatile uint32_t delay;
	SYSCTL_RCGC2_R |= 0x20;									// activate clock for Port F
	delay=SYSCTL_RCGC2_R;										// allow time for clock to start
	GPIO_PORTF_LOCK_R = 0x4C4F434B;					// unlock GPIO Port F (Only for Port C,d and F)
  GPIO_PORTF_CR_R |= 0x1F;								// allow changes to PF4-0
	GPIO_PORTF_AMSEL_R &= ~0x1F;						// disable analog on PF
	GPIO_PORTF_AFSEL_R &= ~0x1F;						// disable alt funct on PF7-0
	GPIO_PORTF_PCTL_R &= ~0x000FFFFF;				// PCTL GPIO on PF4-0. writing 0 makes Pin to work as digital IO
	GPIO_PORTF_DIR_R |= 0x0E;								// Set Direction. 1 for output, 0 for input
	GPIO_PORTF_DIR_R &= ~0x11;
	GPIO_PORTF_PUR_R |= 0x11;								// enable pull-up on PF0 and PF4
	GPIO_PORTF_DEN_R |= 0x1F;								// enable digital I/O on PF4-0
}

// prototypes for functions defined in startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

char TXBuffer1[1024];
char Fetch[] = "GET /data/2.5/weather?q=Lexington&APPID=ffbcddd5ba2073a603ff59e9491c3261 HTTP/1.1\r\nHost:api.openweathermap.org\r\n\r\n";
// 1) go to http://openweathermap.org/appid#use
// 2) Register on the Sign up page
// 3) get an API key (APPID) replace the 1234567890abcdef1234567890abcdef with your APPID

///////////////////////////
//  main() for PART1
int main(void){
	uint8_t SWs = 0;
	uint8_t count=0;
	
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  LED_Init();
  Output_Init();       // UART0 only used for debugging
  PortF_Init();     // to use switches in Part2
  printf("\n\r-----------\n\rSystem starting...\n\r");
  ESP8266_Init(115200);      // connect to access point, set up as client
  ESP8266_GetVersionNumber();
  while(1){
    ESP8266_GetStatus();
		if (part == 1){
				if(ESP8266_MakeTCPConnection("api.openweathermap.org")){ // open socket in server
					LED_GreenOn();
					ESP8266_SendTCP(Fetch);
				}
				ESP8266_CloseTCPConnection();
				while(Board_Input()==0){// wait for touch
				};
				LED_GreenOff();
				LED_RedToggle();
			}
		
		else if (part == 2){
			if(ESP8266_MakeTCPConnection("charmander.atwebpages.com")){  // open a socket in server. TODO: Replace your own web host here 
				
				LED_GreenOn(); //green shows tcp connection started
				
				if ((GPIO_PORTF_DATA_R & 0x11) == 0x10){   // SWITCH 1
					SWs=1;			}	
				else if ((GPIO_PORTF_DATA_R & 0x11) == 0x01){   // SWITCH 2
					SWs=2;			}
				else if ((GPIO_PORTF_DATA_R & 0x11) == 0x00){   // Both SWITCHE 1 and 2
					SWs=3;			} 
				else {
					SWs=0;      }
				
				sprintf((char*)TXBuffer1, "POST /index.php?temperature=%d&switches=%d HTTP/1.1\r\nHost:charmander.atwebpages.com\r\nAccept:*/*\r\nContent-Type:application/x-www-form-urlencoded\r\n\r\n", count, SWs);  //TODO: Replace your own web host here. try using POST request instead of GET
				ESP8266_SendTCP(TXBuffer1);
				UART_OutString(TXBuffer1);
			
			}
			LED_RedOn();   //light shows tcp message sent

			ESP8266_CloseTCPConnection();
		 	LED_GreenOff();    //green off once TCP is closed
			LED_RedOff();
			count=count+1;
		}
  }
}
